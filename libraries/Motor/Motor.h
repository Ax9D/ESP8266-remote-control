#pragma once
#include <Direction.h>
class Motor
{
    private:
        byte fpin,bpin;
        Direction dir;
    public:
        Motor(byte fpin,byte bpin,short maxSpeed);
        void forward(float speed);
        void reverse(float speed);
        void forward();
        void reverse();
        void stop();
        short maxSpeed;
};