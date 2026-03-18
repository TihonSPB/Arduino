// Пример из https://github.com/h4fide/ESP32-C6-RGB-LED-Control/tree/main

#include <Adafruit_NeoPixel.h> // Подключаем библиотеку для работы с адресными RGB светодиодами NeoPixel
// constexpr - константа, вычисляется на этапе компиляции (экономит память)
constexpr uint8_t LED_PIN = 8; // Номер пина 8, RGB светодиод на ESP32-C6
constexpr uint8_t NUM_LEDS = 1; // Количество подключенных светодиодов (один встроенный)
// Создаем объект rgbLed для управления светодиодом
/*
Параметры: 
          NUM_LEDS - количество светодиодов, 
          LED_PIN - пин, 
          NEO_GRB + NEO_KHZ800 - тип светодиода (цветовая схема GRB и частота 800 кГц)
*/
Adafruit_NeoPixel rgbLed(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Структура для хранения цвета в формате RGB
// Каждый компонент (красный, зеленый, синий) занимает 1 байт (0-255)
struct RGB {
    uint8_t r, g, b;
};

// Константа для выключенного состояния (все компоненты = 0)
constexpr RGB COLOR_OFF   = {0, 0, 0};
// Свой цвет {255,0,0} - красный, {0,255,0} - зеленый, {0,0,255} - синий
constexpr RGB CUSTOM_COLOR = {255, 0, 255}; // Пурпурный (красный + синий)

// Примеры дополнительных цветов
constexpr RGB COLOR_RED     = {255, 0, 0};     // Красный
constexpr RGB COLOR_GREEN   = {0, 255, 0};     // Зеленый  
constexpr RGB COLOR_BLUE    = {0, 0, 255};     // Синий
constexpr RGB COLOR_YELLOW  = {255, 255, 0};   // Желтый
constexpr RGB COLOR_CYAN    = {0, 255, 255};   // Голубой
constexpr RGB COLOR_WHITE   = {255, 255, 255}; // Белый

/*
 * Функция установки цвета с регулировкой яркости
 * color - структура RGB с желаемым цветом
 * brightness - яркость в процентах (0-100), по умолчанию 100%
 */
void setColor(const RGB& color, uint8_t brightness = 100) {
    // Переводим проценты (0-100) в шкалу 0-255
    // Используем uint16_t (16 бит) чтобы избежать переполнения при умножении
    uint16_t scale = (uint16_t)brightness * 255 / 100;
    /*
    Для каждого цветового канала применяем коэффициент яркости
    Сначала умножаем значение цвета (0-255) на scale (0-255)
    Потом делим на 255 для нормализации обратно в диапазон 0-255
    (uint16_t) нужен для временного хранения результата умножения
    */ 
    uint8_t r = (uint8_t)(((uint16_t)color.r * scale) / 255);
    uint8_t g = (uint8_t)(((uint16_t)color.g * scale) / 255);
    uint8_t b = (uint8_t)(((uint16_t)color.b * scale) / 255);
    /*
    Устанавливаем цвет для первого (и единственного) светодиода
    rgbLed.Color() собирает три компонента в одно число для библиотеки
    */
    rgbLed.setPixelColor(0, rgbLed.Color(r, g, b));
    // Отправляем данные светодиоду (обновляем его состояние)
    rgbLed.show();
}

void setup() {
    rgbLed.begin(); // Инициализируем библиотеку NeoPixel (настраивает пин и тайминги)
    // Отправляем начальные данные (все светодиоды выключены)
    // Так как мы ничего не устанавливали до этого, светодиод будет выключен
    rgbLed.show(); 
}

void loop() {
    setColor(CUSTOM_COLOR, 50); // Устанавливаем пурпурный цвет с яркостью 50%
    delay(1000);
    // Выключаем светодиод (яркость не указана, используется 100% по умолчанию,
    // но так как COLOR_OFF = {0,0,0}, яркость не имеет значения)
    setColor(COLOR_OFF);
    delay(1000);

    setColor(COLOR_RED, 50);
    delay(1000);

    setColor(COLOR_GREEN, 50);
    delay(1000);

    setColor(COLOR_BLUE, 50);
    delay(1000);

    setColor(COLOR_YELLOW, 50);
    delay(1000);

    setColor(COLOR_CYAN, 50);
    delay(1000);

    setColor(COLOR_WHITE, 50);
    delay(1000);
}
