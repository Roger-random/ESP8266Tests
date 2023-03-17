# ESP8266 Tests
This repository holds minor projects that explores some aspect of an ESP8266.

* __[VoltageLogger](VoltageLogger/)__: Arduino framework project to read ADC value, scale it back to original voltage, and submit that data to an InfluxDB 2.0 server.
* __[MicroPythonADC](MicroPythonADC/)__: MicroPython project to read ADC value and publish it to MQTT. Calculating original pre-divided voltage and submitting to InfluxDB is done elsewhere. (Currently a Node-RED flow.)
* __[fan_tach_led](fan_tach_led/)__: Arduino project using `attachInterrupt()` to pulse a LED in time with a cooling fan's tachometer wire. Results in a strobe light that visually freezes fan blade movement.
* __[pio_webserver](pio_webserver/)__: PlatformIO Arduino project to play with [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) and other web server related things.
