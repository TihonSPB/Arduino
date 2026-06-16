//Тестирование работы с байтами символа

/* Определяет длину UTF-8 символа по первому байту
 * Правила UTF-8:
 * - 0xxxxxxx - 1 байт (ASCII)
 * - 110xxxxx - 2 байта
 * - 1110xxxx - 3 байта
 * - 11110xxx - 4 байта
 */
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


// Функция: возвращает количество символов в UTF-8 строке
int getUtf8CharCount(const String& str) {
  int count = 0;
  int i = 0;
  int len = str.length();
  
  while (i < len) {
    uint8_t firstByte = (uint8_t)str[i];
    int charLen = getUtf8CharLength(firstByte);
    
    if (charLen == 0) {
      // Некорректный символ - пропускаем байт
      i++;
    } else {
      i += charLen;
    }
    count++;
  }    
  return count;
}


// Функция для вывода информации о строке
void stringInfo(const String& str) {
  Serial.print("Исходная строка: ");
  Serial.println(str);
  Serial.print("Количество символов: ");
  Serial.println(getUtf8CharCount(str));
  Serial.print("Длина в байтах: ");
  Serial.println(str.length());
}


// Функция возвращает Alt-код символа
int altCodeChar(const String& ch) {

  const char* str = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~⌂АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмноп░▒▓│┤╡╢╖╕╣║╗╝╜╛┐└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀рстуфхцчшщъыьэюяЁёЄєЇїЎў°∙·√№¤■ ";
  
  if (ch.length() == 0) {
    return -1;  // Пустая строка
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
      return -1;  // Ошибка в UTF-8 строке
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
  return -1;  // Символ не найден
}

/*
// Перегруженная версия для одиночного символа char
int altCodeChar(char ch) {
    String strCh = String(ch);
    return altCodeChar(strCh);
}
*/


// Функция для вывода информации о символе
void charInfoSimple(const String& charStr) {
  if (charStr.length() == 0) return;
  
  Serial.print("Символ '");
  Serial.print(charStr);
  Serial.print("' Alt-код = ");
  Serial.print(altCodeChar(charStr));
  Serial.print(", Байт = ");
  Serial.print(charStr.length());
  Serial.print(" [");
  
  // Выводим все байты символа
  for (size_t i = 0; i < charStr.length(); i++) {
    if (i > 0) Serial.print(" ");

    Serial.print("0x");
    Serial.print((uint8_t)charStr[i], HEX);

    Serial.print("(");
    Serial.print((int)charStr[i]);
    Serial.print(")");
  }
  
  Serial.println("] ");
}


// Разбивает UTF-8 строку на отдельные символы и выводит информацию о каждом
void stringToChar(const String& str) {
  int pos = 0; // Текущая позиция в строке (в байтах)
  int charNum = 1;
  
  while (pos < str.length()) { // Проходим по всей строке
    uint8_t firstByte = (uint8_t)str[pos]; // Читаем первый байт текущего символа
    uint8_t charLen = getUtf8CharLength(firstByte); // Определяем длину UTF-8 символа по первому байту
    
    // Проверяем, что символ корректный и полностью помещается в строку
    if (charLen > 0 && pos + charLen <= str.length()) {
      // Извлекаем полный UTF-8 символ (1-4 байта)
      String oneChar = str.substring(pos, pos + charLen);

      // Выводим информацию о символе
      Serial.print(charNum);
      Serial.print(" Первый байт символа в строке:");
      Serial.println(pos);
      charInfoSimple(oneChar);
      
      // Перемещаем указатель на следующий символ
      pos += charLen;

    } else {
      // Некорректная UTF-8 последовательность
      Serial.print("Некорректный UTF-8 по позиции ");
      Serial.println(pos);
      pos++; // Пропускаем проблемный байт и продолжаем
    }
    charNum++; // Увеличиваем счетчик символов
  }
}


void setup() {
  delay(2000);
  Serial.begin(115200);

  //const char* s1 = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~⌂АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмноп░▒▓│┤╡╢╖╕╣║╗╝╜╛┐└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀рстуфхцчшщъыьэюяЁёЄєЇїЎў°∙·√№¤■ ";
  const char* s1 = "Привет Hello ▲ ■ 🌍";

  Serial.println("=== Информация о строке ===");

  stringInfo(s1);

  Serial.println("=== Информация о символах в строке ===");

  stringToChar(s1);

  Serial.println("=== Информация о символах ===");
  
  // ASCII символы
  charInfoSimple(" ");
  charInfoSimple("!");
  charInfoSimple("@");
  charInfoSimple("Z");
  charInfoSimple("z");
  charInfoSimple("~");
  
  // Поиск русских символов
  charInfoSimple("⌂");
  charInfoSimple("А");
  charInfoSimple("Я");
  charInfoSimple("а");
  charInfoSimple("я");
  
  // Поиск псевдографики
  charInfoSimple("░");
  charInfoSimple("▒");
  charInfoSimple("█");
  charInfoSimple("▄");
  
  // Поиск специальных символов
  charInfoSimple("Ё");
  charInfoSimple("ё");
  charInfoSimple("°");
  charInfoSimple("√");
  charInfoSimple("№");
  charInfoSimple("¤");
  
  // Поиск отсутствующего символа
  charInfoSimple("▲");
  charInfoSimple("123");
  charInfoSimple("☺");
  charInfoSimple("€");
}

void loop() {
  // put your main code here, to run repeatedly:

}