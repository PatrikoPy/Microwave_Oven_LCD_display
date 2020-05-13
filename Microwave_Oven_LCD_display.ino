#include <LiquidCrystal.h>
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int btn     = 0;
int analogBtnRead  = 0;
unsigned long previousMillis = 0;
int timer = 0;
short powerMode = 3; // 800W, 600W, 350W, 200W
short mode = 0;


// read the buttons
int ReadLcdButtons()
{
  analogBtnRead = analogRead(0);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (analogBtnRead > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result

  if (analogBtnRead < 50)   return btnRIGHT;
  if (analogBtnRead < 195)  return btnUP;
  if (analogBtnRead < 380)  return btnDOWN;
  if (analogBtnRead < 555)  return btnLEFT;
  if (analogBtnRead < 790)  return btnSELECT;

  return btnNONE;  // when all others fail, return this...
}
void DisplayPowerMode()
{
  lcd.setCursor(0, 0);
  lcd.print("Power: ");
  switch (powerMode)
  {
    case 0:
      {
        lcd.print("200W ");
         lcd.print((char) 255);
        lcd.print((char) 254);
        lcd.print((char) 254);
        lcd.print((char) 254);
        break;
      }
    case 1:
      {
        lcd.print("350W ");
         lcd.print((char) 255);
        lcd.print((char) 255);
        lcd.print((char) 254);
        lcd.print((char) 254);
        break;
      }
    case 2:
      {
        lcd.print("600W ");
         lcd.print((char) 255);
        lcd.print((char) 255);
        lcd.print((char) 255);
        lcd.print((char) 254);
        break;
      }
    case 3:
      {
        lcd.print("800W ");
        lcd.print((char) 255);
        lcd.print((char) 255);
        lcd.print((char) 255);
        lcd.print((char) 255);
        break;
      }
  }

}
void DisplayTimer(int timeLeft)
{
  lcd.setCursor(9, 1);
  lcd.print(timeLeft/60);
  lcd.print(":");
  if (timeLeft%60 > 9)
  {
  lcd.print(timeLeft%60);
  }
  else
  {
    lcd.print("0");
    lcd.print(timeLeft%60);
  }
  
}

void setup()
{
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0, 0);
}

void loop()
{
  unsigned long currentMillis = millis();
  DisplayPowerMode();
  lcd.setCursor(9, 1);           // move cursor to second line "1" and 9 spaces over
//  lcd.print(millis() / 1000);    // display seconds elapsed since power-up
DisplayTimer(millis()/1000);

  lcd.setCursor(0, 1);           // move to the begining of the second line
  btn = ReadLcdButtons();  // read the buttons

  switch (btn)               // depending on which button was pushed, we perform an action
  {
    case btnRIGHT: // reset
      {
        lcd.print("RESET ");
        timer = 0;
        break;
      }
    case btnLEFT: // start/stop
      {
        lcd.print("LEFT   ");
        break;
      }
    case btnUP: // +1min
      {
        lcd.print("+60    ");

        break;
      }
    case btnDOWN: // +10sec
      {
        lcd.print((char) 255);
        break;
      }
    case btnSELECT: // power mode
      {
        lcd.print("SELECT");
        if (powerMode == 0)
        powerMode = 3;
        else
        powerMode--;
        delay(200);
        break;
      }
    case btnNONE:
      {
        lcd.print("NONE  ");
        break;
      }
  }

}
