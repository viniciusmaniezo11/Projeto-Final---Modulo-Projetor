#include <Arduino.h>
#include <ArduinoJson.h>
#include <EpsonIR.h>

#include "DebugManager.h"
#include "MqttManager.h"
#include "WiFiManager.h"

void tratarJsonComando(const String &mensagem);
void tratarMensagemRecebida(const char *topico, const String &mensagem);
void alterarEstadoPower(bool estadoPower);
void alterarEstadoCongela(bool estadoCongela);

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
  static bool estadoPowerAnterior = 0;
  static bool estadoCongelaAnterior = 0;
  static bool teste = 0;
  teste = !teste;
  Serial.println(teste);

  DeserializationError erro = deserializeJson(doc, mensagem);

  if (erro)
  {
    debugErro("Erro ao interpretar o JSON.");
    debugErro(erro.c_str());
    return;
  }
  if (doc["projetor"].is<JsonObject>())
  {
    if (!doc["projetor"]["estadoProjetor"].is<bool>() || !doc["projetor"]["estadoCongela"].is<bool>())
    {
      debugErro("JSON INVALIDO. use projetor.estadoProjetor, projetor.estadoCongela");
      return;
    }
    else
    {
      bool estadoCongela = doc["projetor"]["estadoCongela"].as<bool>();
      bool estadoPower = doc["projetor"]["estadoProjetor"].as<bool>();
      alterarEstadoPower(estadoPower);
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
