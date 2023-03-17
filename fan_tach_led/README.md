# Cooling fan LED strobe light effect.
A computer cooling fan usually has a tachometer wire that outputs a square
wave with a period corresponding to half a fan revolution. (Two pulses per
revolution.) This is used by system hardware to monitor fan speed, but in
this sketch we use it to trigger an interrupt. Every other trigger (once 
per revolution) an LED briefly illuminates. The result is that the fan
looks to be frozen in place when primarily/solely illuminated by the strobing
LED.

More details:
https://newscrewdriver.com/2022/09/01/led-strobing-to-fan-speed-signal/

Written for an ESP8266 microcontroller on a Wemos D1 Mini module, but can
be adapted to any other Arduino microcontroller. The biggest constraint is
probably interrupt support. See Arduino documentation to see which pins
are supported in attachInterrupt()

https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/

ESP8266 Arduino Core documentation on interrupts

https://arduino-esp8266.readthedocs.io/en/3.0.2/reference.html#interrupts
