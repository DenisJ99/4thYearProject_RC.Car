#define EN1_PIN  34
#define IN1_PIN  35
#define IN2_PIN  32

void setup() {
  // Set pin modes
  pinMode(EN1_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  // Enable the motor driver channel
  digitalWrite(EN1_PIN, HIGH);
}

void loop() {
  // To move the motor "forward":
  digitalWrite(IN1_PIN, HIGH);  // IN1 HIGH
  digitalWrite(IN2_PIN, LOW);   // IN2 LOW
  delay(2000);

  // To stop:
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  delay(2000);

  // To reverse the motor direction:
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  delay(2000);

  // Stop again:
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  delay(2000);
}
