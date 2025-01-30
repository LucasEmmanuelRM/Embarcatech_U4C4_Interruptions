#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "main.pio.h"

#define FREQUENCY 800000 // PIO clock em 8 MHz
#define NUM_PIXELS 25 // Número de pixels da matriz de LEDs 5x5


// Definição dos pinos
#define WS2812 7
#define LED_G 11
#define LED_B 12
#define LED_R 13
#define BUTTON_A 5
#define BUTTON_B 6


// Variáveis globais para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t R = 25; // Intensidade do vermelho
uint8_t G = 0; // Intensidade do verde
uint8_t B = 0; // Intensidade do azul


// Variáveis globais. Não devem ser alteradas manualmente!
static volatile int num = 0; // Altera o número exibido na matriz de LEDS
static volatile uint32_t last_time_contador = 0; // Armazena o tempo do contador (em microssegundos)
static volatile uint32_t last_time_led = 0; // Armazena o tempo do led (em microssegundos)


// Protótipos das funções
void inicializar_pinos();
static void gpio_irq_handler(uint gpio, uint32_t events); // Lida com interrupções
static inline uint32_t definir_cor(uint8_t r, uint8_t g, uint8_t b); // Define a cor dos pixels
static inline void colorir(uint32_t pixel_grb); // Colore os pixels da matriz
void piscar_led(); // Pisca o LED vermelho a cada 5 vezes por segundo
void contador(); // Exibe o numero exibido na matriz de LEDs ws2812
    

int main(){

    printf("Inicializando sistema...\n");
    stdio_init_all();
    inicializar_pinos();
    uint offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, offset, WS2812, 800000);
    contador();
    printf("Sistema inicializado, numero atual = %d\n", num);

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true) {
        piscar_led();
    }

}

void inicializar_pinos(){

    // Inicializa LEDs
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, 0);

    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_B, 0);

    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_put(LED_R, 0);

    // Inicializa botões A e B
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);          // Habilita o pull-up interno

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);          // Habilita o pull-up interno

}


static inline uint32_t definir_cor(uint8_t r, uint8_t g, uint8_t b){

    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b); // Estabelece a palavra de 4 bytes com as cores

}


static inline void colorir(uint32_t pixel_grb){

    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u); // Descarta o byte extra na palavra e a utiliza na máquina de estados para colorir

}


void gpio_irq_handler(uint gpio, uint32_t events){

    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    
    // Verifica se passou tempo suficiente desde o último evento
    if(current_time - last_time_contador > 300000){ // 300 ms de debouncing

        if(gpio == BUTTON_A && num < 9)
            printf("Novo numero = %d\n", ++num);
        if(gpio == BUTTON_B && num > 0)
            printf("Novo numero = %d\n", --num);
        last_time_contador = current_time; // Atualiza o tempo do último evento
        contador();
    }
}


void piscar_led(){

    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se passou 100 ms, alterando o estado do LED caso verdadeiro
    if(current_time - last_time_led > 100000){
        gpio_put(LED_R, !(gpio_get(LED_R)));
        last_time_led = current_time; // Atualiza o tempo do último evento
    }
}


void contador(){
    
    bool numero[10][NUM_PIXELS] = {

    // Número 0
    {0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 1, 1, 0},

    // Número 1
    {0, 0, 1, 0, 0, 
     0, 0, 1, 0, 0, 
     0, 0, 1, 0, 0, 
     0, 0, 1, 0, 0, 
     0, 0, 1, 0, 0},

    // Número 2
    {0, 1, 1, 1, 0, 
     0, 1, 0, 0, 0, 
     0, 1, 1, 1, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0},

    // Número 3
    {0, 1, 1, 1, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0},

    // Número 4
    {0, 1, 0, 0, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 0, 1, 0},

    // Número 5
    {0, 1, 1, 1, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 1, 0, 0, 0, 
     0, 1, 1, 1, 0},

    // Número 6
    {0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 1, 0, 0, 0, 
     0, 1, 1, 1, 0},

    // Número 7
    {0, 1, 0, 0, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 0, 0, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0},

    // Número 8
    {0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 1, 1, 0},

    // Número 9
    {0, 1, 1, 1, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 1, 1, 0},
     
     };

    uint32_t cor = definir_cor(R, G, B);
    for (int i = 0; i < NUM_PIXELS; i++){

        if (numero[num][i])
            colorir(cor); // Liga o LED com um no buffer
        else
            colorir(0);  // Desliga os LEDs com zero no buffer
    }    
}