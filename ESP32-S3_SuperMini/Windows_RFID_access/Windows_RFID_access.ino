#include <Adafruit_NeoPixel.h> // Подключаем библиотеку для работы с адресными RGB светодиодами NeoPixel
// Подключаем модули
#include "Rfid.h"
#include "Access.h"
#include "Alt_keyboard.h"

// Создаем объект rgbLed для управления светодиодом
Adafruit_NeoPixel rgbLed(1, 48, NEO_GRB + NEO_KHZ800);

void inputPassword(String str)
{
    sendEsc();
    delay(500);
    printString(str);
    sendEnter();
    delay(10);
}

void setup()
{
    // Инициализация USB Keyboard
    initKeyboard();

    // Инициализация RFID
    initRFID();
    delay(20);

    // Инициализируем библиотеку NeoPixel (настраивает пин и тайминги)
    rgbLed.begin();
    rgbLed.setBrightness(10); // яркость
    delay(20);
}

void loop()
{
    delay(100);
    rgbLed.fill(0xa5ff00); //Оранжевый
    rgbLed.show(); // Отправляем данные светодиоду (обновляем его состояние)

    /* 
    Создаём переменную для UID карты
    Здесь создаётся структура:
    card.uid[]
    card.size
    */
    CardUID card;

    // Пробуем считать карту
    if (readCard(card)) // Структура передаётся в функцию
    {
        // Проверяем доступ
        AccessInfo info = checkAccess(card);

        if (!info.allowed)
        {
            // Доступ запрещен
            rgbLed.fill(0x00ff00); //Красный
            rgbLed.show(); // Обновляем состояние светодиода
            
            // Печатаем UID карты 
            inputPassword(uidToString(card));

            return;
        }

        switch (info.type)
        {
            case CARD_RED:
                rgbLed.fill(0x00ff00); //Красный
                rgbLed.show(); // Обновляем состояние светодиода
                inputPassword("Пa$$w0rД_Red");
                break;
            case CARD_GREEN:
                rgbLed.fill(0xff0000); //Зеленый
                rgbLed.show(); // Обновляем состояние светодиода
                inputPassword("Пa$$w0rД_Green");
                break;
            case CARD_BLUE:
                rgbLed.fill(0x0000ff); //Синий
                rgbLed.show(); // Обновляем состояние светодиода
                inputPassword("Пa$$w0rД_Blue");
                break;
            default:
                rgbLed.fill(0xffffff); //Белый
                rgbLed.show(); // Обновляем состояние светодиода
                inputPassword("Пa$$w0rД_none");
                break;
        }
    }
    // После задержки loop() выполнится снова и начнёт искать следующую карту
}