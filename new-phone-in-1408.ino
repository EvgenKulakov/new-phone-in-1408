#include <Keypad.h>
const byte ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; // подключаем R1→D9, R2→D8, ...
byte colPins[COLS] = {5, 4, 3, 2}; // C1→D5, C2→D4, ...
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char k = keypad.getKey();
  if (k) {
    Serial.println(k); // при нажатии выводим символ
  }
}