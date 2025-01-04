#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define IRout 5 //attach pin D5 Arduino to out pin of infrared sensor module
#define servo 9 //attach pin D9 Arduino to signal pin of servo
#define green 6
#define red 7

/* 
 *  LCD Screen with I2C module with Arduino UNO
 *  LCD   -->   Arduino UNO
 *  Vcc   -->   Vcc
 *  GND   -->   GND
 *  SDA   -->   Analog Pin 4
 *  SCL   -->   Analog Pin 5
*/

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int percentage = 0, close_lid;
String msg;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(IRout, INPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  myservo.attach(servo);
  myservo.write(90);
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed  
  lcd.begin();
  lcd.backlight();
}

void loop() {
  close_lid = digitalRead(IRout);
  //myservo.write(90);
  if(close_lid == 0 && percentage < 70)
  {
    //open lid for 6 seconds and after closing the lid check the dustbin percentage here only
    //Rotate servo 90 degrees to open the lid
    myservo.write(0);
    delay(6000);
    //close the lid
    myservo.write(90);
    delay(1000); 
    
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    percentage = map(distance, 0, 21, 100, 0);
  }
  else
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    percentage = map(distance, 0, 21, 100, 0);
  }
  
  if(percentage >= 70)//Threshold after which dustbin won't open until emptied
  {
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
  }
  else
  {
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
  }

  /*
   * This Arduino UNO is connected to nodeMCU using the default pins as TX AND RX
   * This establishes serial communication between the two devices.
   * Arduino UNO            <==>    nodeMCU
   * Digital Pin 1(TX)        =>      D1(RX)  
   * Digital Pin 0(RX)        <=      D2(TX)
  */
  
  // Displays the percentage on the Serial Monitor
  Serial.println(percentage);
  msg = String(percentage, DEC);
  
  //Displays the percentage on the LCD screen
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("DUSTBIN ID 040");
  lcd.setCursor(4,1);
  lcd.print(msg);
  lcd.print("%");
  lcd.print("FULL");
  
  delay(1000);
}
