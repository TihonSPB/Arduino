// Модуль AltKeyboard только для windows
// Отвечает за:
// - работу с USB HID клавиатурой;
// - ввод текста через Alt-коды;
// - запуск Win+R;
// - блокировка компьютера;
// - отправку специальных клавиш.

#pragma once // Запрещает повторное включение файла
// Подключаем типы Arduino: String, byte и другие.
#include <Arduino.h>

// Инициализация USB клавиатуры + NumLock (вкл/выкл)
void initKeyboard();

// Ввод строки через Alt-коды. Пример: printString("text текст");
void printString(const String& str);

// Открывает окно "Выполнить" и вводит команду. Пример: winRun("notepad");
void winRun(const String& command);

// Блокировка компьютера;
void winLock();

// Нажать Enter
void sendEnter();

// Нажать Tab
void sendTab();

// Нажать Esc
void sendEsc();