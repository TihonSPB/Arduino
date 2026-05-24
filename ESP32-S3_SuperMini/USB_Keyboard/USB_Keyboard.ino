// Скетч для эмулятора USB клавиатуры ввод команд на ESP32-S3 SuperMini

#include <USB.h>
#include <USBHIDKeyboard.h>

USBHIDKeyboard Keyboard;

void setup() {
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
  USB.begin();
  Keyboard.begin();
  
  delay(2000); 

  // Нажимаем Win+R (открывает окно "Выполнить")
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(50);
  Keyboard.releaseAll(); // Отпускаем все
  
  delay(500); // Ждем открытия окна
  
  // Вводим команду
  Keyboard.print("notepad");
  Keyboard.write(KEY_RETURN); // Нажимаем Enter

  delay(2000);
  
  Keyboard.println("Test USB keyboard ESP32-S3!");

}

void loop() {

}
