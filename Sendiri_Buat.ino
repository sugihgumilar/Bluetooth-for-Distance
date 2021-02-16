#include <SoftwareSerial.h>
SoftwareSerial BTserial(3, 2); // RX | TX

char c = ' ';
String d;
int ceknilai=0;
String cekid= " ";
float pangkat;
float jarak;
int buzzer = 8;

void(* resetFunc) (void) = 0;

void delayAndRead()
{
  delay(200);
  while(BTserial.available())
  {
    c = BTserial.read();
  }
  delay(500);
}

void initHC05ToInq()
{
    BTserial.println("AT+ORGL");
    BTserial.println("AT+CMODE=1");// Enable connect to any device
    delayAndRead();
    BTserial.println("AT+ROLE=1");// Set to master in order to enable scanning
    delayAndRead();
    BTserial.println("AT+CLASS=0x00");// Disable COD filter
    delayAndRead();
    BTserial.println("AT+INQM=1,5,24");//RSSI, Max 5 devices, ~30s
    delayAndRead();
}

void setup() 
{
    Serial.begin(9600);
   
 
    // HC-05 default serial speed for AT mode is 38400
    BTserial.begin(38400);  

    // Wait for hardware to initialize
    //delay(1000);

    // Set correct states for inq
    initHC05ToInq();

    // Start inq
   BTserial.println("AT+INQ");
}

void loop(){   
  
  if (BTserial.available())
    {  
        // Read character and append into buffer
        c = BTserial.read();
        d = String(d + c);
        String frssi = getValue(d, ',', 2 );
        //find id
        String fid = getValue(d, ',', 0 );
        if ((frssi.length() == 4) && (fid.length() == 19)) {
          int dec = -(hexToDec(frssi));
//          Serial.println("C");
          if ((ceknilai != dec) && (cekid != fid)){
            cekid = fid;
            ceknilai = dec;
            Serial.println(fid);
            Serial.println(frssi);
            Serial.println(dec);
            pangkat = (-55-dec)*0.03;
            Serial.println(pangkat);
            jarak = pow(10,pangkat);
            Serial.print(jarak);
            Serial.println(" meter");
            if (jarak <= 2){
              tone(buzzer, 1000);
              delay(2000);
              noTone(buzzer);
              delay(2000); 
              resetFunc();
              
            }
            else if (jarak > 2) {
              resetFunc();
            }
            
          }
          
        }
        else if ((frssi.length() == 4) && (fid.length() == 23)) {
          int dec = -(hexToDec(frssi));
          if ((ceknilai != dec) && (cekid != fid)){
            cekid = fid;
            ceknilai = dec;
            Serial.println(fid);
            Serial.println(frssi);
            Serial.println(dec);
            pangkat = (-55-dec)*0.03;
            Serial.println(pangkat);
            jarak = pow(10,pangkat);
            Serial.print(jarak);
            Serial.println(" meter");
            if (jarak <= 2){
              tone(buzzer, 1000);
              delay(2000);
              noTone(buzzer);
              delay(2000);
              
 
              resetFunc();
              
            }
            else if (jarak > 2) {
              resetFunc();
            }
            
          }
          
        }
          
    }
}

//Separator NIlai INQ dengan comma (string)
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

//fungsi hex to bin 2complement
unsigned int hexToDec(String hexString) {
  
  unsigned int decValue = 0;
  int nextInt;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
    
  }
  //2Compelement
  decValue = ~decValue;
  decValue+=1;
  return decValue;
}
