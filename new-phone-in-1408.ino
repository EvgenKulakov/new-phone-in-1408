#include <Keypad.h>

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
  Serial.begin(9600);
}

void loop() {
  char k = keypad.getKey();
  if (k) {
    Serial.println(k);
  }
}