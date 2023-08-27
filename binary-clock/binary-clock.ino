const int minutePins[6] = { 2, 3, 4, 5, 6, 7 };
const int hourPins[5] = { 9, 10, 11, 12, 13 };

void setup() {
  // setup code here, to run once:
  Serial.begin(9600);

  // pins 2-7, minute
  // pin 2 corresponds to the "32" bit of the output; pin 7 corresponds to the "1" bit of the output
  for (int pin : minutePins) {
    pinMode(pin, OUTPUT);
  }

  // pins 9-13, hour
  // pin 9 corresponds to the "16" bit of the output; pin 13 corresponds to the "1" bit of the output
  for (int pin : hourPins) {
    pinMode(pin, OUTPUT);
  }

  Serial.println("Pins assigned");
}

void loop() {
  // main code here, to run repeatedly:
  for (int pin: minutePins) { // proof of concept, to power/test LEDs

    Serial.print("pin ");
    Serial.print(pin);
    Serial.println(" activated");

    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
  }

  for (int pin: hourPins) { // proof of concept, to power/test LEDs

    Serial.print("pin ");
    Serial.print(pin);
    Serial.println(" activated");

    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
  }
}