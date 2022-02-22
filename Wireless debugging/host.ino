/*
 Chat  Server

 A simple server that distributes any incoming messages to all
 connected clients.  To use, telnet to your device's IP address and type.
 You can see the client's input in the serial monitor as well.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.


 Circuit:
 * Board with NINA module (Arduino MKR WiFi 1010, MKR VIDOR 4000 and UNO WiFi Rev.2)

 created 18 Dec 2009
 by David A. Mellis
 modified 31 May 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <WiFiNINA.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Configure all 4 motors. M3 is front left, M4 is front right.
Adafruit_DCMotor *M1 = AFMS.getMotor(1);
Adafruit_DCMotor *M2 = AFMS.getMotor(2);
Adafruit_DCMotor *M3 = AFMS.getMotor(3);
Adafruit_DCMotor *M4 = AFMS.getMotor(4);

Adafruit_DCMotor Motors[] = {*M1, *M2, *M3, *M4};

int step = 500; // Milliseconds between inputs

char ssid[] = "OP7P";        // your network SSID (name)
char pass[] = "BigBlockCarriers";    // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;            // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(23);

boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // wait for serial port to connect. Needed for native USB port only
  }

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found, proceeding to WiFi setup.");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Disconnected, attempting connection to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(1);
  }

  // start the server:
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}


void loop() {
  // wait for a new client:
  WiFiClient client = server.available();


  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello, client!");
      alreadyConnected = true;
    }

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.readString()[0];
      // echo the bytes back to the client:
      //server.write(thisChar);
      // echo the bytes to the server as well:
      Serial.println(thisChar);

      if (thisChar == 'W') {
          // GO FORWARD
          Serial.println("MOVING FORWARD");
          Motors[2].run(FORWARD);
          Motors[3].run(FORWARD);
          Motors[2].setSpeed(255);
          Motors[3].setSpeed(255);
          delay(step);
          Motors[2].setSpeed(0);
          Motors[3].setSpeed(0);
          Motors[2].run(RELEASE);
          Motors[3].run(RELEASE);
      }
      if (thisChar == 'A') {
          // TURN LEFT
          Serial.println("TURNING LEFT");
          Motors[2].run(FORWARD);
          Motors[3].run(BACKWARD);
          Motors[2].setSpeed(255);
          Motors[3].setSpeed(255);
          delay(step);
          Motors[2].setSpeed(0);
          Motors[3].setSpeed(0);
          Motors[2].run(RELEASE);
          Motors[3].run(RELEASE);
      }
      if (thisChar == 'S') {
          // GO BACK
          Serial.println("MOVING BACK");
          Motors[2].run(BACKWARD);
          Motors[3].run(BACKWARD);
          Motors[2].setSpeed(255);
          Motors[3].setSpeed(255);
          delay(step);
          Motors[2].setSpeed(0);
          Motors[3].setSpeed(0);
          Motors[2].run(RELEASE);
          Motors[3].run(RELEASE);
      }
      if (thisChar == 'D') {
          // TURN RIGHT
          Serial.println("TURNING RIGHT");
          Motors[2].run(BACKWARD);
          Motors[3].run(FORWARD);
          Motors[2].setSpeed(255);
          Motors[3].setSpeed(255);
          delay(step);
          Motors[2].setSpeed(0);
          Motors[3].setSpeed(0);
          Motors[2].run(RELEASE);
          Motors[3].run(RELEASE);
      }
    }
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}