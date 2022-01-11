# ESP8266 Tests
This repository holds minor projects that explores some aspect of an ESP8266.

* __[VoltageLogger](VoltageLogger/)__: Arduino framework project to read ADC value, scale it back to original voltage, and submit that data to an InfluxDB 2.0 server.
* __[MicroPythonADC](MicroPythonADC/)__: MicroPython project to read ADC value and publish it to MQTT. Calculating original pre-divided voltage and submitting to InfluxDB is done elsewhere. (Currently a Node-RED flow.)