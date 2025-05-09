#include <Keypad.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
// RX - цифровой вывод 10, необходимо соединить с выводом TX дисплея (RX соединить с 11 пином ардуино через резистор 1 кОм)
// TX - цифровой вывод 11, необходимо соединить с выводом RX дисплея (TX соединить с 10 пином ардуино через резистор 1 кОм)
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX для плеера DFPlayer Mini 
DFRobotDFPlayerMini myDFPlayer;

#define VICTORY_CODE "5150673"
#define RECEPTION_CODE "666"
#define UNDERTAKER_CODE "013"
#define AMBULANCE_CODE "003"
#define CONFESSION_CODE "012"
#define GOODBYE_CODE "555"

#define WAIT_TIME_BEFORE_RESET 4000

// пины для кнопки сброса
#define RESET_OUTPUT A0 
#define RESET_INPUT A1

String number = "";
bool dialingProcess = false;
unsigned long int lastDialing = 0;

// R и C - обозначение проводов ROWS и COLS от матричной клавиатуры
const byte ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'N','N','N','R'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; // R4→D2, R3→D3, R2→D4, R1→D5
byte colPins[COLS] = {5, 4, 3, 2}; // C4→D6, C3→D7, C2→D8, C1→D9
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  mySoftwareSerial.begin(9600); 
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.setTimeOut(300);
  myDFPlayer.volume(30);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  pinMode(RESET_OUTPUT, OUTPUT);
  pinMode(RESET_INPUT, INPUT_PULLUP);

  Serial.begin(115200);

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Unable to begin:");
    Serial.println("1.Please recheck the connection!");
    Serial.println("2.Please insert the SD card!");
  } else {
      Serial.println(F("DFPlayer Mini online."));
  }
}

void loop() {
  // if (dialingProcess) {
  //   bool reset = digitalRead(RESET_INPUT);
  //   if (reset) {
  //     number = "";
  //     dialingProcess = false;
  //     Serial.println("сброс номера");
  //   }
  // }

  char input = keypad.getKey();
  if (input) {
    number += input;
    Serial.println(number);
    dialingProcess = true;
    lastDialing = millis();
  }

  // действия после набора 666 (ресепшон)
  if (number.equals(RECEPTION_CODE)) {
    Serial.println("действия после набора 666 (ресепшон)");
    number = "";
    dialingProcess = false;
    myDFPlayer.playMp3Folder(1);
    delay(11000);
  }

  // действия после набора 013 (гробовщик)
  if (number.equals(UNDERTAKER_CODE)) {
    number = "";
    Serial.println("действия после набора 013 (гробовщик)");
    dialingProcess = false;
    myDFPlayer.playMp3Folder(2);
    delay(9000);
  }

  // действия после набора 003 (скорая помощь)
  if (number.equals(AMBULANCE_CODE)) {
    number = "";
    Serial.println("действия после набора 003 (скорая помощь)");
    dialingProcess = false;
    myDFPlayer.playMp3Folder(3);
    delay(8000);
  }

  // действия после набора 012 (исповедь)
  if (number.equals(CONFESSION_CODE)) {
    number = "";
    Serial.println("действия после набора 012 (исповедь)");
    dialingProcess = false;
    myDFPlayer.playMp3Folder(4);
    delay(9000);
  }

  // действия после набора 555 (проститься с близкими)
  if (number.equals(GOODBYE_CODE)) {
    number = "";
    Serial.println("действия после набора 555 (проститься с близкими)");
    dialingProcess = false;
    myDFPlayer.play(5);
    delay(15000);
  }
 
  // действия после победы
  if (number.equals(VICTORY_CODE)) {
    number = "";
    Serial.println("действия после победы");
    dialingProcess = false;
    myDFPlayer.play(6);
    delay(14000);
    // digitalWrite(MAGNET_PIN, LOW);
    // delay(1000);
    // digitalWrite(MAGNET_PIN, HIGH);
  }

  // неправильный номер
  if (dialingProcess && timeInterval(lastDialing, millis()) > WAIT_TIME_BEFORE_RESET) {
    number = "";
    Serial.println("неправильный номер");
    dialingProcess = false;
    myDFPlayer.play(7);
    delay(3000);
  }
}

unsigned long int timeInterval(unsigned long int time1, unsigned long int time2) {
  if (time2 >= time1) {
    return time2 - time1;
  }
  else {
    return 0xFFFFFFFF - time1 + time2 + 1;
  }
}