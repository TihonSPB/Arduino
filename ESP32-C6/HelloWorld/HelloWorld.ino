#define BUTTON_PIN 9

void setup() {
  // put your setup code here, to run once:
  delay(2000);
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Привет, мир!");
  static bool pState = false;
  bool state = !digitalRead(BUTTON_PIN);
  if (pState != state) {  // состояние изменилось
      pState = state;     // запомнить новое
      if (state) Serial.println("Кнопка нажата");
      else Serial.println("Кнопка отпущена");
  }  
}
