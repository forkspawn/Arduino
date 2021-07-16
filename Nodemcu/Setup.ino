#include "Functions.h"

Pinger pinger; //creating an Pinger object

bool Webpage = 0; //to know if theres input from the webpage
AsyncWebServer server(80);

// Update these with values suitable for your network
const char* ap_ssid = "ESP8266";
const char* ap_password = "12345678";


char *token; //to store the username and password at first and copy to char arrays of username and password
char username[50], password[50]; // to store username and password
const char s[2] = ","; //to separate the username and password
bool flag = 0; //to know whether wifi is connected in stationary mode
bool button_status = 0; //Read the status of button when pressed



const char* PARAM_STRING = "inputString"; // for the input of string entered from the webpage

// HTML web page to handle 3 input fields (inputString, inputInt, inputFloat)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function submitMessage() {
      alert("Saved value to ESP SPIFFS");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
  </script></head><body>
  <form action="/get" target="hidden-form">
    username,password : <input type="text" name="inputString">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);    // LED pin as output.
  pinMode(button, INPUT_PULLUP); //used a button when pressed stays there
  Serial.print("Configuring access point...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("AP Mode started");
  Serial.println("going in");
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.status());
  // Initialize SPIFFS
#ifdef ESP32
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
#else
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
#endif

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // String username,password;

    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    if (request->hasParam(PARAM_STRING)) {
      Webpage = 1;
      inputMessage = request->getParam(PARAM_STRING)->value();
      writeFile(SPIFFS, "/inputString.txt", inputMessage.c_str());

    }
    else
    {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);

  });

  server.begin();
  server.onNotFound(notFound);

}



void loop() {


  button_status = digitalRead(button); //returns 1 when not pressed, returns 0 when pressed
  String yourInputString = readFile(SPIFFS, "/inputString.txt");
  int n = yourInputString.length();

  // declaring character array
  char char_array[n + 1];

  // copying the contents of the
  // string to char array
  strcpy(char_array, yourInputString.c_str());

  /* get the first token */
  token = strtok((char*)char_array, s);

  /* walk through other tokens */
  if ( token != NULL ) {
    //Serial.println( token );
    strcpy(username, token);
    token = strtok(NULL, s);
    //Serial.println(token);
  }
  if ( token != NULL ) {
    //Serial.println( token );
    strcpy(password, token);
    token = strtok(NULL, s);
    //Serial.println(token);
  }
  Serial.println("\n");
  Serial.print("The previous username is ");
  Serial.println(username);
  Serial.print("The previous password is ");
  Serial.println(password);
  //Serial.println(WiFi.status()); // to know if its connected or not

  Button_Connect(button_status);
  delay(2000);
}
