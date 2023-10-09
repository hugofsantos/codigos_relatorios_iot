#include <SPI.h>
#include <MFRC522.h>

#define PIN_RST 8
#define PIN_SS 9
#define PIN_GREEN_LED 11
#define PIN_RED_LED 12
#define PIN_BUZZER 13

MFRC522 mfrc522(PIN_SS, PIN_RST);

int failed_count = 0; 

void setup() {
  Serial.begin(9600); // Inicializa a serial
  SPI.begin();// Inicializa a comunicação SPI
  mfrc522.PCD_Init(); // Inicializa o módulo MFRC522
  Serial.println("Sistema Inicializado: Aproxime o token");

  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_RED_LED, OUTPUT);
}
void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;// Aguarda a aproximação do token
  if (!mfrc522.PICC_ReadCardSerial()) return; // Seleciona qual token vai ser utilizado

  Serial.print("UID da tag:"); // Mostra UID do token na serial 

  String conteudo = ""; //Cria uma variável vazia, do tipo string byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if(mfrc522.uid.uidByte[i] < 0x10) Serial.print(" 0");
    else Serial.print(" ");
  
    Serial.print(mfrc522.uid.uidByte[i], HEX);// Printa a mensagem convertida em hexadecimal

    if(mfrc522.uid.uidByte[i] < 0x10) conteudo.concat(String(" 0"));
    else conteudo.concat(String(" "));
    
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  
  conteudo.toUpperCase(); //Coloca todas as letras da string em maiúscula

  if (conteudo.substring(1) == "15 6B 7F 63") Serial.println("Acesso Permitido!");

  delay(1000);
}

void accessGranted() {
  Serial.println("Acesso liberado!");
  digitalWrite(PIN_RED_LED, LOW);
  digitalWrite(PIN_GREEN_LED, HIGH);

  delay(10000);

  digitalWrite(PIN_RED_LED, HIGH);
  digitalWrite(PIN_GREEN_LED, LOW);
}

void accessDenied() {
  Serial.println("Cartão inválido!");
  digitalWrite(PIN_GREEN_LED, LOW);

  for(int i = 0; i < 3; i++) {
    digitalWrite(PIN_RED_LED, HIGH);
    delay(500);
    digitalWrite(PIN_RED_LED, LOW);
    delay(500);
  }

  digitalWrite(PIN_RED_LED, LOW);
}

void systemLocked() {
  Serial.println("SISTEMA BLOQUEADO!");
  digitalWrite(PIN_BUZZER, HIGH);

  for(int i = 0; i < 30; i++) {
    digitalWrite(PIN_RED_LED, HIGH);
    delay(500);
    digitalWrite(PIN_RED_LED, LOW);
    delay(500);    
  }

  digitalWrite(PIN_RED_LED, HIGH);
}