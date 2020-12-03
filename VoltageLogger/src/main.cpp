#include <Arduino.h>
#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// Application secrets live in this file, GitHub only has a template that needs to be filled in.
// Once secrets.h has been pulled into local repository, run "git update-index --skip-worktree secrets.h"
// to ensure the filled-in secrets values are not sent back to Git.
#include "secrets.h"

// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
// Examples:
//  Pacific Time: "PST8PDT"
//  Eastern: "EST5EDT"
//  Japanesse: "JST-9"
//  Central Europe: "CET-1CEST,M3.5.0,M10.5.0/3"
#define TZ_INFO "PST8PDT"

// We do something once every WAIT_MILLIS milliseconds.
#define WAIT_MILLIS 1000

// Timer value of last action
unsigned long lastMillis;

// Numerator:
//  100 kOhm resistor between ESP8266 A0 and GND
//  220 kOhm resistor between board A0 pin and ESP8266 A0.
//  2 * 1 MOhm resistor between input source and board A0 pin
//  Total resistance = 2.32 MOhm, 100 kOhm between ESP8266 A0 and GND = 23.2 multiplier.
// Denominator:
//  ADC measures 0 to 1V in 10-bit precision.
//  2^10 = 1024 = 1 Volt
#define VOLTAGE_DIVIDER_FACTOR 23.2 / 1024

// VOLTAGE_DIVIDER_FACTOR is the theoretical value. In practice, resistances
// vary. This correction factor is calculated by comparing against value
// reported by a Fluke voltmeter.
#define CORRECTION_FACTOR 0.96

// Aggregrate ADC values
#define ADC_SAMPLES 9
unsigned int adcCount;
unsigned int adcSum;

// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Data point
Point sensor("voltage");

void setup() {
  Serial.begin(115200);

  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Add tags
  sensor.addTag("type", "ADC0");
  sensor.addTag("MAC", WiFi.macAddress());

  // Accurate time is necessary for certificate validation and writing in batches
  // For the fastest time sync find NTP servers in your area: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  // Prepare for analog multi-sampling
  adcCount = 0;
  adcSum = 0;

  // Start the clock for loop() code.
  lastMillis = millis();
}

void loop() {
  unsigned long loopMillis = millis();

  if (loopMillis < lastMillis) {
    Serial.println("millis() wraparound detected.");

    // If precision is important, we can do math about ULONG_MAX-lastMillis + loopMillis and compare.
    // But if we don't, we can just skip a loop.
    lastMillis = loopMillis;
  }
  else if (loopMillis-lastMillis > WAIT_MILLIS)
  {
    // We read ADC through ADC_SAMPLES*WAIT_MILLIS loops, then submit data on the tenth.
    // The period between reported datapoints are thus (ADC_SAMPLES+1) * WAIT_MILLIS.
    if (adcCount < ADC_SAMPLES)
    {
      adcSum += analogRead(A0);
      adcCount++;
    }
    else
    {
      // Clear fields for reusing the point. Tags will remain untouched
      sensor.clearFields();

      // Divide accumulated value by number of samples to get average value.
      int average = adcSum/ADC_SAMPLES;
      sensor.addField("raw", average);

      // See comments for VOLTAGE_DIVIDER_FACTOR and CORRECTION_FACTOR for details on
      // how this translates the average ADC value into average voltage.
      sensor.addField("V", average * VOLTAGE_DIVIDER_FACTOR * CORRECTION_FACTOR);

      // Print what are we exactly writing
      Serial.print("Writing: ");
      Serial.println(client.pointToLineProtocol(sensor));

      // If no Wifi signal, try to reconnect it
      if ((WiFi.RSSI() == 0) && (wifiMulti.run() != WL_CONNECTED)) {
        Serial.println("Wifi connection lost");
      }

      // Write point
      if (!client.writePoint(sensor)) {
        Serial.print("InfluxDB write failed: ");
        Serial.println(client.getLastErrorMessage());
      }

      // Reset for next set of multisampling.
      adcCount = 0;
      adcSum = 0;
    }

    lastMillis = loopMillis;
  }
}
