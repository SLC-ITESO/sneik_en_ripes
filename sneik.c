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
int check_collision();

void toda_la_pantalla_de_un_color(unsigned int color);
void ayanami_rei();

volatile unsigned int *d_pad_u = D_PAD_0_UP;
volatile unsigned int *d_pad_d = D_PAD_0_DOWN;
volatile unsigned int *d_pad_l = D_PAD_0_LEFT;
volatile unsigned int *d_pad_r = D_PAD_0_RIGHT;
volatile unsigned int *switch_base = SWITCHES_0_BASE;

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
int snake_size = 2; // tama?o de la serpiente

// colores:
unsigned int ro = 0x00ff0000;
unsigned int ve = 0x0000ff00;

void main() {
    // Inicializar el juego
    spawn_snake();
    spawn_apple(ve);

    while (1) {
        // Generar la serpiente
        generate_snake();

        // Control del DPAD
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

        // Movimiento de la serpiente
        move_snake();
        if (check_collision() || *switch_base == 1) {
            pantalla_color(0x00abeeff);

            while (*switch_base != 1) {}
            while (*switch_base != 0) {}

            ayanami_rei();
        }
        come_apple();
        delay();
    }
}

void spawn_snake() {
    if (snake_size < MAX_SNAKE_PARTS) {
        SnakePart *head = &snake[0];
        snake[0].x = rand() % 10;
        snake[0].y = rand() % (max_y - 1) + 1;
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

void ayanami_rei(){

    snake_size = 2;
    spawn_snake();
    pantalla_color(0x00000000); 
    spawn_apple(ve);
}

void pantalla_color(unsigned int color) {
    for (unsigned int y = 0; y <= max_y; y++) {
        for (unsigned int x = 0; x <= max_x; x++) {
            set_pixel(x, y, color);
        }
    }
}
void generate_snake() {
    for (int i = 0; i < snake_size; i++) {
        spawn_snake_part(snake[i].x, snake[i].y, snake[i].xx, snake[i].yy, ro);
    }
}

int check_collision() {
    // Verifica si la cabeza está fuera de los límites
    SnakePart *head = &snake[0];
    if (head->x > max_x || head->y > max_y || head->x < 0 || head->y < 0 ||
        head->xx > max_x || head->yy > max_y || head->xx < 0 || head->yy < 0) {
        return 1; // Colisión con un borde
    }

    // Verifica si la cabeza choca con el cuerpo
    for (int i = 1; i < snake_size; i++) {
        if ((head->x == snake[i].x && head->y == snake[i].y) || 
            (head->xx == snake[i].xx && head->yy == snake[i].yy)) {
            return 1; // Colisión consigo misma
        }
    }

    return 0; // No hay colisión
}

void despawn_apple(){
    set_pixel(ax, ay, 0);
    set_pixel(axx, ay, 0);
    set_pixel(ax, ayy, 0);
    set_pixel(axx, ayy, 0);
}
void spawn_apple(unsigned int color_m) {
int pos_valida = 0;

    while (!pos_valida) {
        ax = rand() % (max_x - 1) + 1;
        ay = rand() % (max_y - 1) + 1;
        axx = ax - 1;
        ayy = ay - 1;

        pos_valida = 1;

        // Verifica que las coordenadas no son las de la serpiente y su cuerpo
        for (int i = 0; i < snake_size; i++) {
            if (
                (snake[i].x == ax && snake[i].y == ay) ||
                (snake[i].x == axx && snake[i].y == ay) ||
                (snake[i].x == ax && snake[i].y == ayy) ||
                (snake[i].x == axx && snake[i].y == ayy) ||
                (snake[i].xx == ax && snake[i].yy == ay) ||
                (snake[i].xx == axx && snake[i].yy == ay) ||
                (snake[i].xx == ax && snake[i].yy == ayy) ||
                (snake[i].xx == axx && snake[i].yy == ayy)
            ) {
                pos_valida = 0;
                break;
            }
        }
    }

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
    unsigned int offset = x + (max_y - y) * LED_MATRIX_0_WIDTH;
    unsigned int *address = led_base + offset;
    *address = color;
}

void delay() {
    for (volatile int i = 0; i < 1000; i++) {}
}