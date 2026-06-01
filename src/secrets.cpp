#include "secrets.h"
#include <Arduino.h>

//============================================================//
// WiFi
//============================================================//

const char *WIFI_SSID = "SALA 09";
const char *WIFI_SENHA = "info@134";

//============================================================//
// AWS
//============================================================//

const bool USAR_AWS_IOT = true;

const char *AWS_IOT_CLIENT_ID = "esp32_viniciusM";

const char AWS_CERT_CA[] PROGMEM = R"EOF(-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----)EOF";

const char AWS_CERT_CRT[] PROGMEM = R"EOF(-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUdP4xjJ/T6NE+bG9SMRyqNAxpgIgwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI2MDUxOTE3NDIz
OVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALtPYb35MthZzVNJL8+8
Vdn8b6ahWW42YgZSr39JMvrTCAHPyM3vVjuq6Thle0JPNzwzM60qsF85vH+Lo7d5
VgRdWSyH0xT/EHSGJkzmm4YFDO5PVIgJtxGh74M9YY+bVRKtOpShyBYLYMEOM62J
HaCODbwvHZ+6T0RuVZ2bqQGLnPy2+TKqdyYQs/sLbxHrcslTbL1SWWOTIqaFBanj
05R6hbsfPUPDE66UHHLVmDT4FDt4THv3HIrtWaIPtFW6tAuW2cgqRkyBFLvIfkpT
p1QPuQQvQgIpF6rsv75A3s4KwrkdG5R3ouce9u50HWYKBd+VjmpkzwiLhodev62w
OksCAwEAAaNgMF4wHwYDVR0jBBgwFoAUT4sowO2DF7wrCclvSoYYXLgHY0YwHQYD
VR0OBBYEFGAfaWTFUq1FufEM1/UJXgopukyIMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCoZCqdeaxdTN6hzW2Zkk0p/dN3
5mbcMXJT3O6jc6r2ulG2m6osvZ974H3LnvzRl8M9JhIhmTXHE4/WfVkjMkJlI561
zwIK7G2I+DfclOcIECovvf2+UeRbz+FxCUpbLqKbmTi9J7KTXhQNhA1ruyyeFjc2
qgigHRv+Pbv8VibQyafuC62bK483e/lnzpacyg/kjI5PVtta8s7ysAnYxdFpqRJC
M1VM8WC1yJ2ebX4xvasJ0bzrZcfChPSfs70vVejRUXbn7hBNIgJdkyHRHIiQnISc
vHCTPdpBsA+MM9qQoIlR7pvjkfsUX9ylvbbup50Q0wN3ELf398Vf642hNReE
-----END CERTIFICATE-----
)EOF";

const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAu09hvfky2FnNU0kvz7xV2fxvpqFZbjZiBlKvf0ky+tMIAc/I
ze9WO6rpOGV7Qk83PDMzrSqwXzm8f4ujt3lWBF1ZLIfTFP8QdIYmTOabhgUM7k9U
iAm3EaHvgz1hj5tVEq06lKHIFgtgwQ4zrYkdoI4NvC8dn7pPRG5VnZupAYuc/Lb5
Mqp3JhCz+wtvEetyyVNsvVJZY5MipoUFqePTlHqFux89Q8MTrpQcctWYNPgUO3hM
e/cciu1Zog+0Vbq0C5bZyCpGTIEUu8h+SlOnVA+5BC9CAikXquy/vkDezgrCuR0b
lHei5x727nQdZgoF35WOamTPCIuGh16/rbA6SwIDAQABAoIBAGwZ6UAZlQWV9lEN
2QEcIbQX41wYnKNl0Xbyu4RLe5eUuXdKMwrfCTaQcd7xeoyPaUJeDoHvNDERAj/e
GFNoPINyWGfBz8m7VwTY8zvIj5NuUMMsl42XhccwXSmyAB9AN3366ecj43UNhf6F
MsABdU7XDQhBbOhlPyAwSUsXrhdMPkApDymV7hwvS/3u5qOd5fENEnOUgP57xLA/
oaYKadYV/GUhzUwPOcRyg3IniaOAoa+ubgvUX3nqrGV+h/HdTiFMHfar6hhVMDng
14sUwXr/H93qCqFJYBHEPMcGXxqGlUCY2SsAPFBgYfLJH92hjSjPE8QGnixKbmf1
HqI0x0ECgYEA299GXBifP/Nwa6KRekzPMd9D+VMkssxW3Vdh/D5SzeYHQWtCN0ut
CKu08m2DZKRtUybt2Cf243iFnDbQBqpn16daV3YJd6jMZ8Hoi79/Z1lh6FXNs7Vi
JaNMKldVA7DyPoduX3oQllOp8SdB0xEkSbNwZIImqctb+UAyDbrPsuECgYEA2hZp
iCK/T3flySjeUEnQar/7N38q1oZBVRKSQxh9ufXYHnnaXgGV2e3hkVCqrBM8xI6b
iiQNCIwpEA2FV50Pn7xyY+l2goVOnlJ1cAIQv6HftmY7G5uRUvHyNNl32hsZ6YPs
oFcebCJrr9GV3Lg09BVf8W5lX6MZiVKaUYEJfqsCgYEAugY16UWLtveobeilXOwP
gQp84D1QF1Z7R9netSOluz6bdOXiqBYG2AcumM2/FS9or6mIIhHt+JbHtFtLtdgG
85ZMASOE7xCJoF+W+GWUzUP6ai/uUdVsrJt0nX42F2FmEzFSWMVVs1oltjqYnpMq
OJDwzpivTYkYz9FYh/U+NQECgYEAkdYk8f1Sc2DZD4C90XDa51fENWNbkSMsMyh9
GFMbWCx3PkH77ZsfyyDRE2yfBPzESSiYCHN9J7cainqQLXuBBC9vSzLGphdg8Gxx
zdoDuzITOJT6d8a7En1qmz7Fb9tpo4ctyDcZx/PokSUVEwk3l/VnODq7nY2iuPq8
6T3hzIsCgYANK2nk7677+YhIcGg/X6Dwp6J3V5Z5sRIGqDsw3eZzAk7V1jnV/zfK
05zm+PqecH7AnoDLTlzWI54jDT5JFBLYlI8mbg8UZbodXH6Yt/JgBue1ADx2f0c0
xcqcBZyxP0XYOekCkiVV+dqMnh7lVntcLDCQshH3+CPmVVTxhrFqPw==
-----END RSA PRIVATE KEY-----
)EOF";

const char *AWS_IOT_ENDPOINT = "a242lwbu76kea4-ats.iot.us-east-1.amazonaws.com";

const int AWS_IOT_PORT = 8883;

//============================================================//
// MQTT
//============================================================//

const char *MQTT_BROKER = "c39422280a4d43b28ad8d0b3e0c9cde4.s1.eu.hivemq.cloud";
const int MQTT_PORTA = 8883;

const char *MQTT_CLIENT_ID = "eps32_Vinicius_Maniezo";

const char *MQTT_USUARIO = "ViniciusM";
const char *MQTT_SENHA = "Senai@134";

const bool MQTT_USAR_TLS = false; //* Modo de conexão sem segurança.

const char MQTT_CERTIFICADO_CA[] PROGMEM = "";

const char *TOPICOS_PUBLICAR[] = {
    "senai/esp32/status",
    "senai/esp32/log",
    "senai/esp32/resposta"};

const int TOTAL_TOPICOS_PUBLICAR = sizeof(TOPICOS_PUBLICAR) / sizeof(TOPICOS_PUBLICAR[0]);

const char *TOPICOS_RECEBER[] = {
    "senai134/viniciusM/esp32/comando",
    "senai134/viniciusM/esp32/config",
    "senai134/viniciusM/esp32/display"};

const int TOTAL_TOPICOS_RECEBER = sizeof(TOPICOS_RECEBER) / sizeof(TOPICOS_RECEBER[0]);

//============================================================//
// DEBUG
//============================================================//

// 0 = SEM MENSAGENS
// 1 = APENAS ERROS
// 2 = TODAS AS MENSAGENS
const int DEBUG_NIVEL_INICIAL = 2;

// PINO USADO PARA FORÇAR TODAS AS MENSAGENS NA INICIALIZAÇÃO
const int PINO_HABILITA_DEBUG_COMPLETO = 4;