/*
Схема 
RFID ---> UID ---> ACCESS

Позже можно добавить
RFID ----+
         |
         +--> ACCESS
         |
WEB -----+
         |
KEYPAD --+
*/

// Подключаем модули
#include "Rfid.h"
#include "Access.h"

void setup()
{
    Serial.begin(115200);
    // Инициализация RFID
    initRFID();
}

void loop()
{
    delay(10);

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
        Serial.print("UID: ");
        Serial.println(uidToString(card));

        // Проверяем доступ
        AccessInfo info = checkAccess(card);

        if (!info.allowed)
        {
            Serial.println("Доступ запрещен");
            return;
        }

        switch (info.type)
        {
            case CARD_RED:
                Serial.println("Красная карта");
                break;
            case CARD_GREEN:
                Serial.println("Зеленая карта");
                break;
            case CARD_BLUE:
                Serial.println("Синяя карта");
                break;
            default:
                Serial.println("Неизвестная карта");
                break;
        }

        Serial.print("Пользователь: ");
        Serial.println(info.userName);

        if (info.needPassword)
        {
            Serial.println("Требуется пароль");
        }

        Serial.println("Доступ разрешен");

        Serial.println();
    }
    delay(1000);
    // После задержки loop() выполнится снова и начнёт искать следующую карту
}