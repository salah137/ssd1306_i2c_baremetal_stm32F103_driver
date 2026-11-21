#include <stdint.h>
#include "I2c_driver/I2C_driver.h"

#define IRQ(n) [16 + (n)]

extern uint32_t _sidata;
extern uint32_t _estack;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

extern void enable_exti();
extern int main();
extern void I2C_ERR_IRQ(void);

void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void) __attribute((weak, alias("Default_Handler")));
void HardFault_Handler(void);


__attribute__((section(".vector_table"))) const uint32_t vector_table[] = {
    [0]  = (uint32_t)&_estack,
    [1]  = (uint32_t)&Reset_Handler,
    [2]  = (uint32_t)&NMI_Handler,
    [3]  = (uint32_t)&HardFault_Handler,

    IRQ(I2C1_ER) = (uint32_t)&I2C_ERR_IRQ
};

void Reset_Handler(void) {
  uint32_t *_sptr = &_sidata;
  uint32_t *d_ptr = &_sdata;

  while (d_ptr < &_edata) {
    (*d_ptr++) = *(_sptr++);
  }

  d_ptr = &_sbss;

  while (d_ptr < &_ebss) {
    (*d_ptr++) = 0;
  }

  main();
  
  while (1) {
  
  };
}

void Default_Handler(void){
    while (1) {
    
    }
}

void HardFault_Handler(void){
    
}