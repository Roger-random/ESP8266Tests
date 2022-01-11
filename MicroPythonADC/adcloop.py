import machine
import time

class adcloop:
  def __init__(self):
    self.min = 1025
    self.max = 0
    self.vals = list()

  def run(self):
    while True:
      val = machine.ADC(0).read()
      if self.max < val:
        self.max = val
      if self.min > val:
        self.min = val
      self.vals.append(val)
      print("Latest: {:4d} Max: {:4d} Min: {:4d} Average: {:4.2f}".format(val, self.max, self.min, sum(self.vals)/len(self.vals)))
      time.sleep(1)
