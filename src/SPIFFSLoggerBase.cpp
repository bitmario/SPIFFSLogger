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

#include "SPIFFSLoggerBase.h"

// internal strings stored in flash for efficiency
const char _logFilenameFormat[] PROGMEM = "%s/%d%02d%02d";

SPIFFSLoggerBase::SPIFFSLoggerBase(const char *directory, uint16_t daysToKeep, uint16_t processInterval)
    : _daysToKeep(daysToKeep), _processInterval(processInterval)
{
    strncpy(this->_directory, directory, sizeof(this->_directory) - 1);
}

void SPIFFSLoggerBase::init()
{
    this->process();
}

void SPIFFSLoggerBase::process()
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

void SPIFFSLoggerBase::_pathFromDate(char *output, time_t date)
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

void SPIFFSLoggerBase::_updateCurPath()
{
    SPIFFSLoggerBase::_pathFromDate(this->_curPath, this->_today);
}

void SPIFFSLoggerBase::_runRotation()
{
    const uint8_t dirLen = strlen(this->_directory);
    Dir tempDir = SPIFFS.openDir(this->_directory);

    while (tempDir.next())
    {
        const char *dateStart = tempDir.fileName().c_str() + dirLen + 1;
        const time_t midnight = SPIFFSLoggerBase::_filenameToDate(dateStart);

        // check if file is too old and, if so, delete it
        if (midnight < (this->_today - this->_daysToKeep * 86400))
            SPIFFS.remove(tempDir.fileName());
    }
}

time_t SPIFFSLoggerBase::_filenameToDate(const char *filename)
{
    // we do a manual conversion since strptime() acts funny with YYYYMMDD dates
    // https://github.com/esp8266/Arduino/issues/5141
    struct tm tm = {0};
    char datePart[5] = {0};

    strncpy(datePart, filename, 4);
    tm.tm_year = atoi(datePart) - 1900;

    strncpy(datePart, filename + 4, 2);
    datePart[2] = '\0';
    tm.tm_mon = atoi(datePart) - 1;

    strncpy(datePart, filename + 6, 2);
    tm.tm_mday = atoi(datePart);

    return SPIFFSLoggerBase::_timegm(&tm) / 86400 * 86400;
}

time_t SPIFFSLoggerBase::_timegm(struct tm *tm)
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
