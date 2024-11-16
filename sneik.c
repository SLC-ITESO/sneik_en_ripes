#include "ripes_system.h"
#include <stdio.h> 
#include <stdlib.h> 


void set_pixel(unsigned int x, unsigned int y, unsigned int color);
void delay();
void spawn_square(unsigned int x, unsigned int y, unsigned int color);
void spawn_apple();
void spawn_snake();
void spawn_snake_part(unsigned short x, unsigned short y, unsigned short xx, unsigned short yy);

volatile unsigned int * d_pad_u = D_PAD_0_UP;
volatile unsigned int * d_pad_d = D_PAD_0_DOWN;
volatile unsigned int * d_pad_l = D_PAD_0_LEFT;
volatile unsigned int * d_pad_r = D_PAD_0_RIGHT;

//led max
unsigned short max_x = LED_MATRIX_0_WIDTH - 1;
unsigned short max_y = LED_MATRIX_0_HEIGHT - 1;

//apple coords
unsigned int ax = 0;
unsigned int ay = 0;

//snake coords
unsigned int x = 17;
unsigned int y = 12;

//snake stuffs
struct snake {
    
    unsigned short x;
    unsigned short xx;
    unsigned short y;
    unsigned short yy;
    int dir;

    struct snake *next;

};
typedef struct snake Snake;

Snake *head;
Snake *tail;

//color
unsigned int ro = 0x00ff0000;

void main (){

    //spawn_square(x,y,ro);
    
    unsigned int mov = 4;    // 1 = arriba, 2 = abajo, 3 = izq, 4 = derecha
      
    spawn_apple();
    spawn_snake();
    
    
    while (1){
        if (*d_pad_u == 1 && mov != 2) {
            mov = 1;
        }
        if (*d_pad_d == 1 && mov != 1) {
            mov = 2;
        }
        if (*d_pad_l == 1 && mov != 4) {
            mov = 3;
        }
        if (*d_pad_r == 1 && mov != 3) {
            mov = 4;
        }
        //movimiento(mov);
        delay();
    }
}

void movimiento(unsigned int mov){
    switch(mov){
        case 1://arriba
            spawn_square(x,y,0);
            y += 1;
            spawn_square(x,y,ro);
            break;
        case 2://abajo
            spawn_square(x,y,0);
            y -= 1;
            spawn_square(x,y,ro);
            break;
        case 3://izq
            spawn_square(x,y,0);
            x -= 1;
            spawn_square(x,y,ro);
            break;
        case 4://derecha
            spawn_square(x,y,0);
            x += 1;
            spawn_square(x,y,ro);
            break;
    }
}

void spawn_snake(){
    Snake *new = malloc(sizeof(Snake));
    new->x = rand() % (max_x - 8); 
    new->y = rand() % (max_y - 4); 
    new->xx = new->x-1;
    new->yy = new->y-1;
    
    // 1 = arriba, 2 = abajo, 3 = izq, 4 = derecha
    new->dir = 4;
    new->next = NULL;
    
    head = new;
    tail = new;
    
    spawn_snake_part(new->x,new->y,new->xx,new->yy);
    spawn_snake_part(new->x+2,new->y,new->xx+2,new->yy);
    return;
}

void grow_snake(){
    Snake *new = malloc(sizeof(Snake));
    
    // 1 = arriba, 2 = abajo, 3 = izq, 4 = derecha
    switch(tail->dir) {
        case 1:
            new->y = tail->y - 2;
            new->yy = tail->y - 2;
            break;
        case 2:
            new->y = tail->y + 2;
            new->yy = tail->y + 2;
            break;
        case 3:
            new->y = tail->x + 2;
            new->yy = tail->xx + 2;
            break;
        case 4:
            new->y = tail->x - 2;
            new->yy = tail->xx - 2;
            break;
    }
    new->next = NULL;
    tail->next = new;

    tail = new;
}


void spawn_snake_part(unsigned short x, unsigned short y, unsigned short xx, unsigned short yy){
    unsigned int color = 0x00ffff00;
    set_pixel(x, y, color);
    set_pixel(xx, y, color);
    set_pixel(x, yy, color);
    set_pixel(xx, yy, color);
    return;
}


void spawn_square(unsigned int x, unsigned int y, unsigned int color){
    set_pixel(x, y, color);
    set_pixel(x, y+1, color);
    set_pixel(x+1, y, color);
    set_pixel(x+1, y+1, color);
}

void spawn_apple(){
    ax = rand() % (max_x + 1);
    ay = rand() % (max_y + 1);
    
    printf("%d ", ax); 
    printf("%d ", ay);
    
    unsigned int apple_c = 0x0000ff00;
    set_pixel(ax, ay, apple_c);
    set_pixel(ax, ay+1, apple_c);
    set_pixel(ax+1, ay, apple_c);
    set_pixel(ax+1, ay+1, apple_c);
}

void set_pixel(unsigned int x, unsigned int y, unsigned int color) {
    unsigned int *led_base = LED_MATRIX_0_BASE;
    unsigned int *address; 
    unsigned int offset;
    
     offset = x + (24-y) * LED_MATRIX_0_WIDTH;    //PARA QUE EL ORIGEN SEA LA ESQUINA DE ABAJO
    //offset = y + x * LED_MATRIX_0_WIDTH;
    address = led_base + offset;
    *(address) = color;
}

void delay() {
    for (volatile int i = 0; i < 500; i++) {}
}

