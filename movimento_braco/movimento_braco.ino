

#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h" 
#include <Servo.h>

#define SP_PIN 5 //servo_conetor
Servo p;
#define SC_PIN 9 //servo_conetor
Servo m;
#define SB_PIN 10 //servo_base
Servo b;

float coef_velocity_float = 0;
int coef_velocity = 0;
int velocidade = 0;
int contador_de_posicao = 0;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key index number (needed only for WEP)




int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  
  Serial.begin(9600); //set serial monitor baud rate to match

  
  p.write(90); //sentido rotaçao 90_clw 180_ccw
  p.attach(SP_PIN);

  m.write(90); //sentido rotaçao 90_clw 180_ccw
  m.attach(SC_PIN);
  
  b.write(90); //sentido rotaçao 90_clw 180_ccw
  b.attach(SB_PIN);



  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Access Point Web Server");

//  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // by default the local IP address will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}


void loop() {

  
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }
  
  WiFiClient client = server.available();   // listen for incoming clients

  
   if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      
      //por default estava a 10 ms mas tava a dar problemas assim embora esteja com delay parece estar a funcionar
      delayMicroseconds(10);                // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
//            client.println("HTTP/1.1 200 OK");
//            client.println("Content-type:text/html");
//            client.println();


            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            // Seja o link http://192.168.4.1/p_c/xxx
// O http request vai ser GET /p_c/xxx entao o substring(14 a 20 sao os numeros) a começar no 14 e o 20 ja não está incluido     
                        //012345678901234
         

   //mensagem do tipo GET /a_b/xxx
            String x = currentLine.substring(9,12); 
            int x_int = x.toInt();
            coef_velocity =90 *  x_int * 0.01 ;                // devolve um valor entre 0 e 100 entao multiplica-se por 0.01 para dar um valor de 0 a 1
//            coef_velocity = coef_velocity_float.toInt;
        
       //PONTA
        if (currentLine.startsWith("GET /p_c")) {
          
          contador_de_posicao = contador_de_posicao + 10 ;
          p.write(contador_de_posicao);      
        }

        if (currentLine.startsWith("GET /p_b")) {
          
          contador_de_posicao = contador_de_posicao - 10;
          p.write(contador_de_posicao);      
        }

        //CONETOR
        if (currentLine.startsWith("GET /c_c")) {

          velocidade = 90 + coef_velocity;
          m.write(velocidade);      
        }
        
        if (currentLine.startsWith("GET /c_b")) {

          velocidade = 90 - coef_velocity ;
          m.write(velocidade);      
        }

        //BASE

        if (currentLine.startsWith("GET /b_e")) {

          velocidade = 90 + coef_velocity;
          b.write(velocidade);      
        }
        
        if (currentLine.startsWith("GET /b_d")) {
          
          velocidade = 90 - coef_velocity ;
          b.write(velocidade);      
        }

        if (currentLine.startsWith("GET /stop")) {
          
          
         // p.write(90);
          m.write(90);
          b.write(90);      
        }
       


            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }


        

      }
    }
    // close the connection:
    client.stop();
   // Serial.println("client disconnected");
  }

}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  //Serial.print("To see this page in action, open a browser to http://");
  //Serial.println(ip);

}
