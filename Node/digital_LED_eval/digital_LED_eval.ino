#include <WiFi.h>
#include <WiFiUdp.h>

#include "esp32_digital_led_lib.h"



//LED setup
#define LED_COUNT 64
#define LED_PIN 22
#define LED_BRIGHTNESS 255

#if defined(ARDUINO) && ARDUINO >= 100
  // No extras
#elif defined(ARDUINO) // pre-1.0
  // No extras
#elif defined(ESP_PLATFORM)
  #include "arduinoish.hpp"
#endif

//Network Setup
const char * networkName = "";
const char * networkPswd = "";
const int ipPort = 1234;    
boolean connected = false;
WiFiUDP udp;
const int udpRecvBufferSize = LED_COUNT*3;
static uint8_t udpData[udpRecvBufferSize];


// **Required** if debugging is enabled in library header
// TODO: Is there any way to put this in digitalLeds_initStrands() and avoid undefined refs?
#if DEBUG_ESP32_DIGITAL_LED_LIB
  int digitalLeds_debugBufferSz = 1024;
  char * digitalLeds_debugBuffer = static_cast<char*>(calloc(digitalLeds_debugBufferSz, sizeof(char)));
#endif

strand_t strip =   {.rmtChannel = 1, 
                    .gpioNum = LED_PIN, 
                    .ledType = LED_WS2812B_V3, 
                    .brightLimit = LED_BRIGHTNESS, 
                    .numPixels =  LED_COUNT,
                    .pixels = nullptr, 
                    ._stateVars = nullptr};

void startUpSeq(){
  int currentLED = LED_COUNT - 1;


  while(currentLED > 0){
    
  }
  
}
   
// Forward declarations
void dumpDebugBuffer(int, char *);

//**************************************************************************//
int getMaxMalloc(int min_mem, int max_mem) {
  int prev_size = min_mem;
  int curr_size = min_mem;
  int max_free = 0;
//  Serial.print("checkmem: testing alloc from ");
//  Serial.print(min_mem);
//  Serial.print(" : ");
//  Serial.print(max_mem);
//  Serial.println(" bytes");
  while (1) {
    void * foo1 = malloc(curr_size);
//    Serial.print("checkmem: attempt alloc of ");
//    Serial.print(curr_size);
//    Serial.print(" bytes --> pointer 0x");
//    Serial.println((uintptr_t)foo1, HEX);
    if (foo1 == nullptr) {  // Back off
      max_mem = min(curr_size, max_mem);
//      Serial.print("checkmem: backoff 2 prev = ");
//      Serial.print(prev_size);
//      Serial.print(", curr = ");
//      Serial.print(curr_size);
//      Serial.print(", max_mem = ");
//      Serial.print(max_mem);
//      Serial.println();
      curr_size = (int)(curr_size - (curr_size - prev_size) / 2.0);
//      Serial.print("checkmem: backoff 2 prev = ");
//      Serial.print(prev_size);
//      Serial.print(", curr = ");
//      Serial.print(curr_size);
//      Serial.println();
    }
    else {  // Advance
      free(foo1);
      max_free = curr_size;
      prev_size = curr_size;
      curr_size = min(curr_size * 2, max_mem);
//      Serial.print("checkmem: advance 2 prev = ");
//      Serial.print(prev_size);
//      Serial.print(", curr = ");
//      Serial.print(curr_size);
//      Serial.println();
    }
    if (abs(curr_size - prev_size) == 0) {
      break;
    }
  }
  Serial.print("checkmem: max free heap = ");
  Serial.print(esp_get_free_heap_size());
  Serial.print(" bytes, max allocable = ");
  Serial.print(max_free);
  Serial.println(" bytes");
  return max_free;
}

void dumpSysInfo() {
  esp_chip_info_t sysinfo;
  esp_chip_info(&sysinfo);
  Serial.print("Model: ");
  Serial.print((int)sysinfo.model);
  Serial.print("; Features: 0x");
  Serial.print((int)sysinfo.features, HEX);
  Serial.print("; Cores: ");
  Serial.print((int)sysinfo.cores);
  Serial.print("; Revision: r");
  Serial.println((int)sysinfo.revision);
}

void dumpDebugBuffer(int id, char * debugBuffer)
{
  Serial.print("DEBUG: (");
  Serial.print(id);
  Serial.print(") ");
  Serial.println(debugBuffer);
  debugBuffer[0] = 0;
}

int simpleFiller(strand_t * pStrand,pixelColor_t color){
  Serial.println("Simple Filler");
  for(int i=0;i< pStrand->numPixels;i++){
      pStrand->pixels[i] = color;
      digitalLeds_updatePixels(pStrand);
  }
  
  
  delay(1100);
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),ipPort);          
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
      default:
          Serial.print("WiFi Event ");  
          Serial.println((int)event);
    }
}

const static uint8_t gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


int reqData = 1;
const char * udpAddress = "192.168.0.100";
const int udpPort = 2600;

void setup()
{
  //udpData = (uint8_t*)malloc(udpRecvBufferSize);
  Serial.begin(115200);
  Serial.println("Initializing...");
  dumpSysInfo();
  getMaxMalloc(1*1024, 16*1024*1024);

  if (digitalLeds_initStrands(&strip, 1)) {
    Serial.println("Init FAILURE: halting");
    while (true) {};
  }
  Serial.println("Init complete");

  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);

}

int newData = 0, reqDelayCount=0;

void loop()
{

  pixelColor_t tmpColor,randColor;
  tmpColor.w = 0;
  randColor.w = 0;
  
  strand_t * stripP = &strip;
  //startUpSeq();
    //int m1 = getMaxMalloc(1*1024, 16*1024*1024);
    
  if(connected){
    if(reqData){//Request data
      //Send request
      //Serial.println("Requesting data");
      udp.beginPacket(udpAddress,ipPort);
      udp.printf("a");
      udp.endPacket();
      reqData = 0;
      reqDelayCount = 0;
    }else{//Recv data
      
      if(udp.parsePacket() > 0)
      {
        reqDelayCount =0 ;
        //Serial.println("Data available");
        udp.read(udpData,udpRecvBufferSize);
        
        int dataIdx = 0;
        for(int i=0; i < strip.numPixels; i++) 
        {
          tmpColor.r = udpData[dataIdx++];
          tmpColor.g = udpData[dataIdx++];
          tmpColor.b = udpData[dataIdx++];
          strip.pixels[i] = tmpColor;
        }
        newData = 1;
        
      }else{
        if(newData){
          //Serial.println("Updating LED");
          digitalLeds_updatePixels(stripP);
          newData = 0;  
          reqData = 1;
        }
      }
      

      
    }
    if(reqDelayCount > 600){//Send new request
      reqData = 1;
    }else
      reqDelayCount++;
  }
 
  //int m2 = getMaxMalloc(1*1024, 16*1024*1024);
  //assert(m2 >= m1); // Sanity check
 // delay(1);
  #if DEBUG_ESP32_DIGITAL_LED_LIB
    dumpDebugBuffer(0, digitalLeds_debugBuffer);
  #endif
}
