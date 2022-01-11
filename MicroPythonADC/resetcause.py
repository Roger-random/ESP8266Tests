import machine

cause = machine.reset_cause()

# Translate reset code to human-friendly string
if cause == machine.PWRON_RESET:
    cause_string = 'Powered On'
elif cause == machine.HARD_RESET:
    cause_string = 'Hard Reset'
elif cause == machine.WDT_RESET:
    cause_string = 'Watchdog Timer'
elif cause == machine.DEEPSLEEP_RESET:
    cause_string = 'Deep Sleep'
elif cause == machine.SOFT_RESET:
    cause_string = 'Soft Reset'

print('Reset cause is: {}'.format(cause_string))
