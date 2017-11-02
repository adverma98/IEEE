
#include <NewPing.h>

#include <DRV8835.h>
#include <HC020K.h>
#define ENC_1_PIN 2
#define ENC_2_PIN 3

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

/**
 * EXTREMELY IMPORTANT!!!
 * 
 * When uploading code to your Arduino, make sure it is NOT getting power from the 9V
 * and it is only getting power from USB
 * Things become screwy if it gets power from both...
 */

// Object instantiation in C++ (no new keyword needed)
DRV8835 motors(7, 5, 8, 6, 4);
HC020K enc1(ENC_1_PIN);
HC020K enc2(ENC_2_PIN);



// Treat these as magic for now, for more information, look at interrupt handlers
// Also look at function pointers for more information on how to pass this to the encoder class
void counterL() {
  enc1.countTicks();
}

void counterR() {
  enc2.countTicks();
}

void setup() {
  motors.init();
  motors.setInverted(1, true);
  
  enc1.init(&counterL);
  enc2.init(&counterR);
  
  enc1.setDirection(HC020K::FORWARD);
  enc2.setDirection(HC020K::FORWARD);
  
  Serial.begin(115200);
  //Serial.begin(9600);
  Serial.print("Starting");
  delay(1000);
}

int leftSpeed = 100;
int rightSpeed=100;
float kp=5, ki=0.1,kd=.2;
float prevError=0, sum=0,error=1;
bool turned1=false,turned2=false,turned3=false;

void loop() {
  // There is probably a better way to print this using string formatting but I'm writing this pretty fast...

//  Serial.print("Step \t");
//  Serial.print(enc1.getTicks(), DEC);
//  Serial.print("encoder: ");
//  Serial.print(enc2.getTicks());

  /*Serial.print("Ping: ");
  Serial.print(sonar.ping_cm());
  Serial.println("cm");*/

  if(sonar.ping_cm()<28){
    rightTurn();   
 }
 else if(enc2.getTicks()>100&&!turned1)
 {
  Serial.print("turn 1");
  turned1=true;
  rightTurn();
  
 }
 else if (enc2.getTicks()>150&&!turned2){
  Serial.print("turn 2");
  turned2=true;
  rightTurn();
 }
 else if ((enc2.getTicks()>200&&!turned3)){
  Serial.print("turn 3");
  turned3=true;
  leftTurn();
 }
 else
 {
  rightSpeed=100;
  error=enc2.getTicks()-enc1.getTicks();
//  Serial.print("error is: ");
//  Serial.println(error);
  sum+=error;

  int out=kp*error+ki*sum+kd*(error-prevError);  
  prevError=error;
  leftSpeed=rightSpeed+out;
 }
  motors.setSpeed(0, leftSpeed);
  motors.setSpeed(1, rightSpeed);

 
  delay(100);
  /*motors.setSpeed(0, speed);
  motors.setSpeed(1, speed);
  delay(1000);
  for (int i=0; i<2; i++) {
    motors.setSpeed(i, 0);
  }
  delay(500);*/

}

void rightTurn()
{
    rightSpeed=-60;
    leftSpeed=60;
    delay(500);
    
}
void leftTurn()
{
    rightSpeed=60;
    leftSpeed=-60;
    delay(500);
    
}
