#include<Wire.h>

#define SLAVE_ADDR 9 

#define ANSWERSIZE 5
void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(9600);
  Serial.println("I2C Master Demosntration");
}

void loop() {
  delay(50);
  Serial.println("Wire data to slave");

  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(0);
  Wire.endTransmission();

  Serial.println("Receive Data");

  Wire.requestFrom(SLAVE_ADDR,ANSWERSIZE);

  String response = "";
  while (Wire.available()){
    char b = Wire.read();
    response += b;
  }
  Serial.println(response);
}
