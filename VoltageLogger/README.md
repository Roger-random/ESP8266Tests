# Voltage Logger
This project uses a Wemos D1 Mini Lite clone, based around an ESP8266, to
periodically measure voltage using ESP8266's onboard ADC. After several
measurements are taken, their average value is submitted to an InfluxDB 2.0
server.

This project uses the Arduino library for ESP8266 via PlatformIO extension
for Visual Studio Code.

InfluxDB interaction was made easy with the
[InfluxDB Arduino Client](https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino)
by Tobias Schürg

Application-specific secrets like WiFI and Database information are in the
header file template ```secrets.h```. Once filled in, run
```git update-index --skip-worktree secrets.h``` to make sure access
information is not accidentally submitted to GitHub. Thanks to
[this StackOverflow thread](https://stackoverflow.com/questions/1274057/how-to-make-git-forget-about-a-file-that-was-tracked-but-is-now-in-gitignore)
for the technique.

Here is
[an Amazon affiliate link](https://amzn.to/2JFcuxw)
for the D1 Mini module I used. As an Amazon Associate I earn from qualifying
purchases.