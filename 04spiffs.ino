#include "SPIFFS.h"
#include <Arduino.h>
#include <FS.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Configurações do Wifi
const char* ssid = "NPITI-IoT";
const char* password = "NPITI-IoT";

const int ledPin = 26; // Pino do LED
const int touchPin = 27; // Pino do botão

const char *fileName = "/ledState.txt"; // Nome do arquivo para salvar o estado do LED
const char *logFileName = "/ledLogs.txt";

bool ledState = false; // Estado inicial do LED

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "a.st1.ntp.br", -3 * 3600, 60000); // Configuração do cliente NTP ("0.br.pool.ntp.org")

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  //formatFile();
  if (SPIFFS.begin()) { // Tentar abrir o arquivo do estado do LED
    connectToWiFi();
    timeClient.begin();
    
    File file = SPIFFS.open(fileName, "r");
    Serial.println("Abriu o arquivo");
    if (file != NULL) {
      String state = file.readString();
      if (state == "1") ledState = true;

      Serial.print("Leu ");
      Serial.println(state);
      file.close();
    }

    readAndPrintLogs();
  } else Serial.println("Falha ao montar o sistema de arquivos SPIFFS");
 
  digitalWrite(ledPin, ledState ? HIGH : LOW);
}

void loop() {
  timeClient.forceUpdate();
  bool isTouched = touchRead(touchPin) < 30 ;

  if (isTouched) {
    Serial.println("Foi tocado");
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);

    // Salvar o estado do LED no arquivo
    File file = SPIFFS.open(fileName, "w");

    if (file) {
      file.print(ledState ? "1" : "0");
      file.close();
    }else {
      Serial.println("Falha ao salvar estado do LED no arquivo");
    }

    logLEDState(ledState);

    delay(500); // Debounce para evitar múltiplas leituras do botão
 }
}

void logLEDState(bool state) {
  File logFile = SPIFFS.open(logFileName, "a");

  if (logFile) {
    String logEntry = state ? "LED Ligado" : "LED Desligado";
    logEntry += " - ";
    logEntry += timeClient.getFormattedTime();

    Serial.print("LOG SALVO: ");
    Serial.println(logEntry);

    logFile.println(logEntry);
    logFile.close();
  }
}

void readAndPrintLogs() {
  File logFile = SPIFFS.open(logFileName, "r");

  if (logFile) {
    String logEntry;
    
    while (logFile.available()) {
      logEntry = logFile.readStringUntil('\n');
      Serial.println(logEntry);
    }

    logFile.close();
  }  
}

void connectToWiFi() {
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi");
}

void formatFile() {
  Serial.println("Formantando SPIFFS");
  SPIFFS.format();
  Serial.println("Formatou SPIFFS");
}
