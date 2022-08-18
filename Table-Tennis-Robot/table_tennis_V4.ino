#include <Servo.h>
Servo esc1,esc2,esc3,pitch,roll,lift;

int e1[]= {1460}; //topsin-left     right-left backspin long
int e2[]= {1570}; //backspin
int e3[]= {1460}; //topspin-right
int p[] = {50,50,50};  //pitch up(+)    down(-) (0=middle)
int r[] = {-10,-10,-10};  //roll right(+)  left(-) (0=middle) range(-40 - 0)
int t[] = {2500,2500,2200};

//int t = 1400;

unsigned long prev_t=0;
const int pitch_zero=90,roll_zero=80;
int shot_number = 0, max_shots;
boolean ball_ready=false;
char c = '0';

void setup() {
  TCCR1B = TCCR1B & B11111000 | B00000010;
  TCCR2B = TCCR2B & B11111000 | B00000010;
  Serial.begin(9600);
  pinMode(4,INPUT_PULLUP); pinMode(A1,OUTPUT); pinMode(A2,OUTPUT); pinMode(A3,OUTPUT);
  esc1.attach(9); esc2.attach(10); esc3.attach(11);
  pitch.attach(5); roll.attach(6); lift.attach(3);
  esc1.writeMicroseconds(1500); esc2.writeMicroseconds(1500); esc3.writeMicroseconds(1500);
  pitch.write(pitch_zero);
  roll.write(roll_zero);
  lift.writeMicroseconds(1500);
  max_shots = sizeof(e1) / sizeof(e1[0]);  
  delay(8000);
  lift.writeMicroseconds(2600);
  esc1.writeMicroseconds(e1[shot_number]);
  esc2.writeMicroseconds(e2[shot_number]);
  esc3.writeMicroseconds(e3[shot_number]);
  pitch.write(p[shot_number]+pitch_zero);  
  roll.write(r[shot_number]+roll_zero);
  led(e1[shot_number],e2[shot_number],e3[shot_number]);
  prev_t = millis();
  while (Serial.available()){
    char v = Serial.read();
  }
}

void loop() {
    //sets the speed to the brushless motors and postition to servos depending on the arrays above
//    if ((millis()-prev_t)>t && ball_ready == true){
    if ((millis()-prev_t)>t[shot_number] && ball_ready == true){
      lift.writeMicroseconds(2600);
      prev_t = millis();
      delay(500);
      ball_ready = false;
      if (shot_number<max_shots-1) { shot_number++;}
            else  { shot_number=0; } 
      esc1.writeMicroseconds(e1[shot_number]);
      esc2.writeMicroseconds(e2[shot_number]);
      esc3.writeMicroseconds(e3[shot_number]);
      pitch.write(p[shot_number]+pitch_zero);  
      roll.write(r[shot_number]+roll_zero);
      led(e1[shot_number],e2[shot_number],e3[shot_number]);
    }
    
    //controls the ball elevator
    //checks if a ball is available
    if (digitalRead(A0)==0 && ball_ready == false){
//    if ((millis()-prev_t)<t){
    if ((millis()-prev_t)<t[shot_number]){
      lift.writeMicroseconds(1000);
      delay(40);
      lift.writeMicroseconds(1500);
    }
      ball_ready = true;
    }


   //turns robot off and on with a switch
//  while (Serial.available()){
//    c = Serial.read();
//  }
//   if (digitalRead(4)==0 || c == '0'){
   if (digitalRead(4)==0){
      lift.writeMicroseconds(1500);
      esc1.writeMicroseconds(1500);
      esc2.writeMicroseconds(1500);
      esc3.writeMicroseconds(1500);
      led(0,0,0);
      while (digitalRead(4)==0){}
      delay(1000);
      prev_t = millis();
      esc1.writeMicroseconds(e1[shot_number]);
      esc2.writeMicroseconds(e2[shot_number]);
      esc3.writeMicroseconds(e3[shot_number]);
      lift.writeMicroseconds(2600);
      led(e1[shot_number],e2[shot_number],e3[shot_number]);
     }

}

void led(int x1, int x2, int x3){
  float d;
  if (x1==0) {digitalWrite(A1,LOW); digitalWrite(A2,LOW); digitalWrite(A3,LOW);}
  else{
  d = (x1+x3)/2;
  d = d/x2;
  if (d<1) {digitalWrite(A1,HIGH); digitalWrite(A2,LOW); digitalWrite(A3,LOW);}
  else if (d>1) {digitalWrite(A1,LOW); digitalWrite(A2,LOW); digitalWrite(A3,HIGH);}
  else {digitalWrite(A1,LOW); digitalWrite(A2,HIGH); digitalWrite(A3,LOW);}
  }
}
