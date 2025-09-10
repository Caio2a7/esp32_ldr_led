/* 
  |---------------------------------------|
  | Alunos:                               |
  |  - Caio Daniel Fonseca de Araújo      |
  |  - João Pedro                         |
  |---------------------------------------|
*/

// CONSTANTES da numeração de cada pino
#define PIN_POT 33
#define PIN_POT_LDR 34
#define PIN_BTN 26
#define PIN_LED_1 18
#define PIN_LED_2 17

// CONSTANTES de estado
#define MANUAL 0
#define AUTOMATIC 1

//CONSTANTES ADC
#define ADC_INPUT_MIN 0
#define ADC_INPUT_MAX 1240

//CONSTANTES LED
#define LED_BRIGHTNESS_MIN 0
#define LED_BRIGHTNESS_MAX 255
#define BLINK_DELAY_MIN 100  // 100ms
#define BLINK_DELAY_MAX 1000 //1000ms

// CONSTANTES PWM
#define PWM_LED_1_CHANNEL 0
#define PWM_FREQ 5000 //5kHz
#define PWM_RESOLUTION 8 // 2^8 (0 - 255) faixa de brilho que leds operam 

// VARS BOTÃO
int lastBtnState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// VARS ESTADO do CIRCUITO
unsigned int modeState = MANUAL;
int led2State = LOW;
unsigned long lastBlinkTime = 0;

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);

  pinMode(PIN_BTN, INPUT_PULLDOWN);

  ledcAttach(PWM_LED_1_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
}

void manualMode(){
  // (ADC) - Leitura dos dados analógicos no PIN_POT
  int adc = analogRead(PIN_POT);

  // (ADC -> PWM) - Conversão da faixa analógica do Potenciômetro para PWM do pro LED 
  // "potenciômetro deverá variar de 0 a 1 V" para tal fim representaremos 
    // esta faixa passando o current_top 1240, se estiver fora da faixa não
    // afeta o brilho

  // valor_a_mapear, current_bottom, current_top, to_bottom, to_top 
  int pwm = map(adc, ADC_INPUT_MIN, ADC_INPUT_MAX, LED_BRIGHTNESS_MIN, LED_BRIGHTNESS_MAX);
  pwm = constrain(pwm, LED_BRIGHTNESS_MIN, LED_BRIGHTNESS_MAX);
  
  // (PWM) - Modulação de sinais digitais para analógicos
  // Envio de sinal para o LED via PWM
  // Passaremos o duty cicle como sendo o valor lido do adc
  ledcWrite(PWM_LED_1_CHANNEL, pwm);

  Serial.printf("\nADC: %d -- DUTY: %d -- BLINK: %d", adc, pwm, pwm);
  
  int adcConstrained = constrain(adc, ADC_INPUT_MIN, ADC_INPUT_MAX);
  // Além de mapear a faixa do adc para o do blinkness como também de 0v a 1v
  int blinkDelay = map(adcConstrained, ADC_INPUT_MIN, ADC_INPUT_MAX, BLINK_DELAY_MIN, BLINK_DELAY_MAX);
  blinkDelay = constrain(blinkDelay, BLINK_DELAY_MIN, BLINK_DELAY_MAX);
  blinkDelay = constrain(blinkDelay, BLINK_DELAY_MIN, BLINK_DELAY_MAX);

  if(millis() - lastBlinkTime >= blinkDelay){
    lastBlinkTime = millis();
    led2State = !led2State;
    digitalWrite(PIN_LED_2, led2State);
  }
}

void automaticMode(){
  // LDR - ADC
  int adc = analogRead(PIN_POT_LDR); 

  // LED 1
  int pwm = map(adc, ADC_INPUT_MIN, ADC_INPUT_MAX, LED_BRIGHTNESS_MIN, LED_BRIGHTNESS_MAX);
  pwm = constrain(pwm, LED_BRIGHTNESS_MIN, LED_BRIGHTNESS_MAX);
  ledcWrite(PWM_LED_1_CHANNEL, pwm);

  // LED 2
  // Limitar a faixa 0v a 1v
  int adcConstrained = constrain(adc, ADC_INPUT_MIN, ADC_INPUT_MAX);
  // Além de mapear a faixa do adc para o do blinkness como também de 0v a 1v
  int blinkDelay = map(adcConstrained, ADC_INPUT_MIN, ADC_INPUT_MAX, BLINK_DELAY_MIN, BLINK_DELAY_MAX);
  blinkDelay = constrain(blinkDelay, BLINK_DELAY_MIN, BLINK_DELAY_MAX);
  blinkDelay = constrain(blinkDelay, BLINK_DELAY_MIN, BLINK_DELAY_MAX);

  if(millis() - lastBlinkTime >= blinkDelay){
    lastBlinkTime = millis();
    led2State = !led2State;
    digitalWrite(PIN_LED_2, led2State);
  }
}

void changeMode(){
  modeState = !modeState;
    Serial.println("\nINFO: Estado do circuito alterado");
}

void loop() {
  int btnState = digitalRead(PIN_BTN);

  if (btnState != lastBtnState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (btnState == HIGH && lastBtnState == LOW) { 
      changeMode();
    }
  }
  lastBtnState = btnState;

  if(modeState == MANUAL){
    manualMode();
  } else if(modeState == AUTOMATIC){
    automaticMode();
  }else{
    Serial.println("\nERRO: problemas na configuração do modo do circuito");
  }
}
