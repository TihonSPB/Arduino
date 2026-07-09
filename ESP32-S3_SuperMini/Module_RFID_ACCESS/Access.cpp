#include "Access.h"

// --------------------------------------------------
// Описание пользователя
// --------------------------------------------------
// UID карты +
// информация о владельце
struct UserCard
{
    CardUID uid;
    CardType type;
    const char* userName;
    bool needPassword;
};


// --------------------------------------------------
// База карт
// --------------------------------------------------
// Хранятся все карты.
// В будущем можно перенести:
// - в LittleFS
// - во Flash
// - в EEPROM

const UserCard users[] =
{
    {
        {{0x04, 0xA7, 0x12, 0xBC}, 4},
        CARD_RED,
        "Иван",
        false
    },

    {
        {{0x04, 0x59, 0x61, 0xA2, 0xDC, 0x4E, 0xA7}, 7},
        CARD_GREEN,
        "Петр",
        true
    },

    {
        {{0x5E, 0x9E, 0x2C, 0x07}, 4},
        CARD_BLUE,
        "Тестовая метка",
        false
    }
};


// Количество карт в массиве.
// Если добавите карту, значение пересчитается автоматически.
const int usersCount = sizeof(users) / sizeof(users[0]);


// --------------------------------------------------
// Сравнение двух UID
// --------------------------------------------------
// Возвращает:
// true  - карты одинаковые
// false - карты разные
bool compareCards(const CardUID& card1, const CardUID& card2)
{
    // Если длина UID отличается, карты точно разные.
    if (card1.size != card2.size)
    {
        return false;
    }
    // Проверяем каждый байт.
    for (byte i = 0; i < card1.size; i++)
    {
        if (card1.uid[i] != card2.uid[i])
        {
            return false;
        }
    }
    // Все байты совпали.
    return true;
}

// --------------------------------------------------
// Проверка доступа
// --------------------------------------------------
// На вход получает UID карты.
// Возвращает:
// структура AccessInfo
AccessInfo checkAccess(const CardUID& card)
{
    // Изначально считаем, что карта неизвестна.
    AccessInfo result;

    result.allowed = false;
    result.type = CARD_UNKNOWN;
    result.userName = "Unknown";
    result.needPassword = false;

    // Перебираем все карты в базе..
    for (int i = 0; i < usersCount; i++)
    {
        // Сравниваем считанную карту с картой из списка.
        if (compareCards(card, users[i].uid))
        {
            result.allowed = true;
            result.type = users[i].type;
            result.userName = users[i].userName;
            result.needPassword = users[i].needPassword;

            return result;
        }
    }
    // Если ни одна карта не подошла.
    return result;
}