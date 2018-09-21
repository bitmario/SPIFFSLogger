#include <time.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <SPIFFSLogger.h>

#define SSID "Lab"
#define PSK "WifiPassword"

#define LOGGING_INTERVAL 5000

// struct that defines the data we would like to log
struct MyData
{
    float temp;
    float humidity;
};

// create a new logger which will store records of our MyData type in files with names like
// /log/mydata/YYYYMMDD, keeping 1 day of history
SPIFFSLogger<MyData> logger("/log/mydata", 1);

unsigned long lastLog = 0;

void setup()
{
    Serial.begin(115200);

    // we need wifi+NTP (or local/RTC) ESP8266 time
    wifiSetup();
    timeSetup();

    // we must always initialize the SPIFFS before doing anything with the logger
    SPIFFS.begin();

    // initialize our logger
    logger.init();

    // our data record size
    Serial.printf("sizeof(MyData): %u\n", sizeof(MyData));
    // time_t timestamp + our data, as stored in SPIFFS
    Serial.printf("sizeof(SPIFFSLogData<MyData>): %u\n", sizeof(SPIFFSLogData<MyData>));
}

void loop()
{
    const unsigned long currentMillis = millis();

    if (currentMillis - lastLog > LOGGING_INTERVAL)
    {
        // this is where you would fetch the values, but we just generate them randomly
        struct MyData data = {
            random(200, 300) / 10.0, // temp
            random(300, 700) / 10.0  // humidity
        };
        // log the data
        Serial.printf("Writing temp: %.1f, humidity: %.1f\n", data.temp, data.humidity);
        logger.write(data);

        // fetch and display last values
        const time_t now = time(nullptr);
        const size_t rowCount = logger.rowCount(now);
        struct SPIFFSLogData<MyData> readback;

        // read 1 item starting at index rowCount-1 from the logfile corresponding to the date now
        // and into the readback variable
        logger.readRows(&readback, now, rowCount - 1, 1);
        Serial.printf("Readback time: %d, temp: %.1f, humidity: %.1f\n",
                      readback.timestampUTC,
                      readback.data.temp,
                      readback.data.humidity);

        Serial.printf("Current logfile has %u rows.\n", rowCount);

        // filtering example
        logger.readRowsBetween(&readback,      // output
                               now - (60 * 5), // time start (inclusive)
                               now,            // time end (inclusive)
                               0,              // start index
                               1               // max number of rows to fetch (size your output buffer accordingly!)
        );
        Serial.printf("First record in the last 5 minutes: %d, temp: %.1f, humidity: %.1f\n",
                      readback.timestampUTC,
                      readback.data.temp,
                      readback.data.humidity);

        Serial.println();

        lastLog = currentMillis;
    }

    // Processes file name changes and old file deletion according to the interval set
    // at initialization. Should always be called in the loop.
    logger.process();
}

void wifiSetup()
{
    WiFi.begin(SSID, PSK);

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}

void timeSetup()
{
    // details on ESP8266 time: https://github.com/d-a-v/Arduino/blob/goodies/libraries/esp8266/examples/NTP-TZ-DST/NTP-TZ-DST.ino

    // automatically sync UTC time using NTP
    configTime(0, 0, "pool.ntp.org");

    // optionally, we could set a timezone as well, e.g.:
    // setenv("TZ", "WET0CET-1,M3.5.0/2:00:00,M10.5.0/2:00:00", 1);
    // tzset();
}
