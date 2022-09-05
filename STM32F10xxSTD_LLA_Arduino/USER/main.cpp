///*
// * This program is a simple binary write/read benchmark.
// */
//#include "SdFat.h"
//#include "sdios.h"


//// SD_FAT_TYPE = 0 for SdFat/File as defined in SdFatConfig.h,
//// 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
//#define SD_FAT_TYPE 3
///*
//  Change the value of SD_CS_PIN if you are using SPI and
//  your hardware does not use the default value, SS.
//  Common values are:
//  Arduino Ethernet shield: pin 4
//  Sparkfun SD shield: pin 8
//  Adafruit SD shields and modules: pin 10
//*/
//// SDCARD_SS_PIN is defined for the built-in SD on some boards.
//#ifndef SDCARD_SS_PIN
//const uint8_t SD_CS_PIN = -1;
//#else  // SDCARD_SS_PIN
//// Assume built-in SD is used.
//const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
//#endif  // SDCARD_SS_PIN

//// Try max SPI clock for an SD. Reduce SPI_CLOCK if errors occur.
//#define SPI_CLOCK SD_SCK_MHZ(50)

//// Try to select the best SD card configuration.
//#if HAS_SDIO_CLASS
//#define SD_CONFIG SdioConfig(FIFO_SDIO)
//#elif ENABLE_DEDICATED_SPI
//#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)
//#else  // HAS_SDIO_CLASS
//#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)
//#endif  // HAS_SDIO_CLASS

//// Set PRE_ALLOCATE true to pre-allocate file clusters.
//const bool PRE_ALLOCATE = true;

//// Set SKIP_FIRST_LATENCY true if the first read/write to the SD can
//// be avoid by writing a file header or reading the first record.
//const bool SKIP_FIRST_LATENCY = true;

//// Size of read/write.
//const size_t BUF_SIZE = 512;

//// File size in MB where MB = 1,000,000 bytes.
//const uint32_t FILE_SIZE_MB = 5;

//// Write pass count.
//const uint8_t WRITE_COUNT = 2;

//// Read pass count.
//const uint8_t READ_COUNT = 2;
////==============================================================================
//// End of configuration constants.
////------------------------------------------------------------------------------
//// File size in bytes.
//const uint32_t FILE_SIZE = 1000000UL*FILE_SIZE_MB;

//// Insure 4-byte alignment.
//uint32_t buf32[(BUF_SIZE + 3)/4];
//uint8_t* buf = (uint8_t*)buf32;

//#if SD_FAT_TYPE == 0
//SdFat SD;
//File file;
//#elif SD_FAT_TYPE == 1
//SdFat32 SD;
//File32 file;
//#elif SD_FAT_TYPE == 2
//SdExFat SD;
//ExFile file;
//#elif SD_FAT_TYPE == 3
//SdFs SD;
//FsFile file;
//#else  // SD_FAT_TYPE
//#error Invalid SD_FAT_TYPE
//#endif  // SD_FAT_TYPE

//#define sd SD
//// Specifically for use with the Adafruit Feather, the pins are pre-set here!

//// include SPI, MP3 and SD libraries
//#include <SPI.h>
//#include <Adafruit_VS1053.h>

//#define VS1053_RESET PE6
//#define VS1053_CS PF7
//#define VS1053_DCS PF6
//#define VS1053_DREQ PC13
//#define VS1053_DCS PF6
//#define CARDCS -1

//Adafruit_VS1053_FilePlayer musicPlayer = 
//  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);
//void printDirectory(File dir, int numTabs);
//void setup() {
//  Serial.begin(115200);

//  // if you're using Bluefruit or LoRa/RFM Feather, disable the radio module
//  //pinMode(8, INPUT_PULLUP);

//  // Wait for serial port to be opened, remove this line for 'standalone' operation
//  while (!Serial) { delay(1); }
//  delay(500);
//  Serial.println("\n\nAdafruit VS1053 Feather Test");
//  
//  if (! musicPlayer.begin()) { // initialise the music player
//     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
//     while (1);
//  }

//  Serial.println(F("VS1053 found"));
// 
//  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
//  
//  if (!SD.begin(SdioConfig(FIFO_SDIO))) {
//    Serial.println(F("SD failed, or not present"));
//    while (1);  // don't do anything more
//  }
//  Serial.println("SD OK!");
//  
//  // list files
//  printDirectory(SD.open("/"), 0);
//  
//  // Set volume for left, right channels. lower numbers == louder volume!
//  musicPlayer.setVolume(10,10);
//  
//#if defined(__AVR_ATmega32U4__) 
//  // Timer interrupts are not suggested, better to use DREQ interrupt!
//  // but we don't have them on the 32u4 feather...
//  musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int
//#else
//  // If DREQ is on an interrupt pin we can do background
//  // audio playing
//  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
//#endif
//	musicPlayer.GPIO_pinMode(4,OUTPUT);
//  musicPlayer.GPIO_digitalWrite(4,0);
//  // Play a file in the background, REQUIRES interrupts!
//  Serial.println(F("Playing full track 001"));
//  musicPlayer.playFullFile("/1.mp3");

////  Serial.println(F("Playing track 002"));
////  musicPlayer.startPlayingFile("/track002.mp3");
//}

//void loop() {
//  Serial.print(".");
//  // File is playing in the background
//  if (musicPlayer.stopped()) {
//    Serial.println("Done playing music");
//    while (1) {
//      delay(10);  // we're done! do nothing...
//    }
//  }
//  if (Serial.available()) {
//    char c = Serial.read();
//    
//    // if we get an 's' on the serial console, stop!
//    if (c == 's') {
//      musicPlayer.stopPlaying();
//    }
//    
//    // if we get an 'p' on the serial console, pause/unpause!
//    if (c == 'p') {
//      if (! musicPlayer.paused()) {
//        Serial.println("Paused");
//        musicPlayer.pausePlaying(true);
//      } else { 
//        Serial.println("Resumed");
//        musicPlayer.pausePlaying(false);
//      }
//    }
//  }
//  delay(100);
//}



///// File listing helper
//void printDirectory(File dir, int numTabs) {
//   while(true) {
//     
//     File entry =  dir.openNextFile();
//     if (! entry) {
//       // no more files
//       //Serial.println("**nomorefiles**");
//       break;
//     }
//     for (uint8_t i=0; i<numTabs; i++) {
//       Serial.print('\t');
//     }


//     if (entry.isDirectory()) {
//       Serial.println("/");
//       printDirectory(entry, numTabs+1);
//     } else {
//       // files have sizes, directories do not
//       Serial.print("\t\t");
//       Serial.println(entry.size(), DEC);
//     }
//     entry.close();
//   }
//}
#include "Arduino.h"
#include "FreeRTOS.h"
#include "task.h"


#include <SPI.h>
#include <Ethernet.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <EthernetClient.h>
EthernetClient client;
#define AIO_SERVER      "Your mqtt server"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "Your mqtt username"
#define AIO_KEY         "Your mqtt password"


Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt,   "/feeds/photocell");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt,  "/feeds/onoff");

#define LED0 PB5
#define LED1 PE5

#define START_TASK_PRIO 1 //任务优先级
#define START_STK_SIZE 128 //任务堆栈大小
TaskHandle_t StartTask_Handler; //任务句柄
void start_task(void *pvParameters); //任务函数

#define LED0_TASK_PRIO 2 //任务优先级
#define LED0_STK_SIZE 512 //任务堆栈大小
TaskHandle_t LED0Task_Handler; //任务句柄
void led0_task(void *p_arg); //任务函数

#define LED1_TASK_PRIO 3 //任务优先级
#define LED1_STK_SIZE 512 //任务堆栈大小
TaskHandle_t LED1Task_Handler; //任务句柄
void led1_task(void *p_arg); //任务函数

#define UART_TASK_PRIO 4 //任务优先级
#define UART_STK_SIZE 512 //任务堆栈大小
TaskHandle_t uartTask_Handler; //任务句柄
void uart_task(void *p_arg); //任务函数

#define UART_TASK_PRIO 4 //任务优先级
#define UART_STK_SIZE 512 //任务堆栈大小
TaskHandle_t mqttTask_Handler; //任务句柄
void mqtt_task(void *p_arg); //任务函数

void start_task(void *pvParameters)
{
taskENTER_CRITICAL(); //进入临界区
//创建 LED0 任务
xTaskCreate((TaskFunction_t )led0_task,
(const char* )"led0_task",
(uint16_t )LED0_STK_SIZE,
(void* )NULL,
(UBaseType_t )LED0_TASK_PRIO,
(TaskHandle_t* )&LED0Task_Handler);
//创建 LED1 任务
xTaskCreate((TaskFunction_t )led1_task,
(const char* )"led1_task",
(uint16_t )LED1_STK_SIZE,
(void* )NULL,
(UBaseType_t )LED1_TASK_PRIO,
(TaskHandle_t* )&LED1Task_Handler);

//创建 串口 任务
xTaskCreate((TaskFunction_t )uart_task,
(const char* )"uart_task",
(uint16_t )UART_STK_SIZE,
(void* )NULL,
(UBaseType_t )UART_TASK_PRIO,
(TaskHandle_t* )&uartTask_Handler);


//创建 mqtt 任务
xTaskCreate((TaskFunction_t )mqtt_task,
(const char* )"mqtt_task",
(uint16_t )UART_STK_SIZE,
(void* )NULL,
(UBaseType_t )UART_TASK_PRIO,
(TaskHandle_t* )&mqttTask_Handler);

vTaskDelete(StartTask_Handler); //删除开始任务
taskEXIT_CRITICAL(); //退出临界区
}


//LED0 任务函数
void led0_task(void *pvParameters)
{
	while(1)
	{
		static uint8_t io_state = 0;
		digitalWrite(LED0,io_state);
		io_state=!io_state;
		Serial.printf("[%s] LED0 state:%d\r\n",pcTaskGetName(LED0Task_Handler),io_state);
		vTaskDelay(500);
	}
}
//LED1 任务函数
void led1_task(void *pvParameters)
{
	while(1)
	{
		static uint8_t io_state = 0;
		digitalWrite(LED1,io_state);
		io_state=!io_state;
		Serial.printf("[%s] LED1 state:%d\r\n",pcTaskGetName(LED1Task_Handler),io_state);
		vTaskDelay(200);
	}
}

void uart_task(void *pvParameters)
{
	while(1){
		if(Serial.available()){
			Serial.print("Hello,I'm got:");
			while(Serial.available()){
				Serial.write(Serial.read());
			}
			Serial.println();
		}
		vTaskDelay(200);
	}
}


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       vTaskDelay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}

uint32_t x=0;
void mqtt_task(void *p_arg){
	while(1){
// Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }

	}
}




// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0xD2, 0xA2, 0xBC, 0xD9, 0xD2, 0x09
};
void setup(){
	pinMode(LED0,OUTPUT);
	pinMode(LED1,OUTPUT);
	pinMode(PD7,OUTPUT);
	digitalWrite(PD7,LOW);
	delay(1000);
	digitalWrite(PD7,HIGH);
	delay(1000);
	Ethernet.init(PG9);
// start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
	mqtt.subscribe(&onoffbutton);
//创建开始任务
xTaskCreate((TaskFunction_t )start_task, //任务函数
(const char* )"start_task", //任务名称
(uint16_t )START_STK_SIZE, //任务堆栈大小
(void* )NULL, //传递给任务函数的参数
(UBaseType_t )START_TASK_PRIO, //任务优先级
(TaskHandle_t* )&StartTask_Handler); //任务句柄
vTaskStartScheduler();
}


void loop(){



}