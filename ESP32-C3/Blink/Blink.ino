// Скетч для проверки диода на ESP32-C3
/*
          ┌───┐
  ┌───────┤   ├───────┐
  │○ 5v ◘ └───┘ ◘ 10 ○│
  │○ GND GPIO8 PWR 9 ○│
  │○ 3.3v          8 ○│
  │○ RX            7 ○│
  │○ TX ┌───────┐  6 ○│
  │○ 2  │  0.42 │  5 ○│
  │○ 1  │  OLED │  4 ○│
  │○ 0  └───────┘  3 ○│
  │  ◯ BOOT  RST ◯  │
  └───────────────────┘
*/
#define LED_BUILTIN 8

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Настройка GPIO8 на выход
  delay(1000);
}

void loop() {
  shimTest(40);
}

void shimTest( int hZ ) {
  
  double time = 2500.0;
  double period = 1000.0 / hZ;
  double procent = period / (time / period);

  double off = 0.0;
  double on = period;
  double i = 0;
  while ( i <= time ) {    
    digitalWrite(LED_BUILTIN, LOW);
    delay( on );
    digitalWrite(LED_BUILTIN, HIGH);
    delay( off );
    off += procent;
    on = period - off;
    i += period;
  }
  off = period;;
  on = 0.0;
  i = 0;
  while ( i <= time ) {
    digitalWrite(LED_BUILTIN, LOW);
    delay( on );
    digitalWrite(LED_BUILTIN, HIGH);
    delay( off );
    on += procent;
    off = period - on;    
    i += period;
  }
}