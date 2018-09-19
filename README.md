# SPIFFSLogger
A minimal library for binary data logging in ESP8266 systems.

Makes it easy to read, write and keep logs of relevant data, such as sensor readings in an efficient way,
by storing data in raw, binary format along with a time_t. Depending on the data, this should offer about
50% space savings vs. saving the same data in CSV. Daily log file rotation is performed automatically.

See the [example](examples/Basic/Basic.ino) and documentation below.

## Installing
Clone the library into your libraries folder, or download the zip and extract it manually.

## Documentation

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`SPIFFSLogger`](#class_s_p_i_f_f_s_logger) | Minimal class template for binary data logging in ESP8266 SPIFFS.
`struct `[`SPIFFSLogData`](#struct_s_p_i_f_f_s_log_data) | Represents a data element as stored in SPIFFS, along with the creation timestamp.

## class `SPIFFSLogger<T>` 

Minimal class template for binary data logging in ESP8266 SPIFFS.

Makes it easy to read, write and keep logs of relevant data, such as sensor readings in an efficient way, by storing data in raw, binary format along with a time_t. One file is created per UTC day to store the respective data, and deleted once it has reached the defined maximum age.

#### Parameters
* `T` type to store, e.g. a struct.

### Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`SPIFFSLogger`](#class_s_p_i_f_f_s_logger_1afa8152a5d5d29fedbb04265cb4589359)`(const char * directory,uint16_t daysToKeep,uint16_t processInterval)` | Default constructor for [SPIFFSLogger](#class_s_p_i_f_f_s_logger).
`public void `[`init`](#class_s_p_i_f_f_s_logger_1abfc7ce5e1a8022705c6704e50edf087d)`()` | Initialize the logger.
`public void `[`process`](#class_s_p_i_f_f_s_logger_1a34f24a3c6229653a31c166b899d43537)`()` | Process the file rotation and other required operations according to the defined processInterval.
`public size_t `[`write`](#class_s_p_i_f_f_s_logger_1a2e914dd1884de9900288564a41bc55a4)`(const T & value)` | Write the specified value to the end of the current log file, with the current timestamp.
`public size_t `[`readRows`](#class_s_p_i_f_f_s_logger_1aeceb2db3a41feb387e874f247657e19c)`(`[`SPIFFSLogData`](#struct_s_p_i_f_f_s_log_data)`< T > * output,time_t date,size_t startIdx,size_t maxCount)` | Read data from a daily logfile into a buffer.
`public size_t `[`rowCount`](#class_s_p_i_f_f_s_logger_1ac539407d1bfebe9443393ea2e4047e29)`(time_t date)` | Get the number of entries for the specified date.
`protected time_t `[`_today`](#class_s_p_i_f_f_s_logger_1a0a83d73217bc8bada843092ae7d055e8) | current date, set in the last processing run
`protected unsigned long `[`_lastProcess`](#class_s_p_i_f_f_s_logger_1ad48c3983258885410e6b7d0a299e8e69) | last processing millis()
`protected const uint16_t `[`_processInterval`](#class_s_p_i_f_f_s_logger_1a42017253e85b5f4978e4ba778960a62d) | ms between processing runs
`protected const uint16_t `[`_daysToKeep`](#class_s_p_i_f_f_s_logger_1afa47e4215bf7dc8a27704e5c10324fa8) | number of days to keep logs for
`protected bool `[`_processNow`](#class_s_p_i_f_f_s_logger_1af6227914a7bd060a26b5f83fa5641665) | force processing now, even if the processing interval hasn't passed
`protected char `[`_directory`](#class_s_p_i_f_f_s_logger_1a294e4b04bb7d26c6d63d4168d574221a) | base directory for log files
`protected char `[`_curPath`](#class_s_p_i_f_f_s_logger_1af6c68f5bf3ceccde605a89cb929bb787) | path for today's file
`protected void `[`_pathFromDate`](#class_s_p_i_f_f_s_logger_1ad366d57e6ca7b721b27164e40fb342b3)`(char * output,time_t date)` | Converts a time_t to that day's file path.
`protected void `[`_updateCurPath`](#class_s_p_i_f_f_s_logger_1ae6edbf5e924957c107068fce73554737)`()` | Updates the current path to match today's date.
`protected void `[`_runRotation`](#class_s_p_i_f_f_s_logger_1afbb2b0583a0500e3e5eae006a6740992)`()` | Deletes files older than the defined age limit.

### Members

#### `public  `[`SPIFFSLogger`](#class_s_p_i_f_f_s_logger_1afa8152a5d5d29fedbb04265cb4589359)`(const char * directory,uint16_t daysToKeep,uint16_t processInterval)` 

Default constructor for [SPIFFSLogger](#class_s_p_i_f_f_s_logger).

#### Parameters
* `directory` char array with the base directory where files will be stored. Should not include trailing slash and should be 20 characters or less. 

* `daysToKeep` number of days to keep in flash. Once files are past this age, they are deleted. 

* `processInterval` milliseconds between file directory updates and file rotation.

#### `public void `[`init`](#class_s_p_i_f_f_s_logger_1abfc7ce5e1a8022705c6704e50edf087d)`()` 

Initialize the logger.

Should be called after initializing SPIFFS and before before any other method.

#### `public void `[`process`](#class_s_p_i_f_f_s_logger_1a34f24a3c6229653a31c166b899d43537)`()` 

Process the file rotation and other required operations according to the defined processInterval.

Should be called as often as possible, i.e. in loop().

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

#### `public size_t `[`rowCount`](#class_s_p_i_f_f_s_logger_1ac539407d1bfebe9443393ea2e4047e29)`(time_t date)` 

Get the number of entries for the specified date.

#### Parameters
* `date` a time_t representing the day to check 

#### Returns
number of entries

#### `protected time_t `[`_today`](#class_s_p_i_f_f_s_logger_1a0a83d73217bc8bada843092ae7d055e8) 

current date, set in the last processing run

#### `protected unsigned long `[`_lastProcess`](#class_s_p_i_f_f_s_logger_1ad48c3983258885410e6b7d0a299e8e69) 

last processing millis()

#### `protected const uint16_t `[`_processInterval`](#class_s_p_i_f_f_s_logger_1a42017253e85b5f4978e4ba778960a62d) 

ms between processing runs

#### `protected const uint16_t `[`_daysToKeep`](#class_s_p_i_f_f_s_logger_1afa47e4215bf7dc8a27704e5c10324fa8) 

number of days to keep logs for

#### `protected bool `[`_processNow`](#class_s_p_i_f_f_s_logger_1af6227914a7bd060a26b5f83fa5641665) 

force processing now, even if the processing interval hasn't passed

#### `protected char `[`_directory`](#class_s_p_i_f_f_s_logger_1a294e4b04bb7d26c6d63d4168d574221a) 

base directory for log files

#### `protected char `[`_curPath`](#class_s_p_i_f_f_s_logger_1af6c68f5bf3ceccde605a89cb929bb787) 

path for today's file

#### `protected void `[`_pathFromDate`](#class_s_p_i_f_f_s_logger_1ad366d57e6ca7b721b27164e40fb342b3)`(char * output,time_t date)` 

Converts a time_t to that day's file path.

#### `protected void `[`_updateCurPath`](#class_s_p_i_f_f_s_logger_1ae6edbf5e924957c107068fce73554737)`()` 

Updates the current path to match today's date.

#### `protected void `[`_runRotation`](#class_s_p_i_f_f_s_logger_1afbb2b0583a0500e3e5eae006a6740992)`()` 

Deletes files older than the defined age limit.

## struct `SPIFFSLogData<T>` 

Represents a data element as stored in SPIFFS, along with the creation timestamp.

#### Parameters
* `T` type to store, e.g. a struct.

### Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public time_t `[`timestampUTC`](#struct_s_p_i_f_f_s_log_data_1a692b3adc90780d9388d6833d2170400c) | creation time in UTC
`public T `[`data`](#struct_s_p_i_f_f_s_log_data_1a071121efdc7f6d95adc4dcdf736a84ec) | data of type T

### Members

#### `public time_t `[`timestampUTC`](#struct_s_p_i_f_f_s_log_data_1a692b3adc90780d9388d6833d2170400c) 

creation time in UTC

#### `public T `[`data`](#struct_s_p_i_f_f_s_log_data_1a071121efdc7f6d95adc4dcdf736a84ec) 

data of type T

## License
Licensed under the GNU LGPLv3, see the [LICENSE](LICENSE) file.
