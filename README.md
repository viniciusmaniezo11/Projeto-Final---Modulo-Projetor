
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

# Estrutura de configuração 





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








