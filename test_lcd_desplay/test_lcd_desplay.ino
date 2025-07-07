#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 (or 0x3f, etc.) for a 16x2 LCD. Check your LCD module's documentation.
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address and dimensions if needed

void setup() {
  // Initialize the LCD
  lcd.init();
  // Turn on the backlight
  lcd.backlight();
}

void loop() {
  // Clear the display
  lcd.clear();
  // Set the cursor to the first column and first row
  lcd.setCursor(0, 0);
  // Print the first line

  const char* text = "Hello, World !!";
  
  lcd.print(text);
  // Set the cursor to the first column of the second row
  lcd.setCursor(0, 1);
  // Print the second line
  lcd.print("From Arduino");
  delay(2000); // Wait for 2 seconds
}
