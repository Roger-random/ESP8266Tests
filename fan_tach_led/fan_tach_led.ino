/*
 * Cooling fan LED strobe light effect.
 * 
 * A computer cooling fan usually has a tachometer wire that outputs a square
 * wave with a period corresponding to half a fan revolution. (Two pulses per
 * revolution.) This is used by system hardware to monitor fan speed, but in
 * this sketch we use it to trigger an interrupt. Every other trigger (once 
 * per revolution) an LED briefly illuminates. The result is that the fan
 * looks to be frozen in place if primarily/solely illuminated by the strobing
 * LED.
 *
 * Written for an ESP8266 microcontroller on a Wemos D1 Mini module, but can
 * be adapted to any other Arduino microcontroller. The biggest constraint is
 * probably interrupt support. See Arduino documentation to see which pins
 * are supported in attachInterrupt()
 * https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
 *
 * ESP8266 Arduino Core documentation on interrupts
 * https://arduino-esp8266.readthedocs.io/en/3.0.2/reference.html#interrupts
 *
 *
 * Copyright (c) Roger Cheng
 *
 * MIT License
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Volatile keyword tells the compiler not to be clever about this data. This prefix
// is recommended for variables touched by an interrupt handler routine because
// interrupts occur at unpredictable times.
volatile int evenOdd;

// Configure IO pins and attach interrupt handler
void setup() {
  // Attach interrupt handler to fan tachometer wire. Make sure this pin supports interrupts.
  evenOdd = 0;
  pinMode(12, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(12), tach_pulse_handler, RISING);

  // Configure LED power control and turn it off.
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  // Configure fan power control and turn it on.
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
}

// Loop function executes repeatedly, typically once every mid-teens of milliseconds but not guaranteed.
void loop() {
  // Turn off the strobe LED which may or may not have been turned on by the
  // tachometer pulse interrupt handler.
  digitalWrite(13, LOW);
}

// IRAM_ATTR prefix is required specifically for ESP8266 interrupt handlers.
// https://arduino-esp8266.readthedocs.io/en/3.0.2/reference.html#interrupts
// IRAM_ATTR should be removed for other microcontroller architectures or substituted
// with a different architecture-specific keyword. See your microcontroller's
// documentation for details on interrupt handlers.
IRAM_ATTR void tach_pulse_handler() {
  if (0 == evenOdd) {
    // Ignore every other pulse
    evenOdd = 1;
  } else {
    // Illuminate the LED
    digitalWrite(13, HIGH);

    // Do not use delay() in an interrupt handler, but delayMicroseconds()
    // is usually OK. Shorter times will result in dimmer illumination, longer
    // times will result in motion blur. The ideal delay will depend on fan RPM,
    // response time of LED circuit (including LED themselves), and ambient
    // light. It is possible the ideal delay is none at all (delete this line.)
    delayMicroseconds(200);

    // Ignore next pulse
    evenOdd = 0;
  }
}
