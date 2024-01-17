#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Crypto.h>
#include <AES.h>
#include <string.h>

byte registered[][16] = {
  {0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x32, 0x31, 0x42, 0x43, 0x59, 0x30, 0x30, 0x30, 0x39}, 
  {0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x32, 0x31, 0x42, 0x43, 0x59, 0x30, 0x30, 0x30, 0x32},
  {0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x32, 0x31, 0x42, 0x43, 0x59, 0x30, 0x30, 0x32, 0x35}
};

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define SS_PIN 10
#define RST_PIN 9
// Green LED for authorised access
// #define LED_G 5 
// Red LED for unauthorised access
// #define LED_R 4 

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
MFRC522::MIFARE_Key key; 


byte bufferLen = 18;
AES128 aes128;
byte aeskey[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};


// Custom struct that acts as a hash map
typedef struct { 
    char* uid;
    char* rollno;
  } Mapping;


void setup() {
  Serial.begin(9600);  
  SPI.begin();         // Initiate SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522
  lcd.begin();
  lcd.backlight();  // Turn on the LED blacklight
  aes128.setKey(aeskey,16);
}

void loop() {
  // encryption stuff
  byte readBlockData[18];
  // MFRC522::StatusCode status; 

  int blockNum = 2;  
  byte decryptedtext[16];

  
  MFRC522::StatusCode status; 
  /* All keys are set to FFFFFFFFFFFFh at chip delivery from the factory */
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  // define UID and roll number mapping, register students
  const Mapping students[]{
    {"00 A0 00 80", "CS01"},
    {"70 01 00 04", "CS02"}
  };

  
  // Scan for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    lcd.setCursor(1, 0);
    lcd.print("READY TO SCAN");
    return;
  } else {
    lcd.clear();
  }
  
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();
  
  Serial.println("Reading from Data Block...");
  ReadDataFromBlock(blockNum, readBlockData);
  
  /* Print the data read from block */
  Serial.print("Data in Block ");
  Serial.print(blockNum);
  Serial.print(" : ");
  for (int j=0 ; j<16 ; j++)
  {
    Serial.write(readBlockData[j]);
  }

  aes128.decryptBlock(decryptedtext,readBlockData);
  Serial.println();
  Serial.print("After Decryption: ");
  for(int i=0; i<sizeof(decryptedtext); i++){
    Serial.write(decryptedtext[i]);
  }
  Serial.print("\n");
    if (verify(decryptedtext)){
      Serial.print("Verified");
    }
    else{
      Serial.print("Unauthorised access");
      lcd.print("UNAUTHORISED");
      lcd.setCursor(0, 1);
      lcd.print("ACCESS");
      // digitalWrite(LED_R, HIGH);
      delay(2000);
      // digitalWrite(LED_R, LOW);
      lcd.clear();
    }
  

  // Loop through registred roll numbers 
  for(uint8_t i = 0; i < sizeof(students)/sizeof(Mapping); ++i){
    if (content.substring(1) == students[i].uid){
      lcd.print(students[i].rollno);
      // flag = 1;
      lcd.setCursor(0, 1);
      lcd.print("AUTHORISED");
      // digitalWrite(LED_G, HIGH);
      delay(2000);
      // digitalWrite(LED_G, LOW);
      lcd.clear();
    }
  }
  
  Serial.println("\n");
}

/* mechanism to verify student registration */
bool verify(byte *decryptedText){
  for (uint8_t i = 0; i < sizeof(registered)/(sizeof(byte)*16); i++){
    bool flag = true;
    for (uint8_t j = 0; j < 16; j++){
      if (decryptedText[j] != registered[i][j]){
        flag = false;
        break;
      }
    }
    if (flag){
      return true;
    }
  }
  return false;  
}

void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{
  /* Authenticating the desired data block for Read access using Key A */
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");  
  }
  
}
