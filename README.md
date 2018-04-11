# ESP8266-MQTTS-Sensors
Arduino programs using secure mqtt to publish sensor data.  

Deepsleep is set to 300 seconds (5 minutes) by default, don't forget to connect GPIO 16 to RST so the ESP can wake up.  
Deepsleep will be activated when the program finishes, or when mqtt or wifi can't connect.

Note: i only tested the DHT code with a DHT11

Common library:  
https://github.com/adafruit/Adafruit_MQTT_Library  
  
Library for the BME280:  
https://github.com/finitespace/BME280/blob/master/src/BME280I2C.h  
  
Library for the DHT:  
https://github.com/adafruit/DHT-sensor-library

Library for the DS18B20:  
https://github.com/milesburton/Arduino-Temperature-Control-Library
