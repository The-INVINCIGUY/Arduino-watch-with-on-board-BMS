#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Melopero_RV3028.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Wire.h>

//Defs
#define OLEDX 128
#define OLEDY 64
#define DC 9
#define CS 10
#define OLEDRESET -1
#define minimal 0
#define analogue 1
#define lowPoly 2
//temporary values, will find out button directions with a different sketch once hardware arrives
#define UP 255
#define DOWN 255
#define LEFT 255
#define RIGHT 255
#define CENTRE 6 //datasheet only confirms this pin

//variables
volatile bool onWatchFace = true;//TODO
volatile bool pinWoke = false;
volatile bool sleeping=true;
volatile bool RTC_wake=false;
uint8_t day=1;
uint8_t weekday=1;
uint8_t month=1;
uint16_t year=2026;
volatile uint8_t minute=0;
volatile uint8_t hour=0;
volatile uint8_t second=0;
uint8_t volumeAndContrast = 0b11111111; //TODO
uint8_t watchFace = 0;
volatile bool updateScreen = false;
bool lazyUpdate = true; //this means it doesnt sleep between as many screen updates and allows for more complex animations (TODO)
bool showYear = false; //TODO
Melopero_RV3028 rtc;

//Create display object
Adafruit_SSD1306 display(
    OLEDX,
    OLEDY,
    &SPI,
    DC,
    OLEDRESET,
    CS
);

ISR(PCINT0_vect) //pins 8-13, no need to check pin because it just needs to wakeup
{
    if (sleeping) sleeping = false;
    pinWoke = true;
    updateScreen = true;
}

ISR(PCINT2_vect) //pin 0-7
{
    if (sleeping) sleeping = false;
    pinWoke = true;
    updateScreen = true;
}

void wakeISR() {
    RTC_wake = true; // these have different uses
    updateScreen = true;
}

void minimalTime(){ //this function is only used directly before enterSleep() in the sleeping mode
    resetOLED();
    display.setCursor(34,24);
    if (hour<10){
        display.print(F("0"));
    }
    display.print(hour);
    display.print(F(":"));
        if (minute<10){
        display.print(F("0"));
    }
    display.print(minute);
    RTC_wake = false;
    display.display();
    if (minute==0){
        sleeping = false; 
    }
}

void resetOLED() {
    display.clearDisplay();
    display.setCursor(0,0);
}


void setup() {
    //Pin definitions
    pinMode(A6, INPUT); //This is battery measerment pin
    pinMode(2, INPUT_PULLUP); //D2 is special, because it is connected to the RTC
    attachInterrupt(digitalPinToInterrupt(2), wakeISR, FALLING);
    pinMode(3, OUTPUT);
    pinMode(4,INPUT_PULLUP);
    pinMode(5,INPUT_PULLUP);
    pinMode(CENTRE,INPUT_PULLUP); //pin 6 to F1
    pinMode(7,INPUT_PULLUP);
    pinMode(8,INPUT_PULLUP);
    //got ChatGPT to do the low level register settings
    PCICR |= (1 << PCIE2);     // Enable Port D (PCINT_2) pin change interrupts

    PCMSK2 |= (1 << PCINT20);  // D4
    PCMSK2 |= (1 << PCINT21);  // D5
    PCMSK2 |= (1 << PCINT22);  // D6
    PCMSK2 |= (1 << PCINT23);  // D7

    PCICR |= (1 << PCIE0);     // Enable Port B (PCINT_0) pin change interrupts
    PCMSK0 |= (1 << PCINT0);   // D8

    //RTC setup
    Wire.begin();
    rtc.initI2C();
    rtc.set24HourMode();
    rtc.setTime(year, month, weekday, day, hour, minute, second);

    //Display setup
    display.begin(SSD1306_SWITCHCAPVCC);
    resetOLED();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void resyncTime(){
    hour = rtc.getHour();
    minute = rtc.getMinute();
    second = rtc.getSecond();
    month = rtc.getMonth();
    day = rtc.getDate();
    weekday = rtc.getWeekday();
    year = rtc.getYear();
}

void enterSleep() {
    cli();
    if (not(sleeping)) { //resync time if sleeping is false
        minute = rtc.getMinute();
        hour = rtc.getHour();
    }
    sleeping = true;
    PCIFR |= (1 << PCIF0) | (1 << PCIF2);
    PCICR |= (1 << PCIE2) | (1 << PCIE0);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    rtc.enablePeriodicTimeUpdate(false, true);
    sei();
    sleep_cpu();
    sleep_disable();
}

void analogueTime(){
    uint16_t minuteAngle = map(minute,0,60,0,360);
    uint16_t hourAngle = map(hour%12,0,12,0,360);
    resetOLED();
    display.drawCircle(64, 32, 16, SSD1306_WHITE);
    display.drawLine(64,32,cos(radians(minuteAngle)),sin(radians(minuteAngle)),SSD1306_WHITE);
    display.drawLine(64,32,(cos(radians(hourAngle))*8)+64,(sin(radians(hourAngle))*8)+32,SSD1306_WHITE);
    display.display();
}

void analogueTimeSeconds(){
    uint16_t minuteAngle = map(minute,0,60,0,360);
    uint16_t hourAngle = map(hour%12,0,12,0,360);
    resetOLED();
    display.drawCircle(64, 32, 16, SSD1306_WHITE);
    display.drawLine(64,32,cos(radians(minuteAngle)),sin(radians(minuteAngle)),SSD1306_WHITE);
    display.drawLine(64,32,(cos(radians(hourAngle))*8)+64,(sin(radians(hourAngle))*8)+32,SSD1306_WHITE);
    display.display();
}

void loopUntilSleep() {
    onWatchFace = true;
    while (not(sleeping)){
        if (updateScreen&&onWatchFace) {
            if (watchFace==minimal){
                minimalTimeExtra();
            }
            if (watchFace==analogue){
                //todo
            }
            if (watchFace==lowPoly){
                //todo
            }
        }
        updateScreen = false;
        if (pinWoke) {
            //do some gpio stuff then sleep
            pinWoke = false;
        }

        if (RTC_wake){ //in here, it is every second
            RTC_wake = false;
            second++;
            if (second==60){
                resyncTime();
            }
        }

        enterSleep();
        sleeping = false; //prevents it from exiting loop after enter sleep
        if (digitalRead(CENTRE)==LOW&&onWatchFace){ 
            sleeping = true; //exit loop
        }
    }
    resyncTime();
}

void minimalTimeExtra() {
    resetOLED();
    resyncTime();
    display.setCursor(28,24);
    if (hour<10){
        display.print(F("0"));
    }
    display.print(hour);
    display.print(F(":"));
        if (minute<10){
        display.print(F("0"));

    }
    display.print(minute);
    display.print(F(":"));
    if (second<10) {
        display.print(F("0"));
    }
    display.print(second);
    display.setCursor(10,33);
    if (weekday==1){
        display.print(F("Monday"));
    }
    if (weekday==2){
        display.print(F("Tuesday"));
    }
    if (weekday==3){
        display.print(F("Wednesday"));
    }
    if (weekday==4){
        display.print(F("Thursday"));
    }
    if (weekday==5){
        display.print(F("Friday"));
    }
    if (weekday==6){
        display.print(F("Saturday"));
    }
    if (weekday==7){
        display.print(F("Sunday"));
    }
    display.print(F(", "));
    display.print(day);
    display.print(F(" of "));
    display.print(month);
    if (showYear){
        display.setCursor(12,40);
        display.print(F("Year: "));
        display.print(year);
    }
    display.display();
}

void lowPolyTime(){
    display.setTextSize(2);
    //TODO
    display.setTextSize(1);
}

void loop() {
    if (pinWoke){ 
        pinWoke = false;
        resyncTime();//done once here
        rtc.enablePeriodicTimeUpdate(true, true);//done once here
        loopUntilSleep();
        sleeping = false; //reset to false so time syncs minute and hour before sleep
    }
    if (RTC_wake){ //only when UI is sleep mode
        minute++; //it will re
        if (minute==60){
            hour++;
            minute=0;
            if (hour==24){
                hour=0; //resync after here
            }
        }
    if (watchFace==minimal){
        minimalTime();
    }
    if (watchFace==analogue){
        //todo
    }
    if (watchFace==lowPoly){
        //todo
    }
    enterSleep();
    }
}
