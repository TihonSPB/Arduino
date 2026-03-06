// Скетч для проверки ШИМ диодов
/*
      ┌───┐
  ┌───┤USB├───┐
  │○  └───┘  ○│
  │○         ○│
  │○         ○│
  │○         ○│
  │○         ○│
  │○         ○│
  │○         ○│
  │○         ○│
  │○         ○│
  │○  ◘   ◘  ○│
  │○ RX   TX ○│
  │○         ○│
  └───────────┘

*/

void setup() {
  //Serial.begin(9600);
  pinMode(LED_BUILTIN_TX, OUTPUT);
  pinMode(LED_BUILTIN_RX, OUTPUT);
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
    digitalWrite(LED_BUILTIN_TX, LOW);
    digitalWrite(LED_BUILTIN_RX, LOW);
    delay( on );
    digitalWrite(LED_BUILTIN_TX, HIGH);
    digitalWrite(LED_BUILTIN_RX, HIGH);
    delay( off );
    off += procent;
    on = period - off;
    i += period;
  }

  off = period;;
  on = 0.0;
  i = 0;
  while ( i <= time ) {
    digitalWrite(LED_BUILTIN_TX, LOW);
    digitalWrite(LED_BUILTIN_RX, LOW);
    delay( on );
    digitalWrite(LED_BUILTIN_TX, HIGH);
    digitalWrite(LED_BUILTIN_RX, HIGH);
    delay( off );
    on += procent;
    off = period - on;    
    i += period;
    /*
    Serial.print("on ");
    Serial.print(on);
    Serial.print("; off ");
    Serial.print(off);
    Serial.print("; period ");
    Serial.println(period);
    */
  }
}

