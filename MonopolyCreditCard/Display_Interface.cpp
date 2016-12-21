#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void LCD_INIT() {
	lcd.begin();
	lcd.backlight();
}

void Print_Raw_One(char* RawOne, char Mode) {

}

void Print_Raw_Two(char* RawTwo, char Mode) {

}