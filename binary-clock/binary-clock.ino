#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc; // create rtc for the DS3231 RTC module
const int hourPins[5] = { 9, 10, 11, 12, 13 };
const int minutePins[6] = { 2, 3, 4, 5, 6, 7 };
const int secondPins[6] = { 0, 1, 14, 17, 16, 15 };
const int switchPin = 8;

void setup() {
  // setup code here, to run once:
  // Serial.begin(9600); // commented out to load onto the arduino uno; the uno doesn't have enough pins to use Serial and display seconds
  rtc.begin();

  // assorted rest of the pins, second
  for (int pin : secondPins) {
    pinMode(pin, OUTPUT);
  }

  // pins 2-7, minute
  // pin 2 corresponds to the "1" bit of the output; pin 7 corresponds to the "32" bit of the output
  for (int pin : minutePins) {
    pinMode(pin, OUTPUT);
  }

  // pins 9-13, hour
  // pin 9 corresponds to the "1" bit of the output; pin 13 corresponds to the "16" bit of the output
  for (int pin : hourPins) {
    pinMode(pin, OUTPUT);
  }

  pinMode(switchPin, INPUT);

  Serial.println("Pins assigned");
}

void loop() {
  updateOutputLEDs();

  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'u') updateRTC();  // update RTC time
  }

  
  if (rtc.lostPower()) {
    Serial.println("RTC has lost power. Please reset the time by typing 'u'");
    
    for (int pin : minutePins) {
      digitalWrite(pin, HIGH);
    }

    for (int pin : hourPins) {
      digitalWrite(pin, HIGH);
    }

    delay(1000);
    
    for (int pin : minutePins) {
      digitalWrite(pin, LOW);
    }

    for (int pin : hourPins) {
      digitalWrite(pin, LOW);
    }
    
    delay(5000);
  }
}

void updateRTC() { // modified from https://www.circuitbasics.com/how-to-use-a-real-time-clock-module-with-the-arduino/
  // ask user to enter new date and time
  const char txt[6][15] = { "year [4-digit]", "month [1~12]", "day [1~31]",
                            "hours [0~23]", "minutes [0~59]", "seconds [0~59]"};
  String inputStr = "";
  long newDate[6] = { 0, 0, 0, 0, 0, 0 };

  Serial.println("Updating time...");

  while (Serial.available()) {
    Serial.read();  // clear serial buffer
  }

  for (int i = 3; i <= 5; i++) {
    // only ask for hour and minute
    Serial.print("Enter ");
    Serial.print(txt[i]);
    Serial.print(": ");

    while (!Serial.available()) {
      ; // wait for user input
    }

    inputStr = Serial.readString();  // read user input
    newDate[i] = inputStr.toInt();   // convert user input to number and save to array

    Serial.println(newDate[i]); // show user input
  }

  // update RTC
  rtc.adjust(DateTime(newDate[0], newDate[1], newDate[2], newDate[3], newDate[4], newDate[5]));
  Serial.println("RTC Updated!");
}

void updateOutputLEDs() {
  DateTime now = rtc.now(); // read rtc current time
  int hour = now.hour(); // get current hour
  int minute = now.minute(); // and current minute
  int second = now.second(); // and current second

  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

  Serial.print("hours: ");
  for (int i = 4; i >= 0; i--) { // loop over indices from 4 to 0 (most significant to least significant bits)
    int pin = hourPins[i]; // target pin
    int state = (( 1 << i ) & ( hour )) >> i; // returns boolean whether the lamp should be lit or not
    Serial.print(state); // append the state to the current line
    digitalWrite(pin, state); // set the pin to the state
  }

  Serial.print(" minutes: ");
  for (int i = 5; i >= 0; i--) {
    int pin = minutePins[i];
    int state = (( 1 << i ) & ( minute )) >> i; // returns boolean whether the lamp should be lit or not
    Serial.print(state);
    digitalWrite(pin, state);
  }


  int secondSwitchState = digitalRead(switchPin);
  Serial.print(" seconds: ");
  for (int i = 5; i >= 0; i--) {
    int pin = secondPins[i];
    int state = (( 1 << i ) & ( second )) >> i; // returns boolean whether the lamp should be lit or not
    Serial.print(state);
    digitalWrite(pin, (state & secondSwitchState));
  }

  Serial.println(" ");
  Serial.print("second switch: ");
  Serial.println(secondSwitchState);
  Serial.println(" ");
  
  delay(1000);
}