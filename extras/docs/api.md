# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`SPIFFSLogger`](#class_s_p_i_f_f_s_logger) | Minimal class template for binary data logging in ESP8266 SPIFFS.
`class `[`SPIFFSLoggerBase`](#class_s_p_i_f_f_s_logger_base) | Base class for the [SPIFFSLogger](#class_s_p_i_f_f_s_logger) template, implementing common methods.
`struct `[`SPIFFSLogData`](#struct_s_p_i_f_f_s_log_data) | Represents a data element as stored in SPIFFS, along with the creation timestamp.

# class `SPIFFSLogger` 

```
class SPIFFSLogger
  : public SPIFFSLoggerBase
```  

Minimal class template for binary data logging in ESP8266 SPIFFS.

Makes it easy to read, write and keep logs of relevant data, such as sensor readings in an efficient way, by storing data in raw, binary format along with a time_t. One file is created per UTC day to store the respective data, and deleted once it has reached the defined maximum age.

#### Parameters
* `T` type to store, e.g. a struct.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`SPIFFSLogger`](#class_s_p_i_f_f_s_logger_1afa8152a5d5d29fedbb04265cb4589359)`(const char * directory,uint16_t daysToKeep,uint16_t processInterval)` | Default constructor for [SPIFFSLogger](#class_s_p_i_f_f_s_logger).
`public size_t `[`write`](#class_s_p_i_f_f_s_logger_1a2e914dd1884de9900288564a41bc55a4)`(const T & value)` | Write the specified value to the end of the current log file, with the current timestamp.
`public size_t `[`readRows`](#class_s_p_i_f_f_s_logger_1aeceb2db3a41feb387e874f247657e19c)`(`[`SPIFFSLogData`](#struct_s_p_i_f_f_s_log_data)`< T > * output,time_t date,size_t startIdx,size_t maxCount)` | Read data from a daily logfile into a buffer.
`public size_t `[`readRowsBetween`](#class_s_p_i_f_f_s_logger_1a8d82d01f694528becba720e0612dc48d)`(`[`SPIFFSLogData`](#struct_s_p_i_f_f_s_log_data)`< T > * output,time_t fromTime,time_t toTime,size_t startIdx,size_t maxCount)` | 
`public size_t `[`rowCount`](#class_s_p_i_f_f_s_logger_1ac539407d1bfebe9443393ea2e4047e29)`(time_t date)` | Get the number of entries for the specified date.
`public void `[`init`](#class_s_p_i_f_f_s_logger_base_1ad2270960852a999b0340fa4eab50f063)`()` | Initialize the logger.
`public void `[`process`](#class_s_p_i_f_f_s_logger_base_1a22fdd2b540717853ae403edbc0ed1b7f)`()` | Process the file rotation and other required operations according to the defined processInterval.
`protected time_t `[`_today`](#class_s_p_i_f_f_s_logger_base_1a77a967ae68a680dea5a647200ba49815) | current date, set in the last processing run
`protected unsigned long `[`_lastProcess`](#class_s_p_i_f_f_s_logger_base_1ac1b33a4c97b2a45b2b178a261ebbb12c) | last processing millis()
`protected const uint16_t `[`_processInterval`](#class_s_p_i_f_f_s_logger_base_1a517fa4d3854283ba7b2dba9bf0d09c84) | ms between processing runs
`protected const uint16_t `[`_daysToKeep`](#class_s_p_i_f_f_s_logger_base_1a8feb62f012879aca79d600cf217c2098) | number of days to keep logs for
`protected bool `[`_processNow`](#class_s_p_i_f_f_s_logger_base_1a978f0a636ce4d7a7f1640df124cc25ff) | force processing now, even if the processing interval hasn't passed
`protected char `[`_directory`](#class_s_p_i_f_f_s_logger_base_1a829da5d8bf3d83ed24e608bb60fc623c) | base directory for log files
`protected char `[`_curPath`](#class_s_p_i_f_f_s_logger_base_1a7f79553c53c7b5b6c87aaaa104d5a1e3) | path for today's file
`protected void `[`_pathFromDate`](#class_s_p_i_f_f_s_logger_base_1abfba26125f8b32b4c8e2e3d29d65b31f)`(char * output,time_t date)` | Converts a time_t to that day's file path.
`protected void `[`_updateCurPath`](#class_s_p_i_f_f_s_logger_base_1aa5679e47e144e591b5fa90fb31b65cbd)`()` | Updates the current path to match today's date.
`protected void `[`_runRotation`](#class_s_p_i_f_f_s_logger_base_1a2e9e45428e1ffbe989dd7679c5def4fe)`()` | Deletes files older than the defined age limit.

## Members

#### `public  `[`SPIFFSLogger`](#class_s_p_i_f_f_s_logger_1afa8152a5d5d29fedbb04265cb4589359)`(const char * directory,uint16_t daysToKeep,uint16_t processInterval)` 

Default constructor for [SPIFFSLogger](#class_s_p_i_f_f_s_logger).

#### Parameters
* `directory` char array with the base directory where files will be stored. Should not include trailing slash and should be 20 characters or less. 

* `daysToKeep` number of days to keep in flash. Once files are past this age, they are deleted. 

* `processInterval` milliseconds between file directory updates and file rotation.

#### `public size_t `[`write`](#class_s_p_i_f_f_s_logger_1a2e914dd1884de9900288564a41bc55a4)`(const T & value)` 

Write the specified value to the end of the current log file, with the current timestamp.

#### Parameters
* `value` the value to write

#### `public size_t `[`readRows`](#class_s_p_i_f_f_s_logger_1aeceb2db3a41feb387e874f247657e19c)`(`[`SPIFFSLogData`](#struct_s_p_i_f_f_s_log_data)`< T > * output,time_t date,size_t startIdx,size_t maxCount)` 

Read data from a daily logfile into a buffer.

#### Parameters
* `output` buffer where data will be written 

* `date` time_t representing the date of the file 

* `startIdx` 0-based index of the entries to read (row number) 

* `maxCount` maximum number of entries to read 

#### Returns
number of entries read

#### `public size_t `[`readRowsBetween`](#class_s_p_i_f_f_s_logger_1a8d82d01f694528becba720e0612dc48d)`(`[`SPIFFSLogData`](#struct_s_p_i_f_f_s_log_data)`< T > * output,time_t fromTime,time_t toTime,size_t startIdx,size_t maxCount)` 

#### `public size_t `[`rowCount`](#class_s_p_i_f_f_s_logger_1ac539407d1bfebe9443393ea2e4047e29)`(time_t date)` 

Get the number of entries for the specified date.

#### Parameters
* `date` a time_t representing the day to check 

#### Returns
number of entries

#### `public void `[`init`](#class_s_p_i_f_f_s_logger_base_1ad2270960852a999b0340fa4eab50f063)`()` 

Initialize the logger.

Should be called after initializing SPIFFS and before before any other method.

#### `public void `[`process`](#class_s_p_i_f_f_s_logger_base_1a22fdd2b540717853ae403edbc0ed1b7f)`()` 

Process the file rotation and other required operations according to the defined processInterval.

Should be called as often as possible, i.e. in loop().

#### `protected time_t `[`_today`](#class_s_p_i_f_f_s_logger_base_1a77a967ae68a680dea5a647200ba49815) 

current date, set in the last processing run

#### `protected unsigned long `[`_lastProcess`](#class_s_p_i_f_f_s_logger_base_1ac1b33a4c97b2a45b2b178a261ebbb12c) 

last processing millis()

#### `protected const uint16_t `[`_processInterval`](#class_s_p_i_f_f_s_logger_base_1a517fa4d3854283ba7b2dba9bf0d09c84) 

ms between processing runs

#### `protected const uint16_t `[`_daysToKeep`](#class_s_p_i_f_f_s_logger_base_1a8feb62f012879aca79d600cf217c2098) 

number of days to keep logs for

#### `protected bool `[`_processNow`](#class_s_p_i_f_f_s_logger_base_1a978f0a636ce4d7a7f1640df124cc25ff) 

force processing now, even if the processing interval hasn't passed

#### `protected char `[`_directory`](#class_s_p_i_f_f_s_logger_base_1a829da5d8bf3d83ed24e608bb60fc623c) 

base directory for log files

#### `protected char `[`_curPath`](#class_s_p_i_f_f_s_logger_base_1a7f79553c53c7b5b6c87aaaa104d5a1e3) 

path for today's file

#### `protected void `[`_pathFromDate`](#class_s_p_i_f_f_s_logger_base_1abfba26125f8b32b4c8e2e3d29d65b31f)`(char * output,time_t date)` 

Converts a time_t to that day's file path.

#### `protected void `[`_updateCurPath`](#class_s_p_i_f_f_s_logger_base_1aa5679e47e144e591b5fa90fb31b65cbd)`()` 

Updates the current path to match today's date.

#### `protected void `[`_runRotation`](#class_s_p_i_f_f_s_logger_base_1a2e9e45428e1ffbe989dd7679c5def4fe)`()` 

Deletes files older than the defined age limit.

# class `SPIFFSLoggerBase` 

Base class for the [SPIFFSLogger](#class_s_p_i_f_f_s_logger) template, implementing common methods.

Should not be used directly.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`init`](#class_s_p_i_f_f_s_logger_base_1ad2270960852a999b0340fa4eab50f063)`()` | Initialize the logger.
`public void `[`process`](#class_s_p_i_f_f_s_logger_base_1a22fdd2b540717853ae403edbc0ed1b7f)`()` | Process the file rotation and other required operations according to the defined processInterval.
`protected time_t `[`_today`](#class_s_p_i_f_f_s_logger_base_1a77a967ae68a680dea5a647200ba49815) | current date, set in the last processing run
`protected unsigned long `[`_lastProcess`](#class_s_p_i_f_f_s_logger_base_1ac1b33a4c97b2a45b2b178a261ebbb12c) | last processing millis()
`protected const uint16_t `[`_processInterval`](#class_s_p_i_f_f_s_logger_base_1a517fa4d3854283ba7b2dba9bf0d09c84) | ms between processing runs
`protected const uint16_t `[`_daysToKeep`](#class_s_p_i_f_f_s_logger_base_1a8feb62f012879aca79d600cf217c2098) | number of days to keep logs for
`protected bool `[`_processNow`](#class_s_p_i_f_f_s_logger_base_1a978f0a636ce4d7a7f1640df124cc25ff) | force processing now, even if the processing interval hasn't passed
`protected char `[`_directory`](#class_s_p_i_f_f_s_logger_base_1a829da5d8bf3d83ed24e608bb60fc623c) | base directory for log files
`protected char `[`_curPath`](#class_s_p_i_f_f_s_logger_base_1a7f79553c53c7b5b6c87aaaa104d5a1e3) | path for today's file
`protected  `[`SPIFFSLoggerBase`](#class_s_p_i_f_f_s_logger_base_1ace02cadd2e608c940728ae9f5242bbcc)`(const char * directory,uint16_t daysToKeep,uint16_t processInterval)` | 
`protected void `[`_pathFromDate`](#class_s_p_i_f_f_s_logger_base_1abfba26125f8b32b4c8e2e3d29d65b31f)`(char * output,time_t date)` | Converts a time_t to that day's file path.
`protected void `[`_updateCurPath`](#class_s_p_i_f_f_s_logger_base_1aa5679e47e144e591b5fa90fb31b65cbd)`()` | Updates the current path to match today's date.
`protected void `[`_runRotation`](#class_s_p_i_f_f_s_logger_base_1a2e9e45428e1ffbe989dd7679c5def4fe)`()` | Deletes files older than the defined age limit.

## Members

#### `public void `[`init`](#class_s_p_i_f_f_s_logger_base_1ad2270960852a999b0340fa4eab50f063)`()` 

Initialize the logger.

Should be called after initializing SPIFFS and before before any other method.

#### `public void `[`process`](#class_s_p_i_f_f_s_logger_base_1a22fdd2b540717853ae403edbc0ed1b7f)`()` 

Process the file rotation and other required operations according to the defined processInterval.

Should be called as often as possible, i.e. in loop().

#### `protected time_t `[`_today`](#class_s_p_i_f_f_s_logger_base_1a77a967ae68a680dea5a647200ba49815) 

current date, set in the last processing run

#### `protected unsigned long `[`_lastProcess`](#class_s_p_i_f_f_s_logger_base_1ac1b33a4c97b2a45b2b178a261ebbb12c) 

last processing millis()

#### `protected const uint16_t `[`_processInterval`](#class_s_p_i_f_f_s_logger_base_1a517fa4d3854283ba7b2dba9bf0d09c84) 

ms between processing runs

#### `protected const uint16_t `[`_daysToKeep`](#class_s_p_i_f_f_s_logger_base_1a8feb62f012879aca79d600cf217c2098) 

number of days to keep logs for

#### `protected bool `[`_processNow`](#class_s_p_i_f_f_s_logger_base_1a978f0a636ce4d7a7f1640df124cc25ff) 

force processing now, even if the processing interval hasn't passed

#### `protected char `[`_directory`](#class_s_p_i_f_f_s_logger_base_1a829da5d8bf3d83ed24e608bb60fc623c) 

base directory for log files

#### `protected char `[`_curPath`](#class_s_p_i_f_f_s_logger_base_1a7f79553c53c7b5b6c87aaaa104d5a1e3) 

path for today's file

#### `protected  `[`SPIFFSLoggerBase`](#class_s_p_i_f_f_s_logger_base_1ace02cadd2e608c940728ae9f5242bbcc)`(const char * directory,uint16_t daysToKeep,uint16_t processInterval)` 

#### `protected void `[`_pathFromDate`](#class_s_p_i_f_f_s_logger_base_1abfba26125f8b32b4c8e2e3d29d65b31f)`(char * output,time_t date)` 

Converts a time_t to that day's file path.

#### `protected void `[`_updateCurPath`](#class_s_p_i_f_f_s_logger_base_1aa5679e47e144e591b5fa90fb31b65cbd)`()` 

Updates the current path to match today's date.

#### `protected void `[`_runRotation`](#class_s_p_i_f_f_s_logger_base_1a2e9e45428e1ffbe989dd7679c5def4fe)`()` 

Deletes files older than the defined age limit.

# struct `SPIFFSLogData` 

Represents a data element as stored in SPIFFS, along with the creation timestamp.

#### Parameters
* `T` type to store, e.g. a struct.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public time_t `[`timestampUTC`](#struct_s_p_i_f_f_s_log_data_1a692b3adc90780d9388d6833d2170400c) | creation time in UTC
`public T `[`data`](#struct_s_p_i_f_f_s_log_data_1a071121efdc7f6d95adc4dcdf736a84ec) | data of type T

## Members

#### `public time_t `[`timestampUTC`](#struct_s_p_i_f_f_s_log_data_1a692b3adc90780d9388d6833d2170400c) 

creation time in UTC

#### `public T `[`data`](#struct_s_p_i_f_f_s_log_data_1a071121efdc7f6d95adc4dcdf736a84ec) 

data of type T

Generated by [Moxygen](https://sourcey.com/moxygen)