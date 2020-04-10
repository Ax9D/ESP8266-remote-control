#include <Arduino.h>
#include <Debugger.h>
#include "Motor.h"

Motor::Motor(byte fpin,byte bpin,short maxSpeed)
{
    this->fpin=fpin;
    this->bpin=bpin;
    pinMode(fpin,OUTPUT);
    pinMode(bpin,OUTPUT);
    analogWrite(fpin,0);
    analogWrite(bpin,0);
    this->maxSpeed=maxSpeed;
}

void Motor::forward(float speed)
{
    //These checks are probably unnecessary, done to save execution cycles
    if(dir==Direction::rev)
    {
        analogWrite(bpin,0);
        dir=Direction::forward;
    }
   // dlogln("analogWrite to pin:"+);
    analogWrite(fpin,(short)(speed*maxSpeed));
}
void Motor::reverse(float speed)
{ 
    if(dir==Direction::forward)
    {
        analogWrite(fpin,0);
        dir=Direction::rev;
    }
   // dlogln("analogWrite to pin:"+String(bpin));
    analogWrite(bpin,(short)(speed*maxSpeed));
}
void Motor::forward()
{
    if(dir==Direction::rev)
    {
        //analogWrite(bpin,0);
        dir=Direction::forward;
    }
   // dlogln("analogWrite to pin:"+);
    analogWrite(fpin,maxSpeed);
}
void Motor::reverse()
{
    if(dir==Direction::forward)
    {
        analogWrite(fpin,0);
        dir=Direction::rev;
    }
   // dlogln("analogWrite to pin:"+String(bpin));
    analogWrite(bpin,maxSpeed);
}
void Motor::stop()
{
    if(dir==Direction::forward)
    analogWrite(fpin,0);
    else if(dir==Direction::rev)
    analogWrite(bpin,0);
    
}