#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <EpsonIR.h>
#include <ezTime.h>

#include "DebugManager.h"
#include "MqttManager.h"
#include "WiFiManager.h"

void tratarJsonComando(const String &mensagem);
void tratarJsonComando2(const String &mensagem);
void tratarMensagemRecebida(const char *topico, const String &mensagem);
void enviarMensagemProDisplay(int projetor);
void tratamentoControleProjetor09();
void tratamentoControleProjetor10();

const int PINO_PROJETOR_IR = 16;
bool ComandoPowerProjetor09 = 0;
bool ComandoPowerAnteriorProjetor09 = 0;
bool ComandoCongelaProjetor09 = 0;
bool ComandoCongelaAnteriorProjetor09 = 0;
bool ComandoPowerProjetor10 = 0;
bool ComandoPowerAnteriorProjetor10 = 0;
bool ComandoCongelaProjetor10 = 0;
bool ComandoCongelaAnteriorProjetor10 = 0;
int projetor = 0;

EpsonIR projector(PINO_PROJETOR_IR);

Timezone tempo;

const char TOPICO_COMANDO_SALA9[] = "senai134/shared/projeto/projetor09";
const char TOPICO_COMANDO_SALA10[] = "senai134/shared/projeto/projetor10";
const char TOPICOS_PUBLICAR[] = "senai134/shared/projeto/status";


void setup()
{
  Serial.begin(9600);
  configurarDebug();
  configurarMQTT();
  conectarWiFi();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
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
  events();
  tratamentoControleProjetor09();
  tratamentoControleProjetor10();
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

    if (strcmp(topico, TOPICO_COMANDO_SALA9) == 0)
  {
    tratarJsonComando(mensagem);
    projetor = 1;
    debugInfo("Mensagem recebida");
    return;
  }

    if (strcmp(topico, TOPICO_COMANDO_SALA10) == 0)
  {
    tratarJsonComando2(mensagem);
    projetor = 2;
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
  if (doc["projetor"].is<JsonObject>())
  {
    if (!doc["projetor"]["estadoPower"].is<int>() || !doc["projetor"]["estadoCongelamento"].is<int>())
    {
      debugErro("JSON INVALIDO. use projetor.estadoProjetor, projetor.estadoCongelamento");
      return;
    }
    else
    {
      ComandoPowerProjetor09 = doc["projetor"]["estadoPower"].as<int>();
      ComandoCongelaProjetor09 = doc["projetor"]["estadoCongelamento"].as<int>();
    }
  }
}

void tratarJsonComando2(const String &mensagem)
{
  JsonDocument doc3;

  DeserializationError erro = deserializeJson(doc3, mensagem);

  if (erro)
  {
    debugErro("Erro ao interpretar o JSON.");
    debugErro(erro.c_str());
    return;
  }
  if (doc3["projetor"].is<JsonObject>())
  {
    if (!doc3["projetor"]["estadoPower"].is<int>() || !doc3["projetor"]["estadoCongelamento"].is<int>())
    {
      debugErro("JSON INVALIDO. use projetor.estadoProjetor, projetor.estadoCongelamento");
      return;
    }
    else
    {

      ComandoPowerProjetor10 = doc3["projetor"]["estadoPower"].as<int>();
      ComandoCongelaProjetor10 = doc3["projetor"]["estadoCongelamento"].as<int>();
    }
  }
}
//dfhuhrfurhurhugugrug
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

void tratamentoControleProjetor09()
{
  if (ComandoPowerProjetor09 != ComandoPowerAnteriorProjetor09)
    if (ComandoPowerProjetor09)
    {
      debugInfo("projetor ligado");
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(projetor);
    }
    else
    {
      debugInfo("projetor desligado");
      projector.send(EPSON_CMD_POWER);
      delay(1000);
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(projetor);
    }
  ComandoPowerAnteriorProjetor09 = ComandoPowerProjetor09;

  if (ComandoCongelaProjetor09 != ComandoCongelaAnteriorProjetor09)
    if (ComandoPowerProjetor09)
      if (ComandoCongelaProjetor09)
      {
        debugInfo("projetor Congelado");
        projector.send(EPSON_CMD_FREEZE);
        enviarMensagemProDisplay(projetor);
      }
      else
      {
        debugInfo("projetor Descongelado");
        projector.send(EPSON_CMD_FREEZE);
        enviarMensagemProDisplay(projetor);
      }
  ComandoCongelaAnteriorProjetor09 = ComandoCongelaProjetor09;
}

void tratamentoControleProjetor10()
{
  if (ComandoPowerProjetor10 != ComandoPowerAnteriorProjetor10)
    if (ComandoPowerProjetor10)
    {
      debugInfo("projetor ligado");
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(projetor);
    }
    else
    {
      debugInfo("projetor desligado");
      projector.send(EPSON_CMD_POWER);
      delay(1000);
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(projetor);
    }
  ComandoPowerAnteriorProjetor10 = ComandoPowerProjetor10;

  if (ComandoCongelaProjetor10 != ComandoCongelaAnteriorProjetor10)
    if (ComandoPowerProjetor10)
      if (ComandoCongelaProjetor10)
      {
        debugInfo("projetor Congelado");
        projector.send(EPSON_CMD_FREEZE);
        enviarMensagemProDisplay(projetor);
      }
      else
      {
        debugInfo("projetor Descongelado");
        projector.send(EPSON_CMD_FREEZE);
        enviarMensagemProDisplay(projetor);
      }
  ComandoCongelaAnteriorProjetor10 = ComandoCongelaProjetor10;
}