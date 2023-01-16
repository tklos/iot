#include<stdio.h>
#include<string.h>
#define DEBUG true
  int pon=9;
  int poff=6;
  int lowp=5;

void setup()
{

  pinMode(pon, OUTPUT);
  pinMode(poff, OUTPUT);
  pinMode(lowp, OUTPUT);
  digitalWrite(poff, LOW);
  digitalWrite(lowp, HIGH);   
  digitalWrite(pon, HIGH);
  SerialUSB.begin(115200);
   while (!SerialUSB) {
    ; // wait for serial port to connect
  }
  Serial1.begin(115200); 
  digitalWrite(pon, LOW);    
  delay(3000); 
   digitalWrite(pon, HIGH);
   delay(5000); 
   sendData("AT+GPS=1 ",1000,DEBUG);
}

void loop()
{

    sendData("AT+GPSRD=1",1000,DEBUG);
   delay(1000);


}


String sendData(String command, const int timeout, boolean debug)
{
    String response = "";    
    Serial1.println(command); 
    long int time = millis();   
    while( (time+timeout) > millis())
    {
      while(Serial1.available())
      {       
        char c = Serial1.read(); 
        response+=c;
      }  
    }    
    if(debug)
    {
      SerialUSB.print(response);
    }    
    return response;
}


