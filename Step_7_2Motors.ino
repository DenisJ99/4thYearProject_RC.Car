/*
Simular code to the last just with 2 working motors
*/
#define EN1_PIN  35
#define IN1_PIN  32
#define IN2_PIN  33

// Motor 2 Pins

#define EN3_PIN  22
#define IN3_PIN  25
#define IN4_PIN  26

void setup() {
  // Set pin modes for Motor 1
  pinMode(EN1_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  // Set pin modes for Motor 2
  pinMode(EN3_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);

  // Enable both motor driver channels
  digitalWrite(EN1_PIN, HIGH); // Enable Motor 1
  digitalWrite(EN3_PIN, HIGH); // Enable Motor 2
}

void loop() {
  // Motor 1: Forward
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);

  // Motor 2: Forward
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
/*
  // Stop both motors
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  delay(2000);

  // Motor 1: Reverse
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);

  // Motor 2: Reverse
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  delay(2000);

  // Stop both motors again
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  delay(2000);


  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  delay(2000);

  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  delay(2000);
  */
}
