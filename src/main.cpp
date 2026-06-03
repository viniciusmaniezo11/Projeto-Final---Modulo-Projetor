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
void alterarEstadoPower(bool estadoPower);
void alterarEstadoCongela(bool estadoCongela);
void enviarMensagemProDisplay();

const int PINO_PROJETOR_IR = 16;
const int PINO_BOTAO_BOOT = 0;
bool enviarComandoPower = 0;

EpsonIR projector(PINO_PROJETOR_IR);

Timezone tempo;

const char TOPICO_COMANDO[] = "senai134/equipe/bowser/#";
const char TOPICO_COMANDO[] = "senai134/equipe/bowser/devices/…";

void setup()
{
  pinMode(PINO_BOTAO_BOOT, INPUT_PULLUP);
  Serial.begin(9600);
  configurarDebug();
  configurarMQTT();
  conectarWiFi();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
<<<<<<< HEAD
  enviarMensagemProDisplay();
  setInterval(3600);
  waitForSync();
  tempo.setLocation("America/Sao_Paulo");
=======
  projector.begin();
>>>>>>> c918521d754435455a4ea416cb1c5b7906bcfec7
}

void loop()
{
  garantirMQTTConectado();
  garantirWiFiConectado();
  loopMQTT();

  if(enviarComandoPower)
  {
    projector.send(EPSON_CMD_POWER);
    delay(1000);
    projector.send(EPSON_CMD_POWER);
    debugInfo("Projetor Ligado");
  }
  enviarComandoPower = 0;
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
    if (!doc["projetor"]["estadoProjetor"].is<bool>() || !doc["projetor"]["estadoCongela"].is<bool>())
    {
      debugErro("JSON INVALIDO. use projetor.estadoProjetor, projetor.estadoCongela");
      return;
    }
    else
    {
      bool estadoCongela = doc["projetor"]["estadoCongela"].as<bool>();
      bool estadoPower = doc["projetor"]["estadoProjetor"].as<bool>();
      
      enviarComandoPower = estadoPower;
  }
  }
}
void alterarEstadoPower(bool estadoPower)
{
  if (estadoPower)
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
}
void alterarEstadoCongela(bool estadoCongela)
{
  if (estadoCongela)
  {
    debugInfo("projetor congelado");
  }
  else
  {
    debugInfo("projetor descongelado");
  }
}

void enviarMensagemProDisplay()
{
  JsonDocument doc;
  doc["timestamp"] = tempo.now();
  doc["modulo"] = "Módulo Projetor";

  char buffer[200];
  serializeJson(doc, buffer);

  debugInfo("Enviando mensagem para Tópico: ");
  debugInfo(TOPICO_COMANDO);
  debugInfo(buffer);
}