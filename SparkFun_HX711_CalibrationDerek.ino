/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.
 
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch
 
 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).
 
 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE

 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND
 
 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 
*/
// include the library code for the display:
#include <LiquidCrystal.h>
// include the library code for the load cell amplifier:
#include <HX711.h>

// initialize the HX711 library with the numbers of the interface pins
#define DOUT  12
#define CLK  11

HX711 scale(DOUT, CLK);

// initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(8,9,4,5,6,7); 

//float calibration_factor = 886600; //for lbs on the 1kg scale
float calibration_factor = 1955; //1955 for g for my 1kg max scale setup

void setup() {
    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("LCD Key Shield");
  lcd.setCursor(0,1);
  lcd.print("Press Key:");
    
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch modified by Derek.");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.set_scale();
  scale.tare();	//Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);

  lcd.clear();
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  
  float reading = 0;
  reading = scale.get_units(5), 1;//Read scaled units, average 5 readings, display 1 decimal places

  Serial.print("Reading: ");
//  Serial.print(scale.get_units(15), 1); //Read scaled units, average 15 readings, display 1 decimal places
  Serial.print(reading);
  Serial.print(" g"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" cal_fctr: ");
  Serial.print(calibration_factor);
  Serial.print(" RawData: ");
  Serial.println(scale.read());  
  Serial.println();

  lcd.setCursor(0,0);
  lcd.print(reading);
  lcd.setCursor(8,0);
  lcd.print(" Grams");

  //Read inputs for Left, Right, Up, Down and Select buttons that are connected to A0
  
  int x;
  x = analogRead (0);
  lcd.setCursor(10,1);
  if (x < 60) {
    lcd.print ("Right ");
  }
  else if (x < 200) {
    lcd.print ("Up    ");
  }
  else if (x < 400){
    lcd.print ("Down  ");
  }
  else if (x < 600){
    lcd.print ("Left  ");
  }
  else if (x < 800){
    lcd.print ("Zero");
    scale.tare();	//Reset the scale to 0
  }


  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
  }
}
