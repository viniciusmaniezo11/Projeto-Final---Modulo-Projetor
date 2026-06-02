#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <EpsonIR.h>

#include "DebugManager.h"
#include "MqttManager.h"
#include "WiFiManager.h"

void tratarJsonComando(const String &mensagem);
void tratarMensagemRecebida(const char *topico, const String &mensagem);
void alterarEstadoPower(bool estadoPower);
void alterarEstadoCongela(bool estadoCongela);
void enviarMensagemProDisplay();

const int PINO_PROJETOR_IR = 16;
const int PINO_BOTAO_BOOT = 0;

EpsonIR projector(PINO_PROJETOR_IR);

const char TOPICO_COMANDO[] = "senai134/viniciusM/esp32/comando";

void setup()
{
  pinMode(PINO_BOTAO_BOOT, INPUT_PULLUP);
  Serial.begin(9600);
  configurarDebug();
  configurarMQTT();
  conectarWiFi();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
  projector.begin();
  projector.send(EPSON_CMD_POWER);
}

void loop()
{
  garantirMQTTConectado();
  garantirWiFiConectado();
  loopMQTT();
  void tratarJsonComando(const String &mensagem);
}

void tratarMensagemRecebida(const char *topico, const String &mensagem)
{
  debugInfo("==============================");
  debugInfo("Mensagem recebida na aplicação");
  debugInfo("==============================");

  if (topico == nullptr)
  {
    debugErro("Tópico MQTT inválido.");
    return;
  }

  debugInfo("Tópico: " + String(topico));
  debugInfo("Mensagem: " + mensagem);

  if (strcmp(topico, TOPICO_COMANDO) == 0)
  {
    tratarJsonComando(mensagem);
    return;
  }

  debugErro("Tópico não tratado: " + String(topico));
}

void tratarJsonComando(const String &mensagem)
{
  JsonDocument doc;
  static bool estadoPowerAnterior = 0;
  static bool estadoCongelaAnterior = 0;

  DeserializationError erro = deserializeJson(doc, mensagem);

  if (erro)
  {
    debugErro("Erro ao interpretar o JSON.");
    debugErro(erro.c_str());
    return;
  }
  if (!doc["projetor"].is<JsonObject>())
  {
    debugErro("Erro ao interpretar o JSON.");
    return;
  }
  else
  {
    if (doc["projetor"]["estadoProjetor"].is<bool>())
    {
      bool estadoPower = doc["projetor"]["estadoProjetor"].as<bool>();
        alterarEstadoPower(estadoPower);
    }
    if (doc["projetor"]["estadoCongela"].is<bool>())
    {
      bool estadoCongela = doc["projetor"]["estadoCongela"].as<bool>();
        debugInfo("congela");
    }
  }
}

void alterarEstadoPower(bool estadoPower)
{
  if (estadoPower)
  {
    debugInfo("projetor ligado");
  }
  else
  {
    debugInfo("projetor desligado");
  }
}

void enviarMensagemProDisplay()
{
  JsonDocument doc;
  doc[""] = "Bolsonaro";
  doc[""] = "akguma coisa";
  doc[""][""] = "alguma coisa";
  doc[""][""] = "alguma coisa";

  char buffer[200];
  serializeJson(doc, buffer);

  Serial.print("Enviando mensagem para Tópico: ");
  Serial.println(TOPICO_COMANDO);
  Serial.println(buffer);
}
