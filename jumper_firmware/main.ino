#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Motor.h>
#include <Debugger.h>
#include <WController.h>
#include <PServer.h>

//NOT SECURE DOESN'T USE SSL  
extern WiFiClient debugCon;

const char* ssid = "<YourSSIDHere>";
const char* pkey = "<YourPasswordHere>";

Motor lmotor(0,1,1023);
Motor rmotor(2,3,1023);

PServer ps(8080);
WController wc(&lmotor,&rmotor,&ps);

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pkey);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {

    delay(5000);
    ESP.restart();
  }
  debugCon.connect("192.168.2.2",42069);
  dlogln("Booted");

  ArduinoOTA.begin();

  IPAddress ip=WiFi.localIP();
  dlogln("Ready\nIP:"+String("")+ip[0]+"."+ip[1]+"."+ip[2]+"."+ip[3]);
  //char p[100];

 // dlogln("Hjello");
  //dlogln(String(*((int*)p)));
  //delay(10000);
}

void loop() {

  if(!debugCon.connected())
  {
      debugCon.connect("192.168.2.2",42069);
      dlogln("Connected");
  }
  ps.listen();
  ArduinoOTA.handle();
}
