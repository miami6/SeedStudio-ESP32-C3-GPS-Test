# SeedStudio-ESP32-C3-GPS-Test
SeedStudio-ESP32-C3-GPS-Test
This code appears to be for a microcontroller-based project that involves reading GPS data and saving it to an SD card. It also prints GPS data to the Serial monitor for debugging purposes. Let's break down the code and explain each function:

1. **Library Includes and Definitions**:
   - The code includes several libraries for working with WiFi, SD cards, SoftwareSerial (for communication with the GPS module), TinyGPS++, and TimeLib.
   - It defines constants such as `UTC_offset`, `SD_CS`, and `GPSBaud`.
   - `UTC_offset` is set to -14, which represents the Pacific Daylight Time (PDT) UTC offset.

2. **Global Variables**:
   - `chipSelect` is defined as `SD_CS`, which is set to 4.
   - `gpsSerial` is a SoftwareSerial object used for communication with the GPS module.
   - `TinyGPSPlus` object `gps` is used for parsing GPS data.
   - `logFileName` is set to "networks.txt," which seems to be used as a data file name.
   - `File` object `dataFile` is used for interacting with the SD card.

3. **`smartDelay` Function**:
   - This function is static and used to delay execution until a specified number of milliseconds have passed while continuously parsing GPS data from `gpsSerial`.

4. **`setup` Function**:
   - Initializes Serial communication at a baud rate of 115200.
   - Initializes `gpsSerial` for communication with the GPS module.
   - Attempts to initialize the SD card and opens a data file called "test2.txt" for writing.
   - Sets up the WiFi in station mode but disconnects from any previous connections.
   - Waits for a GPS fix, and once obtained, prints the latitude and longitude.

5. **`loop` Function**:
   - In an infinite loop, it prints various GPS-related data to the Serial monitor, including satellite count, HDOP, latitude, longitude, date, time, altitude, and more.
   - Calls the `smartDelay` function to control the loop's timing.
   - Includes a check for GPS data reception and issues a warning if no data has been received within a certain time.

6. **Helper Functions (`printFloat`, `printInt`, `printDateTime`)**:
   - These are static helper functions used to print formatted data to the Serial monitor. They are used in the `loop` function to display GPS data.

7. **Comments**:
   - There are comments throughout the code to explain what each section or function does.

This code primarily reads GPS data, prints it to the Serial monitor, and saves it to an SD card. The debugging functions are the `printFloat`, `printInt`, and `printDateTime` functions, which help format and display the GPS data, and the `Serial.println` statements within the `setup` and `loop` functions used for debugging and logging purposes.
