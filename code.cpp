#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <RTClib.h>
#include <EEPROM.h>
int pushVal = 0;                           
int val;
int val2;
int addr = 0;
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0X27,16,2);
#define getWellsoon 0                                           
#define HELP_SCREEN 1
#define TIME_SCREEN 2
int pushpressed = 0;
const int ledPin =  LED_BUILTIN;                        
int ledState = LOW;
int Signal = 0;
int buzz = 13;                                      
int push1state, push2state, push3state, stopinState = 0;    
int push1Flag, push2Flag, Push3Flag = false;              
int push1pin = 9;
int push2pin = 8;
int push3pin = 7;
int stopPin = A0;
int screens = 0;              
int maxScreen = 2;            
bool isScreenChanged = true;
long previousMillis = 0;           
long interval = 500;                   
unsigned long currentMillis;
long previousMillisLCD = 0;    
long intervalLCD = 2000;          
unsigned long currentMillisLCD;
int buzz8amHH = 8;           
int buzz8amMM = 00;          
int buzz8amSS = 00;          
int buzz2pmHH = 14;          
int buzz2pmMM = 00;          
int buzz2pmSS = 00;          
int buzz8pmHH = 20;         
int buzz8pmMM = 00;          
int buzz8pmSS = 00;          
int nowHr, nowMin, nowSec;                   
void gwsMessege()
{               
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Smart :)");    
    lcd.setCursor(0, 1);
    lcd.print("Medicine reminder");     
}
void helpScreen() 
{              
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("set medicine");
    lcd.setCursor(0, 1);
    lcd.print("reminder time");
 }
void timeScreen() 
{              
    DateTime now = rtc.now();             
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time:");
    lcd.setCursor(6, 0);
    lcd.print(nowHr = now.hour(), DEC);
    lcd.print(":");
    lcd.print(nowMin = now.minute(), DEC);
    lcd.print(":");
    lcd.print(nowSec = now.second(), DEC);
    lcd.setCursor(0, 1);
    lcd.print("Date: ");
    lcd.print(now.day(), DEC);
    lcd.print("/");
    lcd.print(now.month(), DEC);
    lcd.print("/");
    lcd.print(now.year(), DEC);
}
void setup() {
                      
  if (! rtc.begin()) {                      
    
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
  }
          
  rtc.adjust(DateTime(2019, 1, 10, 7, 59, 30));            
  lcd.begin();
  delay(1000);
  pinMode(push1pin, INPUT);                                   
  pinMode(push2pin, INPUT);
  pinMode(push3pin, INPUT);
  pinMode(stopPin, INPUT);
  pinMode(ledPin, OUTPUT);
  delay(200);
  Serial.println(EEPROM.read(addr));
  val2 = EEPROM.read(addr);                        
 
}
void loop() 
{
  push1();                                             
  push2();                                             
  push3();                                            
    if (pushVal == 1) 
    {                               
    at8am();                                            
  }
  else if (pushVal == 2) 
  {                             
    at8am();                                            
    at8pm();                                           
  }
  else if (pushVal == 3)
  {                             
    at8am();
    at2pm();                                            
    at8pm();
  }
  currentMillisLCD = millis();                        
  push1state = digitalRead(push1pin);                  
  push2state = digitalRead(push2pin);
  push3state = digitalRead(push3pin);
  stopinState = digitalRead(stopPin);
  stopPins();                                            
  changeScreen();                                       
}

void push1() 
{                   
  if (push1state == 1) 
  {
    push1state = 0;
    push2state = 0;
    push3state = 0;
    EEPROM.write(addr, 1);
    Serial.print("Push1 Written : "); 
    Serial.println(EEPROM.read(addr)); 
    pushVal = 1;                                            
    
  }
}
void push2() 
{                     
  if (push2state == 1) 
  {
    push2state = 0;
    push1state = 0;
    push3state = 0;
    EEPROM.write(addr, 2);
    Serial.print("Push2 Written : "); 
    Serial.println(EEPROM.read(addr));
    pushVal = 2;
   
  }
}
void push3() 
{                    
  if (push3state == 1) 
  {
    push3state = 0;
    push1state = 0;
    push2state = 0;
    EEPROM.write(addr, 3);
    Serial.print("Push3 Written : ");
    Serial.println(EEPROM.read(addr));
    pushVal = 3;
    
  }
}
void stopPins() {                   
  if (stopinState == 1) 
  {
    pushpressed = 1;
    
  }
}
void startBuzz()
{                    
 if (pushpressed == 0) 
 {
    Serial.println("pushpressed is false in blink");
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;        
      Serial.println("Start Buzzing");
      if (ledState == LOW) 
      {                  
        ledState = HIGH;
      }  else 
      {
        ledState = LOW;
      }
      digitalWrite(ledPin, ledState);
    }
  }
  else if (pushpressed == 1) 
  {
    Serial.println("pushpressed is true");
    ledState = LOW;
    digitalWrite(ledPin, ledState);
  }
}
void at8am() 
{                      
  DateTime now = rtc.now();
  if (int(now.hour()) >= buzz8amHH) {
    if (int(now.minute()) >= buzz8amMM) {
      if (int(now.second()) > buzz8amSS) {
        
        startBuzz();
        
      }
    }
  }
}
void at2pm() {                          
  DateTime now = rtc.now();
  if (int(now.hour()) >= buzz2pmHH) {
    if (int(now.minute()) >= buzz2pmMM) {
      if (int(now.second()) > buzz2pmSS) {
        
        startBuzz();
        
      }
    }
  }
}
void at8pm() {                           
  DateTime now = rtc.now();
  if (int(now.hour()) >= buzz8pmHH) {
    if (int(now.minute()) >= buzz8pmMM) {
      if (int(now.second()) > buzz8pmSS) {
        
        startBuzz();
        
      }
    }
  }
}

void changeScreen() {                 
  
  if (currentMillisLCD - previousMillisLCD > intervalLCD)             
  {
    previousMillisLCD = currentMillisLCD;
    screens++;
    if (screens > maxScreen) {
      screens = 0;  
    }
    isScreenChanged = true;
  }

  if (isScreenChanged)   
  {
    isScreenChanged = false; 
    switch (screens)
    {
      case getWellsoon:
        gwsMessege();                
        break;
      case HELP_SCREEN:              
        helpScreen();               
        break;
      case TIME_SCREEN:
        timeScreen();                 
        break;
      default:
        
        break;
    }
  }
}