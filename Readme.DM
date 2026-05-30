# Estación de monitorización IoT en la nube (ESP32 + MQTT)

Proyecto IoT de extremo a extremo **sin hardware físico**. Un ESP32 simulado en
Wokwi lee temperatura y humedad de un sensor DHT22, las publica por **MQTT** a un
broker en la nube, y un **dashboard web** las muestra en tiempo real, avisa cuando
se supera un umbral y permite controlar un LED del dispositivo de forma remota.

`DHT22 → ESP32 (Wokwi) → MQTT (broker.emqx.io) → MQTT.js → Dashboard web`

## Contenido del repositorio

| Archivo | Descripción |
|---|---|
| `sketch.ino` | Firmware del ESP32: lee el sensor, publica datos y recibe comandos. |
| `diagram.json` | Circuito en Wokwi (ESP32 + DHT22 + LED). |
| `libraries.txt` | Librerías que instala Wokwi (`PubSubClient`, `DHT sensor library for ESPx`). |
| `dashboard.html` | Panel web (MQTT sobre WebSocket). Se abre en el navegador. |

## Cómo ejecutarlo

1. Crea un proyecto ESP32 en [wokwi.com](https://wokwi.com) y pega `sketch.ino`, `diagram.json` y las librerías.
2. En `sketch.ino`, cambia la variable `ID_UNICO` por un identificador propio (el broker es público).
3. Pulsa el boton de Play para iniciar la simulación.
4. Abre `dashboard.html` en el navegador, escribe el **mismo ID** y pulsa **Conectar**.
5. Los datos aparecerán en tiempo real. Sube la temperatura por encima del umbral para ver la alerta.

## Cómo funciona

El ESP32 publica un JSON `{"temp":..,"hum":..}` cada 2 segundos en su topic. El
dashboard está **suscrito** a ese topic, así que la pantalla se actualiza en cuanto
llega cada mensaje (modelo publicador/suscriptor de MQTT = tiempo real). Los botones
de la web publican `ON` / `OFF` en un topic de comandos para controlar el LED.

## Tecnologías

ESP32 · DHT22 · MQTT · broker EMQX (público) · MQTT.js · HTML/CSS/JS · Wokwi

---

**Autor:** Álvaro Buiza
Máster Desarrollo Full Stack + Arquitecturas Cloud · Tajamar Tech · 2025-2026
Post: [URL DEL POST] · Vídeo: [URL DEL VÍDEO]
