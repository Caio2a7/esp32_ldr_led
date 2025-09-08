// --- Mapeamento de Pinos ---
// LED cuja intensidade será controlada por PWM
#define LED_PWM       26  // LED 1 (Pode ser o azul)

// LED que irá piscar com frequência variável
#define LED_PISCA     27  // LED 2 (Pode ser o verde)

// --- Entradas ---
#define POTENCIOMETRO 36  // Potenciômetro para modo manual
#define LDR           39  // LDR para modo automático
#define BOTAO         35  // Botão para alternar entre os modos

// --- Variáveis de Controle ---
// Controla o modo de operação. Inicia em 'true' (Manual), como solicitado.
bool modoManual = true; 

// Variáveis para o controle do botão (evita múltiplas trocas de modo)
bool ultimoEstadoBotao = LOW;

// Variáveis para o controle de tempo do LED_PISCA (não bloqueante)
unsigned long previousMillis = 0;
bool estadoLedPisca = LOW;

void setup() {
  Serial.begin(115200);

  // Configura os pinos dos LEDs como saída
  pinMode(LED_PWM, OUTPUT);
  pinMode(LED_PISCA, OUTPUT);

  // Configura o pino do botão como entrada
  pinMode(BOTAO, INPUT);

  Serial.println("MCU iniciado em MODO MANUAL.");
}

void loop() {
  // --- 1. Leitura e Troca de Modo via Botão ---
  bool estadoAtualBotao = digitalRead(BOTAO);

  // Verifica se o botão foi pressionado (transição de LOW para HIGH)
  if (estadoAtualBotao != ultimoEstadoBotao) {
    if (estadoAtualBotao == HIGH) {
      // Inverte o modo de operação
      modoManual = !modoManual;
      
      if (modoManual) {
        Serial.println("--> Modo MANUAL ativado.");
      } else {
        Serial.println("--> Modo AUTOMATICO ativado.");
      }
    }
    // Um pequeno delay para debounce do botão
    delay(50); 
  }
  ultimoEstadoBotao = estadoAtualBotao;


  // --- 2. Leitura do Sensor Ativo (Potenciômetro ou LDR) ---
  int valorADC; // Variável para armazenar o valor lido

  if (modoManual) {
    // === MODO MANUAL ===
    valorADC = analogRead(POTENCIOMETRO); // Lê o potenciômetro (0–4095)
  } else {
    // === MODO AUTOMÁTICO ===
    valorADC = analogRead(LDR); // Lê o LDR (0–4095)
  }

  // --- 3. Conversão dos Valores (Mapeamento) ---
  
  // Converte o valor do ADC (0-4095) para a escala do PWM (0-255) para o LED 1
  // A função map() faz exatamente o que a dica da atividade pedia.
  int dutyCyclePWM = map(valorADC, 0, 4095, 0, 255);

  // Converte o valor do ADC (0-4095) para o intervalo de pisca (1000-100 ms) para o LED 2
  // Note que 4095 está mapeado para 100ms (pisca rápido) e 0 para 1000ms (pisca lento)
  unsigned long intervaloPisca = map(valorADC, 0, 4095, 1000, 100);

  
  // --- 4. Controle das Saídas (LEDs) ---

  // LED 1: Controle de Intensidade
  // Acende o LED 1 com o brilho calculado
  analogWrite(LED_PWM, dutyCyclePWM);

  // LED 2: Controle de Pisca (usando millis() para não travar o código)
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= intervaloPisca) {
    previousMillis = currentMillis;   // Salva o tempo atual
    estadoLedPisca = !estadoLedPisca; // Inverte o estado do LED
    digitalWrite(LED_PISCA, estadoLedPisca);
  }

  // --- 5. Monitoramento via Serial (Opcional, mas útil para debug) ---
  if (modoManual) {
      Serial.printf("MANUAL | ADC Pot: %d | PWM: %d | Intervalo: %lu ms\n",
                    valorADC, dutyCyclePWM, intervaloPisca);
  } else {
      Serial.printf("AUTO   | ADC LDR: %d | PWM: %d | Intervalo: %lu ms\n",
                    valorADC, dutyCyclePWM, intervaloPisca);
  }
}