# secure-RFID-attendance-system
A smart Radio Frequency Identification (RFID) based attendance system that is secured with AES128 to prevent tampering of tags. The novelty in this approach is the security aspect as there exist no RFID based attendance systems that are secured. RFID is versatile and widely used - as seen in credit/debit cards or US passports. AES128 was chosen since we use passive RFID tags here.

## Approach
Each RFID chip has a unique number - UID (unique identity). We map this UID to a student's roll number thereby registering the student. We write encrypted roll numbers into RFID tags with the paired UID. When a student scans their RFID tag/card, the contents of the tag are read, decrypted and their registration is verified. If the student is not registered for the course, then we say that they are unauthorised to enter the classroom.

## Circuit
### Components
- RFID module
- LCD display
- Arduino UNO
### Connections
RFID module (MFRC522)
- 3.3V to 3.3V slot in Arduino
- RST to pin 9 in Arduino
- GND to GND in Arduino
- skip IRQ
- MISO to pin 12 in Arduino
- MOSI to pin 11 in Arduino
- SCK to pin 13 in Arduino
- SDA to pin 10 in Arduino
LCD display with I2C
- GND to GND
- VCC to 5V slot in Arduino
- SDA to A4 slot in Arduino
- SCL to A5slot in Arduino
### Libraries
- https://github.com/OperatorFoundation/Crypto
- https://github.com/miguelbalboa/rfid
- https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

## Advantages
- Faster and more efficient than traditional methods
- Can collect additional data such as time of entry of student, duration of class attended, late entries etc.

## Challenges
- Proxy is still possible using cards, so RFID reader has to be
supervised.
- RFID chips have limited resources.
- The RFID security protocols we researched about need active
RFID tags - mobiles fit the category.
- But implementing RFID system on mobiles had numerous
challenges (Phase II).
- Many OS used in mobile phones.
- UIDs of mobiles change frequently - this is a major challenge
for extending our work to mobiles.

## Extension
The next step after this project was to extend this logic/blueprint to mobile phones. Almost all college student carry mobile phones everywhere, I wanted to use the fact that usually students carry a phone to class and they pretty much cannot live without it. That is, students (people in general) aren't separable from their phones, so using the RFID chip in phones would eliminate the possibility of proxies. But there were many challenges while implementing the extension so I hope someone else figures out a way to implement the final idea of extending it to phones. In that case, please feel free to fork this repo and add in your work.

## References
- https://www.hackster.io/Techatronic/rfid-based-attendance-system-using-arduino-d3d719
- Daou, Hocine & Kayssi, Ayman & Chehab, Ali. (2009). RFID security protocols. 593 - 597. 10.1109/INNOVATIONS.2008.4781675. 
