#define LED_PIN 6
#include <SoftwareSerial.h>

SoftwareSerial ArduinoMaster(10,11);

String msg = "";
int ledValue = 0;

int sensorValue = 0, oldSensorValue = 0;
bool isPressed = false;

void setup() {
  Serial.begin(9600);
  ArduinoMaster.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  readMasterPort();
  processMasterMsg();

  if(isPressed) {
    digitalWrite(LED_PIN, HIGH);
    oldSensorValue = -1;
  }else if(oldSensorValue != sensorValue){
    Serial.print("Master sent: ");
    Serial.println(sensorValue);
  
    ledValue = map(sensorValue, 0, 1023, 0, 255);
    Serial.print("led value : ");
    Serial.println(ledValue);
  
    ArduinoMaster.print(ledValue);
    analogWrite(LED_PIN, ledValue);
  
    oldSensorValue = sensorValue;      
  }
}

void readMasterPort() {
  while(ArduinoMaster.available() > 0) {
    char c = ArduinoMaster.read();

    msg += c;
    delay(10);
  }
  ArduinoMaster.flush();
}

void processMasterMsg() {
  if(msg.length() > 0) {
      isPressed = msg[0] == '1';

      char carray[6];
      for(int i = 2; i < msg.length(); i++) {
        carray[i-2] = msg[i];       
      }
      
      sensorValue = atoi(carray);
      msg = "";
  }
}
