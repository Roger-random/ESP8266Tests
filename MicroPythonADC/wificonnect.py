import machine
import network
import time

wifi_ssid = '' # Fill in your SSID
wifi_password = '' # Fill in your password
my_hostname = None # Network name for this node

connection_check_ms = 100 # Number of milliseconds between checking for connectivity
connection_timeout = 50 # Number of connection_check_ms before we fail out.

pin = machine.Pin(2, machine.Pin.OUT)
led = machine.Signal(pin, invert=True)
led.off()

# We are not an access point today.
ap = network.WLAN(network.AP_IF)
ap.active(False)

# Connect to an access point.
sta = network.WLAN(network.STA_IF)
if my_hostname != None:
    sta.config(dhcp_hostname=my_hostname)
    print('Starting {}'.format(my_hostname))
sta.active(True)
print('Connecting to {}'.format(wifi_ssid))
sta.connect(wifi_ssid, wifi_password)

connection_checks = 0

while not sta.isconnected() and connection_checks < connection_timeout:
    time.sleep_ms(connection_check_ms)

if sta.isconnected():
    led.on()
    print('{} connected: {}'.format(wifi_ssid, sta.ifconfig()))
else:
    print('Failed to connect to {}'.format(wifi_ssid))
