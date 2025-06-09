# 💧 Sentinela_SP - Sistema de Monitoramento e Alerta de Inundação

---

## 📋 Descrição do Sistema

Este projeto consiste em um sistema embarcado para monitorar o nível da água e alertar sobre potenciais inundações. Utilizando um sensor de água conectado a um ESP32, o sistema indica o nível atual através de LEDs e também disponibiliza essa informação através de um servidor web acessível via Wi-Fi local.

---

## ✅ Funcionalidades

### 📈 Monitoramento Contínuo

Leitura do nível da água com sensor analógico em tempo real.

### 🔦 Alertas Visuais Locais via LEDs

- **LED Azul**: Sistema operacional.
- **LED Branco**: Status da conexão Wi-Fi _(piscando = conectando, aceso = conectado)_.
- **LED Verde**: Nível da água seguro.
- **LED Amarelo**: Nível de perigo, risco de inundação.
- **LED Vermelho**: Nível crítico, inundação iminente.

### 🌐 Interface Web Remota

- Servidor web no ESP32 acessível via navegador na mesma rede Wi-Fi.
- Endpoint `/dados` retorna o estado atual em formato JSON:
  - Valor bruto do sensor.
  - Estado do nível: `"Seco"`, `"Normal"`, `"Perigo"`, `"Inundacao"`.

---

## 🧰 Componentes Necessários

| Componente                        | Quantidade | Observação                              |
| --------------------------------- | ---------- | --------------------------------------- |
| ESP32 DevKit v1                   | 1          | Placa principal de controle             |
| Sensor de nível de água analógico | 1          | Saída analógica                         |
| Protoboard (Placa de ensaio)      | 1          | Para montagem dos componentes           |
| LED Azul                          | 1          | Indica que o sistema está operacional   |
| LED Branco                        | 1          | Indica status da conexão Wi-Fi          |
| LED Verde                         | 1          | Indica nível seguro                     |
| LED Amarelo                       | 1          | Indica risco intermediário              |
| LED Vermelho                      | 1          | Indica risco crítico de inundação       |
| Resistores                        | 5 ou mais  | Um para cada LED                        |
| Jumpers macho-macho               | 10+        | Conexões entre ESP32, protoboard e LEDs |
| Fonte de alimentação para ESP32   | 1          | Cabo Micro-USB ou similar               |
| Rede Wi-Fi de 2.4 GHz             | 1          | Necessária para a interface web         |

> ⚠️ **Importante**: Conecte os LEDs com a polaridade correta:  
> Ânodo (perna mais longa) → Resistor → Pino do ESP32  
> Cátodo (perna mais curta) → GND

---

## 🔌 Diagrama de Conexão

| Componente         | Pino ESP32     |
| ------------------ | -------------- |
| Sensor de Água     | 34 (analógico) |
| LED Azul           | 23             |
| LED Verde          | 26             |
| LED Amarelo        | 33             |
| LED Vermelho       | 21             |
| LED Branco (Wi-Fi) | 22             |

---

## 🧠 Estrutura do Código

### Configurações

Definição de credenciais Wi-Fi, pinos e limiares do sensor.

### setup()

- Inicializa a serial.
- Configura os pinos.
- Testa os LEDs.
- Conecta ao Wi-Fi.
- Inicia o servidor web.

### loop()

- Verifica a conexão Wi-Fi.
- Lê o sensor.
- Atualiza os LEDs.
- Responde às requisições no servidor.

### Funções Auxiliares

| Função                  | Finalidade                                                    |
| ----------------------- | ------------------------------------------------------------- |
| `setup_wifi()`          | Lida com a conexão à rede Wi-Fi.                              |
| `getEstadoAguaString()` | Converte a leitura do sensor em texto legível.                |
| `handleDadosSensor()`   | Responde à requisição em `/dados` com dados em JSON.          |
| `atualizarLedsNivel()`  | Controla qual LED deve acender conforme o nível.              |
| `desligarLedsNivel()`   | Garante que os LEDs estejam desligados antes da nova leitura. |

---

## 📦 Bibliotecas Utilizadas

- `WiFi.h` — Conexão do ESP32 com rede Wi-Fi.
- `WebServer.h` — Criação e gerenciamento do servidor web.
- `ArduinoJson.h` — Formatação eficiente de dados em JSON.

---

## 🚀 Como Usar

### 1. Clone o Repositório:

```bash
git clone https://github.com/saraywebster/Sentinela_SP.git
```
