#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Pins
const int sda_pin = 21;
const int scl_pin = 22;
const int RED_led = 27;
const int GRN_led = 26;
const int brown_pin = 16;
const int grey_pin = 32;
const int orange_pin = 33;
const int black_pin = 25;
const int brown_input = 4;
const int grey_input = 0;
const int orange_input = 17;
const int black_input = 15;

// Set up OLED display
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

// Set up server
AsyncWebServer server(80);

String generateWebPage() {
  String gpio_state, gpio_state1, gpio_state2, gpio_state3, gpio_state4, gpio_state5;
  String html;

  // Get the state of the input pins
  gpio_state = digitalRead(brown_input) ? " DISCONNECTED " : " CONNECTED ";
  gpio_state1 = digitalRead(grey_input) ? " DISCONNECTED " : " CONNECTED ";
  gpio_state2 = digitalRead(orange_input) ? " DISCONNECTED " : " CONNECTED ";
  gpio_state3 = digitalRead(black_input) ? " DISCONNECTED " : " CONNECTED ";

  // Get the state of the LEDs
  gpio_state4 = digitalRead(RED_led) ? "ON" : "OFF";
  gpio_state5 = digitalRead(GRN_led) ? "ON" : "OFF";  html = "<html><head> <title>IVTS Web Server</title> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
         "<link rel=\"icon\" href=\"data:,\"> <style>html{font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}"
         "h1{color: #0F3376; padding: 2vh;}p{font-size: 1.5rem;}.button{display: inline-block; background-color: #F20930; border: none;"
         "border-radius: 4px; color: white; padding: 10px 20px; text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}"
         ".button2{background-color: #02FA06;}</style></head><body> <h1>IVTS: LED Wiring Loom Tester</h1>"
         "<p>ORANGE WIRE: <strong>" + gpio_state2 + "</strong></p>"
         "<p>BLACK WIRE: <strong>" + gpio_state3 + "</strong></p>"
         "<p>BROWN WIRE: <strong>" + gpio_state + "</strong></p>"
         "<p>GREY WIRE: <strong>" + gpio_state1 + "</strong></p><p></p><p></p>"
         "<p>RED LED: <strong>" + gpio_state4 + "</strong></p>"
         "<p>GREEN LED: <strong>" + gpio_state5 + "</strong></p>"
         "<p></p>"
         "<p><a href=\"/toggleRed\"><button class=\"button\">TOGGLE RED</button></a></p>"
         "<p><a href=\"/toggleGreen\"><button class=\"button button2\">TOGGLE GREEN</button></a>"
         "<p></p></body></html>";

  return html;
}


void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Set up output pins
  pinMode(RED_led, OUTPUT);
  pinMode(GRN_led, OUTPUT);
  pinMode(brown_pin, OUTPUT);
  pinMode(grey_pin, OUTPUT);
  pinMode(orange_pin, OUTPUT);
  pinMode(black_pin, OUTPUT);

  // Set up input pins
  pinMode(brown_input, INPUT);
  pinMode(grey_input, INPUT);
  pinMode(orange_input, INPUT);
  pinMode(black_input, INPUT);

  // Set up OLED display
  Wire.begin(sda_pin, scl_pin);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connection successful");
  Serial.println(WiFi.localIP());

  // Display IP address
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  oled.println("IP address:");
  oled.println(WiFi.localIP());
  oled.println("");
  oled.println("Connecting...");
  oled.println("ivts");
  oled.display();

  // Set up server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = generateWebPage();
    request->send(200, "text/html", html);
  });
  
  server.on("/toggleRed", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(RED_led, !digitalRead(RED_led));
    digitalWrite(GRN_led, LOW);
    request->send(200, "text/plain", "Toggled RED LED");
  });

  server.on("/toggleGreen", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(GRN_led, !digitalRead(GRN_led));
    digitalWrite(RED_led, LOW);
    request->send(200, "text/plain", "Toggled GREEN LED");
  });
  
  server.begin();
}

void loop() {
  // Nothing to do here
}

