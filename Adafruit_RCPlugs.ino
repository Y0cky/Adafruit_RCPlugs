/***************************************************
  Adafruit.IO multifeed digital out
 ****************************************************/
#include <ESP8266WiFi.h>
#include <RCSwitch.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

RCSwitch mySwitch = RCSwitch();

void connect(void);
void action( int x );

/************************* WiFi Access Point *********************************/

#define ssid            "...ssid..."
#define password        "...password..."

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "...username..."
#define AIO_KEY         "...key..."

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ AIO_USERNAME;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

// Lamp 1 
const char PLUG1_FEED[] PROGMEM = AIO_USERNAME "/feeds/plug1";
Adafruit_MQTT_Subscribe plug1 = Adafruit_MQTT_Subscribe(&mqtt, PLUG1_FEED);

// Lamp 2 
const char PLUG2_FEED[] PROGMEM = AIO_USERNAME "/feeds/plug2";
Adafruit_MQTT_Subscribe plug2 = Adafruit_MQTT_Subscribe(&mqtt, PLUG2_FEED);

// Lamp 3 
const char PLUG3_FEED[] PROGMEM = AIO_USERNAME "/feeds/plug3";
Adafruit_MQTT_Subscribe plug3 = Adafruit_MQTT_Subscribe(&mqtt, PLUG3_FEED);

// Lamp 4 
const char PLUG4_FEED[] PROGMEM = AIO_USERNAME "/feeds/plug4";
Adafruit_MQTT_Subscribe plug4 = Adafruit_MQTT_Subscribe(&mqtt, PLUG4_FEED);

// Lamp 5 
const char PLUG5_FEED[] PROGMEM = AIO_USERNAME "/feeds/plug5";
Adafruit_MQTT_Subscribe plug5 = Adafruit_MQTT_Subscribe(&mqtt, PLUG5_FEED);


/*************************** Sketch Code ************************************/

void setup() {

  int lamp;

  Serial.begin(115200);

  // Transmitter is connected to ESP8266 PIN #2
  mySwitch.enableTransmit(2);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("███"));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // listen for events
  mqtt.subscribe(&plug1);
  mqtt.subscribe(&plug2);
  mqtt.subscribe(&plug3);
  mqtt.subscribe(&plug4);
  mqtt.subscribe(&plug5);

  // connect to adafruit.io
  connect();

}

void loop() {

  Adafruit_MQTT_Subscribe *subscription;

  if(! mqtt.ping(5)) {
    Serial.println("ping fehlgeschlagen");
    if(! mqtt.connected())
      connect();
  }


  while (subscription = mqtt.readSubscription(1000)) {

    if (subscription == &plug1) {
      action(1);
    }
    else if (subscription == &plug2) {
      action(2);
    }
    else if (subscription == &plug3) {
      action(3);
    }
    else if (subscription == &plug4) {
      action(4);
    }
    else if (subscription == &plug5) {
      action(5);
    }
  }
}

// Steckdosen schalten

void action( int x ) {
  char *value;
  if (x == 1) {
    value = (char *)plug1.lastread;
    if (String(value) == "ON") {mySwitch.switchOn("00001", "10000");}
    if (String(value) == "OFF") {mySwitch.switchOff("00001", "10000");}
  }
  else if (x == 2) {
    value = (char *)plug2.lastread;
    if (String(value) == "ON") {mySwitch.switchOn("00001", "01000");}
    if (String(value) == "OFF") {mySwitch.switchOff("00001", "01000");}
  }
  else if (x == 3) {
    value = (char *)plug3.lastread;
    if (String(value) == "ON") {mySwitch.switchOn("00001", "00100");}
    if (String(value) == "OFF") {mySwitch.switchOff("00001", "00100");}
  }
  else if (x == 4) {
    value = (char *)plug4.lastread;
    if (String(value) == "ON") {mySwitch.switchOn("00001", "00010");}
    if (String(value) == "OFF") {mySwitch.switchOff("00001", "00010");}
  }
  else if (x == 5) {
    value = (char *)plug5.lastread;
    if (String(value) == "ON") {mySwitch.switchOn("00001", "00001");}
    if (String(value) == "OFF") {mySwitch.switchOff("00001", "00001");}
  }

  if ((String(value) == "ON") && (x == 1)) {
    Serial.println("Steckdose 1 eingeschaltet");
  }
  else if ((String(value) == "OFF") && (x == 1)) {
    Serial.println("Steckdose 1 ausgeschaltet");
  }
  else if ((String(value) == "ON") && (x == 2)) {
    Serial.println("Steckdose 2 eingeschaltet");
  }
  else if ((String(value) == "OFF") && (x == 2)) {
    Serial.println("Steckdose 2 ausgeschaltet");
  }
  else if ((String(value) == "ON") && (x == 3)) {
    Serial.println("Steckdose 3 eingeschaltet");
  }
  else if ((String(value) == "OFF") && (x == 3)) {
    Serial.println("Steckdose 3 ausgeschaltet");
  }
  else if ((String(value) == "ON") && (x == 4)) {
    Serial.println("Steckdose 4 eingeschaltet");
  }
  else if ((String(value) == "OFF") && (x == 4)) {
    Serial.println("Steckdose 4 ausgeschaltet");
  }
  else if ((String(value) == "ON") && (x == 5)) {
    Serial.println("Steckdose 5 eingeschaltet");
  }
  else if ((String(value) == "OFF") && (x == 5)) {
    Serial.println("Steckdose 5 ausgeschaltet");
  }
}
// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));

}
