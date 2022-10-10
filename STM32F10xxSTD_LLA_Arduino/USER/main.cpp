//#include <Arduino.h>
//#include <WebSocketClient.h>
//#include <Ethernet.h>

//EthernetClient client;

//WebSocketClient webSocketClient;
//byte mac[] = {
//  0xD2, 0xA2, 0xBC, 0xD9, 0xD2, 0x09
//};

//void setup(){

//	lLA_WDG_Start(20000);
//	pinMode(PD7,OUTPUT);
//	digitalWrite(PD7,LOW);
//	delay(1000);
//	digitalWrite(PD7,HIGH);
//	delay(1000);
//	Ethernet.init(PG9);
//	
//// start the Ethernet connection:
//  Serial.println("Initialize Ethernet with DHCP:");
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
//    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
//      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
//    } else if (Ethernet.linkStatus() == LinkOFF) {
//      Serial.println("Ethernet cable is not connected.");
//    }
//    // no point in carrying on, so do nothing forevermore:
//    while (true) {
//      delay(1);
//    }
//  }
//  // print your local IP address:
//  Serial.print("My IP address: ");
//  Serial.println(Ethernet.localIP());
//	
//  if (client.connect(IPAddress(192,168,2,59),8889)) {
//    Serial.println("Connected");
//  } else {
//    Serial.println("Connection failed.");
//    while(1) {
//      // Hang on failure
//    }
//  }
// // Handshake with the server
//  webSocketClient.path = "/ysmanage/ws/environment";

//  if (webSocketClient.handshake(client)) {
//    Serial.println("Handshake successful");
//  } else {
//    Serial.println("Handshake failed.");
//    while(1) {
//      // Hang on failure
//    }
//  }
//}


//void loop(){
// String data;

//  if (client.connected()) {

//    webSocketClient.getData(data);

//    if (data.length() > 0) {
//      Serial.print("Received data: ");
//      Serial.println(data);
//    }
//		
////    webSocketClient.sendData(String("Hello I'm client.") + millis());

//  } else {

//    Serial.println("Client disconnected.");
//    while (1) {
//      // Hang on disconnect.
//    }
//  }

//  // wait to fully let the client disconnect
//  delay(3000);
//	LLA_WDG_Feed();
//}

///***
//    eeprom_put example.

//    This shows how to use the EEPROM.put() method.
//    Also, this sketch will pre-set the EEPROM data for the
//    example sketch eeprom_get.

//    Note, unlike the single byte version EEPROM.write(),
//    the put method will use update semantics. As in a byte
//    will only be written to the EEPROM if the data is actually
//    different.

//    Written by Christopher Andrews 2015
//    Released under MIT licence.
//***/

//#include <EEPROM.h>

//struct MyObject {
//  float field1;
//  byte field2;
//  char name[10];
//};

//void setup() {
//	
//	EEPROM.begin();
//	
//  Serial.begin(115200);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

//  float f = 123.456f;  //Variable to store in EEPROM.
//  int eeAddress = 0;   //Location we want the data to be put.


//  //One simple call, with the address first and the object second.
//  EEPROM.put(eeAddress, f);

//  Serial.println("Written float data type!");

//  /** Put is designed for use with custom structures also. **/

//  //Data to store.
//  MyObject customVar = {
//    3.14f,
//    65,
//    "Working!"
//  };

//  eeAddress += sizeof(float); //Move address to the next byte after float 'f'.

//  EEPROM.put(eeAddress, customVar);
//  Serial.print("Written custom data type! \n\nView the example sketch eeprom_get to see how you can retrieve the values!");
//	EEPROM.end();
//	
//	
//	EEPROM.begin();
//	eeAddress = 0;
//	float _f;
//	MyObject _customVar;
//	EEPROM.get(eeAddress,_f);
//	EEPROM.get(eeAddress+sizeof(float),_customVar);
//	Serial.printf("_f:%f customVar1:%f customVar2:%d customVar3:%s\r\n",_f,_customVar.field1,_customVar.field2,_customVar.name);
//	EEPROM.end();
//}

//void loop() {
//  /* Empty loop */
//}

#include <Arduino.h>
#include "w25q128_user.h"
#include "lfs.h"
#include "lfs_dev.h"
// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

static uint8_t buf[4096];
void setup(){
	Serial.begin(115200);
	w25q128_user_init();
    // mount the filesystem
  int err = lfs_mount(&lfs, &dev_stm32f10x_hd_cfg);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        lfs_format(&lfs, &dev_stm32f10x_hd_cfg);
        lfs_mount(&lfs, &dev_stm32f10x_hd_cfg);
    }
		
		lfs_file_open(&lfs,&file,"test.txt", LFS_O_RDWR | LFS_O_CREAT);
		char buf_r[256];
		int len = lfs_file_read(&lfs,&file,buf_r,sizeof(buf_r));
		if(len){
			Serial.println("test.txt ÄÚÈÝÎª:");
			Serial.write(buf_r,len);
			Serial.println();
		}
		
		char buf[]="Hello,World!";
		lfs_file_write(&lfs,&file,buf,sizeof(buf));
		lfs_file_close(&lfs,&file);
}


void loop(){
}
