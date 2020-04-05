#include <SevenSegmentFun.h>
#include <SevenSegmentTM1637.h>
#include <SevenSegmentExtended.h>
#include <virtuabotixRTC.h>

const int CLK_Display = 6; //Set the CLK pin connection to the display
const int DIO_Display = 5; //Set the DIO pin connection to the display
 
String timeStr = "0000";
int semiColon = 0;
int btnMinute = 0;
int btnHour = 0;
int hour = 0;
int minute = 0;
String hourStr = "00";
String minuteStr = "00";

virtuabotixRTC myRTC(2, 3, 4);

SevenSegmentTM1637    display(CLK_Display, DIO_Display);
 
void setup()
{
  //myRTC.setDS1302Time(15, 54, 1, 7, 14, 1, 2018);
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  display.print("INIT");      // display INIT on the display
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  Serial.begin(9600);
}
 
void loop()
{
  myRTC.updateTime();
  hour = myRTC.hours;
  minute = myRTC.minutes;

  btnMinute = digitalRead(8);
  btnHour = digitalRead(7);
  if (btnMinute) { 
    minute = minute + 1;
    if (minute == 60) {
      minute = 0;
      hour = hour + 1;
      if (hour == 24)
        hour = 0;      
    }
    myRTC.setDS1302Time(15, minute, hour, 7, 14, 1, 2018);
    myRTC.updateTime();   
  }
  if (btnHour) {
    Serial.println("btnHour pressed");
    Serial.println(hour);
    hour = hour + 1;
    if (hour == 24)
      hour = 0;  
    Serial.println(hour);
    myRTC.setDS1302Time(15, minute, hour, 7, 14, 1, 2018);
    myRTC.updateTime();
  }
  
  hour = myRTC.hours;
  if (hour < 10)
    hourStr = "0" + String(hour);
  else
    hourStr = String(hour);
  minute = myRTC.minutes;
  if (minute < 10)
    minuteStr = "0" + String(minute);
  else
    minuteStr = String(minute);
  timeStr = hourStr + minuteStr;
  //time = myRTC.hours * 100 + myRTC.minutes;
  //time = 1235;
  timeStr = "1235";
  display.print(timeStr);       
  semiColon = semiColon + 1;
  if (semiColon == 6)
    semiColon = 0;
  display.setColonOn(semiColon > 3);
      
  //Serial.println(semiColon);
  delay(200);   
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(1000);
//  digitalWrite(LED_BUILTIN, LOW);
}
