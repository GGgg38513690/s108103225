#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>  // Arduino IDE 內建
                     
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
					 
#define DEBUG true

// Wi-Fi接入點的SSID和密碼
const  char *  ssid  =  "AIOT";
const  char *  password  =  "0277388000";

const char* host = "120.96.57.62";
const int httpPort = 80;
String HTML_respone;

String sendData(String command, const int timeout, boolean debug){
    String response = "";
    Serial1.print(command); // send the read character to the esp8266
    long int time = millis();
    while( (time+timeout) > millis()){
      while(Serial1.available()){
        // The esp has data so display its output to the serial window 
        char c = Serial1.read(); // read the next character.
        response+=c;
      }  
    }
    if(DEBUG) {
      Serial.print(response);
    }
    return response;
}

void setup(){  
 
  Serial1.begin(115200);//RX1 TX1，即 Pin18 及 Pin19 
  Serial.begin(115200);
  while (!Serial) ;       // 等待序列埠開啟完成
  
  sendData("AT+RST\r\n",2000,DEBUG);
  sendData("AT+CWMODE=3\r\n",200,DEBUG);
  String commend = "AT+CWJAP=\"";
  commend += ssid;
  commend += "\",\"";
  commend += password;
  commend += "\"\r\n";
  if (sendData(commend,5000,DEBUG)){  
    //Serial.print("Join AP success\r\n");
    //Serial.print("IP:");
    sendData("AT+CIFSR\r\n",5000,DEBUG);
    sendData("AT+CIPSERVER=0\r\n",200,DEBUG); 
    sendData("AT+CIPMUX=0\r\n",200,DEBUG);
    sendData("AT+CIFSR\r\n",5000,DEBUG);  

    lcd.begin(16,2);                      // initialize the lcd
    lcd.backlight();
    lcd.print("Hello, world!");
  }
}
void loop(){   
  String commend = "AT+CIPSTART=\"TCP\",\"";
  commend += host;
  commend += "\",";
  commend += httpPort; 
  commend += "\r\n";
  sendData(commend,200,DEBUG);
  
  String GET = "GET /123456789.php?username=施勢帆&passwd=12345";
  GET += " HTTP/1.1\r\nHost:";
  GET += host;
  GET += "\r\n\r\n";
  Serial.println(GET);
  String cipSend = "AT+CIPSEND=";
  cipSend += GET.length();
  cipSend += "\r\n";
  sendData(cipSend,300,DEBUG);
  HTML_respone=sendData(GET,400,DEBUG);
  //sendData("AT+CIPCLOSE\r\n",500,DEBUG);
  //HTML_respone=HTML_respone.substring(274,293);
  Serial.println(HTML_respone);
  Serial.println("");
  
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NO : 123456789");
  lcd.setCursor(0, 1);
  //if (HTML_respone.length()!=0) {             
  //  HTML_respone=HTML_respone.substring(5,25);
  //  lcd.print(HTML_respone);
  //}
  //delay(1000);   
} 
