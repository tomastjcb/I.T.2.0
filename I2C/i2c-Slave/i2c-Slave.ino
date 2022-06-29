#include <Wire.h>

#define SLAVE_ADDR 9

#define ANSWERSIZE 5

String answer = "Hello";

void setup() {
  Wire.begin(SLAVE_ADDR);

  Wire.onRequest(requestEvent);

  Wire.onReceive(receiveEvent);

  Serial.begin(9600);
  Serial.println("I2C Slave Demonstration");   
}

void receiveEvent(){
  while (0<Wire.available()){
    byte x = Wire.read();
  }
  Serial.println("Receive event");
}

void requestEvent(){
  byte response[ANSWERSIZE];

  for (byte i=0;i<ANSWERSIZE;i++){
    response[i] = (byte)answer.charAt(i);
  }

  Wire.write(response,sizeof(response));

  Serial.println("Request event");
}

void loop() {
  delay (50);
}
