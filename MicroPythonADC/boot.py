# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import uos, machine
#uos.dupterm(None, 1) # disable REPL on UART(0)

print()
print('--------------------')
# Print reason behind the most recent reset
import resetcause

# Connect to WIFI
import wificonnect

# Start WebREPL
import gc
import webrepl
webrepl.start()
gc.collect()

print('--------------------')
print()
