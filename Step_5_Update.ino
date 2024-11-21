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

    // Initialize ultrasonic sensors
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(trigPin3, OUTPUT);
    pinMode(echoPin3, INPUT);

    // Buzzer PWM setup
    ledcSetup(buzzerChannel, buzzerFrequency, buzzerResolution);
    ledcAttachPin(buzzerPin, buzzerChannel);

    // Start WiFi
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop() {
    WiFiClient client = server.available();  // listen for incoming clients

    // Sensor distances
    int distance1 = getDistance(trigPin1, echoPin1);
    int distance2 = getDistance(trigPin2, echoPin2);
    int distance3 = getDistance(trigPin3, echoPin3);

    // Determine if an object is within warning or critical distance
    bool objectDetected = (distance1 <= warningDistance || distance2 <= warningDistance || distance3 <= warningDistance);
    bool objectClose = (distance1 <= criticalDistance || distance2 <= criticalDistance || distance3 <= criticalDistance);

    // Trigger LED and buzzer alerts based on distance
    if (objectClose) {
        alert(true);  // Fast beep and LED flash for close objects
    } else if (objectDetected) {
        alert(false); // Slow beep and LED flash for objects within warning distance
    } else {
        digitalWrite(Led, LOW);
        ledcWrite(buzzerChannel, 0); // Stop the buzzer
    }

    // Identify which sensor is triggered
    String sensorStatus = "None";
    if (distance1 <= warningDistance) sensorStatus = "Sensor 1";
    else if (distance2 <= warningDistance) sensorStatus = "Sensor 2";
    else if (distance3 <= warningDistance) sensorStatus = "Sensor 3";

    // Handle client request and send webpage
    if (client) {
        Serial.println("New Client.");
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);

                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        // Send HTTP headers
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        // Send webpage with sensor status and auto-refresh
                        client.print("<html><head>");
                        client.print("<meta http-equiv='refresh' content='2'>"); // Refresh page every 2 seconds
                        client.print("</head><body>");
                        client.print("<h1>ESP32 Web Server</h1>");
                        client.print("<p>Click <a href=\"/H\">here</a> to turn the LED on.</p>");
                        client.print("<p>Click <a href=\"/L\">here</a> to turn the LED off.</p>");
                        client.print("<p><b>Sensor Triggered:</b> ");
                        client.print(sensorStatus);  // Display triggered sensor
                        client.print("</p></body></html>");

                        client.println();
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }

                // LED control via client request
                if (currentLine.endsWith("GET /H")) {
                    digitalWrite(Led, HIGH);
                }
                if (currentLine.endsWith("GET /L")) {
                    digitalWrite(Led, LOW);
                }
            }
        }
        client.stop();
        Serial.println("Client Disconnected.");
    }
    delay(50);
}

// Function to calculate distance using ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;
    return distance;
}

// Function to handle alert (buzzer and LED flashing)
void alert(bool fast) {
    int delayTime = fast ? 100 : 500; // Fast or slow alert timing

    digitalWrite(Led, HIGH);
    ledcWrite(buzzerChannel, 128); // Set duty cycle to 50% for buzzer
    delay(delayTime);

    digitalWrite(Led, LOW);
    ledcWrite(buzzerChannel, 0); // Stop the buzzer
    delay(delayTime);
}
