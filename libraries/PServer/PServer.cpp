#include <PServer.h>
#include <Debugger.h>
#include <cstring>


PServer::PServer(int port):server(port)
{
    pkSt=HEADER;
    connected=false;
    server.begin();
    cstate=Unauthenticated;
    packet=new char[PACKET_BUFFER_SIZE];
}
void PServer::onPacket(packetType pt, std::function<void()> lambda)
{
    packet_handler_functions[pt]=lambda;
}       
void PServer::send(String s)
{
    currentClient.print(s);
}
void PServer::listen()
{
    WiFiClient newClient=server.available();

    if(!connected && newClient)
    {
        dlogln("New Connection");
        currentClient=newClient;
        connected=true;
        last_con_time=millis();
    }

    if(currentClient.connected())
    {
        if(cstate==Unauthenticated && millis()-last_con_time>=timeout)
            reset();
        
        {
        switch(pkSt)
        {
            case HEADER:
                /*
                    HEADER FORMAT:
                    [ 4 bytes ] [ 1 byte ] [                 n bytes                ]
                        TYPE    <typecode> <packet of size corresponding to typecode>
                */
                if(currentClient.available()>=5)
                    {   
                        char head_container[5];
                        currentClient.peekBytes(head_container,5);

                        if(!std::strncmp(head_container,"TYPE",4))
                        {
                            short typeCode=head_container[5 - 1]-'0';
                            //If it is a valid packet type
                            if(typeCode>=0 && typeCode < N_TYPES)
                            {
                                pkSt=PACKET;
                                pkTp=(packetType)(typeCode);
                                
                                
                                
                                currentClient.readBytes(head_container,5 - 1);
                            }
                            else
                                currentClient.read();//Move 1 byte forward
                        }
                            currentClient.read();//Move 1 byte forward
                    }
            break;

            case PACKET:
                if(currentClient.available()>=psizes[pkTp])
                {
                    currentClient.readBytes(packet,psizes[pkTp]);
                    pkSt=HEADER;

                    //dlogln(packet);

                    packet_handler_functions[pkTp]();
                }

            break;
        }
    }
    }
    else
        reset();
    
}
void PServer::onDisconnect(std::function<void()> lambda)
{
    disconnect_lambda=lambda;
}
void PServer::reset()
{
    pkSt=HEADER;
    connected=false;
    cstate=Unauthenticated;
    
    currentClient.stop();
    disconnect_lambda();
}
