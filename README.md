# SPIFFSLogger
A minimal library for binary data logging in ESP8266 systems.

## Features

* Easy to use
* Stores data in binary along with a UTC timestamp
* Results in about 50% space spavings when compared to logging the same data in CSV
* Splits data into daily files, allowing for efficient search
* Automatically rotates files, deleting the ones that exceed the specified age
* Uses built-in ESP/newlib time functions

## Installing

1. Download the [latest release](https://github.com/bitmario/SPIFFSLogger/releases/latest)
2. Extract it into your libraries folder
3. Rename the folder from `SPIFFSLogger-x.x.x` to `SPIFFSLogger`

## Getting started

See the summary below and try the [example](examples/Basic/Basic.ino). You may also want to take a peek at our [documentation](extras/docs/api.md).

### Required includes

```cpp
#include <SPIFFSLogger.h>
```

### The SPIFFSLogData<T\> struct

This struct represents the data as stored in SPIFFS (including the timestamp). It is returned in any logger read operations.

Definition:

```cpp
template <class T>
struct SPIFFSLogData
{
    time_t timestampUTC; /** creation time in UTC */
    T data;              /** data of type T */
};
```

### The SPIFFSLogger<T\> class 

This is the core of the library. You can create your logger like so:

```cpp
// store ints, saving files in /log/ and keeping today + 7 days of history
SPIFFSLogger<int> logger("/log", 7);
```

We can also store a struct (which is probably more useful!):

```cpp
struct EnvData {
    float temperature;
    float humidity;
    uint16_t pressure;
};

SPIFFSLogger<EnvData> logger("/log", 7);
```

#### Initializing the logger

This library uses the ESP8266 SPIFFS and built-in time functions so you must initialize those components and then call `logger.init()`.

Your `setup()` should look something like this:

```cpp
void setup() {
    // configure time however you like, we use NTP here
    wifiSetup(); // ommitted for brevity
    configTime(0, 0, "pool.ntp.org");
    
    // initialize SPIFFS
    SPIFFS.begin();
    
    // initialize our logger
    logger.init();
}
```

#### Writing data

Simply call `logger.write()` and the data will be logged with the current timestamp:

```cpp
struct EnvData data = { 23.54, 50.67, 1020 };
logger.write(data);
```

#### Reading data

Reading the first row of today's logfile:

```cpp
SPIFFSLogData<EnvData> data;
logger.readRows(&data, time(nullptr), 0, 1);
Serial.printf("TS: %d, T: %.2f, H: %.2f, %u\n",
              data.timestampUTC,
              data.data.temperature,
              data.data.humidity,
              data.data.pressure);
```

Or the last row:

```cpp
const time_t now = time(nullptr);
const size_t rowCount = logger.rowCount(now);
logger.readRows(&data, now, rowCount - 1, 1);
```

Or 25 rows starting at the 50th:

```cpp
SPIFFSLogData<EnvData> data[25];
logger.readRows(data, time(nullptr), 49, 25);
```

#### Filtering data

Retrieving data from the last 20 minutes:

```cpp
SPIFFSLogData<EnvData> data[25];
size_t count = logger.readRowsBetween(
                       &data,                // output
                       now - (60 * 20),      // time start (inclusive)
                       now,                  // time end (inclusive)
                       0,                    // start index within results
                       25                    // max number of rows to fetch
);

for (int i=0; i<count; i++) {
    Serial.printf("TS: %d, T: %.2f, H: %.2f, %u\n",
                  data[i].timestampUTC,
                  data[i].data.temperature,
                  data[i].data.humidity,
                  data[i].data.pressure);
}
```

## License
Licensed under the GNU LGPLv3, see the [LICENSE](LICENSE) file.
