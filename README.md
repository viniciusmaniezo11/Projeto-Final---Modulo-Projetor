
# Módulo Projetor

Projeto Arduino/ESP32 para controle simples de projetor EPSON conectado ao módulo display principal via MQTT WiFi.

> Autores: [Vinicius Maniezo](https://github.com/viniciusmaniezo11), [Vinicius Atanasio](https://github.com/viniciusaatansio0), [Fabricio Honorato](https://github.com/FabricioHonorato), [Luiz Gustavo](https://github.com/Luiz20100415)

> Depende de: Módulo Display -- Adquirido separadamente.

---
## 💻 Sobre o Projeto
Um sistema de automação simplificado, impulsionado por comunicação em nuvem inteligente para controle de salas de aula.
Elimina a dependência de pilhas e baterias;
Devolve a mobilidade ao professor na sala;
Otimiza o tempo de início das aulas;

---

## Conexão WiFi

    const char *WIFI_SSID = nome da rede;
    const char *WIFI_SENHA = senha da rede;

    WiFi.mode(WIFI_STA); //* Configura o ESP32 como station, ou seja, ele irá se conectar a um roteador existente.
    WiFi.begin(WIFI_SSID, WIFI_SENHA); //* Inicia a conexão com SSID e senha.

## Conexão MQTT
Este módulo é responsável por toda a comunicação do ESP32 via protocolo MQTT. Ele abstrai a complexidade de conexão, segurança TLS, inscrição em tópicos e publicação de mensagens de forma dinâmica.


### 🛠️ Estrutura de Conexão (Modos Suportados)
1. **AWS IoT Core:** Autenticação mútua via Certificado CA, Certificado do Cliente e Chave Privada.
2. **MQTT com TLS:** Conexão criptografada com validação opcional de certificado CA (com fallback automático para modo inseguro caso não fornecido).
3. **MQTT Padrão:** Conexão simples via IP/Porta, com ou sem autenticação por usuário e senha.

### 📌 Principais Funções Disponíveis

| Função | Descrição |
| :--- | :--- |
| `configurarMQTT()` | Inicializa o cliente MQTT escolhendo o modo de segurança apropriado. |
| `conectarMQTT()` | Realiza a conexão com o broker e faz o *subscribe* automático nos tópicos. |
| `garantirMQTTConectado()` | Mantém a conexão ativa, ideal para ser chamada periodicamente. |
| `publicarMensagemNoTopico(indice, msg)` | Publica uma string em um tópico específico baseado no índice do array de tópicos. |
| `registrarCallbackMensagem(callback)` | Vincula a função da sua aplicação principal que irá tratar as mensagens recebidas. |

### ⚙️ Dependências Utilizadas
* `PubSubClient.h` (Gerenciamento do protocolo MQTT)
* `WiFiClientSecure.h` (Suporte a conexões criptografadas/TLS)

---
## 🛠️ Gerenciamento no Monitor Serial (`DebugManager.cpp`)

Este módulo é responsável por centralizar e filtrar todas as mensagens de depuração (logs) do ESP32 enviadas via Monitor Serial. Ele permite controlar a verbosidade do sistema tanto via software quanto via hardware (usando um pino físico).

### 🚀 Funcionalidades Principais
* **Filtragem por Níveis de Log:** Separa as mensagens entre erros críticos (`[ERRO]`) e mensagens informativas de rotina (`[INFO]`).
* **Ativação por Hardware (Pino Físico):** Permite forçar o modo de depuração completa (`DEBUG_TUDO`) na inicialização caso um pino específico do ESP32 esteja aterrado (`LOW`), usando o resistor interno de pull-up.
* **Funções Flexíveis de Escrita:** Oferece opções para imprimir logs com quebra de linha automática ou de forma contínua (sem quebra de linha), facilitando a formatação de tabelas ou blocos de texto.

### 📊 Níveis de Depuração

O comportamento do sistema muda dinamicamente com base na variável `nivelDebugAtual`:

| Nível de Depuração | Mensagens Exibidas no Monitor Serial |
| :--- | :--- |
| **`DEBUG_ERRO`** | Exibe apenas falhas críticas e erros operacionais (`debugErro`). |
| **`DEBUG_TUDO`** | Modo completo. Exibe erros e todas as mensagens informativas de rotina (`debugInfo`). |

---

### 📌 Principais Funções Disponíveis

| Função | Descrição |
| :--- | :--- |
| `configurarDebug()` | Inicializa a comunicação Serial (9600 bps), lê o estado do pino físico de configuração e define o nível inicial de logs. |
| `debugErro(mensagem)` | Imprime uma mensagem de erro crítica prefixada com `[ERRO]` e pula uma linha. |
| `debugErroSemLinha(msg)`| Imprime um texto de erro sem pular linha (ideal para concatenar strings). |
| `debugInfo(mensagem)` | Imprime uma mensagem informativa de rotina prefixada com `[INFO]` e pula uma linha. |
| `debugInfoSemLinha(msg)`| Imprime um texto informativo sem pular linha. |

### ⚙️ Configurações de Hardware Utilizadas
* **`PINO_HABILITA_DEBUG_COMPLETO`**: Configurado como `INPUT_PULLUP`. Se estiver conectado ao `GND` ao ligar o ESP32, ativa o modo de depuração total. Caso contrário, inicia no nível padrão definido em `secrets.h`.
# Gerenciamento de Debug

Este módulo fornece um sistema simples e eficiente de logs de depuração (debug) para projetos baseados em Arduino/ESP32. Ele permite alternar dinamicamente entre dois níveis de verbosidade através da leitura de um pino físico (hardware) configurado como `INPUT_PULLUP`.

## 🚀 Funcionalidades

* **Níveis de Log:** Separação visual e lógica entre mensagens de **Erro** e mensagens de **Informação**.
* **Controle por Hardware:** Ativação do modo de debug completo (`DEBUG_TUDO`) simplesmente aterrando um pino específico na inicialização.
* **Funções Inline/Sem Linha:** Suporte para impressões contínuas na mesma linha (`Serial.print`) e com quebra de linha (`Serial.println`).

---

## 🛠️ Como Funciona a Lógica de Debug

O comportamento do sistema é definido no momento em que a função `configurarDebug()` é chamada (geralmente dentro do `setup()` principal):

| Estado do Pino (`PINO_HABILITA_DEBUG_COMPLETO`) | Nível de Debug Ativo | O que é exibido no Monitor Serial? |
| :--- | :--- | :--- |
| **HIGH** (Desconectado/3.3V) | `DEBUG_ERRO` | Apenas logs de erro críticos (`[ERRO]`). |
| **LOW** (Conectado ao GND) | `DEBUG_TUDO` | Logs de erro **e** mensagens informativas (`[INFO]`). |

---

## 📂 Arquivos Necessários

Para que este módulo funcione corretamente, o projeto deve conter os seguintes cabeçalhos (headers) definidos:

### 1. `DebugManager.h`
Deve conter as declarações das funções e as constantes dos níveis de debug. Exemplo:
´´´cpp
#ifndef DEBUG_MANAGER_H
#define DEBUG_MANAGER_H

#include <Arduino.h>

#define DEBUG_ERRO 1
#define DEBUG_TUDO 2
#define DEBUG_NIVEL_INICIAL DEBUG_ERRO

void configurarDebug();
void debugErro(const String &mensagem);
void debugErroSemLinha(const String &mensagem);
void debugInfo(const String &mensagem);
void debugInfoSemLinha(const String &mensagem);

#endif

# 📹 Controle de Projetores Epson via MQTT e Infravermelho (IR)

Este projeto consiste em um firmware desenvolvido em ambiente Arduino (compatível com ESP32 / ESP8266) voltado para a automação e gerenciamento remoto de projetores industriais da marca **Epson** (identificados como Projetor 09 e Projetor 10). 

A comunicação externa é realizada através do protocolo **MQTT**, recebendo payloads formatados em **JSON** e convertendo-os em pulsos físicos de **Infravermelho (IR)**.

---

## 🚀 Funcionalidades

- **Controle de Energia:** Ligar e desligar projetores remotamente.
- **Controle de Transmissão:** Congelar (`Freeze`) e descongelar a imagem projetada.
- **Filtro de Estado:** Evita disparos desnecessários de infravermelho caso o projetor já esteja no estado desejado.
- **Confirmação Dupla Automática:** Implementação interna automática para o desligamento físico dos projetores Epson (que exigem dois pressionamentos do botão Power).
- **Telemetria de Status:** Envio automático de confirmação para um painel/display contendo a estampa de tempo (*timestamp*) exata via sincronização NTP.

---

## 🛠️ Arquitetura de Hardware e Pinagem

* **Microcontrolador:** ESP32 / ESP8266
* **LED Emissor IR:** Conectado ao pino digital `GPIO 16`
* **Interface de Debug:** Serial UART configurada a `9600 bps`

---

## 🌐 Integração e Protocolo MQTT

O dispositivo se comunica através de dois tópicos principais estruturados dentro do servidor Broker:

### 1. Tópico de Entrada (Comandos assinados)
* **Tópico:** `senai134/shared/projeto/projetor`
* **Exemplo de Payload (JSON):**
```json
{
  "id": 1,
  "power": 1,
  "freeze": 0
}






