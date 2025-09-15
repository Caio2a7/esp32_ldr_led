/*******************************************************************************
 * Projeto: Controle de LEDs com Modos Manual e Automático (Refatorado)
 * Descrição: Este código controla a intensidade de um LED e a frequência de
 * piscada de outro. O código foi refatorado para usar funções separadas
 * para cada modo de operação, melhorando a organização.
 * - Modo Manual: Um potenciômetro controla a intensidade e a frequência.
 * - Modo Automático: Um sensor de luz (LDR) controla a intensidade e a frequência.
 *
 * Placa: ESP32
 *
 * Colaboradores:
 * - Caio Daniel Fonseca de Araújo
 * - João Pedro de França Barboza
 * - Ulisses Folador Damasceno
 *
 * Data: 15/09/2025
 *******************************************************************************/

// --- DEFINIÇÃO DOS PINOS (em inglês) ---
// Mapeamento dos componentes conectados às portas do ESP32.
#define intensityLedPin 0   // Pino para o LED de intensidade (PWM)
#define blinkLedPin 4       // Pino para o LED que pisca (Blink)
#define potentiometerPin 34 // Pino para o sensor analógico do potenciômetro
#define ldrPin 35           // Pino para o sensor analógico de luz (LDR)
#define buttonPin 2         // Pino para o botão de troca de modo

// --- VARIÁVEIS GLOBAIS ---
int operationMode = 0;          // Controla o modo de operação (0 = Manual, 1 = Automático)
int buttonPressState = 0;       // Variável auxiliar para detectar um único toque no botão
unsigned long previousMillis = 0; // Armazena o tempo da última piscada do LED para o controle sem delay()

// --- FUNÇÃO DE SETUP ---
// Executada uma vez quando o ESP32 é ligado ou resetado.
void setup() {
  // Inicializa a comunicação serial com uma taxa de 115200 bps para depuração
  Serial.begin(115200);
  Serial.println("\n\n--- INICIANDO O SISTEMA DE CONTROLE DE LEDS ---");

  // Configuração dos pinos
  pinMode(intensityLedPin, OUTPUT);  // Define o pino do led1 como saída
  pinMode(blinkLedPin, OUTPUT);      // Define o pino do led2 como saída
  pinMode(potentiometerPin, INPUT); // Define o pino do potenciômetro como entrada
  pinMode(ldrPin, INPUT);            // Define o pino do LDR como entrada
  
  // Define o pino do botão como entrada com resistor de pull-up interno.
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.println("Setup concluído. O sistema está no Modo Manual.");
}

// --- FUNÇÃO PARA O MODO MANUAL ---
// Contém toda a lógica de controle via potenciômetro.
void runManualMode() {
  // Leitura do valor analógico do potenciômetro (0 a 4095 no ESP32)
  int potValue = analogRead(potentiometerPin);

  // Controle de intensidade do LED 1
  // Mapeia o valor de 12 bits (0-4095) para 8 bits (0-255) para a função analogWrite
  int ledBrightness = potValue / 16;
  analogWrite(intensityLedPin, ledBrightness);

  // Controle da frequência de piscada do LED 2 (usando millis() para não travar o código)
  unsigned long currentMillis = millis();
  int blinkInterval = (potValue / 4) + 100; // Calcula o intervalo de piscada

  if (currentMillis - previousMillis >= blinkInterval) {
    digitalWrite(blinkLedPin, !digitalRead(blinkLedPin)); // Inverte o estado atual do LED
    previousMillis = currentMillis;                      // Salva o tempo da última piscada
  }

  // Imprime os dados do modo manual no monitor serial
  Serial.print("Mode: Manual | Brightness (0-255): ");
  Serial.print(ledBrightness);
  Serial.print(" | Blink Interval (ms): ");
  Serial.println(blinkInterval);
}

// --- FUNÇÃO PARA O MODO AUTOMÁTICO ---
// Contém toda a lógica de controle via sensor LDR.
void runAutomaticMode() {
  // Leitura do valor analógico do LDR (0 a 4095)
  int ldrValue = analogRead(ldrPin);

  // Controle de intensidade do LED 1
  // Mapeia o valor de 12 bits (0-4095) para 8 bits (0-255)
  int ledBrightness = ldrValue / 16;
  analogWrite(intensityLedPin, ledBrightness);

  // Controle da frequência de piscada do LED 2
  unsigned long currentMillis = millis();
  int blinkInterval = (ldrValue / 4) + 100; // Calcula o intervalo de piscada

  if (currentMillis - previousMillis >= blinkInterval) {
    digitalWrite(blinkLedPin, !digitalRead(blinkLedPin)); // Inverte o estado do LED
    previousMillis = currentMillis;                       // Salva o tempo da última piscada
  }
  
  // Imprime os dados do modo automático no monitor serial
  Serial.print("Mode: Automatic | Brightness (0-255): ");
  Serial.print(ledBrightness);
  Serial.print(" | Blink Interval (ms): ");
  Serial.println(blinkInterval);
}

void loop() {
  // --- LÓGICA DE DETECÇÃO DO BOTÃO ---
  // Verifica se o botão foi pressionado para trocar o modo de operação.
  if (digitalRead(buttonPin) == 1 && buttonPressState == 0) {
    buttonPressState = 1; 
    operationMode++;      
    
    if (operationMode % 2 == 0) {
      Serial.println("\n--- MODO MANUAL ATIVADO ---");
    } else {
      Serial.println("\n--- MODO AUTOMÁTICO ATIVADO ---");
    }
  }
  
  if (digitalRead(buttonPin) == 0 && buttonPressState == 1) {
    buttonPressState = 0; 
  }

  // --- CHAMADA DAS FUNÇÕES DE MODO ---
  // Verifica qual modo está ativo e chama a função correspondente.
  if (operationMode % 2 == 0) {
    runManualMode();
  } else {
    runAutomaticMode();
  }
 
  delay(10);
}