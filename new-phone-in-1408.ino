#include <Keypad.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
// RX - цифровой вывод 10, необходимо соединить с выводом TX дисплея (RX соединить с 11 пином ардуино через резистор 1 кОм)
// TX - цифровой вывод 11, необходимо соединить с выводом RX дисплея (TX соединить с 10 пином ардуино через резистор 1 кОм)
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX для плеера DFPlayer Mini 
DFRobotDFPlayerMini myDFPlayer;

#define RECEPTION_666_CODE "666"
#define UNDERTAKER_013_CODE "013"
#define AMBULANCE_003_CODE "003"
#define CONFESSION_012_CODE "012"
#define GOODBYE_555_CODE "555"
#define VICTORY_5150673_CODE "5150673"

#define WAIT_TIME_BEFORE_RESET 4000

// реле: центральный на плюс питания, левый на плюс магнита (смотреть лапмочками вниз)
#define MAGNET_PIN 12

// кнопка сброса: один провод в A1, другой в GND
#define RESET_BUTTON A1

String number = "";
bool dialingProcess = false;
unsigned long int lastDialing = 0;
bool resetActive = true;

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
  delay(100);
  mySoftwareSerial.begin(9600);
  delay(100);
  myDFPlayer.begin(mySoftwareSerial);
  delay(100);
  myDFPlayer.setTimeOut(300);
  delay(100);
  myDFPlayer.volume(15);
  delay(100);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  delay(100);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  delay(100);

  Serial.begin(115200);
  Serial.println(F("DFPlayer Mini online."));

  pinMode(RESET_BUTTON, INPUT_PULLUP);

  pinMode(MAGNET_PIN, OUTPUT);
  digitalWrite(MAGNET_PIN, HIGH);
}

void loop() {
  if (dialingProcess && resetActive) {
    bool reset = digitalRead(RESET_BUTTON);
    if (reset) {
      stopDialingProcess();
      myDFPlayer.playMp3Folder(8);
      delay(1000);
    }
  }

  char input = keypad.getKey();
  if (input && input != 'R') {
    number += input;
    Serial.println(number);
    if (!dialingProcess) {
      bool reset = digitalRead(RESET_BUTTON);
      if (reset) {
        resetActive = false;
        Serial.println("reset отключен");
      }
      dialingProcess = true;
    }
    lastDialing = millis();
  }

  // действия после набора 666 (ресепшон)
  if (number.equals(RECEPTION_666_CODE)) {
    stopDialingProcess();
    myDFPlayer.playMp3Folder(1);
    delay(12000);
  }

  // действия после набора 013 (гробовщик)
  if (number.equals(UNDERTAKER_013_CODE)) {
    stopDialingProcess();
    myDFPlayer.playMp3Folder(2);
    delay(10500);
  }

  // действия после набора 003 (скорая помощь)
  if (number.equals(AMBULANCE_003_CODE)) {
    stopDialingProcess();
    myDFPlayer.playMp3Folder(3);
    delay(9000);
  }

  // действия после набора 012 (исповедь)
  if (number.equals(CONFESSION_012_CODE)) {
    stopDialingProcess();
    myDFPlayer.playMp3Folder(4);
    delay(8500);
  }

  // действия после набора 555 (проститься с близкими)
  if (number.equals(GOODBYE_555_CODE)) {
    stopDialingProcess();
    myDFPlayer.playMp3Folder(5);
    delay(16000);
  }
 
  // действия после победы
  if (number.equals(VICTORY_5150673_CODE)) {
    stopDialingProcess();
    myDFPlayer.playMp3Folder(6);
    delay(14500);
    digitalWrite(MAGNET_PIN, LOW);
    delay(3000);
    digitalWrite(MAGNET_PIN, HIGH);
  }

  // неправильный номер
  if (dialingProcess && timeInterval(lastDialing, millis()) > WAIT_TIME_BEFORE_RESET) {
    stopDialingProcess();
    myDFPlayer.playMp3Folder(7);
    delay(4500);
  }
}

void stopDialingProcess() {
  number = "";
  dialingProcess = false;
  bool resetActive = true;
  Serial.println("сброс номера");
}

unsigned long int timeInterval(unsigned long int time1, unsigned long int time2) {
  if (time2 >= time1) {
    return time2 - time1;
  }
  else {
    return 0xFFFFFFFF - time1 + time2 + 1;
  }
}