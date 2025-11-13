
✅ **Atuadores**
- **LED RGB** controlado por PWM, com cores suaves baseadas na posição do potenciômetro  
- **LED indicador** de alerta para temperatura/luminosidade fora do padrão  
- **Buzzer** sonoro para aviso de temperatura fora da faixa ideal  

✅ **Integração com Arduino IoT Cloud**
- Envia leituras de temperatura e luminosidade para o dashboard online  
- Permite **controle remoto** do LED RGB e do sistema de sensores  

✅ **Interrupção por botão físico**
- Pressionar o botão alterna o estado geral do LED (liga/desliga RGB)

---

## ⚙️ Hardware Utilizado

| Componente | Função | Pino |
|-------------|--------|------|
| ESP32 | Microcontrolador principal | — |
| DHT11 | Sensor de temperatura | 14 |
| LDR | Sensor de luminosidade | 34 |
| Potenciômetro | Controle de cor RGB | 32 |
| Botão | Liga/desliga LEDs | 33 |
| Buzzer | Alerta sonoro | 4 |
| LED RGB (R, G, B) | Saídas PWM | 25, 26, 27 |
| LED indicador | Alerta visual | 17 |

---

## 🧩 Lógica de Funcionamento

1. **Leitura dos sensores** é feita periodicamente.  
2. Se a **temperatura for < 0°C ou > 26°C**, o buzzer é acionado e o LED de alerta acende.  
3. A **cor do LED RGB** é determinada pelo valor do potenciômetro, com uma transição suave entre as cores.  
4. O **botão físico** alterna o estado geral do LED RGB (ligado/desligado).  
5. Dados são enviados e recebidos via **Arduino IoT Cloud**, permitindo controle remoto.

---

## 🌐 Integração com a Nuvem

O projeto usa a biblioteca **ArduinoIoTCloud**.  
As variáveis principais sincronizadas com a nuvem são:

- `temperatura` → leitura do DHT11  
- `luminosidade` → leitura do LDR  
- `estadoLed` → estado atual do LED RGB  
- `corLed` → cor selecionada via dashboard  
- `estadoTempLdr` → habilita/desabilita leitura dos sensores  

---

## 🖼️ Exemplo de Interface (Dashboard Arduino Cloud)

O dashboard exibe:
- Gauge de **temperatura**
- Gauge de **luminosidade**
- Switch de **LED principal**
- Botão de **habilitar/desabilitar sensores**
- Controle de **cor do LED (vermelho, amarelo, azul)**

---
## 🖥️ Dashboard Arduino IoT Cloud


![Dashboard Arduino IoT Cloud](images/dashboard.png)



## 🧰 Bibliotecas Necessárias

```cpp
#include <DHT.h>
#include <DHT_U.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>





