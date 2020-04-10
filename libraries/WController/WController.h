#pragma once
#include <Arduino.h>
#include <Motor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Control_def.h>
#include <PServer.h>

class WController
{
    private:
        Motor* lmotor;
        Motor*  rmotor;
        PServer* server;
        float lpercent;
        float rpercent;
        
        float turn_adjust_min;
        float turn_adjust_max;

        unsigned long beacon_period=5000;
        short time_since_beacon;
    public:
        WController(Motor* lmotor,Motor* rmotor,PServer* server);
        void reset();
        static constexpr const char* auth_key="24y4novrOt5rr1x!NdWkume2L7PtmLoF"; //I don't know wtf this is 
};
