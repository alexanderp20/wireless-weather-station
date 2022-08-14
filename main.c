/*
Wireless Weather Station Project
By: Alexander Pak and Shrey Varma
*/
#include <stdio.h>
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <Servo.h> 
#include <driverlib.h>

#define NUM_PHOTO 2
#define SPIN_TIME 800


volatile uint8_t curPos = 0;
bool firstRun = true;
uint8_t photoPins[NUM_PHOTO] = {A11, A15};

// Declare the Servo pin 
int servoPin = 11; 
// Create a servo object 
Servo Servo1; 

// your network name also called SSID
char ssid[] = "tempwifi";
// your network password
char password[] = "12345678";
// MQTTServer to use
char server[] = "172.20.10.10";

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message for topic ");
  Serial.print(topic);
  Serial.print("with length ");
  Serial.println(length);
  Serial.println("Message:");
  Serial.write(payload, length);
  Serial.println();
}

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);


void setup()
{
  Serial.begin(115200);
  Serial.println("test");
  Servo1.attach(servoPin); 

  //warmup
  //custom_delay(10000);
  // Start Ethernet with the build in MAC Address
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  // We are connected and have an IP address.
  // Print the WiFi status.
  printWifiStatus();
}

void loop()
{
  int sensorValue = analogRead(A0); 
  float temp_C = ((3.3*sensorValue/1023.0))/0.01; 
  float temp_F = temp_C*(9.0/5.0)+32.0; 
  int sensorValue2 = analogRead(A3); // Solar Panel Positive PIN connect to A0  
  float voltage = sensorValue2 * (5.0 / 1023.0); 
  //Serial.println(temp_F); 
  Serial.println(voltage);

  
  // Reconnect if the connection was lost
  if (!client.connected()) {
    Serial.println("Disconnected. Reconnecting....");

    if(!client.connect("energiaClient")) {
      Serial.println("Connection failed");
    } else {
      Serial.println("Connection success");
    }
  }
  char json_temp[50];
  char json_volt[50];
  snprintf(json_temp, 50, "{\"temperature\":%0.2f}", temp_F);
  snprintf(json_volt, 50, "{\"voltage\":%0.2f}", voltage);
  //Serial.println(json_temp);
  //Serial.println(json_volt);
  
  if(client.publish("temperature_reading",json_temp) && client.publish("voltage_reading",json_volt)) {
    Serial.println("Publish success");
  } else {
    Serial.println("Publish failed");
  }
 
  // Check if any message were received
  // on the topic we subsrcived to
  rotateBest();
  delay(1000);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void rotateServo(uint16_t amount, uint8_t dir){
  Serial.println("Moving Servo...");
  if(dir == 1){
      Servo1.write(0);
      } 
  else{
      Servo1.write(180);
      }
  delay(amount);
  Servo1.write(90);
  }


void rotateBest(void){
  
  uint8_t maxPin = 0;
  for(int i = 1; i<NUM_PHOTO; i++){
      uint16_t avg1 = 0;
      uint16_t avg2 = 0;
      for(int x = 0; x<10; x++){
        avg1 += analogRead(photoPins[i]);
        avg2 += analogRead(photoPins[maxPin]);
        }
        avg1 /= 10;
        avg2 /= 10;
        Serial.print(avg1);
        Serial.print(" ");
        Serial.print(avg2);
        Serial.println();
      if(avg1 >  avg2){
        maxPin = i;
        }
  }
  if(curPos != maxPin){
    rotateServo(400, 1);
    Serial.print("Moving to Position: ");
    Serial.println(maxPin);
    int16_t timeRotate = (maxPin - curPos) * (SPIN_TIME / NUM_PHOTO);
    curPos = maxPin;
    Serial.print("Rotating for: ");
    Serial.println(timeRotate);
    rotateServo(400, 1);
//    if (timeRotate > 0){
//      Serial.println("Turning to the right");
//      rotateServo(timeRotate, 1);
//      }
//    else{
//      Serial.println("Turning to the left");
//      rotateServo(abs(timeRotate), 0);
//      }
    }
    else{
      Serial.print("Current Position is the best: ");
      Serial.println(curPos);
      }
  
  }
