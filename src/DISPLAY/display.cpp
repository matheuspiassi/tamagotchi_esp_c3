#include "display.h"
constexpr uint8_t OLED_ADDR = 0x3C;
Adafruit_SH1106G display(128, 64, &Wire, -1);

void setupDisplay()
{
    Wire.begin(I2C_Pins::SDA, I2C_Pins::SCL);
    Wire.setClock(400000);

    delay(250);

    if (!display.begin(OLED_ADDR, true)){
        Serial.println("Display não encontrado");
        while (true) delay(1000);
    }
    display.clearDisplay();
    
}
