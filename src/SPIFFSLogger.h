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

#include "SPIFFSLoggerBase.h"

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
class SPIFFSLogger : public SPIFFSLoggerBase
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

    size_t readRowsBetween(SPIFFSLogData<T> *output, time_t fromTime, time_t toTime, size_t startIdx, size_t maxCount);

    /**
     * Get the number of entries for the specified date.
     * 
     * @param date  a time_t representing the day to check
     * @return      number of entries
     */
    size_t rowCount(time_t date);
};

template <class T>
SPIFFSLogger<T>::SPIFFSLogger(const char *directory, uint16_t daysToKeep, uint16_t processInterval)
    : SPIFFSLoggerBase(directory, daysToKeep, processInterval)
{
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
    maxCount = min((int32_t)maxCount, (int32_t)(f.size() / sizeof(SPIFFSLogData<T>)) - (int32_t)startIdx);
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

#endif // __SPIFFSLOGGER_H__
