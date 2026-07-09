// Модуль контроля доступа
// Отвечает за:
// - хранение разрешённых карт;
// - проверку UID;
// - принятие решения о доступе.

#pragma once // Запрещает повторное включение файла
// Подключаем типы Arduino:
// String, byte и другие.
#include <Arduino.h>

// Подключаем структуру CardUID, которая объявлена в Rfid.h
#include "Rfid.h"

// Перечислены все возможные типы карт.
// В будущем можно добавить:
// CARD_ADMIN, CARD_SERVICE, CARD_GUEST, и т.д.
enum CardType
{
    CARD_UNKNOWN,
    CARD_RED,
    CARD_GREEN,
    CARD_BLUE
};

// Структура информации о карте
// Возвращается функцией checkAccess().
struct AccessInfo
{
    bool allowed; // - есть ли доступ
    CardType type; // - тип карты
    const char* userName; // - имя владельца
    bool needPassword; // - нужен ли пароль
};

// Проверка доступа по UID карты.
//
// На вход:
//     card - считанная карта.
//
// Возвращает:
//     структура AccessInfo
//
// В будущем здесь можно добавить:
// - журнал событий;
// - работу с базой данных.
AccessInfo checkAccess(const CardUID& card);