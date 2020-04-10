#pragma once
#include <ESP8266WiFi.h>
#include <Control_def.h>
#include <functional>
#include <map>

#define N_TYPES 5
#define PACKET_BUFFER_SIZE 100

class Auth_Packet
{
    public:
    char key[32];
};
class Steer_Packet
{
    public:
    MovementState mstate;
    TurnState tstate;
};
class Beacon_Packet
{
    public:
    char text[4]; 
};
class Adjust_Packet
{
    public:
    bool lmotor;
    float lpercent;
    short lmax;
    bool rmotor;
    float rpercent;
    short rmax;
};
class System_Packet
{
    //TODO
    public:
    int x;
};
enum packetType{AUTH=0,STEER=1,BEACON=2,ADJUST=3,SYSTEM=4};

class PServer
{
    private:
        enum packetState{HEADER,PACKET} pkSt;
        packetType pkTp;
        WiFiServer server;
        WiFiClient currentClient;
        bool connected;
        int last_con_time;
        static const int timeout=2000;


        std::map<packetType,std::function<void()>> packet_handler_functions;
        short psizes[5]={sizeof(Auth_Packet),sizeof(Steer_Packet),sizeof(Beacon_Packet),sizeof(Adjust_Packet),sizeof(System_Packet)};

        std::function<void()> disconnect_lambda;

    public:
        PServer(int port);
        void listen();
        int hasPacket();
        void getPacket(char* pkt);
        int validPacketType(char pktTyp[4]);

        void onPacket(packetType pt, std::function<void()> lambda);
        void onDisconnect(std::function<void()> lambda);
        char* packet;
        ConnectionState cstate;
        void reset();
        void send(String s);

};