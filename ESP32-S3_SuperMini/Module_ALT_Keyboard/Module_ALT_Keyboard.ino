#include "Alt_keyboard.h"

void setup()
{
  initKeyboard();
  delay(500);
  winRun("notepad");
  printString("Пa$$w0rД");
  sendEnter();
  printString("Выключи num lock, для проверки (5сек)");
  sendEnter();
  delay(5000);
  printString("Должен включиться и напичатоть VOT eto.");
  sendEnter();
}

void loop()
{
  
}