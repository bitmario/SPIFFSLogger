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

#ifndef __SPIFFSLOGGERBASE_H__
#define __SPIFFSLOGGERBASE_H__

#include <Arduino.h>
#include <FS.h>
#include <time.h>

/**
 * Base class for the SPIFFSLogger template, implementing common methods. Should not be used directly.
 */
class SPIFFSLoggerBase
{
  public:
    /**
     * Initialize the logger. Should be called after initializing SPIFFS and before before any other method.
     */
    void init();

    /**
     * Process the file rotation and other required operations according to the defined processInterval.
     * Should be called as often as possible, i.e. in loop().
     */
    void process();

  protected:
    time_t _today = 0;               /**< current date, set in the last processing run */
    unsigned long _lastProcess = 0;  /**< last processing millis() */
    const uint16_t _processInterval; /**< ms between processing runs */
    const uint16_t _daysToKeep;      /**< number of days to keep logs for */
    bool _processNow = true;         /**< force processing now, even if the processing interval hasn't passed */
    char _directory[21];             /**< base directory for log files */
    char _curPath[32];               /**< path for today's file */

    SPIFFSLoggerBase(const char *directory, uint16_t daysToKeep, uint16_t processInterval);

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
     * Converts a filename in YYYYMMDD format to a time_t.
     */
    static time_t _filenameToDate(const char *filename);

    /**
     * Simple timegm function, since one is not available in the ESP libs.
     */
    static time_t _timegm(struct tm *tm);
};

#endif // __SPIFFSLOGGERBASE_H__
