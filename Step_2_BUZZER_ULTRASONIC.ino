// Define LED, buzzer, and ultrasonic sensor pins
const int RLed = 4;       // Red LED pin
const int GLed = 2;       // Green LED pin
const int trigPin = 5;    // Ultrasonic sensor trigger pin
const int echoPin = 18;   // Ultrasonic sensor echo pin
const int buzzerPin = 19; // Buzzer pin

// Define PWM channel for the buzzer
const int buzzerChannel = 0;
const int buzzerFrequency = 1000; // Frequency in Hz
const int buzzerResolution = 8;   // 8-bit resolution

// Define distance thresholds (in centimeters)
const int warningDistance = 20; // Start slow beep/flashing at this distance
const int criticalDistance =5; // Start fast beep/flashing at this distance

void setup() {
  // Initialize LED pins as outputs
  pinMode(RLed, OUTPUT);
  pinMode(GLed, OUTPUT);
  
  // Initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Set up PWM for the buzzer
  ledcSetup(buzzerChannel, buzzerFrequency, buzzerResolution);
  ledcAttachPin(buzzerPin, buzzerChannel);
  
  // Begin Serial Communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Get distance from ultrasonic sensor
  int distance = getDistance();
  Serial.print("Distance: ");
  Serial.println(distance);
  
  // Check distance and set behavior accordingly
  if (distance <= criticalDistance) {
    // Close object: fast beep and fast LED flashing
    alert(true);
  } 
  else if (distance <= warningDistance) {
    // Object within warning range: slow beep and slow LED flashing
    alert(false);
  } 
  else {
    // No object detected within range: turn off LEDs and buzzer
    digitalWrite(RLed, LOW);
    digitalWrite(GLed, LOW);
    ledcWrite(buzzerChannel, 0); // Stop the buzzer by setting duty cycle to 0
  }

  delay(50); // Small delay for stability
}

// Function to calculate distance using ultrasonic sensor
int getDistance() {
  // Send a 10 microsecond pulse to trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo pulse
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance (in cm) based on sound speed in air (343 m/s)
  int distance = duration * 0.034 / 2;

  return distance;
}

// Function to handle alert (buzzer and LED flashing)
void alert(bool fast) {
  int delayTime;
  
  if (fast) {
    delayTime = 100; // Fast beep and flash
  } else {
    delayTime = 500; // Slow beep and flash
  }

  // Turn LEDs on, sound buzzer
  digitalWrite(RLed, HIGH);
  digitalWrite(GLed, HIGH);
  ledcWrite(buzzerChannel, 128); // Set duty cycle to 50% to turn on buzzer
  delay(delayTime);

  // Turn LEDs off, stop buzzer
  digitalWrite(RLed, LOW);
  digitalWrite(GLed, LOW);
  ledcWrite(buzzerChannel, 0); // Stop the buzzer by setting duty cycle to 0
  delay(delayTime);
}
