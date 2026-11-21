#include "I2C_driver.h"
#include "stdint.h"
#include <stdint.h>

#define I2C_ADDRESS 0x3C

static i2c_packet_t * volatile current_packet = 0;

static void set_intr_pr(uint8_t irq, uint8_t prio) {
  NVIC_IPR[irq] = (uint8_t)((prio & 0xFU) << 4);
}

void init_i2c(void) {
  // enable clock
  RCC_APB1ENR |= (1 << I2C_1_EN);
  RCC_APB2ENR |= (1 << IOPBEN);

  GPIOB_CRL &=
      ~((0xF << (4 * (SCL))) | (0xF << (4 * (SDA)))); // 11 => CNF 11 => MODE

  GPIOB_CRL |= (0xF << (4 * (SCL))); // 11 => CNF 11 => MODE
  GPIOB_CRL |= (0xF << (4 * (SDA))); // 11 => CNF 11 => MODE

  I2C_CR2 |= 0x08;      // 8MHZ
  I2C_CCR |= 0x07;      // PCLK1/(3 x FS_SPEED)
  I2C_CCR |= (1 << 15); // FM = 400khz

  I2C_CR2 |= (1 << 9);  // event interrupt
  I2C_CR2 |= (1 << 10); // buff intr
  I2C_CR2 |= (1 << 8);  // ITERREN

  I2C_TRISE = 4;

  I2C_CR1 |= (1 << 10); // ACK enabled
  set_intr_pr(I2C1_EV_IRQN, 10);
  NVIC_ISER[0] = (1 << I2C1_EV_IRQN);

  set_intr_pr(I2C1_ER, 10);
  NVIC_ISER[1] = (1 << (I2C1_ER & 31));

  I2C_CR1 |= (1 << 0); // PE
}

void start_bus(void) {
  I2C_CR1 |= (1 << START_BUS); // start byte
  while (!(I2C_SR1 & (1 << SB)))
    ;
}

void send_address(uint8_t address) {
  I2C_DR = (address << 1);
  while (!(I2C_SR1 & (1 << ADDR)))
    ;

  volatile uint32_t temp = I2C_SR1;
  temp = I2C_SR2;
  (void)temp;
}

void send_the_byte(uint8_t byte) {
  while (!(I2C_SR1 & (1 << TXE)))
    ;
  I2C_DR = byte;
}
void stop_bus() {
  I2C_CR1 |= (1 << STOP_BUS); // stop byte
}

void send_playload(i2c_packet_t **sending) {
  current_packet = *sending;
  current_packet->last_error = NO_ERR;
  
  start_bus();
  send_address(current_packet->address);

  for (uint16_t i = 0; i < current_packet->playload_len; i++) {
    send_the_byte(current_packet->playload[i]);
  }
  while (!(I2C_SR1 & (1 << 2))) {
    if (current_packet->last_error != NO_ERR) {
      break;
    }
  }

  stop_bus();
}

void I2C_ERR_IRQ(void) {
  if (I2C_SR1 & (1 << AF)) {
    // acknowledge faillure
    current_packet->last_error = AF_ERR;
    I2C_SR1 &= ~(1 << AF);
    I2C_CR1 |= (1 << STOP_BUS);
  }

  if (I2C_SR1 & (1 << OVR)) {
    // overrun
    current_packet->last_error = OVR_ERR;
    I2C_SR1 &= ~(1 << OVR);
  }

  if (I2C_SR1 & (1 << ARLO)) {
    // detected a misplaced Start or Stop condition
    current_packet->last_error = ARLO_ERR;
    I2C_SR1 &= ~(1 << ARLO);
  }

  if (I2C_SR1 & (1 << BERR)) {
    // lost control of the I2C bus because(signal integrity issues)
    current_packet->last_error = BERR_ERR;
    I2C_SR1 &= ~(1 << BERR);
  }
}
