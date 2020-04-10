#include <Arduino.h>
#include <Debugger.h>

WiFiClient debugCon;
void dlog(String data)
{
   debugCon.print(data);
}
void dlogln(String data)
{
   debugCon.println(data);
}