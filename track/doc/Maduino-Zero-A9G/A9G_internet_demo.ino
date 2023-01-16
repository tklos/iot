#include<stdio.h>
#include<string.h>
#include <DHT.h>
#include <DHT_U.h>

#define DEBUG true
  int pon=9;
  int poff=6;
  int lowp=5;
  bool A9G_ON = false;
//*******for senser **********
float humi;
float temp;
#define DHTPIN 3
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);


//**********api write key******************

const String apiKey="Y453P5LY8SFRZ4DU";


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

//********check open************
sendData("AT",1000,DEBUG);
delay(1000);
//******************************

//********open*********
if (!A9G_ON)
{
  digitalWrite(pon, LOW);    
  delay(3000); 
   digitalWrite(pon, HIGH);
   SerialUSB.println("opening A9G");
   delay(16000);
} 
//**********************

  dht.begin();

//**************Open internet connection*************************  
sendData("AT+CGATT=1",1000,DEBUG);
delay(2000); 
//***************************************************************
   
}

void loop()
{
  
humi = dht.readHumidity();
                     
temp = dht.readTemperature();

SerialUSB.println("humi is " + (String)humi);
SerialUSB.println("temp is " + (String)temp);

String command = "AT+HTTPGET=\"http://api.thingspeak.com/update.json?api_key=" + apiKey + "&field1=" + (String)temp +"&field2=" + (String)humi + "\"";

SerialUSB.println(command);

sendData(command,3000,DEBUG);

delay(60000);
}


String sendData(String command, const int timeout, boolean debug)
{
    String response = "";    
    Serial1.println(command);

    String commandpre = getcommand_pref(command);
    //SerialUSB.println(commandpre);

    long int time = millis();   
    while( (time+timeout) > millis())
    {
       if(commandpre == "AT"){
       
       if(Serial1.find("OK")){
        A9G_ON = true;
       SerialUSB.println("A9G open");
       }
      }

      while(Serial1.available())
      {       
        String c = Serial1.readString(); 
        response+=c;
      } 
   }   

    if(debug)
    {
      SerialUSB.println(response);
    }
        
    return response;
}


String getcommand_pref(String command)
{
    String command_pref = "";
    char *cstr = new char[command.length() + 1];
    strcpy(cstr, command.c_str());
    char * token = strtok(cstr,"=");
    int i = 0;
    while(token != NULL)
    {
      //SerialUSB.print(token);
      //SerialUSB.println("  line" + (String)i);

      switch(i) {
        case 0:
        command_pref = token;
        break;
        
        default:
        break;
      }

      token = strtok(NULL,",");
      i = i+1;
    }

    if(command_pref == "")
    command_pref = command;
    
    return command_pref;
}
