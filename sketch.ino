

#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"


const char* ID_UNICO   = "Simulacion";         
const char* WIFI_SSID  = "Wokwi-GUEST";              
const char* WIFI_PASS  = "";                         
const char* MQTT_HOST  = "broker.emqx.io";           
const int   MQTT_PORT  = 1883;                       


String TOPIC_DATOS;    
String TOPIC_COMANDO;   


const int PIN_DHT = 15;   
const int PIN_LED = 2;     

DHTesp dht;
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long ultimaLectura = 0;
const unsigned long INTERVALO = 2000;  

void conectarWifi() {
  Serial.print("Conectando a WiFi ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println(" OK");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}


void alRecibir(char* topic, byte* payload, unsigned int len) {
  String msg;
  for (unsigned int i = 0; i < len; i++) msg += (char)payload[i];
  Serial.print("Comando recibido: ");
  Serial.println(msg);

  if (msg == "ON")  digitalWrite(PIN_LED, HIGH);
  if (msg == "OFF") digitalWrite(PIN_LED, LOW);
}

void conectarMqtt() {
  while (!mqtt.connected()) {
    Serial.print("Conectando a MQTT... ");
    String clientId = "esp32-" + String(ID_UNICO) + "-" + String(random(0xffff), HEX);
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("OK");
      mqtt.subscribe(TOPIC_COMANDO.c_str());
      Serial.print("Suscrito a: ");
      Serial.println(TOPIC_COMANDO);
    } else {
      Serial.print("fallo, estado=");
      Serial.print(mqtt.state());
      Serial.println(" reintento en 2 s");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  dht.setup(PIN_DHT, DHTesp::DHT22);

  TOPIC_DATOS   = "tajamar/iot/" + String(ID_UNICO) + "/estacion/datos";
  TOPIC_COMANDO = "tajamar/iot/" + String(ID_UNICO) + "/estacion/comando";

  conectarWifi();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(alRecibir);
}

void loop() {
  if (!mqtt.connected()) conectarMqtt();
  mqtt.loop();

  if (millis() - ultimaLectura > INTERVALO) {
    ultimaLectura = millis();

    TempAndHumidity datos = dht.getTempAndHumidity();
    float temp = datos.temperature;
    float hum  = datos.humidity;

    
    float fase = millis() / 15000.0;     
    temp += 7.0 * sin(fase);             
    hum  += 12.0 * sin(fase + 1.5);      
    temp += random(-3, 4) / 10.0;       
    hum  += random(-3, 4) / 10.0;
    

    char payload[80];
    snprintf(payload, sizeof(payload), "{\"temp\":%.1f,\"hum\":%.1f}", temp, hum);
    mqtt.publish(TOPIC_DATOS.c_str(), payload);
    Serial.print("Publicado -> ");
    Serial.println(payload);
  }
}
