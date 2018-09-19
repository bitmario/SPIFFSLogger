/*
SPIFFSLogger - A minimal library for binary data logging in ESP8266
Copyright (C) 2018 Mario Falcao

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __SPIFFSLOGGER_H__
#define __SPIFFSLOGGER_H__

#include <Arduino.h>
#include <FS.h>
#include <time.h>

// internal strings stored in flash for efficiency
static const char _logFilenameFormat[] PROGMEM = "%s/%d%02d%02d";

/**
 * Represents a data element as stored in SPIFFS, along with the creation timestamp.
 * 
 * @tparam T  type to store, e.g. a struct.
 */
template <class T>
struct SPIFFSLogData
{
    time_t timestampUTC; /**< creation time in UTC */
    T data;              /**< data of type T */
};

/**
 * Minimal class template for binary data logging in ESP8266 SPIFFS.
 * 
 * Makes it easy to read, write and keep logs of relevant data, such as sensor readings in an efficient way,
 * by storing data in raw, binary format along with a time_t. One file is created per UTC day to store the 
 * respective data, and deleted once it has reached the defined maximum age.
 * 
 * @tparam T  type to store, e.g. a struct.
 */
template <class T>
class SPIFFSLogger
{
  public:
    /**
     * Default constructor for SPIFFSLogger.
     * 
     * @param directory        char array with the base directory where files will be stored. Should not include
     *                         trailing slash and should be 20 characters or less.
     * @param daysToKeep       number of days to keep in flash. Once files are past this age, they are deleted.
     * @param processInterval  milliseconds between file directory updates and file rotation.
     */
    SPIFFSLogger(const char *directory, uint16_t daysToKeep = 7, uint16_t processInterval = 1000);

    /**
     * Initialize the logger. Should be called after initializing SPIFFS and before before any other method.
     */
    void init();

    /**
     * Process the file rotation and other required operations according to the defined processInterval.
     * Should be called as often as possible, i.e. in loop().
     */
    void process();

    /**
     * Write the specified value to the end of the current log file, with the current timestamp.
     * 
     * @param value  the value to write
     */
    size_t write(const T &value);

    /**
     * Read data from a daily logfile into a buffer.
     * 
     * @param output    buffer where data will be written
     * @param date      time_t representing the date of the file
     * @param startIdx  0-based index of the entries to read (row number)
     * @param maxCount  maximum number of entries to read
     * @return          number of entries read
     */
    size_t readRows(SPIFFSLogData<T> *output, time_t date, size_t startIdx, size_t maxCount);

    /**
     * Get the number of entries for the specified date.
     * 
     * @param date  a time_t representing the day to check
     * @return      number of entries
     */
    size_t rowCount(time_t date);

  protected:
    time_t _today = 0;               /**< current date, set in the last processing run */
    unsigned long _lastProcess = 0;  /**< last processing millis() */
    const uint16_t _processInterval; /**< ms between processing runs */
    const uint16_t _daysToKeep;      /**< number of days to keep logs for */
    bool _processNow = true;         /**< force processing now, even if the processing interval hasn't passed */
    char _directory[21];             /**< base directory for log files */
    char _curPath[32];               /**< path for today's file */

    /**
     * Converts a time_t to that day's file path.
     */
    void _pathFromDate(char *output, time_t date);

    /**
     * Updates the current path to match today's date.
     */
    void _updateCurPath();

    /**
     * Deletes files older than the defined age limit.
     */
    void _runRotation();

    /**
     * Simple timegm function, since one is not available in the ESP libs.
     */
    static time_t _timegm(struct tm *tm);
};

template <class T>
SPIFFSLogger<T>::SPIFFSLogger(const char *directory, uint16_t daysToKeep, uint16_t processInterval)
    : _daysToKeep(daysToKeep), _processInterval(processInterval)
{
    strncpy(this->_directory, directory, sizeof(this->_directory) - 1);
}

template <class T>
void SPIFFSLogger<T>::init()
{
    this->process();
}

template <class T>
void SPIFFSLogger<T>::process()
{
    const unsigned long currentMillis = millis();
    if (currentMillis - this->_lastProcess > this->_processInterval || this->_processNow)
    {
        const time_t now = time(nullptr);
        const time_t today = now / 86400 * 86400; // remove the time part
        if (this->_today != today)
        { // we have switched to another day, let's run the required updates
            this->_today = today;
            this->_updateCurPath();
            this->_runRotation();
        }

        this->_lastProcess = currentMillis;
        this->_processNow = false;
    }
}

template <class T>
size_t SPIFFSLogger<T>::write(const T &value)
{
    struct SPIFFSLogData<T> data;
    data.timestampUTC = time(nullptr);
    data.data = value;

    File f = SPIFFS.open(this->_curPath, "a");
    f.write((uint8_t *)&data, sizeof(data));
    f.close();
}

template <class T>
size_t SPIFFSLogger<T>::readRows(SPIFFSLogData<T> *output, time_t date, size_t startIdx, size_t maxCount)
{
    char path[32];
    this->_pathFromDate(path, date);

    if (!SPIFFS.exists(path))
        return 0;

    File f = SPIFFS.open(path, "r");
    maxCount = min(maxCount, (int32_t)(f.size() / sizeof(SPIFFSLogData<T>)) - (int32_t)startIdx);
    if (maxCount <= 0)
        return maxCount;

    f.seek(startIdx * sizeof(SPIFFSLogData<T>));
    f.read((uint8_t *)output, maxCount * sizeof(SPIFFSLogData<T>));
    f.close();
    return maxCount;
}

template <class T>
size_t SPIFFSLogger<T>::rowCount(time_t date)
{
    char path[32];
    this->_pathFromDate(path, date);

    if (!SPIFFS.exists(path))
        return 0;

    File f = SPIFFS.open(path, "r");
    const size_t rows = f.size() / sizeof(SPIFFSLogData<T>);
    f.close();

    return rows;
}

template <class T>
void SPIFFSLogger<T>::_pathFromDate(char *output, time_t date)
{
    if (date <= 0)
        date = this->_today;

    struct tm *tinfo = gmtime(&date);
    sprintf_P(output,
              _logFilenameFormat,
              this->_directory,
              1900 + tinfo->tm_year,
              tinfo->tm_mon + 1,
              tinfo->tm_mday);
}

template <class T>
void SPIFFSLogger<T>::_updateCurPath()
{
    SPIFFSLogger::_pathFromDate(this->_curPath, this->_today);
}

template <class T>
void SPIFFSLogger<T>::_runRotation()
{
    const uint8_t dirLen = strlen(this->_directory);
    Dir tempDir = SPIFFS.openDir(this->_directory);

    char datePart[5] = {0};
    while (tempDir.next())
    {
        struct tm tm = {0};
        const char *dateStart = tempDir.fileName().c_str() + dirLen + 1;

        strncpy(datePart, dateStart, 4);
        tm.tm_year = atoi(datePart) - 1900;

        strncpy(datePart, dateStart + 4, 2);
        datePart[2] = '\0';
        tm.tm_mon = atoi(datePart) - 1;

        strncpy(datePart, dateStart + 6, 2);
        tm.tm_mday = atoi(datePart);

        const time_t midnight = SPIFFSLogger::_timegm(&tm) / 86400 * 86400;

        // check if file is too old and, if so, delete it
        if (midnight < (this->_today - this->_daysToKeep * 86400))
            SPIFFS.remove(tempDir.fileName());
    }
}

template <class T>
time_t SPIFFSLogger<T>::_timegm(struct tm *tm)
{
    struct tm start2000 = {
        0,   // tm_sec
        0,   // tm_min
        0,   // tm_hour
        1,   // tm_mday
        0,   // tm_mon
        100, // tm_year
        0,   // tm_wday
        0,   // tm_yday
        0,   // tm_isdst
    };

    return mktime(tm) - (mktime(&start2000) - 946684800);
}

#endif // __SPIFFSLOGGER_H__
