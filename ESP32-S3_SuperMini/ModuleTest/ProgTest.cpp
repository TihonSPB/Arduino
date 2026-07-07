#include "ProgTest.h"

int multiplyByTwo(int value)
{
    Serial.print("ProgTest получил значение: ");
    Serial.println(value);

    return value * 2; // удваивает число и возвращает результат
}