Este projeto trata da atividade proposta no capítulo 4 do curso de sistemas embarcados do Embarcatech - CEPEDI. Seu propósito é demonstrar a prática de rotinas interrupções utilizando o microcontrolador Raspberry Pi Pico W e a placa de desenvolvimento BitDogLab.

A proposta da atividade é fazer com que um LED vermelho pisque 5 vezes por segundo enquanto a matriz de LEDs ws2812 exibe um número de 0 a 9. A rotina de interrupção é implementada para que, sempre que os botões A ou B da BitDogLab forem pressionados, a contagem seja incrementada ou decrementada, respectivamente.

As funções são:
- **_inicializar_pinos_**, responsável por inicializar os pinos utilizados;
- **_gpio_irq_handler_**, função de solicitação de interrupção, onde dado o pressionamento dos botões, ocorre a mudança do número exibido na matriz;
- **_definir_cor_**, responsável por receber os valores da intensidade RGB, por sua vez definidos globalmente e manualmente, e definir a palavra em binário que armazena esses valores;
- **_colorir_**, função que irá colorir, segundo a palavra em binário recebida, cada pixel da matriz, desde que estejam definidos para serem acesos;
- **_piscar_led_**, responsável por fazer o led vermelho da BitDogLab piscar 5 vezes por segundo sem impedir que a interação com a matriz ws2812 ocorra;
- **_contador_**, função que armazena vetores que representam o número a ser exibido ao determinar quais pixels da matriz devem ser acionados e, dada a mudança no número a ser exibido, seleciona o vetor correto.

Acrescenta-se apenas que a função de interrupção possui um debouncing via software para evitar que o incremento ou decremento ocorra mais de uma vez por pressionamento devido ao efeito de bouncing.
Dado o pressionamento de um botão, aguarda-se ao menos 0,3 segundos para permitir que uma nova mudança possa ocorrer na matriz, evitando que múltiplos sinais sejam lidos ao considerar apenas o primeiro, desde que os demais ocorram nesse intervalo.

Lógica similar foi utilizada na função de piscar o led para evitar o uso da função sleep_ms, que impediria o uso do sistema enquanto o mesmo estivesse adormecido.

Assim como exigido, um vídeo demonstrativo pode ser acessado em:
[https://drive.google.com/file/d/1bx_Zhot61iC-B0HQ7Cc_j9-7sXm3bD68/view?usp=sharing]
