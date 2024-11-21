/*
Sample code on LED's to check if they are working correctly
*/ 


// Define the LED pin
const int RLed = 4;
const int GLed = 23;

void setup() {
  // Initialize the LED pin as an output
  pinMode(RLed, OUTPUT);
  pinMode(GLed, OUTPUT);
}

void loop() {
  // Turn the LED on
  digitalWrite(GLed, HIGH);
  digitalWrite(RLed, HIGH);
  delay(1000); // Wait for 1 second

  // Turn the LED off
  digitalWrite(GLed, LOW);
  digitalWrite(RLed, LOW);
  delay(1000); // Wait for 1 second
}
