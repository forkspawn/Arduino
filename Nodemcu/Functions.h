/*
      --------------------------------------ver7--------------------------------------------------------
      When powered that on The nodemcu is in ap mode, enter the ap_ssid and ap_password to connect to it.
      After connecting to the nodemcu, goto 192.169.4.1 in the browser which is default local IP in AP
      mode.
      Enter username and password in the format: username,password.
      for example if the username is TRON and password is PASS.
      enter into the box: TRON,PASS

      Press button to turn AP mode on
      Onboard led is on
      After this, the wifi variable is 1 indicating that we have given credentials from the browser And
      by default flag=0.
      when wifi==1 and flag==0
      it goes and checks the given username and password is getting connected or not in the function
      connect_wifi
      connect_wifi returns 1 if its connected and returns 0 if its not.
      This value is catched by flag.

      After connecting to the wifi it checks the internet connection
      by pinging to google.com

      Press button again to turn AP mode off
      Onboard led is off
      And it goes to Stationary mode and check for ping

*/

#ifndef Functions.h
#define Functions.h

#include <ESP8266WiFi.h>
#include <Pinger.h>
#include<SPI.h>
#include <PubSubClient.h>
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <Hash.h>
#include <FS.h>
#include<string.h>
#endif
#include <ESPAsyncWebServer.h>

extern "C"
{
#include <lwip/icmp.h> // needed for icmp packet definitions
}


#define button 5 //connect to D1 pin in nodemcu
#define LED D0

//Replaces placeholder with stored values, the values which we got from the webpage
String processor(const String& var);

//Read from the spiffs file and returns a string
String readFile(fs::FS &fs, const char * path);

//Writes to the spiffs file from the browser input
void writeFile(fs::FS &fs, const char * path, const char * message);

//checks for ping
void pingit();

//connects to the wifi and waits for 15 seconds for connection, if connected returns 1 and if not connected returns 0
int connect_wifi(char ssid[], char password[]);

// When button is pressed,onBoard led is on and it goes into AP_STA mode and connects to the wifi and checks internet by pinging google.com
//When button is not pressed,onBoard led is off and it disconnects from AP mode and stays on STA Mode
void Button_Connect(int button_status) ;

#endif
