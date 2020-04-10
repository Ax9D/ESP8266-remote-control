#include <WController.h>
#include <Debugger.h>

WController::WController(Motor* lmotor, Motor* rmotor,PServer* server)
{
    this->lmotor=lmotor;
    this->rmotor=rmotor;
    this->server=server;
    time_since_beacon=millis();

    server->onDisconnect([this](){
        this->reset();
    });

    server->onPacket(AUTH,[this,server](){
    if(server->cstate==Unauthenticated)
        {
            
            if(strncmp( ((Auth_Packet*) server->packet )-> key , auth_key ,32 ) )
            {
                //Auth failed
                dlogln("Auth Failed");
                server->reset();
                server->send(String("Auth Failed"));
            }
            else
            {
                server->cstate=Authenticated;
                dlogln("Authenticated");
                server->send(String("\n"));
            }
            
        }
    });

    server->onPacket(STEER,[this,server](){
    if(server->cstate==Authenticated)
        {
            Steer_Packet* sp=(Steer_Packet*)( server->packet);

            //sp->mstate;

            dlogln("STEER");
            dlogln(String("Movement State ")+sp->mstate);
            dlogln(String("Turn State ")+sp->tstate);

            switch(sp->tstate)
            {
                case Left:
                    lpercent=0.5f;
                    rpercent=1.0f;
                break;
                case Right:
                    lpercent=1.0f;
                    rpercent=0.5f;
                break;
                case Idle:
                    lpercent=rpercent=1.0f;
                break;
            }
            switch(sp->mstate)
            {
                case Forward:
                    this->lmotor->forward(lpercent);
                    this->rmotor->forward(rpercent);
                break;
                case Reverse:
                    this->lmotor->reverse(lpercent);
                    this->rmotor->reverse(rpercent);
                break;
                case Stat:
                    this->lmotor->stop();
                    this->rmotor->stop();
                break;

            }
        }
        else
            server->reset();
        
    });

    server->onPacket(BEACON,[this,server](){
    if(server->cstate==Authenticated)
        {
            dlogln("BEACON");
            unsigned long now=millis();
            if(now-time_since_beacon>=beacon_period)
            {
                reset();
                time_since_beacon=now;
           }
            //sp->mstate;

        }
        else
            server->reset();
        
    });
    server->onPacket(ADJUST,[this,server](){
    if(server->cstate==Authenticated)
        {
            Adjust_Packet* ap=(Adjust_Packet*)server->packet;
            dlogln("ADJUST");
            dlogln(String("lmax")+ap->lmax);
            dlogln(String("rmax")+ap->rmax);

            if(ap->lmotor)
                this->lmotor->maxSpeed=ap->lmax;
            if(ap->rmotor)
                this->lmotor->maxSpeed=ap->rmax;
        }
        else
            server->reset();
        
    });
}
void WController::reset()
{
    lpercent=rpercent=1;
    lmotor->stop();
    rmotor->stop();
}
void handle_auth(PServer* ps, WController* wc,char* packet,char len)
{
   
}