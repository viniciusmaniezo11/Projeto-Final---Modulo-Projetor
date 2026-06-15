#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <EpsonIR.h>
#include <ezTime.h>

#include "DebugManager.h"
#include "MqttManager.h"
#include "WiFiManager.h"

struct estadoProjetor
{
  bool estadoPower;
  bool estadoPowerAnterior;
  bool estadoCongela;
  bool estadoCongelaAnterior;
  int idProjetor;
};

estadoProjetor projetores[] =
    {
        {false, false, false, false, 1},
        {false, false, false, false, 2},
};

void tratarJsonComando(estadoProjetor &p, const String &mensagem);
void tratarJsonComando2(const String &mensagem);
void tratarMensagemRecebida(const char *topico, const String &mensagem);
void tratamentoProjetores(estadoProjetor &p);
void enviarMensagemProDisplay(const estadoProjetor &p);

const int PINO_PROJETOR_IR = 16;

const int numeroProjetores = sizeof(projetores) / sizeof(projetores[0]);

EpsonIR projector(PINO_PROJETOR_IR);

Timezone tempo;

const char TOPICOS_RECEBER[] = "senai134/shared/projeto/projetor";

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

  for (int i = 0; i < numeroProjetores; i++)
  {
    tratamentoProjetores(projetores[i]);
  }
}

void tratarJsonComando(estadoProjetor &p, const String &mensagem)
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
    debugErro("JSON INVÁLIDO. Use { id: 1+ }");
    return;
  }
  int id = doc["id"].as<int>();

  int indice = -1;

  for (int i = 0; i < numeroProjetores; i++)
  {
    if (projetores[i].idProjetor == id)
      indice = i;
  }

  if (indice == -1)
  {
    debugErro("Projetor não encontrado.");
    return;
  }

  if (doc["power"].is<int>())
    projetores[indice].estadoPower = doc["power"].as<int>();
  else
    debugErro("JSON INVÁLIDO. Use { power: 1 ou 0 }");

  if (doc["freeze"].is<int>())
    projetores[indice].estadoCongela = doc["freeze"].as<int>();
  else
    debugErro("JSON INVÁLIDO. Use { freeze: 1 ou 0 }");
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

  for (int i = 0; i < numeroProjetores; i++)
  {
    if (strcmp(topico, TOPICOS_RECEBER) == 0)
    {
      tratarJsonComando(projetores[i], mensagem);
      return;
    }
  }

  debugErro("Tópico não tratado: " + String(topico));
}

void tratamentoProjetores(estadoProjetor &p)
{
  if (p.estadoPower != p.estadoPowerAnterior)
  {
    if (p.estadoPower)
    {
      debugInfo("Projetor ligado");
      projector.send(EPSON_CMD_POWER);
    }
    else
    {
      debugInfo("Projetor desligado");
      projector.send(EPSON_CMD_POWER);
      delay(1000);
      projector.send(EPSON_CMD_POWER);
    }
    enviarMensagemProDisplay(p);
  }
  p.estadoPowerAnterior = p.estadoPower;

  if (p.estadoCongela != p.estadoCongelaAnterior)
  {
    if (p.estadoCongela)
    {
      debugInfo("Projetor Congelado");
      projector.send(EPSON_CMD_FREEZE);
    }
    else
    {
      debugInfo("Projetor Descongelado");
      projector.send(EPSON_CMD_FREEZE);
    }
    enviarMensagemProDisplay(p);
  }
  p.estadoCongelaAnterior = p.estadoCongela;
}
<<<<<<< HEAD
//dfhuhrfurhurhugugrug
void enviarMensagemProDisplay(int projetor)
=======

void enviarMensagemProDisplay(const estadoProjetor &p)
>>>>>>> 9a4e59969ab9bdc5b1af9f70072133304ba90e64
{
  {
    JsonDocument doc2;
    doc2["timestamp"] = tempo.now();
    doc2["modulo"] = "Projetor:" + String(p.idProjetor);

    char buffer[200];
    serializeJson(doc2, buffer);

    debugInfo("Enviando mensagem para Tópico: ");
    debugInfo(TOPICOS_PUBLICAR);
    debugInfo(buffer);
  }
}