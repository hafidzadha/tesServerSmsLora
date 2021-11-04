#include <SoftwareSerial.h>
SoftwareSerial SIM900A(3, 2);
#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95;
int led = 4;
unsigned long int millisBefore;
String latitude, longitude;

//int turn = 1;
void setup() {
  pinMode(led, OUTPUT);
//  digitalWrite(led, LOW);
//  delay(10);
  digitalWrite(led, HIGH);
//  delay(10);
  SIM900A.begin(9600);
  Serial.begin(9600);
  delay(100);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");
  rf95.setFrequency(915.0);
  delay(100);
//  SIM900A.println("AT+CMGD=ALL");
//  SIM900A.println("AT+CNMI=2,2,0,0");
}

void loop() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
//    if (rf95.waitAvailableTimeout(500)) {
        if (rf95.recv(buf, &len)) {
//            digitalWrite(led, HIGH);
            delay(10);
            Serial.print("Received at Server: ");
            Serial.println((char*)buf);
            String dataTotal = (char*)buf;
//          Serial.println(dataTotal);
            latitude = getValue(dataTotal,' ',0);
            longitude = getValue(dataTotal,' ',2);
//            digitalWrite(led, LOW);
            delay(10);
            SIM900A.println("AT+CNMI=2,2,0,0");
//            Serial.println(latitude);
//            Serial.println(longitude);
        }
        else
        {
            Serial.println("recv failed");
        } 
//    }
  }
  String c = SIM900A.readString();
//  c.trim();
  if (c.indexOf("req") > -1) {
    SendMessage(latitude,longitude);
  }
//  simcommand(latitude, longitude);
}


void SendMessage(String lati, String longi) {
//  SIM900A.listen();
//    if (SIM900A.available() > 0) {
//      String c = SIM900A.readString();
//      c.trim();
//      if (c.indexOf("req") > -1) { // >= 0
//        SIM900A.print("\r");
//        delay(1000);
        SIM900A.println("AT+CMGF=1\r");
        delay(1000);
        SIM900A.println("AT+CMGS=\"+62XXXXXXXXXX\"\r");
        delay(1000);
        //The text of the message to be sent.
//        SIM900A.print("Latitude :");
//        SIM900A.println(lati);
//        SIM900A.print("Longitude:");
//        SIM900A.println(longi);
//        Serial.println(lati);
//        Serial.println(longi);
        SIM900A.println("https://www.google.com/maps/?q=" + lati + "," + longi);
//        SIM900A.print(lati);
//        SIM900A.print(",");
//        SIM900A.print(longi);
        delay(100);
        SIM900A.println((char)26);// ASCII code of CTRL+Z
        delay(1000);
//        delay(1000);
//        SIM900A.write(0x1A);
//        delay(1000);
      }
//    }
//}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
