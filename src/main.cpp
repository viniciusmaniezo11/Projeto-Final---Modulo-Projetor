
//* Bibliotecas //
#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <EpsonIR.h>
#include <ezTime.h>

#include "DebugManager.h"
#include "MqttManager.h"
#include "WiFiManager.h"

//* Funções //
void tratarJsonComando(const String &mensagem);
void tratarMensagemRecebida(const char *topico, const String &mensagem);
void enviarMensagemProDisplay(int id);
void comandosProjetor09();
void comandosProjetor10();

const int PINO_PROJETOR_IR = 16;

//* Variaveis Projetor 09 //
bool comandoPowerProjetor09 = 0;
int comandoPowerAnteriorProjetor09 = -1;
bool comandoCongelaProjetor09 = 0;
int comandoCongelaAnteriorProjetor09 = -1;
bool statusPower09 = false;
bool statusFreeze09 = false;

//* Variaveis Projetor 10 //
bool comandoPowerProjetor10 = 0;
int comandoPowerAnteriorProjetor10 = -1;
bool comandoCongelaProjetor10 = 0;
int comandoCongelaAnteriorProjetor10 = -1;
bool statusPower10 = false;
bool statusFreeze10 = false;

int id;

//* Tópicos //
const char TOPICO_COMANDO_SALA[] = "senai134/shared/projeto/projetor";
const char TOPICOS_PUBLICAR[] = "senai134/shared/projeto/status";

EpsonIR projector(PINO_PROJETOR_IR);
Timezone tempo;

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

  //* Projetores //
  comandosProjetor09();
  comandosProjetor10();
}

void tratarMensagemRecebida(const char *topico, const String &mensagem) //* Responsavel por tratar a mensagem recebida //
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

void tratarJsonComando(const String &mensagem) //* Responsavel por desserializar o JSON recebido //

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

  id = doc["id"].as<int>(); //* Desserialize ID //

  if (doc.containsKey("power")) //* Desserialize POWER //
  {
    if (doc["power"].is<int>())
    {
      if (id == 1)
      {
        comandoPowerProjetor09 = doc["power"].as<int>();

        if (comandoPowerProjetor09 != comandoPowerAnteriorProjetor09)
        {
          comandoPowerAnteriorProjetor09 = comandoPowerProjetor09;
          statusPower09 = true;
        }
        else
        {
          debugInfo("Power recebeu valor igual. Nada foi feito.");
        }
      }

      if (id == 2)
      {
        comandoPowerProjetor10 = doc["power"].as<int>();

        if (comandoPowerProjetor10 != comandoPowerAnteriorProjetor10)
        {
          comandoPowerAnteriorProjetor10 = comandoPowerProjetor10;
          statusPower10 = true;
        }
        else
        {
          debugInfo("Power recebeu valor igual. Nada foi feito.");
        }
      }
    }
  }

  if (doc.containsKey("freeze")) //* Desserialize FREEZE //
  {

    if (doc["freeze"].is<int>())
    {
      if (id == 1)
      {
        comandoCongelaProjetor09 = doc["freeze"].as<int>();

        if (comandoCongelaProjetor09 != comandoCongelaAnteriorProjetor09)
        {
          comandoCongelaAnteriorProjetor09 = comandoCongelaProjetor09;
          statusFreeze09 = true;
        }
        else
        {
          debugInfo("Freeze recebeu valor igual. Nada foi feito.");
        }
      }

      if (id == 2)
      {
        comandoCongelaProjetor10 = doc["freeze"].as<int>();
        if (comandoCongelaProjetor10 != comandoCongelaAnteriorProjetor10)
        {
          comandoCongelaAnteriorProjetor10 = comandoCongelaProjetor10;
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

void enviarMensagemProDisplay(int id) //* Responsavel por enviar o timestamp para o tópico de status //
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

void comandosProjetor09() //* Responsavel por realizar os comandos do projetor 09 //
{

  if (statusPower09) //* Comando POWER //
  {
    if (comandoPowerProjetor09) //* Liga //
    {
      debugInfo("projetor ligado");
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(id);
    }
    else //* Desliga //
    {
      debugInfo("projetor desligado");
      projector.send(EPSON_CMD_POWER);
      delay(1000);
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(id);
    }
    statusPower09 = false;
  }

  if (statusFreeze09) //* Comando FREEZE //
  {
    if (comandoCongelaProjetor09) //* Congela //
    {
      debugInfo("projetor Congelado");
      projector.send(EPSON_CMD_FREEZE);
      enviarMensagemProDisplay(id);
    }
    else //* Descongela //
    {
      debugInfo("projetor Descongelado");
      projector.send(EPSON_CMD_FREEZE);
      enviarMensagemProDisplay(id);
    }
    statusFreeze09 = false;
  }
}

void comandosProjetor10() //* Responsavel por realizar os comandos do projetor 10 //
{
  if (statusPower10) //* Comando POWER //
  {
    if (comandoPowerProjetor10) //* Liga //
    {
      debugInfo("projetor ligado");
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(id);
    }
    else //* Desliga //
    {
      debugInfo("projetor desligado");
      projector.send(EPSON_CMD_POWER);
      delay(1000);
      projector.send(EPSON_CMD_POWER);
      enviarMensagemProDisplay(id);
    }
    statusPower10 = false;
  }

  if (statusFreeze10) //* Comando FREEZE //
  {
    if (comandoCongelaProjetor10) //* Congela //
    {
      debugInfo("projetor Congelado");
      projector.send(EPSON_CMD_FREEZE);
      enviarMensagemProDisplay(id);
    }
    else //* Descongela //
    {
      debugInfo("projetor Descongelado");
      projector.send(EPSON_CMD_FREEZE);
      enviarMensagemProDisplay(id);
    }
    statusFreeze10 = false;
  }
}