#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
int gas=0;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd or start
  lcd.init();
  Serial.begin(9600);
  pinMode(5,OUTPUT);
  // Print a message to the LCD.
  lcd.backlight();
  
   /*lcd.setCursor(0,2);
  lcd.print("Arduino LCM IIC 2004");
   lcd.setCursor(2,3);
  lcd.print("Power By Ec-yuan!");*/
}


void loop()
{
  gas=analogRead(33);
  int per=map(gas,0,4095,0,100);
  Serial.print("gas likage:");
  Serial.println(per);
 // lcd.setCursor(3,0);
 // lcd.print("gas likage:");
 lcd.setCursor(3,0);
  lcd.print(per);
  lcd.setCursor(6,1);
  lcd.print("%");
  if(per>=20)
  {
    lcd.setCursor(0,1);
    lcd.print("LPG detected");
    digitalWrite(5,HIGH);
    Serial.println("LPG detected");

  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print("LPG not detected");
    digitalWrite(5,LOW);
    Serial.println("LPG not detected");
  }
}