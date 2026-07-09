// Модуль RFID
// Отвечает за:
// - инициализацию MFRC522;
// - обнаружение карт;
// - чтение UID

#pragma once // Запрещает повторное включение файла
// Подключаем типы Arduino:
// String, byte и другие.
#include <Arduino.h>

// Максимальный размер UID,
// который может вернуть MFRC522.
// UID имеет 4, 7 или 10 байт.
#define RFID_MAX_UID_SIZE 10

// Структура для хранения UID карты.
// Например карта:
// 04 A7 12 BC
//храниться так:
// uid[0] = 0x04
// uid[1] = 0xA7
// uid[2] = 0x12
// uid[3] = 0xBC
// size = 4
struct CardUID
{
    // Массив байтов UID
    byte uid[RFID_MAX_UID_SIZE];
    // Количество реально используемых байтов
    byte size;
};

// Инициализация RFID-модуля.
// Вызывается один раз в setup().
void initRFID();

// Считать карту.
// Если карта найдена: возвращает true и заполняет структуру card.
// Если карты нет: возвращает false.
bool readCard(CardUID& card);

// Преобразовать UID в строку.
// Например:
// 04:A7:12:BC
String uidToString(const CardUID& card);