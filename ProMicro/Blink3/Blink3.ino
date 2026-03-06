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
  delay(2000);
}

void loop() {

  int on = 1000;
  int off = 200;
  int count = 3;
  int start = 50;
  int end = 0;
  int step = 1;

  delay(2000);
  variableFlashes( off, count );
  synchronizedFlashes( on, off, count );
  digitalWrite(LED_BUILTIN_TX, LOW);
  digitalWrite(LED_BUILTIN_RX, LOW);
  delay(1000);
  ledRx( on, off, count );
  ledTx( on, off, count );
  delay(1000);
  acceleratingFlashes( start, end, step );
  slowingFlashes( start, end, step );
  synchronizedFlashes( on, off, count );
  
}


// ускорение поочередного мирцания диодов
void acceleratingFlashes( int start, int end, int step ) {
  for(int i = start; i >= end; i -= step) {
    variableFlashes( i, 1 );
  }
  digitalWrite(LED_BUILTIN_TX, LOW);
  digitalWrite(LED_BUILTIN_RX, LOW);
}
// замедление поочередного мирцания диодов
void slowingFlashes( int start, int end, int step ) {
  digitalWrite(LED_BUILTIN_TX, LOW);
  digitalWrite(LED_BUILTIN_RX, LOW);
  for(int i = end; i <= start; i += step) {
    variableFlashes( i, 1 );
  }
}
// поочередные мирцания диодов
void variableFlashes( int pause,  int count ) {
  int i = 0;
  while ( i < count ) {
    digitalWrite(LED_BUILTIN_TX, HIGH);
    digitalWrite(LED_BUILTIN_RX, LOW); 
    delay( pause );
    digitalWrite(LED_BUILTIN_TX, LOW);
    digitalWrite(LED_BUILTIN_RX, HIGH);
    delay( pause );
    i++;
  }  
}
// синхронизированные мирцания диодов
void synchronizedFlashes( int pauseOn, int pauseOff, int count ) { 
  int i = 0;
  while ( i < count ) {
    digitalWrite(LED_BUILTIN_TX, LOW);
    digitalWrite(LED_BUILTIN_RX, LOW);
    delay( pauseOn );
    digitalWrite(LED_BUILTIN_TX, HIGH);
    digitalWrite(LED_BUILTIN_RX, HIGH);
    delay( pauseOff );
    i++;
  }  
}
// диод RX вкл. выкл.
void ledRx( int pauseOn, int pauseOff, int count ) { 
  int i = 0;
  while ( i < count ) {
    digitalWrite(LED_BUILTIN_RX, LOW);
    delay( pauseOn );
    digitalWrite(LED_BUILTIN_RX, HIGH);
    delay( pauseOff );
    i++;
  }  
}
// диод TX вкл. выкл.
void ledTx( int pauseOn, int pauseOff, int count ) { 
  int i = 0;
  while ( i < count ) {
    digitalWrite(LED_BUILTIN_TX, LOW);
    delay( pauseOn );
    digitalWrite(LED_BUILTIN_TX, HIGH);
    delay( pauseOff );
    i++;
  }  
}
