// Скетч для проверки диода на ESP32-S3 SuperMini


#include <Adafruit_NeoPixel.h> // Подключаем библиотеку для работы с адресными RGB светодиодами NeoPixel

// Создаем объект rgbLed для управления светодиодом
/*
Параметры: 
          1 - количество светодиодов (один встроенный), 
          48 - пин RGB светодиод на ESP32-S3 SuperMini, 
          NEO_GRB + NEO_KHZ800 - тип светодиода (цветовая схема GRB и частота 800 кГц)
*/
Adafruit_NeoPixel rgbLed(1, 48, NEO_GRB + NEO_KHZ800);

void setup() {
    rgbLed.begin(); // Инициализируем библиотеку NeoPixel (настраивает пин и тайминги)
    rgbLed.setBrightness(20); // яркость
    delay(1000);
}

void loop() {
    
    rgbLed.fill(0x00ff00); //Красный
    rgbLed.show(); // Отправляем данные светодиоду (обновляем его состояние)
    delay(500);

    rgbLed.fill(0xa5ff00); //Оранжевый
    rgbLed.show();
    delay(500);

    rgbLed.fill(0xffff00); //Желтый
    rgbLed.show();
    delay(500);

    rgbLed.fill(0xff0000); //Зеленый
    rgbLed.show();
    delay(500);

    rgbLed.fill(0xbf00ff); //Голубой
    rgbLed.show();
    delay(500);

    rgbLed.fill(0x0000ff); //Синий
    rgbLed.show();
    delay(500);

    rgbLed.fill(0x004b82); //Фиолетовый
    rgbLed.show();
    delay(500);

    rgbLed.fill(0x82eeee); //Розовый
    rgbLed.show();
    delay(500);

    rgbLed.fill(0xffffff); //Белый
    rgbLed.show();
    delay(500);

    rgbLed.fill(0x000000); //Выключен
    rgbLed.show();
    delay(500);
    
}