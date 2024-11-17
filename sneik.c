#include "ripes_system.h"
#include <stdio.h> 
#include <stdlib.h> 

void set_pixel(unsigned int x, unsigned int y, unsigned int color);
void delay();
void spawn_apple();
void despawn_apple();
void come_apple();

void spawn_snake_part(unsigned short x, unsigned short y, unsigned short xx, unsigned short yy, unsigned int color);
void spawn_snake();
void grow_snake();
void move_snake();
void generate_snake();

volatile unsigned int *d_pad_u = D_PAD_0_UP;
volatile unsigned int *d_pad_d = D_PAD_0_DOWN;
volatile unsigned int *d_pad_l = D_PAD_0_LEFT;
volatile unsigned int *d_pad_r = D_PAD_0_RIGHT;

// cosos del tablero
unsigned short max_x = LED_MATRIX_0_WIDTH - 1;
unsigned short max_y = LED_MATRIX_0_HEIGHT - 1;

//cosos del manzano
unsigned int ax = 0;
unsigned int ay = 0;
unsigned int axx = 0;
unsigned int ayy = 0;
// Culebra
#define MAX_SNAKE_PARTS 100

typedef struct {
    unsigned short x, xx;
    unsigned short y, yy;
    unsigned short dir;
} SnakePart;

SnakePart snake[MAX_SNAKE_PARTS];
int snake_size = 2; // tamaño de la serpiente

// colores:
unsigned int ro = 0x00ff0000;
unsigned int ve = 0x0000ff00;

void main() {
    unsigned int mov = 4; // 1 = , 2 = down, 3 = left, 4 = right
    
    // spawnea todo
    spawn_apple(ve);
    spawn_snake();
    
    while (1) {
        //spawn_apple(ve);
        generate_snake();
        
        // DPAD
        if (*d_pad_u == 1 && snake[0].dir != 2) {
            snake[0].dir = 1;
        }
        if (*d_pad_d == 1 && snake[0].dir != 1) {
            snake[0].dir = 2;
        }
        if (*d_pad_l == 1 && snake[0].dir != 4) {
            snake[0].dir = 3;
        }
        if (*d_pad_r == 1 && snake[0].dir != 3) {
            snake[0].dir = 4;
        }

        //Movimiento de la culebra
        move_snake();
        come_apple();
        delay();
    }
}

void spawn_snake() {
    if (snake_size < MAX_SNAKE_PARTS) {
        SnakePart *head = &snake[0];
        snake[0].x = rand() % (max_x - 8);
        snake[0].y = rand() % (max_y - 4);
        snake[0].xx = snake[0].x - 1;
        snake[0].yy = snake[0].y - 1;
        snake[0].dir = 4; // Comienza moviendose a la derecha
        snake_size = 2;
    }
}
void crece_snake(){
    snake_size+=2;
    return;
}
void come_apple(){
if ((snake[0].x == ax && snake[0].y == ay) ||
    (snake[0].x == axx && snake[0].y == ay) ||
    (snake[0].x == ax && snake[0].y == ayy) ||
    (snake[0].x == axx && snake[0].y == ayy) ||
    (snake[0].xx == ax && snake[0].yy == ay) ||
    (snake[0].xx == axx && snake[0].yy == ay) ||
    (snake[0].xx == ax && snake[0].yy == ayy) ||
    (snake[0].xx == axx && snake[0].yy == ayy)) {
         despawn_apple();
         crece_snake();
         spawn_apple(ve);
     }
     else{
         return;
     }
}

void move_snake() {
    if (snake_size <= 0) return;

    // Se elimina la cola para que se vea movimiento
    SnakePart *tail = &snake[snake_size - 1];
    spawn_snake_part(tail->x, tail->y, tail->xx, tail->yy, 0x00000000);

    // Actualiza las posiciones de las partes, desde la cola
    for (int i = snake_size - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Mueve la cabeza { snake[0] es la cabeza}
    SnakePart *head = &snake[0];
    switch (head->dir) {
        case 1: // Up
            head->y++;
            head->yy++;
            break;
        case 2: // Down
            head->y--;
            head->yy--;
            break;
        case 3: // Left
            head->x--;
            head->xx--;
            break;
        case 4: // Right
            head->x++;
            head->xx++;
            break;
    }
    //Dibuja la nueva cabeza
    spawn_snake_part(head->x, head->y, head->xx, head->yy, ro);
}

void generate_snake() {
    for (int i = 0; i < snake_size; i++) {
        spawn_snake_part(snake[i].x, snake[i].y, snake[i].xx, snake[i].yy, ro);
    }
}
void despawn_apple(){
    set_pixel(ax, ay, 0);
    set_pixel(axx, ay, 0);
    set_pixel(ax, ayy, 0);
    set_pixel(axx, ayy, 0);
}
void spawn_apple(unsigned int color_m) {
    ax = rand() % (max_x + 1);
    ay = rand() % (max_y + 1);
    axx = ax - 1;
    ayy = ay -1;
    
    set_pixel(ax, ay, color_m);
    set_pixel(axx, ay, color_m);
    set_pixel(ax, ayy, color_m);
    set_pixel(axx, ayy, color_m);
}

void spawn_snake_part(unsigned short x, unsigned short y, unsigned short xx, unsigned short yy, unsigned int color) {
    set_pixel(x, y, color);
    set_pixel(xx, y, color);
    set_pixel(x, yy, color);
    set_pixel(xx, yy, color);
}

void set_pixel(unsigned int x, unsigned int y, unsigned int color) {
    if (x > max_x || y > max_y) return;

    unsigned int *led_base = LED_MATRIX_0_BASE;
    unsigned int offset = x + (24 - y) * LED_MATRIX_0_WIDTH;
    unsigned int *address = led_base + offset;
    *address = color;
}

void delay() {
    for (volatile int i = 0; i < 1100; i++) {}
}