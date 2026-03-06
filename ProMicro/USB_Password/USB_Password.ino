// Скетч для эмулятора USB клавиатуры ввод команд на pro micro
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

Keyboard.begin();   // начать работу;
Keyboard.end();     // завершить;

Keyboard.print();   //вводит заданный текст;
Keyboard.println(); //то же самое что и Keyboard.print(), но при этом автоматически происходит переход на новую строку;

Keyboard.press('a'); //зажимает указанную клавишу;
Keyboard.release('a'); // отпускает указанную клавишу;
Keyboard.releaseAll(); //отпускает все клавиши;

Keyboard.write('a'); //быстрое нажатие (нажал+отпустил);
Keyboard.write(KEY_RETURN); //эмулирует нажатие клавиши «Enter» на виртуальной клавиатуре.

// Специальные клавиши
KEY_RETURN
KEY_ESC
KEY_TAB
KEY_BACKSPACE
KEY_DELETE
KEY_UP_ARROW
KEY_DOWN_ARROW
KEY_LEFT_ARROW
KEY_RIGHT_ARROW
KEY_F1 … KEY_F24

// Модификаторы
KEY_LEFT_CTRL
KEY_LEFT_SHIFT
KEY_LEFT_ALT
KEY_LEFT_GUI (Win / Cmd)
*/

#include <Keyboard.h>

void setup() {
  //Serial.begin(9600);
  Keyboard.begin();
  pinMode(LED_BUILTIN_TX, OUTPUT);
  pinMode(LED_BUILTIN_RX, OUTPUT);

  delay(1000);

  diodStart();

  //inputProgram();  
  //shimTest(30);

  //diodEnd();

  //Keyboard.end();
}

void loop() {
  shimTest(40);
}

void shimTest( int hZ ) {
    
  double period = 1000.0 / hZ;
  double time = 3000.0;
  double procent = period / (time / period);

  double off = 0.0;
  double on = period;
/*
  for(double i = 0; i <= time; i += period) {
    
    synchronizedFlashes( off , on , 1 );
    off += procent;
    on = period - off;

  }
*/
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
    Serial.println("; period ");
    Serial.println(period);
    */
  }

 /* 
  synchronizedFlashes( 1, 29, 50 );
  synchronizedFlashes( 2, 28, 50 );
  synchronizedFlashes( 3, 27, 50 );
  synchronizedFlashes( 4, 26, 50 );
  synchronizedFlashes( 5, 25, 50 );
  synchronizedFlashes( 6, 24, 50 );
  synchronizedFlashes( 7, 23, 50 );
  synchronizedFlashes( 8, 22, 50 );
  synchronizedFlashes( 9, 21, 50 );
  synchronizedFlashes( 10, 20, 50 );
  synchronizedFlashes( 11, 19, 50 );
  synchronizedFlashes( 12, 18, 50 );
  synchronizedFlashes( 13, 17, 50 );
  synchronizedFlashes( 14, 16, 50 );
  synchronizedFlashes( 15, 15, 50 );
  synchronizedFlashes( 16, 14, 50 );
  synchronizedFlashes( 17, 13, 50 );
  synchronizedFlashes( 18, 12, 50 );
  synchronizedFlashes( 19, 11, 50 );
  synchronizedFlashes( 20, 10, 50 );
  synchronizedFlashes( 21, 9, 50 );
  synchronizedFlashes( 22, 8, 50 );
  synchronizedFlashes( 23, 7, 50 );
  synchronizedFlashes( 24, 6, 50 );
  synchronizedFlashes( 25, 5, 50 );
  synchronizedFlashes( 26, 4, 50 );
  synchronizedFlashes( 27, 3, 50 );
  synchronizedFlashes( 28, 2, 50 );
  synchronizedFlashes( 29, 1, 50 );
*/

  
  
 
  /*
  int i = 0;
  while ( i < 50 ) {
    acceleratingFlashes( 20, 3, 1 );
    slowingFlashes( 20, 3, 1 );
    i++;
  }
  variableFlashes( 15, 50 );
  variableFlashes( 3, 50 );
  */
}


// Программа ввода
void inputProgram() { 
  Keyboard.write(KEY_ESC);
  delay(200);
  Keyboard.println( "Pa$$w0rD" );
}

// ДИОДЫ
void diodStart(){
  synchronizedFlashes ( 1000, 200, 3 );
  digitalWrite(LED_BUILTIN_TX, LOW);
  digitalWrite(LED_BUILTIN_RX, LOW);
}

void diodEnd(){
  acceleratingFlashes( 200, 0, 5 );
  delay(2000);
  ledRx( 300, 300, 4 );
  delay(2000);
  ledTx( 300, 300, 4 );
  delay(2000);
  synchronizedFlashes ( 300, 300, 3 );
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
void synchronizedFlashes( double pauseOn, double pauseOff, double count ) { 
  double i = 0;
  while ( i <= count ) {
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

