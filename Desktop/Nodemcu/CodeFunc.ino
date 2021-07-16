#include "Functions.h"

// Replaces placeholder with stored values
String processor(const String& var) {
  Serial.println("Var value");
  Serial.println(var);
  if (var == "inputString") {
    return readFile(SPIFFS, "/inputString.txt");
  }
  return String();
}

String readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}



int connect_wifi(char ssid[], char password[])
{
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  delay(10);


  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  unsigned int delayStart = millis(); //start the delayStart when it enters the function
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
    if (millis() > (delayStart + 15000)) //wait for 15 seconds
    {
      Serial.println("NOT CONNECTED");
      Serial.println(delayStart);
      return 0; //if not connected
    }
  }
  Serial.println("CONNECTED ");
  return 1; //if connected


}

void pingit()
{
  int stat = 0;
  int flag = 0;
  unsigned long Start = millis();
  Serial.printf("\nPinging google.com\n");
  stat = pinger.Ping("www.google.com"); //returns 1 when successfull, 0 when not successfull
  //Serial.println(stat);

  while (flag == 0) {
    Serial.print(".");
    delay(300);
    if (millis() > (Start + 15000)) //wait for 15 seconds
    {
      flag = 1;
    }
  }
  if (stat == 0)
  {
    Serial.println("Error during ping command.");
    delay(500);
  }

  else if (stat == 1)
  {
    Serial.println("Pinging Successfull");
    delay(500);
  }


}

void Button_Connect(int button_status)
{

  if (button_status == LOW) //LOW means on , its in INPUT_PULLUP MODE
  {

    int stat = 0; // to store the ping state
    flag = 0;
    Serial.println("Button Pressed AP mode is ON");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ap_ssid, ap_password);
    digitalWrite(LED, LOW); // turn the LED on.

    if (Webpage == 1 and flag == 0) //Webpage equals 1 when theres input from the Webpage
    {
      flag = connect_wifi(username, password); //returns 1 when its connected returns 0 when not connected
      if (flag == 1)
      {
        Serial.println("Wifi is connected"); //set Webpage=0 if you want to stop checking
        Webpage = 0;
        pingit();

      }
      else if (flag == 0)
      {

        Serial.println("Wifi isn't connected");
        Serial.println("Use another credentials");
        Webpage = 0;
      }

    }
  }

  else if (button_status == HIGH) //HIGH means off
  {
    int flag = 0;
    digitalWrite(LED, HIGH);// turn the LED off.(Note that LOW is the voltage level but actually
    //the LED is on; this is because it is acive low on the ESP8266.
    Serial.println("AP mode if OFF");
    WiFi.softAPdisconnect();
    pingit();

  }




}
