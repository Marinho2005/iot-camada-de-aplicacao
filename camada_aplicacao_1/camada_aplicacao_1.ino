#include <DHT.h>
#include <DHT_U.h>
#include "thingProperties.h"

// --- Mapeamento de Pinos  ---
#define POT_PIN 32
#define LDR_PIN 34
#define BUTTON_PIN 33
#define DHTPIN 14
#define BUZZER_PIN 4
#define PIN_R 25
#define PIN_G 26
#define PIN_B 27
#define LED_PIN 17

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// --- Variáveis ---
volatile bool led_enabled = true;
volatile bool temp_enabled = true;
volatile unsigned long last_interrupt_time = 0;

String corEscolhida = "";

float temperature = 0.0;
int pot_value = 0;

bool temp_warning = false;
int analogValue;

// --- Canais PWM antigos ---
#define CH_R 0
#define CH_G 1
#define CH_B 2

// --- Interrupção ---
void IRAM_ATTR handleButtonInterrupt() {
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) {
    led_enabled = !led_enabled;
    estadoLed = led_enabled;
    last_interrupt_time = interrupt_time;
  }
}

// --- Funções PWM ---
void setRGB(uint16_t r, uint16_t g, uint16_t b) {
  ledcWrite(CH_R, r);
  ledcWrite(CH_G, g);
  ledcWrite(CH_B, b);
}

// --- Cores do potenciômetro (VERMELHO GARANTIDO) ---
void setLedColorFromPot(int value) {
  int r=0, g=0, b=0;

  if (value < 820) { 
    r = 4095;              // VERMELHO FORTE
    g = map(value, 0, 819, 0, 2650);
    b = 0;
  } 
  else if (value < 1640) { 
    r = 4095;
    g = map(value, 820, 1639, 2650, 4095);
    b = 0;
  } 
  else if (value < 2460) { 
    r = 4095;
    g = 4095;
    b = map(value, 1640, 2459, 0, 4095);
  } 
  else if (value < 3280) { 
    r = map(value, 2460, 3279, 4095, 0);
    g = map(value, 2460, 3279, 4095, 3200);
    b = 4095;
  } 
  else { 
    r = 0;
    g = map(value, 3280, 4095, 3200, 0);
    b = 4095;
  }

  setRGB(r,g,b);
}

// --- Sensores ---
void readSensors() {
  if(temp_enabled){
  temperature = dht.readTemperature();
  temp_warning = (temperature < 0 || temperature > 26);

  analogValue = analogRead(LDR_PIN);
  luminosidade = analogValue;
  temperatura = temperature;
  pot_value = analogRead(POT_PIN);
  }else{
  luminosidade = 0;
  temperatura = 0;
  pot_value = analogRead(POT_PIN);
  }
}

// ✅ RGB nunca é apagado por alertas
// ✅ Só desliga quando led_enabled = false
void updateLed() {

  if (!led_enabled) {
    setRGB(0,0,0);       // APAGAR RGB TOTALMENTE
    digitalWrite(LED_PIN, LOW);
    return;
  }

  // RGB SEMPRE segue o potenciômetro
  setLedColorFromPot(pot_value);

  // LED de alerta (separado do RGB)
  if (temp_warning || analogValue > 3000) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void updateBuzzer() {
  if (temp_warning) tone(BUZZER_PIN, 2000);
  else noTone(BUZZER_PIN);
}

// --- Setup ---
void setup() {
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  Serial.begin(115200);
  dht.begin();

  pinMode(LDR_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // PWM antigo perfeito
  ledcSetup(CH_R, 5000, 12);
  ledcSetup(CH_G, 5000, 12);
  ledcSetup(CH_B, 5000, 12);

  ledcAttachPin(PIN_R, CH_R);
  ledcAttachPin(PIN_G, CH_G);
  ledcAttachPin(PIN_B, CH_B);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);
}

// --- Loop ---
void loop() {
  ArduinoCloud.update();

  readSensors();
  updateBuzzer();
  updateLed();

  // comando remoto
  if (corEscolhida != "") {
    if (corEscolhida == "vermelho") setRGB(4095,0,0);
    if (corEscolhida == "amarelo")  setRGB(4095,4095,0);
    if (corEscolhida == "azul")     setRGB(0,0,4095);

    delay(1000);
    corEscolhida = "";
  }

  delay(50);
}

// --- Cloud Callbacks ---
void onEstadoLedChange() { led_enabled = estadoLed; }
void onCorLedChange()    { corEscolhida = corLed; }
void onEstadoTempLdrChange() { temp_enabled = estadoTempLdr; }
