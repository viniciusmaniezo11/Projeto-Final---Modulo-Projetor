#include <Arduino.h>
#include <ArduinoJson.h>
#include <EpsonIR.h>

#include "DebugManager.h"
#include "MqttManager.h"
#include "WiFiManager.h"

void tratarJsonComando(const String &mensagem);
void tratarMensagemRecebida(const char *topico, const String &mensagem);

#define PINO_PROJETOR_IR 16

EpsonIR projector(PINO_PROJETOR_IR);

const char TOPICO_COMANDO[] = "senai134/viniciusM/esp32/comando";

void setup()
{
    Serial.begin(9600);
    configurarDebug();
    configurarMQTT();
    conectarWiFi();
    registrarCallbackMensagem(tratarMensagemRecebida);
    conectarMQTT();
}

void loop()
{
    garantirMQTTConectado();
    garantirWiFiConectado();
    loopMQTT();
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

  DeserializationError erro = deserializeJson(doc, mensagem);

  if (erro)
  {
    debugErro("Erro ao interpretar o JSON.");
    debugErro(erro.c_str());
    return;
  }
}