/* 
    Line following test, using two sensors.
    Includes WiFi debugging.
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <Adafruit_MotorShield.h>

const int leftPin = 3;
const int rightPin = 4;
bool leftDetect;
bool rightDetect;

// WIFI SETUP
char ssid[] = "TheBigBlockCarriers"; // your network SSID (name)
char pass[] = "IDPL209BBC";          // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(23);

//SHIELD SETUP
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Configure all 4 motors outputs. M3 is front left, M4 is front right.
Adafruit_DCMotor *M1 = AFMS.getMotor(1);
Adafruit_DCMotor *M2 = AFMS.getMotor(2);
Adafruit_DCMotor *M3 = AFMS.getMotor(3);
Adafruit_DCMotor *M4 = AFMS.getMotor(4);
Adafruit_DCMotor Motors[] = {*M1, *M2, *M3, *M4};

void setup() {
      //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found, proceeding to acess point setup.");

  Serial.println("Begin access point setup.");
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING)
  {
    Serial.println("Creating access point failed.");
    // don't continue
    while (true)
      ;
  }
    // Begin WiFi server
    server.begin();
    printWiFiStatus();

    // Set sensor pins for interrupts
    pinMode(leftPin, INPUT_PULLUP);
    pinMode(rightPin, INPUT_PULLUP);

    Motors[0].setSpeed(255);
    Motors[1].setSpeed(255);
}

void loop() {
  leftDetect = digitalRead(leftPin);
  rightDetect = digitalRead(rightPin);
  // compare the previous status to the current status
  delay(1);
  //Serial.println("Alive");
  if (status != WiFi.status())
  {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED)
    {
      // a device has connected to the AP
      Serial.println("Device connected to access point.");
      server.write("Client connected.");
    }
    else
    {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from access point.");
      server.write("Client disconnected.");
    }
  }

   WiFiClient client = server.available();
  // listen for incoming clients
  if (client.available() > 0)
  {
    // read the bytes incoming from the client:
    char thisChar = client.readString()[0];
    // echo the bytes back to the client:
    server.write(thisChar);
    // echo the bytes to the server as well:
    Serial.println((String) "Received: " + thisChar);
 }

  if (leftDetect == LOW && rightDetect == LOW) {
    Serial.println("Both sensors are now low. GO STRAIGHT.");
    server.write("Both sensors are now low. GO STRAIGHT.\r\n");
    Motors[1].run(FORWARD);
    Motors[0].run(FORWARD);
    delay(100);
    Motors[0].run(RELEASE);
    Motors[1].run(RELEASE);
  }
  else if ((leftDetect == HIGH && rightDetect == LOW)) {
    Serial.println("Left is high, right is low. TURN LEFT.");
    server.write("Left is high, right is low. TURN LEFT.\r\n");
    Motors[0].run(FORWARD);
    Motors[1].run(BACKWARD);
    delay(100);
    Motors[0].run(RELEASE);
    Motors[1].run(RELEASE);
  }
  else if (leftDetect == LOW && rightDetect == HIGH) {
    Serial.println("Left is low, right is high. TURN RIGHT.");
    server.write("Left is low, right is high. TURN RIGHT.\r\n");
    Motors[1].run(BACKWARD);
    Motors[0].run(FORWARD);
    delay(100);
    Motors[0].run(RELEASE);
    Motors[1].run(RELEASE);
  }
  else if (leftDetect == HIGH && rightDetect == HIGH) {
    Serial.println("Both sensors are now high. TURN RIGHT.");
    server.write("Both sensors are now high. TURN RIGHT.\r\n");
    Motors[1].run(BACKWARD);
    Motors[0].run(FORWARD);
    delay(100);
    Motors[0].run(RELEASE);
    Motors[1].run(RELEASE);
  }
}

/*void follow() {
    leftDetect = digitalRead(leftPin);
    rightDetect = digitalRead(rightPin);

    if (leftDetect == LOW && rightDetect == LOW) {
      Serial.println("Both sensors are now low. GO STRAIGHT.");
      server.write("Both sensors are now low. GO STRAIGHT.");
      forwardFlag = true;
    }
    else if (leftDetect == LOW && rightDetect == HIGH) {
        Serial.println("Left is low, right is high. TURN RIGHT.");
        server.write("Left is low, right is high. TURN RIGHT.");
        rightFlag = true;
    }
    else if (leftDetect == HIGH && rightDetect == LOW) {
        Serial.println("Left is high, right is low. TURN LEFT.");
        server.write("Left is high, right is low. TURN LEFT.");
        leftFlag = true;
    }
    else {
        Serial.println("Both sensors are now high. CONFUSED, TURNING LEFT.");
        server.write("Both sensors are now high. CONFUSED, TURNING LEFT.");
        leftFlag = true;
    }
}*/

void printWiFiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
