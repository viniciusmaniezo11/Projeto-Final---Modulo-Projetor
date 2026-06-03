#include <Arduino.h>
#include <ArduinoJson.h>
#include <EpsonIR.h>

#include "DebugManager.h"
#include "MqttManager.h"
#include "WiFiManager.h"

void tratarJsonComando(const String &mensagem);
void tratarMensagemRecebida(const char *topico, const String &mensagem);

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

  Serial.println("Comando enviado");
  projector.send(EPSON_CMD_POWER);
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
      debugInfo("Comando recebido");

      if (estadoPower == 1)
      {
        projector.send(EPSON_CMD_POWER);
      }
    }
  }
}
