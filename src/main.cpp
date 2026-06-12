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
void enviarMensagemProDisplay(int id);
void tratamentoControleProjetor09();
void tratamentoControleProjetor10();

const int PINO_PROJETOR_IR = 16;
bool ComandoPowerProjetor09;
int ComandoPowerAnteriorProjetor09 = -1;
bool ComandoCongelaProjetor09;
int ComandoCongelaAnteriorProjetor09 = -1;
bool ComandoPowerProjetor10;
int ComandoPowerAnteriorProjetor10 = -1;
bool ComandoCongelaProjetor10;
int ComandoCongelaAnteriorProjetor10 = -1;
bool statusPower09 = false;
bool statusPower10 = false;
bool statusFreeze09 = false;
bool statusFreeze10 = false;
int id;

EpsonIR projector(PINO_PROJETOR_IR);

Timezone tempo;

const char TOPICO_COMANDO_SALA[] = "senai134/shared/projeto/projetor";
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

  if (strcmp(topico, TOPICO_COMANDO_SALA) == 0)
  {
    tratarJsonComando(mensagem);
    debugInfo("Mensagem recebida");
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

  if (!doc["id"].is<int>())
  {
    debugErro("JSON INVALIDO. O JSON precisa conter 'id'");
    return;
  }

  id = doc["id"].as<int>();

  if (doc.containsKey("power"))
  {
    if (doc["power"].is<int>())
    {
      if (id == 1)
      {
        ComandoPowerProjetor09 = doc["power"].as<int>();
        if (ComandoPowerProjetor09 != ComandoPowerAnteriorProjetor09)
        {
          ComandoPowerAnteriorProjetor09 = ComandoPowerProjetor09;
          statusPower09 = true;
        }
        else
        {
          debugInfo("Power recebeu valor igual. Nada foi feito.");
        }
      }

      if (id == 2)
      {
        ComandoPowerProjetor10 = doc["power"].as<int>();

        if (ComandoPowerProjetor10 != ComandoPowerAnteriorProjetor10)
        {
          ComandoPowerAnteriorProjetor10 = ComandoPowerProjetor10;
          statusPower10 = true;
        }
        else
        {
          debugInfo("Power recebeu valor igual. Nada foi feito.");
        }
      }
    }
  }

  if (doc.containsKey("freeze"))
  {
    if (doc["freeze"].is<int>())
    {
      if (id == 1)
      {
        ComandoCongelaProjetor09 = doc["freeze"].as<int>();

        if (ComandoCongelaProjetor09 != ComandoCongelaAnteriorProjetor09)
        {
          ComandoCongelaAnteriorProjetor09 = ComandoCongelaProjetor09;
          statusFreeze09 = true;
        }
        else
        {
          debugInfo("Freeze recebeu valor igual. Nada foi feito.");
        }
      }

      if (id == 2)
      {
        ComandoCongelaProjetor10 = doc["freeze"].as<int>();

        if (ComandoCongelaProjetor10 != ComandoCongelaAnteriorProjetor10)
        {
          ComandoCongelaAnteriorProjetor10 = ComandoCongelaProjetor10;
          statusFreeze10 = true;
        }
        else
        {
          debugInfo("Freeze recebeu valor igual. Nada foi feito.");
        }
      }
    }
  }
}

void enviarMensagemProDisplay(int id)
{
  {
    JsonDocument doc2;
    doc2["timestamp"] = tempo.now();

    if (id == 1)
      doc2["modulo"] = "Projetor 1";

    else if (id == 2)
      doc2["modulo"] = "Projetor 2";

    char buffer[200];
    serializeJson(doc2, buffer);
    publicarMensagem(TOPICOS_PUBLICAR, buffer);

    debugInfo("Enviando mensagem para Tópico: ");
    debugInfo(TOPICOS_PUBLICAR);
    debugInfo(buffer);
  }
}

void tratamentoControleProjetor09()
{

  if (statusPower09)
  {
    if (ComandoPowerProjetor09)
    {
      debugInfo("projetor ligado");
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(id);
    }
    else
    {
      debugInfo("projetor desligado");
      projector.send(EPSON_CMD_POWER);
      delay(1000);
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(id);
    }
    statusPower09 = false;
  }

  if (statusFreeze09)
  {
    if (ComandoCongelaProjetor09)
    {
      debugInfo("projetor Congelado");
      projector.send(EPSON_CMD_FREEZE);
      enviarMensagemProDisplay(id);
    }
    else
    {
      debugInfo("projetor Descongelado");
      projector.send(EPSON_CMD_FREEZE);
      enviarMensagemProDisplay(id);
    }
    statusFreeze09 = false;
  }
}

void tratamentoControleProjetor10()
{
  if (statusPower10)
  {
    if (ComandoPowerProjetor10)
    {
      debugInfo("projetor ligado");
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(id);
    }
    else
    {
      debugInfo("projetor desligado");
      projector.send(EPSON_CMD_POWER);
      delay(1000);
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(id);
    }
    statusPower10 = false;
  }

  if (statusFreeze10)
  {
    if (ComandoCongelaProjetor10)
    {
      debugInfo("projetor Congelado");
      projector.send(EPSON_CMD_FREEZE);
      enviarMensagemProDisplay(id);
    }
    else
    {
      debugInfo("projetor Descongelado");
      projector.send(EPSON_CMD_FREEZE);
      enviarMensagemProDisplay(id);
    }
    statusFreeze10 = false;
  }
}