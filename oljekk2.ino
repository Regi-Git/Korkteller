/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Esp.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/

#define NETTVERK_NAVN "Samfundet"
#define LCD_I2C_ADDR 0x27
LiquidCrystal_PCF8574 lcd(LCD_I2C_ADDR);


/* 
 *  Variabler som blir bruk til å registrere korker med laseren 
 *  og som sørger for debounce
*/

int forrige_kork_tid = 0;
bool kork_registrert = false;


int registrer_kork(){
  
   if ((WiFi.status() == WL_CONNECTED)) 
   {

      WiFiClient client;
      HTTPClient http;
  
      // configure url
      http.begin(client, "http://regi.samfundet.no/kork"); //HTTP
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // start connection and send HTTP header and body
      int httpCode = http.POST("1_OL=Tissetass");

      lcd.clear();
      lcd.setBacklight(255);
      lcd.setCursor(0, 1);
      lcd.print("-- Kork detektert -- ");
      lcd.setCursor(0, 2);
  
      if (httpCode > 0) 
      {
        if (httpCode == HTTP_CODE_OK) 
        {
          lcd.print("-- Kork registrert --");
          delay(2000);
          lcd.clear();
          lcd.setBacklight(0);
          return HTTP_CODE_OK;
        }
        
      } else 
      {
         lcd.clear();
         lcd.setCursor(0,1);
         lcd.print("!Kan ikke koble til!");
         lcd.setCursor(0,2);
         lcd.print("!----- RESTART ----!");
         delay(5000);
         ESP.restart();
         return httpCode;
      }

      http.end();
  } else
  {
     lcd.clear();
     lcd.setCursor(0,1);
     lcd.print("!Finner ikke server!");
     lcd.setCursor(0,2);
     lcd.print("!----- RESTART ----!");
     delay(5000);
     ESP.restart();
  }
  
}


void setup() 
{
  // Sett opp LCD-displayet
  
  lcd.begin(20, 4); 
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("LCD selvtest - OK");


  // Sett opp interrupt_pins på fotodioden
  
  attachInterrupt(digitalPinToInterrupt(14), tissetass, FALLING);
  pinMode(14, INPUT_PULLUP);
  forrige_kork_tid = millis();


  // Sett opp WiFi
  WiFi.begin(NETTVERK_NAVN);

  lcd.clear();
  lcd.home();
  lcd.print("Kobler til: ");
  lcd.setCursor(0,1);
  lcd.print(NETTVERK_NAVN);
  lcd.setCursor(0,2);
  
  int retry_count = 0;
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(3000);
    lcd.print(".");
    retry_count++;
    if(retry_count > 20){
       lcd.clear();
       lcd.home();
       lcd.print("!Kan ikke koble til!");
       lcd.setCursor(0,1);
       lcd.print(NETTVERK_NAVN);
       lcd.setCursor(0,2);
       lcd.print("!----- RESTART ----!");
       delay(5000);
       ESP.restart();
    }
  }
  
  lcd.clear();
  lcd.home();
  lcd.print("Koblet til!: ");
  lcd.setCursor(0,1);
  lcd.print(NETTVERK_NAVN);
  lcd.setCursor(0,2);
  lcd.print("IP: ");
  lcd.print(WiFi.localIP());

  delay(3000);
  lcd.clear();
  lcd.home();
  lcd.setCursor(0,1);
  lcd.print("--Korkteller klar --");
  delay(3000);
  lcd.setBacklight(0);
}


void loop() 
{
  // wait for WiFi connection
  if(kork_registrert)
  {
    kork_registrert = false; 
    registrer_kork();
  }
  delay(100);
}



ICACHE_RAM_ATTR void tissetass(){
  int tid = millis();

  if (tid-forrige_kork_tid > 1000) {
     forrige_kork_tid = tid;
     kork_registrert = true;
  }

}
