# Trabalho Plataformas de Hardware para IOT
Problema:
○ Desenvolva uma aplicação com uso de microcontroladores
(Arduino ou esp32).
○ Sua aplicação deve ter:
 Entradas:
• 1 potenciômetro.
• 1 LDR (Light Dependent Resistor) ou resistor dependente de luz.
• 1 Botão como entradas.
 Saídas:
• 2 LEDs.
○ O botão controla o modo do MCU:
 Modo manual: O MCU deve iniciar nesse modo, onde ele deverá ler,
via ADC, o valor do potenciômetro.
 Modo automático: O MCU deverá ler, via outro ADC, o valor do LDR.
○ O potenciômetro deverá variar de 0 a 1 V, assim como o LDR.
Atividade prática 1
• Problema:
○ Os valores lidos servirão para acender os LEDs, variando o
funcionamento.
○ Funcionamento dos LEDs:
 LED 1: Deverá acender com intensidade controlada por PWM, onde o
duty cycle deverá ser o valor lido pelo ADC.
 LED 2: Deverá pisca com tempo variando entre 100 ms e 1000 ms,
baseado no valor lido pelo ADC.
○ Lembrando que, o ADC de cada MCU possui resoluções
diferentes e a escala de saída é diferente desses valores.
○ Isso significa que você deverá converter esses valores na faixa a
ser utilizada para cada LED.
○ Dica:
 Existe uma função nas bibliotecas do Arduino que faz essa
conversão entre faixas diferentes (tarefa de pesquisa)

adc=120, duty=7,blink=126ms
