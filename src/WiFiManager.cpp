#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include "WiFiManager.h"
#include "secrets.h"
#include "DebugManager.h"

bool wifiEstaConectado()
{
    return WiFi.status() == WL_CONNECTED;
}

void conectarWiFi()
{
    debugInfo("===================================");
    debugInfo("Iniciando conexao WiFi...");
    debugInfo("===================================");

    WiFi.mode(WIFI_STA); //* Configura o ESP32 como station, ou seja, ele irá se conectar a um roteador existente.

    WiFi.begin(WIFI_SSID, WIFI_SENHA); //* Inicia a conexão com SSID e senha.

    debugInfo("Conectando");

    int tentativasWiFi = 0;
    const int maxTentativasWiFi = 30;

    while (WiFi.status() != WL_CONNECTED && tentativasWiFi < maxTentativasWiFi) //* Aguarda a conexão por até 30 tentivas.
    {
        delay(500);
        debugInfoSemLinha(".");
        tentativasWiFi++;
    }

    debugInfo("");

    if (WiFi.status() == WL_CONNECTED)
    {
        debugInfo("WiFi conectado com sucesso!");
        debugInfoSemLinha("[INFO] Endereço IP: ");
        debugInfo(WiFi.localIP().toString());
        debugInfoSemLinha("\n\r");
    }
    else
    {
        debugErro("Falha ao conectar no WiFi.");
        debugErro("Verifique SSID, senha e sinal de rede");
    }
}

void garantirWiFiConectado()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        debugErro("WiFi desconectado. Tentando reconectar...");
        conectarWiFi();
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        debugErro("Não foi possivel reconectar ao WiFi");
    }
}