#include <LiquidCrystal.h>
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int interval = 1000;
int btn     = 0;
int analogBtnRead  = 0;
unsigned long previousMillis = 0;
int timer = 0;
short powerMode = 3; // 800W, 600W, 350W, 200W
bool start = false;


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
  if (timeLeft < 600)
  {
    lcd.print("0");
    lcd.print(timeLeft / 60);
  }
  else
    lcd.print(timeLeft / 60);
  lcd.print(":");
  if (timeLeft % 60 > 9)
  {
    lcd.print(timeLeft % 60);
  }
  else
  {
    lcd.print("0");
    lcd.print(timeLeft % 60);
  }
}
void ClearCmdDisp()
{
  for (int i = 0; i < 8 ; i++)
  {
    lcd.setCursor(i, 1);
    lcd.print((char) 254);
  }
}

void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
}

void loop()
{
  unsigned long currentMillis = millis();
  DisplayPowerMode();
  DisplayTimer(timer);
  btn = ReadLcdButtons();
  switch (btn)
  {
    case btnRIGHT: // reset
      {
        lcd.setCursor(0, 1);
        lcd.print("RESET ");
        timer = 0;
        start = false;
        delay(200);
        ClearCmdDisp();
        break;
      }
    case btnLEFT: // start/stop
      {
        if (start == true)
        {
          lcd.setCursor(0, 1);
          lcd.print("PAUSE ");
          start = false;
        }
        else
        {
          lcd.setCursor(0, 1);
          lcd.print("START  ");
          start = true;
        }
        delay(200);
        ClearCmdDisp();
        break;
      }
    case btnUP: // +1min
      {
        lcd.setCursor(0, 1);
        lcd.print("+60    ");
        if (timer < 5939)
          timer += 60;
        else
          timer = 5999;
        delay(200);
        ClearCmdDisp();
        break;
      }
    case btnDOWN: // +10sec
      {
        lcd.setCursor(0, 1);
        lcd.print("+10");
        if (timer < 5989)
          timer += 10;
        else
          timer = 5999;
        delay(200);
        ClearCmdDisp();
        break;
      }
    case btnSELECT: // power mode
      {
        if (start == true)
          break;
        lcd.setCursor(0, 1);
        if (powerMode == 0)
          powerMode = 3;
        else
          powerMode--;
        delay(200);
        ClearCmdDisp();
        break;
      }
    case btnNONE:
      {
        break;
      }
  }
  if ((unsigned long)(currentMillis - previousMillis) >= interval && start == true && timer > 0)
  {
    previousMillis = currentMillis;
    timer--;
  }
  if (timer == 0 && start == true)
  {
    start = false;
    lcd.setCursor(0, 1);
    lcd.print("READY ");
  }
}
