#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>
#define I2C_BASE 0x40005400UL
#define RCC_BASE 0x40021000UL
#define GPIOB_BASE 0x40010C00UL

#define I2C_CR1 (*((volatile uint32_t *)I2C_BASE + 0x00))
#define I2C_CR2 (*((volatile uint32_t *)I2C_BASE + 0x04))

#define I2C_OAR1 (*((volatile uint32_t *)I2C_BASE + 0x08))
#define I2C_OAR2 (*((volatile uint32_t *)I2C_BASE + 0x0C))

#define I2C_DR (*((volatile uint32_t *)I2C_BASE + 0x10))

#define I2C_SR1 (*((volatile uint32_t *)I2C_BASE + 0x14))
#define I2C_SR2 (*((volatile uint32_t *)I2C_BASE + 0x18))

#define I2C_CCR (*((volatile uint32_t *)I2C_BASE + 0x1C))
#define I2C_TRISE (*((volatile uint32_t *)I2C_BASE + 0x20))

#define RCC_APB2ENR (*((volatile uint32_t *)RCC_BASE + 0x18))
#define RCC_APB1ENR (*((volatile uint32_t *)RCC_BASE + 0x1C))

#define GPIOB_CRL (*((volatile uint32_t *) GPIOB_BASE + 0x00))
#define GPIOB_CRH (*((volatile uint32_t *) GPIOB_BASE + 0x04))

#define NVIC_ISER ((volatile uint32_t *)0xE000E100UL)
#define NVIC_IPR ((volatile uint8_t *)(0xE000E400UL))

#define I2C1_EV_IRQN 31 
#define I2C1_ER 32

#define I2C_1_EN 21
#define IOPBEN 3

// register consts(SR1)
#define START_BUS 8
#define STOP_BUS 9
#define SB 0
#define ADDR 1
#define TXE 7

// error flags
#define AF 10
#define OVR 11
#define ARLO 9
#define BERR 8

#define SDA 7
#define SCL 6

typedef enum {
    AF_ERR,
    OVR_ERR,
    ARLO_ERR,
    BERR_ERR,
    NO_ERR
} errors_t;

typedef  struct {
    errors_t last_error;
    uint8_t address;
    uint8_t* playload;
    uint16_t playload_len;
} i2c_packet_t;

#endif