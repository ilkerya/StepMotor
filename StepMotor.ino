#include <Stepper.h>

//#include <LiquidCrystal.h>
//#include <Adafruit_SSD1306.h>
//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

unsigned int timer = 0;
signed int Direction = 0;

bool AutoMode = 0;
bool AutoModeUpDown = 0;
bool AutoSecondStep = 0;
#define ON 1
#define OFF 0
signed int LeftDirection =840;
signed int RightDirection = 1408; // 2048 - 640 = 1408;

const int stepsPerRevolution = 2048;

Stepper myStepper(stepsPerRevolution, 12, 13);

const int pwmA = 3;
const int pwmB= 11;
const int brakeA = 9;
const int brakeB = 8;
const int dirA = 12;
const int dirB = 13;

int x = 0;

//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
LiquidCrystal lcd(2, A5, 4, 5, 6, 7);

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
// read the buttons   
int read_LCD_buttons()
{
 adc_key_in = analogRead(4);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 /*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
*/
 // For V1.0 comment the other threshold and use the one below:

 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   

 return btnNONE;  // when all others fail, return this...
}

void DirectionCheck(void){
  if (Direction > 2048)Direction = 0;
  if (Direction < 0)Direction = 2048; 
}

int delaylegnth = 1000;
bool EndOfAuto(){
    if(Direction == 0) AutoSecondStep = ON;   
    if( AutoSecondStep == ON){
      if(RightDirection == Direction) return ON;   
         else return OFF;
    }
    else return OFF;

}
bool AutoCheck(){
      if(AutoMode == ON){
        AutoMode = OFF;
         AutoModeUpDown = OFF; 
         AutoSecondStep = OFF; 
        return ON;
      }
      else 
      return OFF;
}

void setup() {
  Serial.begin(9600);

  pinMode(A4, INPUT); // analog

  pinMode(A5, OUTPUT); // LCD
  pinMode(2, OUTPUT); 
  //  pinMode(10, OUTPUT);   

 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("StepMotor Contrl"); // print a simple message
 lcd.print("                "); // print a simple message 
  delay(1000);
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);
  pinMode(brakeB, OUTPUT);
  digitalWrite(pwmA, HIGH); //
  digitalWrite(pwmB, HIGH); //
  digitalWrite(brakeA, LOW); //
  digitalWrite(brakeB, LOW); //

  myStepper.setSpeed(5);

   Serial.println("Starting: "); 
   Serial.println("AntiClockwise Start: ");
  /*
  // put your setup code here, to run once:
  //establish motor direction toggle pins
  pinMode(12, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(13, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  
  //establish motor brake pins
  pinMode(9, OUTPUT); //brake (disable) CH A
  pinMode(8, OUTPUT); //brake (disable) CH B
*/
}

void loop(){ 

/*
    myStepper.step(2048);
    Serial.println("AntiClockwise Finished, Clockwise Start "); 
    delay(500);
    myStepper.step(-2048);
    Serial.println("Clockwise Finished, AntiClockwise Start  ");  

*/
    timer++;

    lcd.setCursor(8,1);
    unsigned int temp = timer/100;
    lcd.print(temp);

       lcd.setCursor(13,1);
       if(AutoMode == ON){
          lcd.print("AUT");
       }
       else
        lcd.print("MAN");

     lcd_key = read_LCD_buttons();  // read the buttons

  //  lcd.setCursor(6,0);
  //  lcd.print(adc_key_in);
    
     
     lcd.setCursor(0,1);
     lcd.print("       ");
     lcd.setCursor(0,1);

      if(AutoMode == ON){
        if(!AutoModeUpDown){       
            if(LeftDirection == Direction) {
             //AutoMode = OFF;
             AutoModeUpDown = ON;         
            }
            else{        
              myStepper.step(1);         
              Direction++;
              DirectionCheck();
              lcd.print("> ");
            }     
         }
         else{      
            if(EndOfAuto()){
             
          //  if(RightDirection == Direction) {
               
             AutoMode = OFF;
             AutoModeUpDown = OFF;
             AutoSecondStep = OFF;          
            }
            else{        
              myStepper.step(-1);         
              Direction--;
             // if(Direction == 2048)Direction = 0; 
              DirectionCheck();
              lcd.print("< ");
            }            
         
         
         
         }
      }

     
      switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
         // if(AutoMode == OFF){
          if(Direction == 0){
          if(AutoMode == OFF)AutoMode = ON;
           //LeftDirection
            }
     break;
     
   case btnLEFT:


       
   
     break;
    
   case btnUP:
    if(!AutoCheck()){
     lcd.print("> ");
     myStepper.step(1);
     Direction++;
     DirectionCheck(); 
    }    
     break;
   
   case btnDOWN:
    if(!AutoCheck()){
      lcd.print("< ");
      myStepper.step(-1);
      Direction--;
      DirectionCheck(); 
    }       
     break;

   case btnSELECT:
    if(!AutoCheck()){
     lcd.print("  ");
     Direction = 0;
    }
     break;
     
     case btnNONE:
     break;
     
   }
 lcd.setCursor(2,1);
 lcd.print(Direction);
  delay(10);      
  /*
  // put your main code here, to run repeatedly:
    digitalWrite(9, LOW);  //ENABLE CH A
  digitalWrite(8, HIGH); //DISABLE CH B

  digitalWrite(12, HIGH);   //Sets direction of CH A
  analogWrite(3, 255);   //Moves CH A
  
  delay(delaylegnth);
  
  digitalWrite(9, HIGH);  //DISABLE CH A
  digitalWrite(8, LOW); //ENABLE CH B

  digitalWrite(13, LOW);   //Sets direction of CH B
  analogWrite(11, 255);   //Moves CH B
  
  delay(delaylegnth);
  
  digitalWrite(9, LOW);  //ENABLE CH A
  digitalWrite(8, HIGH); //DISABLE CH B

  digitalWrite(12, LOW);   //Sets direction of CH A
  analogWrite(3, 255);   //Moves CH A
  
  delay(delaylegnth);
    
  digitalWrite(9, HIGH);  //DISABLE CH A
  digitalWrite(8, LOW); //ENABLE CH B

  digitalWrite(13, HIGH);   //Sets direction of CH B
  analogWrite(11, 255);   //Moves CH B
  */
 // delay(delaylegnth);
  
}
