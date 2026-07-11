#include "Alt_keyboard.h"

#include <USB.h>
#include <USBHIDKeyboard.h>
/*
Keyboard.begin() — инициализирует библиотеку и запускает режим эмуляции клавиатуры.
Keyboard.end() — останавливает работу эмуляции.
Keyboard.print("text") — вводит заданную строку текста.
Keyboard.write(char) — отправляет один символ (байт).
Keyboard.press(key) — зажимает конкретную клавишу (без отпускания).
Keyboard.release(key) — отпускает зажатую ранее клавишу.
Keyboard.releaseAll() — отпускает абсолютно все нажатые ранее клавиши.

Модификаторы: KEY_LEFT_CTRL, KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_LEFT_GUI (он же Win/Command), а также их аналоги для правых клавиш (например, KEY_RIGHT_CTRL).
Клавиши управления: KEY_BACKSPACE, KEY_TAB, KEY_RETURN, KEY_ESC, KEY_INSERT, KEY_DELETE, KEY_PAGE_UP, KEY_PAGE_DOWN, KEY_HOME, KEY_END, KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW.
Дополнительные: KEY_CAPS_LOCK, KEY_F1 — KEY_F24.
*/

// Объект USB HID клавиатуры
USBHIDKeyboard Keyboard;

// Флаг, что хотя бы один LED Report от Windows получен
volatile bool numLockKnown = false;
// Текущее состояние Num Lock
volatile bool numLockState = false;

// --------------------------------------------------
// Callback от USB HID клавиатуры
// Windows вызывает его при изменении NumLock/CapsLock/ScrollLock
// --------------------------------------------------
static void keyboardEvent(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
  // Интересует только событие изменения LED
  if (event_base == ARDUINO_USB_HID_KEYBOARD_EVENTS && event_id == ARDUINO_USB_HID_KEYBOARD_LED_EVENT)
  {
    auto *data = (arduino_usb_hid_keyboard_event_data_t *)event_data;
    // Запоминаем состояние Num Lock
    numLockState = data->numlock;
    // Сообщаем программе, что состояние уже известно
    numLockKnown = true;
  }
}

// --------------------------------------------------
// Определяет длину UTF-8 символа по первому байту
// --------------------------------------------------
static uint8_t getUtf8CharLength(uint8_t firstByte) {
  if ((firstByte & 0x80) == 0) {
    return 1;  // ASCII: 0xxxxxxx
  }
  else if ((firstByte & 0xE0) == 0xC0) {
    // Проверка на overlong sequences (C0, C1 недопустимы)
    if (firstByte == 0xC0 || firstByte == 0xC1) {
      return 0;
    }
    return 2;  // 110xxxxx 10xxxxxx
  }
  else if ((firstByte & 0xF0) == 0xE0) {
    return 3;  // 1110xxxx 10xxxxxx 10xxxxxx
  }
  else if ((firstByte & 0xF8) == 0xF0) {
    return 4;  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
  }
  return 0;  // Некорректный первый байт
}

// --------------------------------------------------
// Поиск Alt-кода символа
// Возвращает: Alt-код символа
// --------------------------------------------------
static int altCodeChar(const String& ch) {
  const char* str = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~⌂АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмноп░▒▓│┤╡╢╖╕╣║╗╝╜╛┐└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀рстуфхцчшщъыьэюяЁёЄєЇїЎў°∙·√№¤■ ";
  
  if (ch.length() == 0) {
    return 63;  // Пустая строка (63 = ?)
  }
  int pos = 0;           // Текущая позиция в байтах
  int charNum = 32;      // Номер текущего символа (начинаем с 32)
  int strLen = strlen(str);  // Длина строки в байтах
  int chLen = ch.length();  // Длина искомого символа в байтах
  
  while (pos < strLen) {  // Проходим по всей строке
    // Читаем первый байт текущего символа
    uint8_t firstByte = (uint8_t)str[pos];
    // Определяем длину UTF-8 символа по первому байту
    uint8_t charLen = getUtf8CharLength(firstByte);    
    // Проверяем, не выходим ли за границы строки
    if (pos + charLen > strLen) {
      return 63;  // Ошибка в UTF-8 строке (63 = ?)
    }    
    // Побайтовое сравнение
    bool match = true;
    if (charLen == chLen) {  // Сначала проверяем длину
      for (uint8_t i = 0; i < charLen; i++) {
        if (str[pos + i] != ch[i]) {
          match = false;
          break;
        }
      }
    } else {
      match = false;
    }    
    if (match) {
      return charNum;
    }    
    pos += charLen;
    charNum++;
  }
  return 63;  // Символ не найден (63 = ?)
}

// --------------------------------------------------
// Отправка цифр через NumPad
// --------------------------------------------------
static void sendDigit(int digit) {
  switch(digit) {
    case 0: Keyboard.write(KEY_KP_0); break;
    case 1: Keyboard.write(KEY_KP_1); break;
    case 2: Keyboard.write(KEY_KP_2); break;
    case 3: Keyboard.write(KEY_KP_3); break;
    case 4: Keyboard.write(KEY_KP_4); break;
    case 5: Keyboard.write(KEY_KP_5); break;
    case 6: Keyboard.write(KEY_KP_6); break;
    case 7: Keyboard.write(KEY_KP_7); break;
    case 8: Keyboard.write(KEY_KP_8); break;
    case 9: Keyboard.write(KEY_KP_9); break;
  }
}

// --------------------------------------------------
// Ввод одного символа через Alt-код
// --------------------------------------------------
static void sendAltChar(const String& charStr) {
  if (charStr.length() == 0) return;
  
  int altCode = altCodeChar(charStr);
  String numStr = String(altCode);

  Keyboard.press(KEY_LEFT_ALT);
  delay(20);
  // Отправляем каждую цифру
  for (int i = 0; i < numStr.length(); i++) {
    sendDigit(numStr[i] - '0'); // Преобразуем символ в число, отправляем цифру через NumPad
    delay(10);
  }
  Keyboard.releaseAll(); // Отпускаем все
  delay(10);
}

// --------------------------------------------------
// Проверка на включенный NumLock
// --------------------------------------------------
static bool ensureNumLock()
{
  if (!numLockKnown) // Не получили ни одного LED Report
    return false;
  if (!numLockState) // Num Lock выключен — включаем
  {
    Keyboard.write(KEY_NUM_LOCK); 
    uint32_t start = millis();
    // Ждём подтверждение от Windows
    while (!numLockState && millis() - start < 500)
    {
      delay(10);
    }
  }
  return true;
}

// --------------------------------------------------
// Инициализация Keyboard
// --------------------------------------------------
void initKeyboard()
{
  // Подключаем обработчик событий клавиатуры
  Keyboard.onEvent(keyboardEvent);

  USB.begin(); // Запускаем USB
  Keyboard.begin(); // Запускаем HID клавиатуру
  
  // Даём Windows время обнаружить устройство
  delay(1000);
  // ждём первый LED report от Windows (состояние Num/Caps/Scroll Lock)
  uint32_t start = millis();

  while (!numLockKnown && millis() - start < 5000) {
    delay(10);
  }
  // Если получили состояние индикаторов, приводим Num Lock в нужное состояние
  ensureNumLock();
}

// --------------------------------------------------
// Ввод строки через Alt-код. Разбивает UTF-8 строку на отдельные символы.
// --------------------------------------------------
void printString(const String& str) {
  if (!ensureNumLock()) // Проверка на включенный NumLock
    return;
  int pos = 0; // Текущая позиция в строке (в байтах)
  Keyboard.releaseAll(); // Отпускаем все
  delay(20);
  // Проходим по всей строке
  while (pos < str.length()) {
    uint8_t charLen = getUtf8CharLength(str[pos]); // Определяем длину UTF-8 символа по первому байту
    if (charLen > 0) { 
      // Извлекаем полный UTF-8 символ (1-4 байта)
      String oneChar = str.substring(pos, pos + charLen);
      // Вводим alt-код символа
      sendAltChar(oneChar);
      // Перемещаем указатель на следующий символ
      pos += charLen;
    } else {
      // Некорректная UTF-8 последовательность
      pos++; // Пропускаем проблемный байт и продолжаем
    }
  }
}

// --------------------------------------------------
// Функция win+r (Выполнить)
// --------------------------------------------------
void winRun(const String& str) {
  Keyboard.releaseAll(); // Отпускаем все
  delay(20);
  // Нажимаем Win + R
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(50);
  Keyboard.releaseAll(); // Отпускаем все
  delay(500); // Ждем открытия окна
  printString(str); // Вводим команду
  Keyboard.write(KEY_RETURN); // Нажимаем Enter
  delay(500);
}

// --------------------------------------------------
// Функция win+l (Заблокировать windows)
// --------------------------------------------------
void winLock(){
  Keyboard.releaseAll(); // Отпускаем все
  delay(20);
  // Нажимаем Win + L
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('l');
  delay(50);
  Keyboard.releaseAll(); // Отпускаем все
  delay(20);
}

// --------------------------------------------------
// Специальные клавиши
// --------------------------------------------------
void sendEnter()
{
  Keyboard.write(KEY_RETURN);
}

void sendTab()
{
  Keyboard.write(KEY_TAB);
}

void sendEsc()
{
  Keyboard.write(KEY_ESC);
}