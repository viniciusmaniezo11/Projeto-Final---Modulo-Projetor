#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <EpsonIR.h>
#include <ezTime.h>

#include "DebugManager.h"
#include "MqttManager.h"
#include "WiFiManager.h"

void tratarJsonComando(const String &mensagem);
void tratarMensagemRecebida(const char *topico, const String &mensagem);
void enviarMensagemProDisplay();

const int PINO_PROJETOR_IR = 16;
const int PINO_BOTAO_BOOT = 0;
bool enviarComandoPower = 0;
static bool enviarComandoPowerAnterior = 0;
bool enviarComandoCongela = 0;
static bool enviarComandoCongelaAnterior = 0;

EpsonIR projector(PINO_PROJETOR_IR);

Timezone tempo;

const char TOPICO_COMANDO[] = "senai134/equipe/bowser/devices/…";
const char TOPICOS_PUBLICAR[] = " ";

void setup()
{
  pinMode(PINO_BOTAO_BOOT, INPUT_PULLUP);
  Serial.begin(9600);
  configurarDebug();
  configurarMQTT();
  conectarWiFi();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
  enviarMensagemProDisplay();
  setInterval(3600);
  waitForSync();
  tempo.setLocation("America/Sao_Paulo");
  projector.begin();
}

void loop()
{
  garantirMQTTConectado();
  garantirWiFiConectado();
  loopMQTT();

  if (enviarComandoPower != enviarComandoPowerAnterior)
    if (enviarComandoPower)
    {
      debugInfo("projetor ligado");
      projector.send(EPSON_CMD_POWER);
    }
    else
    {
      debugInfo("projetor desligado");
      projector.send(EPSON_CMD_POWER);
      delay(1000);
      projector.send(EPSON_CMD_POWER);
    }
  enviarComandoPowerAnterior = enviarComandoPower;

  if (enviarComandoCongela != enviarComandoCongelaAnterior)
    if (enviarComandoPower)
      if (enviarComandoCongela)
      {
        debugInfo("projetor Congelado");
        projector.send(EPSON_CMD_FREEZE);
      }
      else
      {
        debugInfo("projetor Descongelado");
        projector.send(EPSON_CMD_FREEZE);
      }
  enviarComandoCongelaAnterior = enviarComandoCongela;
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
  if (doc["projetor"].is<JsonObject>())
  {
    if (!doc["projetor"]["estadoProjetor"].is<int>() || !doc["projetor"]["estadoCongela"].is<int>())
    {
      debugErro("JSON INVALIDO. use projetor.estadoProjetor, projetor.estadoCongela");
      return;
    }
    else
    {
      bool estadoCongela = doc["projetor"]["estadoCongelamento"].as<int>();
      bool estadoPower = doc["projetor"]["estadoProjetor"].as<int>();

      enviarComandoPower = estadoPower;
      enviarComandoCongela = estadoCongela;
    }
  }
}

void enviarMensagemProDisplay(int projetor)
{
  {
    JsonDocument doc2;
    doc2["timestamp"] = tempo.now();

    if(projetor == 1)
    doc2["modulo"] = "Projetor 1";

    else if(projetor == 2)
    doc2["modulo"] = "Projetor 2";

    char buffer[200];
    serializeJson(doc2, buffer);

    debugInfo("Enviando mensagem para Tópico: ");
    debugInfo(TOPICOS_PUBLICAR);
    debugInfo(buffer);
  }
}