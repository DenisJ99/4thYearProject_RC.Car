#include <WiFi.h>

// WiFi credentials
const char* ssid     = "DJ";
const char* password = "Johnson1";

// Server setup
WiFiServer server(80);

// Define LED, buzzer, and ultrasonic sensor pins
const int Led = 23;          // Red LED pin
const int buzzerPin = 19;    // Buzzer pin
const int trigPin1 = 5;      // Ultrasonic sensor 1 trigger pin
const int echoPin1 = 18;     // Ultrasonic sensor 1 echo pin
const int trigPin2 = 12;     // Ultrasonic sensor 2 trigger pin
const int echoPin2 = 13;     // Ultrasonic sensor 2 echo pin
const int trigPin3 = 14;     // Ultrasonic sensor 3 trigger pin
const int echoPin3 = 27;     // Ultrasonic sensor 3 echo pin

// Motor 1 Pins

#define EN1_PIN  35
#define IN1_PIN  32
#define IN2_PIN  33

// Motor 2 Pins
#define EN3_PIN  22
#define IN3_PIN  25
#define IN4_PIN  26

// PWM setup for the buzzer
const int buzzerChannel = 0;
const int buzzerFrequency = 1000;
const int buzzerResolution = 8;

// Distance thresholds (in centimeters)
const int warningDistance = 10;
const int criticalDistance = 5;

void setup() {
    Serial.begin(115200);
    pinMode(Led, OUTPUT);

    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(trigPin3, OUTPUT);
    pinMode(echoPin3, INPUT);

    pinMode(EN1_PIN, OUTPUT);
    pinMode(IN1_PIN, OUTPUT);
    pinMode(IN2_PIN, OUTPUT);
    pinMode(EN3_PIN, OUTPUT);
    pinMode(IN3_PIN, OUTPUT);
    pinMode(IN4_PIN, OUTPUT);

    digitalWrite(EN1_PIN, HIGH);
    digitalWrite(EN3_PIN, HIGH);

    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, LOW);

    ledcSetup(buzzerChannel, buzzerFrequency, buzzerResolution);
    ledcAttachPin(buzzerPin, buzzerChannel);

    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected.");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop() {
    WiFiClient client = server.available();
    int distance1 = getDistance(trigPin1, echoPin1);
    int distance2 = getDistance(trigPin2, echoPin2);
    int distance3 = getDistance(trigPin3, echoPin3);

    bool objectDetected = (distance1 <= warningDistance || distance2 <= warningDistance || distance3 <= warningDistance);
    bool objectClose = (distance1 <= criticalDistance || distance2 <= criticalDistance || distance3 <= criticalDistance);

    if (objectClose) {
        alert(true);
    } else if (objectDetected) {
        alert(false);
    } else {
        digitalWrite(Led, LOW);
        ledcWrite(buzzerChannel, 0);
    }

    if (client) {
        Serial.println("New Client.");
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();
                        client.println("<html><body>");
                        client.println("<h1>ESP32 Control Panel</h1>");
                        client.println("<button onclick=\"location.href='/F'\">Forward</button>");
                        client.println("<button onclick=\"location.href='/B'\">Backward</button>");
                        client.println("<button onclick=\"location.href='/L'\">Left</button>");
                        client.println("<button onclick=\"location.href='/R'\">Right</button>");
                        client.println("<button onclick=\"location.href='/S'\">Stop</button>");
                        client.println("</body></html>");
                        client.println();
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
                if (currentLine.endsWith("GET /S")) stopMotors();
                if (currentLine.endsWith("GET /F")) moveForward();
                if (currentLine.endsWith("GET /B")) moveBackward();
                if (currentLine.endsWith("GET /L")) turnLeft();
                if (currentLine.endsWith("GET /R")) turnRight();
                
            }
        }
        client.stop();
        Serial.println("Client Disconnected.");
    }
}

int getDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2;
}

void alert(bool fast) {
    int delayTime = fast ? 100 : 500;
    digitalWrite(Led, HIGH);
    //ledcWrite(buzzerChannel, 128);
    delay(delayTime);
    digitalWrite(Led, LOW);
    //ledcWrite(buzzerChannel, 0);
    delay(delayTime);
}

void moveForward() {
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);
}

void moveBackward() {
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, HIGH);
    delay(1000);
}

void turnLeft() {
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);
}

void turnRight() {
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, LOW);
}

void stopMotors() {
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, LOW);
    delay(1000);
}
