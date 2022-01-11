import machine
import network
import time

retry_limit = 120 # = one minute on default 10+490 = 500ms per blink

def blink(on_ms=10, off_ms=490):
  led = machine.Signal(machine.Pin(2,machine.Pin.OUT), invert=True)
  led.on()
  time.sleep_ms(on_ms)
  led.off()
  time.sleep_ms(off_ms)

sta = network.WLAN(network.STA_IF)
blink(500,500)

tries = 0
while not sta.isconnected() and tries < retry_limit:
  tries += 1
  blink()

blink()
