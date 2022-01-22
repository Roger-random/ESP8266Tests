import machine
import network
import time

import secrets
import simple

led = machine.Signal(machine.Pin(2,machine.Pin.OUT), invert=True)

# In case of success, how many seconds to sleep before waking up to repeat.
sleep_time = 60

# In case of failure, how many seconds to wait before resetting.
# During this time we can connect and interrupt the script (Control-C) to
# gain access to REPL and debug.
stay_time = 30

def blink(on_ms=10, off_ms=490):
  led.on()
  time.sleep_ms(on_ms)
  led.off()
  time.sleep_ms(off_ms)

try:
  sta = network.WLAN(network.STA_IF)
  if sta.isconnected():
    print(sta.ifconfig())
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
      print("Connected to MQTT broker.")
      blink(10,100)
      blink(10,880)
      adc0_value = machine.ADC(0).read()
      print("Read ADC value {}".format(adc0_value))
      payload = '{{ "ADC0" : {} }}'.format(adc0_value)
      print("Publishing payload {}".format(payload))
      broker.publish(secrets.mqtt_broker.topic, bytearray(payload))
      print("Disconnecting from MQTT.")
      broker.disconnect()
      blink(10,100)
      blink(10,880)
      print("Going to sleep.")
      machine.deepsleep(sleep_time*1000)
    else:
      raise Exception("MQTT broker connect failed with error code {}".format(connect_code))
  else:
    raise Exception("Network not connected.")
except Exception as error:
  print(type(error))
  print(error)
  print("Staying alive for a bit before reset and retry...")
  led.on()
  for i in range(stay_time):
    print(stay_time-i)
    time.sleep(1)
  machine.reset()
  