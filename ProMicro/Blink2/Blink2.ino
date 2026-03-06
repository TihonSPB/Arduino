// Скетч для проверки диодов на pro micro
/*
      ┌───┐
  ┌───┤   ├───┐
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
  pinMode(LED_BUILTIN_TX, OUTPUT);
  pinMode(LED_BUILTIN_RX, OUTPUT);
}

void loop() {
  
  for(int i = 0; i <= 200; i+=10) {
     variableFlashes( i );
  }

  for(int i = 200; i >= 0; i-=10) {
     variableFlashes( i );
  }
}

// поочередные
void variableFlashes( int pause ) {
  digitalWrite(LED_BUILTIN_TX, HIGH);
  digitalWrite(LED_BUILTIN_RX, LOW); 
  delay( pause );
  digitalWrite(LED_BUILTIN_TX, LOW);
  digitalWrite(LED_BUILTIN_RX, HIGH);
  delay( pause );
}
