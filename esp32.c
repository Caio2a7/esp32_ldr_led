#include <Arduino.h>

#define PIN_POT 34
#define PIN_LDR 33
#define PIN_LED_LIGADO 21
#define PIN_LED_PISCA 19

#define PIN_CHANGE_BTN 25

// Configurações para o PWM (ledc) no ESP32
const int ledChannel = 0;
const int freq = 5000;
const int resolution = 8;

unsigned long tempoAnterior = 0;
int tempoControle = 500;
bool estaAceso = false;
bool ultimoEstadoBotao = true;

void setup() {
  Serial.begin(115200);

  // Configura o PWM para o PIN_LED_LIGADO
  pinMode(PIN_LED_LIGADO, OUTPUT);

  pinMode(PIN_LED_PISCA, OUTPUT);

  pinMode(PIN_CHANGE_BTN, INPUT_PULLUP);
}

void manualMode() {
  int potValue = analogRead(PIN_POT);

  // controla o brilho do LED ligado
  int brilho = map(potValue, 0, 4095, 0, 255); // Mapeamento de brilho
  analogWrite(PIN_LED_LIGADO, brilho); // Usando a função correta para o ESP32

  // controla o tempo de piscada do LED
  tempoControle = map(potValue, 0, 4095, 100, 1000); // Mapeamento de tempo de blink entre 100ms e 1000ms

  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoAnterior >= tempoControle) {
    tempoAnterior = tempoAtual;
    estaAceso = !estaAceso; // inverte o estado

    digitalWrite(PIN_LED_PISCA, estaAceso); // Não precisa do ternário, o booleano já funciona
  }

  // Imprimindo o log da forma correta e recomendada para ESP32
  Serial.printf("Manual Mode | Pot: %4d | Brilho: %3d | Tempo Blink: %4d ms\n", potValue, brilho, tempoControle);

}

void automaticMode() {
  int potValue = analogRead(PIN_LDR);

  // controla o brilho do LED ligado
  int brilho = map(potValue, 0, 4095, 0, 255); // Mapeamento de brilho
  analogWrite(PIN_LED_LIGADO, brilho); // Usando a função correta para o ESP32

  // controla o tempo de piscada do LED
  tempoControle = map(potValue, 0, 4095, 100, 1000); // Mapeamento de tempo de blink entre 100ms e 1000ms

  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoAnterior >= tempoControle) {
    tempoAnterior = tempoAtual;
    estaAceso = !estaAceso; // inverte o estado

    digitalWrite(PIN_LED_PISCA, estaAceso); // Não precisa do ternário, o booleano já funciona
  }

  // Imprimindo o log da forma correta e recomendada para ESP32
  Serial.printf("Automatic Mode | Pot: %4d | Brilho: %3d | Tempo Blink: %4d ms\n", potValue, brilho, tempoControle);

}

void loop() {
  bool mode = analogRead(PIN_CHANGE_BTN);

  if (mode == LOW) {
    ultimoEstadoBotao = !ultimoEstadoBotao;
  } 

  if (ultimoEstadoBotao) {
    manualMode();
  } else {
    automaticMode();
  }

  delay(100);
}
