#include <CytronEZMP3.h>
#include <CytronWiFiShield.h>
#include <CytronWiFiServer.h>
#include <SoftwareSerial.h>
#define WiFi wifi

ESP8266Server server(80);       //port number

#include <Adafruit_NeoPixel.h>   //RainbowStrip
#ifdef __AVR__
#include <avr/power.h>
#endif
                                 // on a live circuit...if you must, connect GND first.
#define PIN 6                   //Output pin connected to RainbowStrip and EZMP3 Shield

const char ssid[] = "CytronESPShield";      //  your network SSID (name)
const char pass[] = "12345678";             //your network password
int keyIndex = 0;                      

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

CytronEZMP3 mp3;

void setup () {
  Serial.begin(9600);
  Serial.begin(115200);     
  pinMode(13, OUTPUT);      
 
  if(!mp3.begin(Serial2))                 //I was using port 8 and 9 in EZMP3 Shield
  {                                       //If you are using port 10 and 11, write "Serial5"                                                 
  {                                       //This is because I was using CT ARM board instead of arduino.
    Serial.println("Init failed");
    while(1);
  }                                       //// check for the presence of the shield:
  if (!WiFi.begin(2, 3)) {                 //ports for ESP8266 Wifi Shield
    Serial.println("WiFi shield not present");
    while (true);      
    }

  String fv = WiFi.firmwareVersion();
  Serial.println(fv);

  WiFi.setMode(WIFI_AP);
  if(!WiFi.softAP(ssid, pass))
 
  Serial.println("Setting softAP failed");
  Serial.println(WiFi.softAPIP());
  server.begin();       
  mp3.setVolume(22);
  Serial.print("Device: ");
  Serial.println(mp3.getCurrentDevice()==1?"U-Disk":"microSD");

  Serial.print("Total files: ");
  Serial.println(mp3.getTotalFiles());
  
  strip.begin();
  strip.show();             // Initialize all pixels to 'off'
}
}
void loop () {   
  ESP8266Client client = server.available();   // listen for incoming clients

   if(!client) return;
  
  if (client.connected())         //if client is present and connected
  {           
  char c = client.read();
  {
  if (c=='F')                    //Touch Front/Up button on Wifly for song to start playing
  {
    c==8;
    c==0;
    
  mp3.play();
  Serial.println("Now playing track " + (String)mp3.getTrackNo());
  delay (10);
  Serial.println("play");
  
  // Some example procedures showing how to display to the pixels:
  theaterChase(strip.Color(127, 127, 127), 1000); // White    //You can change the color by changing the rgb codes 
  theaterChase(strip.Color(162,0,37), 1000); // Magenta       //You also can change the delay of every styles 
  theaterChase(strip.Color(81, 46, 95), 1000); // Purple
  
  colorWipe(strip.Color(100, 30, 22), 60); // Salmon      // Fill the dots one after the other with a color

  theaterChaseRainbow(18);                                //Theatre-style crawling lights with rainbow effect

  rainbow(110);
   
  theaterChase(strip.Color(52, 152, 219), 1000); // Blue   //Theatre-style crawling lights.
  theaterChase(strip.Color(186, 74, 0), 1000); // Orange
  theaterChase(strip.Color(108, 52, 131), 1000); // Purple
  theaterChase(strip.Color(0, 255, 0), 2500); // Lime
  theaterChase(strip.Color(127, 127, 127), 1000); // White
  theaterChase(strip.Color(100, 30, 22), 1000); // Salmon
  rainbowCycle(5);
  
  colorWipe(strip.Color(0, 255, 255), 85); // Cyan
  
  theaterChaseRainbow(38);  //Theatre-style crawling lights with rainbow effect
  rainbow(10);
  rainbowCycle(10);
  rainbow(10);
  rainbowCycle(10);
  colorWipe(strip.Color(162,0,37), 200); //Magenta
  theaterChaseRainbow(30);
  rainbowCycle(20);
  colorWipe(strip.Color(127, 127, 127), 800); // White
  rainbowCycle(10);
  theaterChase(strip.Color(108, 52, 131), 2000); // Purple
  rainbow(100);
  }
  Serial.write(client.read()); 
  client.println();
}
}
}
void colorWipe(uint32_t c, int wait)    // Fill the dots one after the other with a color
{
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void rainbow(int wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
void rainbowCycle(uint8_t wait) {        // Slightly different, this makes the rainbow equally distributed throughout
  uint16_t i, j;

  for(j=0; j<256*3; j++) { // 3 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<5; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;  
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
