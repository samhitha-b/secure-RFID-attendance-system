#include <Crypto.h>
#include <AES.h>
#include <string.h>

//key[16] contains a 16-byte key (128 bits) for encryption
byte key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
//plaintext[16] contains the text we need to encrypt
byte plaintext[16] = {"000002021BCY0009"};
//cypher[16] stores the encrypted text
byte cypher[16];
//decryptedtext[16] stores decrypted text after decryption
byte decryptedtext[16];
//creating an object of AES128 class
AES128 aes128;

void printHex(byte data[], int length) {
  for (int i = 0; i < length; i++) {
    if (data[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  aes128.setKey(key, 16); // Setting Key for AES

  Serial.print("Before Encryption: ");
  printHex(plaintext, sizeof(plaintext));

  aes128.encryptBlock(cypher, plaintext); // cypher->output block and plaintext->input block
  Serial.print("After Encryption: ");
  printHex(cypher, sizeof(cypher));

  aes128.decryptBlock(decryptedtext, cypher);

  Serial.print("After Decryption: ");
  printHex(decryptedtext, sizeof(decryptedtext));
}

void loop() {
  // put your main code here, to run repeatedly:
}
