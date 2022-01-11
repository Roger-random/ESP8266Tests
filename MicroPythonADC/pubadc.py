import machine
import time

import secrets
import simple

led = machine.Signal(machine.Pin(2,machine.Pin.OUT), invert=True)

def blink(on_ms=10, off_ms=490):
  led.on()
  time.sleep_ms(on_ms)
  led.off()
  time.sleep_ms(off_ms)

# keepalive=30 is workaround to avoid MQTTException 2 upon connect. However,
# it isn't "real" keepalive so... we need to get everything done within 30s?
# See https://github.com/micropython/micropython-lib/issues/445
broker = simple.MQTTClient(
  secrets.mqtt_broker.username, # Client ID is same as username. (Is this a bad idea?)
  secrets.mqtt_broker.address ,
  user=secrets.mqtt_broker.username,
  password=secrets.mqtt_broker.password,
  keepalive=30)

connect_code = broker.connect()

if 0 == connect_code:
  print("Connected to MQTT broker")
  blink(10,100)
  blink(10,880)
  adc0_value = machine.ADC(0).read()
  print("Read ADC value {}".format(adc0_value))
  payload = '{{ "ADC0" : {} }}'.format(adc0_value)
  print("Publishing payload {}".format(payload))
  broker.publish(secrets.mqtt_broker.topic, bytearray(payload))
  print("Disconnecting from MQTT")
  broker.disconnect()
  blink(10,100)
  blink(10,880)
  print("Going to sleep")
  machine.deepsleep(60000)
else:
  print("Error: MQTTClient.connect() returned nonzero code {}".format(connect_code))
  led.on()
