#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

#include <SevenSegmentFun.h>
#include <SevenSegmentTM1637.h>
#include <SevenSegmentExtended.h>
#include <virtuabotixRTC.h>

const int CLK_Display = 6; //Set the CLK pin connection to the display
const int DIO_Display = 5; //Set the DIO pin connection to the display
const int btnHourPin = 2;
const int btnMinutePin = 3;

int hourInt = 0;
int minuteInt = 0;
String timeStr = "0000";
String hourStr = "00";
String minuteStr = "00";
int semiColon = 0;
tmElements_t tm;
bool rtcIsReady = false;
int btnHourValue = 0;
int btnMinuteValue = 0;
int btnHourLastValue = 0;
int btnMinuteLastValue = 0;
int btnHourPressCount = 0;
int btnMinutePressCount = 0;

SevenSegmentTM1637    display(CLK_Display, DIO_Display);

void setup() {
  Serial.begin(9600);

  pinMode(btnHourPin, INPUT);
  pinMode(btnMinutePin, INPUT);

  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  display.print("INIT");      // display INIT on the display
}

void displayTime() {
  hourInt = tm.Hour;
  if (hourInt < 10)
    hourStr = "0" + String(hourInt);
  else
    hourStr = String(hourInt);
  minuteInt = tm.Minute;
  if (minuteInt < 10)
    minuteStr = "0" + String(minuteInt);
  else
    minuteStr = String(minuteInt);
  timeStr = hourStr + minuteStr;
  display.print(timeStr);       
}
void readTime() {
  //Serial.println("okuyorum");
  if (RTC.read(tm)) {
    //Serial.println("okudum");
    rtcIsReady = true; 
    displayTime();   
  } else {
    if (RTC.chipPresent()) {
      tm.Hour=0;tm.Minute=0;tm.Second=0;tm.Day=1;tm.Month=1;tm.Year=2020;
      if (RTC.write(tm)) {
        rtcIsReady = true;
        readTime();
      }
      //Serial.println("initialized");
    } else {
      //read error
      //Serial.println("read Error");
    }
  }
}

void increaseHour() {
  int value;
  value = tm.Hour;
  value = value + 1;
  if (value == 24)
    value = 0;
  tm.Hour = value;
  RTC.write(tm);
}

void btnClicked(String btnName) {
  Serial.println(btnName);
  int value;
  if (btnName == "Hour") {
    increaseHour();    
  } else {
    value = tm.Minute;
    value = value +1;
    if (value == 60) {
      value = 0;
      increaseHour();
    }
    tm.Minute = value;
    RTC.write(tm);
  }
  displayTime();
}

void checkButtonClicked(String btnName){
  int *btnPin;
  int btnValue;
  int *btnLastValue;
  int *btnPressCount;

  if (btnName == "Hour") {
    btnPin = &btnHourPin;
    btnLastValue = &btnHourLastValue;
    btnPressCount = &btnHourPressCount;
  } else {
    btnPin = &btnMinutePin;
    btnLastValue = &btnMinuteLastValue;
    btnPressCount = &btnMinutePressCount;  
  }
  //Serial.println(*btnPin);
  btnValue = digitalRead(*btnPin);

  if (btnValue) {
    if (!(*btnLastValue))
      btnClicked(btnName);
    else {
      if ((*btnPressCount)>5)
        btnClicked(btnName);
      (*btnPressCount) = (*btnPressCount) + 1;
    }    
  } else {
    *btnPressCount = 0;
  }
  *btnLastValue = btnValue;
}

void checkButtons() {
  if (!rtcIsReady)
    return;
  checkButtonClicked("Hour");
  checkButtonClicked("Minute");
}
 
void loop() {
  checkButtons();
  readTime();
  semiColon = semiColon + 1;
  if (semiColon == 6)
    semiColon = 0;
  display.setColonOn(semiColon > 3);
  delay(200);
}

