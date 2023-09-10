 #include <WiFi.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TimeLib.h>

#define UTC_offset -14  // PDT
#define SD_CS 4
#define GPSBaud 9600  // GPS module baud rate

const int chipSelect = SD_CS;

SoftwareSerial gpsSerial(D7, D6); // RX, TX for GPS module
TinyGPSPlus gps;

const char *logFileName = "networks.txt";
File dataFile;

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(GPSBaud);

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
  }
  else {
    Serial.println("SD card initialized successfully!");

    // Create and open the data file for writing
    dataFile = SD.open("test2.txt", FILE_WRITE);
    if (dataFile) {
      Serial.println("Data file (test2.txt) opened successfully!");
      // Write a header to the data file
      dataFile.println("BSSID,SSID,Signal Strength (dBm)");
    }
    
    
  }

  // Initialize Wi-Fi in station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Wait for GPS fix
  Serial.println("Waiting for GPS fix...");
  while (!gps.location.isValid()) {
    while (gpsSerial.available()) {
      gps.encode(gpsSerial.read());
    }
  }

  Serial.print("GPS Fix obtained. Location: ");
  Serial.print(gps.location.lat(), 6);
  Serial.print(", ");
  Serial.println(gps.location.lng(), 6);
   
}

void loop() {
   

  Serial.print("Sats HDOP  Latitude   Longitude   Fix  Date    Time   Alt  \n");
   
  Serial.print("------------------------------------------------------------\n");

  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 1,1 );
  printInt(gps.charsProcessed(), true, 1);
  printInt(gps.sentencesWithFix(), true, 1);
  //printInt(gps.failedChecksum(), true, 9);
  Serial.println();

  smartDelay(900);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  
  smartDelay(0);
}
