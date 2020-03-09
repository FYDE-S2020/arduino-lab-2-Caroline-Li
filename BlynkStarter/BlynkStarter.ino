/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define LED 2

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;




#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;
int time_count = 0; // timer counter global variable
String content = "";  // null string constant ( an empty string )

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4cU-BGwmlref_B_kQRrChLp0P-HS3V8Z";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 
char ssid[32] = "Caroline's iPhone";
char pass[32] = "03fk2rm8yavv";

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  //pinMode(2, OUTPUT);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(2, ledChannel);
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(10L, myTimerEvent);
  
  
}

void loop()
{
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();

  if (pinValue == 0)
  {
      ledcWrite(ledChannel, 0);
  }
  else
  {
    ledcWrite(ledChannel, 255);
  }
  
}
BLYNK_READ(V2)
{
  Blynk.virtualWrite(V2, millis() / 1000);
}
BLYNK_WRITE(V1)
{
  int val = param.asInt();
  ledcWrite(ledChannel, val); 
}

void myTimerEvent() // Every 10 ms
{
    //Blynk.virtualWrite(V2, millis() / 1000);
    if (time_count == 100){
        // Do thing that needs to happen every 1 second
        time_count = 0;
        Blynk.virtualWrite(V2, millis() / 1000); // Write the arduino uptime every second
      }
    
    else {
        // Send serial data to Blynk terminal
        char character;
        while(Serial.available()) { // Check if serial is available every 10 ms
            character = Serial.read();
            content.concat(character);
        }
        if (content != "") {
            Blynk.virtualWrite(V3, content);
            content = ""; // Clear String
        }  
    }
    time_count += 1; // Increment on every tick
}
