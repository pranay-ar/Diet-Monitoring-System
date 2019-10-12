#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "HX711.h"

#define BACKLIGHT_PIN  13
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const int pinPIR = 4;


LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7);
//LiquidCrystal_I2C lcd(0x3f, 16, 2);


volatile int valPIR = 0;
long scaleFactor = 0;

HX711 scale;

void setup() {
  Serial.begin(9600);
  pinMode(pinPIR, INPUT);
  pinMode ( BACKLIGHT_PIN, OUTPUT );
  lcd.begin(16,2); 
  lcd.setCursor(0,0);
  lcd.print("Display");
  digitalWrite ( BACKLIGHT_PIN, HIGH );
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.print("INIT Weight : ");
  scaleFactor = calculateChangeLoad();
  Serial.println(scaleFactor);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(pirCheck()) {            // check if the input is HIGH
    //digitalWrite(ledPin, HIGH);  // turn LED ON
    // we have just turned on
    lcd.clear();
    lcd.println("Motion detected!");
    Serial.println("Motion detected!");
    // We only want to print on the output change, not state
    // pirState = HIGH;
    Serial.print("Scale Factor : ");
    Serial.println(scaleFactor);
    float weight1 = calculateChangeLoad()-scaleFactor;
    weight1 = (weight1 + 1600) / (4.38 * 24);
    weight1 += 50;
    lcd.clear();
    lcd.print("Feeding time...");
    delay(12000);
    lcd.clear();
    lcd.print("Calc Calorie Intake");
    delay(3000);
    float weight2 = calculateChangeLoad()-scaleFactor;
    weight2 = (weight2 + 1600) / (4.38 * 24);
    weight2 += 50;

    lcd.clear();
    lcd.print((weight1 - weight2) * 4.2);
    lcd.print(" cal");
    Serial.println(" cal");
    delay(2000);

    lcd.clear();
    lcd.print("Calc Total Feed");
    delay(1000);
    lcd.clear();
    lcd.print(weight1 - weight2);
    lcd.print(" g");
    Serial.println(" g");
    
    
    Serial.println(weight1 - weight2);
    lcd.clear();
    lcd.print(weight1 - weight2);
    lcd.print(" g");
    Serial.println(" g");
    
  }
}

boolean pirCheck(){
  valPIR = digitalRead(pinPIR);  // read input value
  if(valPIR == HIGH)
    return true;
  else return false;
}

long calculateChangeLoad(){
  delay(200);
  if (scale.is_ready()) {
    long reading = scale.read();
    //Serial.print("HX711 reading: ");
    Serial.println(reading);
    return reading;
  }
}
