# 💧 Sentinela_SP - Sistema de Monitoramento e Alerta de Inundação

## 📋 Descrição do Sistema

Este projeto consiste em um **sistema embarcado** para monitorar o nível da água e alertar sobre potenciais inundações. Utilizando um sensor de água conectado a um **ESP32**, o sistema indica o nível atual através de **LEDs** e também disponibiliza essa informação através do **IP** acessível via **Wi-Fi local**.

> 💡 Ideal para áreas propensas a enchentes, fornecendo alerta

---

## ✅ Funcionalidades

- **📈 Monitoramento Contínuo**: Leitura do nível da água com sensor analógico.
- **🔦 Alertas Visuais Locais via LEDs**:
  - **LED Azul**: Sistema operacional.
  - **LED Branco**: Status da conexão Wi-Fi (piscando = conectando, aceso = conectado).
  - **LED Verde**: Nível seguro.
  - **LED Amarelo**: Nível de alerta.
  - **LED Vermelho**: Inundação iminente.
- **🌐 Interface Web Remota**:
  - Servidor web no ESP32 acessível via navegador na mesma rede Wi-Fi.
  - Endpoint `/dados` retorna:
    - Valor bruto do sensor.
    - Estado atual em JSON: `"Seco"`, `"Normal"`, `"Perigo"`, `"Inundacao"`.

---

## 🧰 Componentes Necessários

- 1x Placa **ESP32**
- 1x **Sensor de água analógico**
- 5x LEDs: Azul, Verde, Amarelo, Vermelho, Branco
- 5x Resistores
- Fios de conexão Jumpers
- Fonte de alimentação para o ESP32 (USB)
- Rede Wi-Fi

---

## 🔌 Diagrama de Conexão (Texto)

| Componente               | Pino ESP32              |
| ------------------------ | ----------------------- |
| Sensor de Água           | Pino analógico 34 (ADC) |
| LED Azul (Operação)      | 23                      |
| LED Verde (Normal)       | 26                      |
| LED Amarelo (Perigo)     | 33                      |
| LED Vermelho (Inundação) | 21                      |
| LED Branco (Wi-Fi)       | 22                      |

> ⚠️ **Importante**: LEDs devem ser conectados corretamente:  
> **Ânodo (positivo)** → Resistor → Pino do ESP32  
> **Cátodo (negativo)** → GND

---

## 🧠 Estrutura do Código

- **Configurações iniciais**:

  - Credenciais da rede Wi-Fi.
  - Pinos utilizados.
  - Limiares de nível da água.

- **Função `setup()`**:

  - Inicia a comunicação serial.
  - Configura pinos.
  - Testa LEDs.
  - Conecta ao Wi-Fi.
  - Inicia o servidor web.

- **Função `loop()`**:
  - Verifica conexão Wi-Fi.
  - Lê o sensor periodicamente.
  - Atualiza LEDs.
  - Mantém o servidor web.

### 🔧 Funções Auxiliares

| Função                  | Finalidade                              |
| ----------------------- | --------------------------------------- |
| `setup_wifi()`          | Conecta à rede Wi-Fi                    |
| `getEstadoAguaString()` | Converte leitura em texto ("Seco", etc) |
| `handleDadosSensor()`   | Responde a `/dados` com JSON            |
| `atualizarLedsNivel()`  | Atualiza LEDs conforme o nível          |
| `desligarLedsNivel()`   | Desliga todos os LEDs de nível          |

---

## 📦 Bibliotecas Utilizadas

- `WiFi.h` — Conexão com rede Wi-Fi
- `WebServer.h` — Servidor web embutido
- `ArduinoJson.h` — Formatação de resposta JSON

---

## 🚀 Como Usar

1. **Clone o repositório**:
   ```bash
   git clone https://github.com/saraywebster/Sentinela_SP.git
   ```
