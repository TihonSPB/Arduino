// Скетч для эмулятора USB клавиатуры ввод строк через Alt-код на ESP32-S3 SuperMini
/*
Функции: 
printString("text текст"); - ввод строки через Alt-коды
winRun("notepad"); - выполняет команду WIN+r 
*/

#include <USB.h>
#include <USBHIDKeyboard.h>

USBHIDKeyboard Keyboard;

// Определяет длину UTF-8 символа по первому байту
uint8_t getUtf8CharLength(uint8_t firstByte) {
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


// Функция возвращает Alt-код символа
int altCodeChar(const String& ch) {
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


// Ввод через NumPad
void sendDigit(int digit) {
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


// Функция для ввода alt-кода символа
void charInfoSimple(const String& charStr) {
  if (charStr.length() == 0) return;
  
  int altCode = altCodeChar(charStr);
  String numStr = String(altCode);

  Keyboard.press(KEY_LEFT_ALT);
  delay(5);
  // Отправляем каждую цифру
  for (int i = 0; i < numStr.length(); i++) {
    int digit = numStr.charAt(i) - '0'; // Преобразуем символ в число
    if (digit >= 0 && digit <= 9) {
      sendDigit(digit); // Отправляем цифру через NumPad
    }
  }
  Keyboard.releaseAll(); // Отпускаем все
}


// Разбивает UTF-8 строку на отдельные символы, ввод каждого через Alt-код
void printString(const String& str) {
  int pos = 0; // Текущая позиция в строке (в байтах)
  int charNum = 1;
  Keyboard.releaseAll(); // Отпускаем все
  delay(50);
  // Проходим по всей строке
  while (pos < str.length()) {
    uint8_t firstByte = (uint8_t)str[pos]; // Читаем первый байт текущего символа
    uint8_t charLen = getUtf8CharLength(firstByte); // Определяем длину UTF-8 символа по первому байту
    // Проверяем, что символ корректный и полностью помещается в строку
    if (charLen > 0 && pos + charLen <= str.length()) {
      // Извлекаем полный UTF-8 символ (1-4 байта)
      String oneChar = str.substring(pos, pos + charLen);
      // Вводим alt-код символа
      charInfoSimple(oneChar);
      // Перемещаем указатель на следующий символ
      pos += charLen;
    } else {
      // Некорректная UTF-8 последовательность
      pos++; // Пропускаем проблемный байт и продолжаем
    }
    charNum++; // Увеличиваем счетчик символов
  }
}


// Функция win+r (Выполнить)
void winRun(const String& str) {
  Keyboard.releaseAll(); // Отпускаем все
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(50);
  Keyboard.releaseAll(); // Отпускаем все
  delay(500); // Ждем открытия окна
  printString(str);
  Keyboard.write(KEY_RETURN); // Нажимаем Enter
  delay(2000);
}


void setup() {
  
  delay(500);  
  USB.begin();
  Keyboard.begin();
  delay(2000);

  // Нажимаем Win+R (открывает окно "Выполнить")
  winRun("notepad");

  const char* password = "Пa$$w0rД";
  printString(password);
}


void loop() {}
