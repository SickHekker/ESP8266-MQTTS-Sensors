#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <DHT.h>

// Deepsleep duration in seconds
int sleep = 300;

// DHT setup
#define DHTPIN 12 //Change this if you want, this is D6 on a nodemcu board

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Set the MQTT feeds to be used
#define temperature_feed "/sensors/DHT/temperature"
#define humidity_feed "/sensors/DHT/humidity"

/************************* WiFi Access Point *********************************/

WiFiClientSecure client;
#define WLAN_SSID "wifissid"
#define WLAN_PASS "wifipassword"

/************************* MQTT Broker Setup *********************************/

#define AIO_SERVER      "mqttserver"
#define AIO_SERVERPORT  8883                   // 8883 for MQTTS
#define AIO_USERNAME    "mqttusername"
#define AIO_KEY         "mqttpassword"

/****************************** Feeds ***************************************/

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish temperature_topic = Adafruit_MQTT_Publish(&mqtt, temperature_feed);
Adafruit_MQTT_Publish humidity_topic = Adafruit_MQTT_Publish(&mqtt, humidity_feed);

/*************************** Sketch Code ************************************/

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(115200);
  delay(10);
  
  int wifitry = 0;
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  delay(500);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifitry++;
    if (wifitry >= 20) {
      Serial.println("Can't connect to WiFi, activating deepsleep");
      wifitry = 0;
      ESP.deepSleep(sleep * 1000000);
    }
  }
  dht.begin();

  MQTT_connect();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  Serial.println(temperature);
  Serial.println(humidity);

  temperature_topic.publish(temperature);
  humidity_topic.publish(humidity);

  Serial.println("Data posted to MQTT");
  mqtt.disconnect();
  Serial.println("MQTT disconnected, activating deepsleep");
  ESP.deepSleep(sleep * 1000000);
}

void loop() {
  ESP.deepSleep(sleep * 1000000);
}

void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.println("Connecting to MQTT... ");
  uint8_t retries = 2;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 1 second...");
    mqtt.disconnect();
    delay(1000);  // wait 1 second
    retries--;
    if (retries == 0) {
      ESP.deepSleep(sleep * 1000000);
    }
  }
  
  Serial.println("MQTT Connected!");
}
