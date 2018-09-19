# SPIFFSLogger
A minimal library for binary data logging in ESP8266.

Makes it easy to read, write and keep logs of relevant data, such as sensor readings in an efficient way,
by storing data in raw, binary format along with a time_t. One file is created per UTC day to store the 
respective data, and deleted once it has reached the defined maximum age.

Depending on the data, this should offer about 50% space savings vs. saving the same data in CSV.

See the [example](examples/Basic/Basic.ino) and [documentation](docs/api.md).

## Installing
Clone the library into your libraries folder, or download the zip and extract it manually.

## License
Licensed under the GNU LGPLv3, see the [LICENSE](LICENSE) file.
