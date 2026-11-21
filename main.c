#include "stdint.h"

extern void start_ssd1306(void);
extern void ssd1306_draw_pixel(uint8_t x, uint8_t y, int color);
extern void ssd1306_draw_rectangle_empty(uint8_t x, uint8_t y, uint8_t hight, uint8_t width);

int main(){
    start_ssd1306();
    ssd1306_draw_rectangle_empty(62,32,10,22);

    while (1) {
    
    }
   return 0; 
}