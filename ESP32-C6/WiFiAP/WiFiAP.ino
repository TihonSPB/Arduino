// Пример точки доступа WiFi (AP) из https://arduino-tex.ru/news/15/urok-1-veb-server-esp32-esp8266-v-srede-arduino-ide.html
/*
Настройки прошивки:
  Выбрать плату:  Tools/Board/esp32/ESP32C6 Dev Module
  Для вывода через USB в Serial Monitor: Tools/USB CDC On Boot: "Enabled"

*/
// подключение библиотек
#include <WiFi.h>
#include <WebServer.h>
/* Свои SSID и пароль */
const char* ssid = "ServerESP32";  
const char* password = "01234567";  

/* Настройки IP адреса */
IPAddress local_ip(192,168,2,1);
IPAddress gateway(192,168,2,1);
IPAddress subnet(255,255,255,0);
WebServer server(80); // порт по умолчанию для HTTP для получения доступа к серверу без необходимости указывать его в url-адресе

bool LED1status = LOW;  
bool COMANDstatus = false;  

void setup() {
  delay(2000); // Задержка на 2000 миллисекунд (2 секунды)
  Serial.begin(115200); // Инициализируем последовательный порт для связи с компьютером для отладки 
  // устанавливаем для GPIO значение OUTPUT
  pinMode(LED_BUILTIN, OUTPUT);
  // настраиваем программную точку доступа для создания сети Wi-Fi
  WiFi.softAP(ssid, password); // проверяем SSID, пароль
  WiFi.softAPConfig(local_ip, gateway, subnet); // проверяем IP-адрес, маску IP-подсети и IP-шлюз
  delay(100); // Задержка на 100 миллисекунд
  // указываем какой код выполнять по входящим HTTP-запроам
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/buttonOn", handle_buttonOn);
  server.on("/buttonOff", handle_buttonOff);
  // любой URL-адрес, отличный от указанного в server.on()
  server.onNotFound(handle_NotFound);
  // запускаем сервер, вызываем метод begin объекта server
  server.begin();
  Serial.println("HTTP сервер запущен");
}
void loop() {
  // для обработки входящих html-запросов вызываем handleClient() метод объекта сервера
  server.handleClient();
  // меняем состояние светодиода по запросу
  if(LED1status)
  {digitalWrite(LED_BUILTIN, HIGH);}
  else
  {digitalWrite(LED_BUILTIN, LOW);}
}
// (/) функция корневого URL
void handle_OnConnect() {
  // исходное состояние светодиодов
  LED1status = LOW;
  COMANDstatus = false;
  // Выводим сообщение в монитор порта
  Serial.println("Светодиод: OFF | Кнопка: OFF");
  /* 
  отправляем: 
      код 200 (один из кодов состояния HTTP) ответ получен 
      указываем тип содержимого как «text/html»
      вызываем пользовательскую функцию SendHTML()
  */
  server.send(200, "text/html", SendHTML(LED1status,COMANDstatus)); 
}

void handle_led1on() {
  LED1status = HIGH;
  // Выводим сообщение в монитор порта
  Serial.println("Светодиод: ON");
  /* отправляем: 
      код 200 (один из кодов состояния HTTP) ответ получен 
      указываем тип содержимого как «text/html»
      вызываем пользовательскую функцию SendHTML()
  */
  server.send(200, "text/html", SendHTML(true,COMANDstatus)); 
}

void handle_led1off() {
  LED1status = LOW;
  // Выводим сообщение в монитор порта
  Serial.println("Светодиод: OFF");
  /* отправляем: 
      код 200 (один из кодов состояния HTTP) ответ получен 
      указываем тип содержимого как «text/html»
      вызываем пользовательскую функцию SendHTML()
  */
  server.send(200, "text/html", SendHTML(false,COMANDstatus)); 
}

void handle_buttonOn() {
  COMANDstatus = true;
  // Выводим сообщение в монитор порта
  Serial.println("Кнопка: ON");
  /* отправляем: 
      код 200 (один из кодов состояния HTTP) ответ получен 
      указываем тип содержимого как «text/html»
      вызываем пользовательскую функцию SendHTML()
  */
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}

void handle_buttonOff() {
  COMANDstatus = false;
  // Выводим сообщение в монитор порта
  Serial.println("Кнопка: OFF");
  /* отправляем: 
      код 200 (один из кодов состояния HTTP) ответ получен 
      указываем тип содержимого как «text/html»
      вызываем пользовательскую функцию SendHTML()
  */
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void handle_NotFound(){
  /* отправляем: 
      код ошибки 404 (один из кодов состояния HTTP) 
      указываем тип содержимого как «text/plain»  
  */
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Управление светодиодом</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Веб сервер</h1>\n";
  ptr +="<h3>Режим точка доступа WiFi (AP)</h3>\n";
  if(led1stat)
  {ptr +="<p>Состояние LED: ВКЛ.</p><a class=\"button button-off\" href=\"/led1off\">ВЫКЛ.</a>\n";}
  else
  {ptr +="<p>Состояние LED: ВЫКЛ.</p><a class=\"button button-on\" href=\"/led1on\">ВКЛ.</a>\n";}
  if(led2stat)
  {ptr +="<p>Состояние кнопки: ВКЛ.</p><a class=\"button button-off\" href=\"/buttonOff\">ВЫКЛ.</a>\n";}
  else
  {ptr +="<p>Состояние кнопки: ВЫКЛ.</p><a class=\"button button-on\" href=\"/buttonOn\">ВКЛ.</a>\n";}
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
