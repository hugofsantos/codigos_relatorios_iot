
#include <SoftwareSerial.h>
#define BUTTON_PIN 52


SoftwareSerial ArduinoSlave(10, 11);

String answer;
String msg;
String oldMsg = "";

String msgSensor = "0";
String msgButton = "0";

int sensorValue = 0, oldSensorValue = 0;
bool isPressed = false;

void setup() {
  Serial.begin(9600);
  ArduinoSlave.begin(9600);
}

void loop() {
  sensorValue = analogRead(A0); // Ler sensor 
  isPressed = digitalRead(BUTTON_PIN);

  msgButton = isPressed ? "1" : "0";
  if((oldSensorValue != sensorValue) && !isPressed) { // Envia dados do sensor para o escravo
    msgSensor = String(sensorValue);
    oldSensorValue = sensorValue;
  }

  msg = msgButton + " " + msgSensor; // "0 150" 

  if(msg != oldMsg) {
    Serial.print("Master sent: ");
    Serial.println(msg);
    
    ArduinoSlave.print(msg);    
    oldMsg = msg;
  }

  readSlavePort(); // Ler escravo

  if(answer != "") { // Lê variável de resposta
    Serial.print("Slave LED PMW value: ");
    Serial.println(answer);
    answer = "";
  }

  delay(1000);
}

/**
* Lê os dados enviados pelo escravo e atribui à variàvel de resposta
*/
void readSlavePort() {
  while(ArduinoSlave.available() > 0) {
    char c = ArduinoSlave.read();
    answer += c;
    delay(10);
  }
}