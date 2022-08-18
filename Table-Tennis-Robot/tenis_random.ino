#include <Servo.h>
Servo esc1,esc2,shot,pitch,elev,stoper;

int e1[]= {1040,1024,1065,1080,1050,1000,1000,1000,1000,1000}; //backspin
int e2[]= {1040,1065,1024,1000,1023,1050,1060,1060,1060,1060}; //topspin
int p[] = {45  ,35  ,30  ,30  ,30  ,30  ,30  ,30  ,30  ,30  }; //pitch
int t[] = {800 ,1200 ,800,1200,800 ,1200,800 ,800 ,800 ,800 }; //timez

int val,esc,shot_mode=1,state=1,leng,shot_number=0;
unsigned long prev_t=0,elev_t=0;
void setup() {
  pinMode(A0,INPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(2,OUTPUT);
  randomSeed(analogRead(A6));
  Serial.begin(9600);
  esc1.attach(4);
  esc2.attach(6);
  shot.attach(10);
  pitch.attach(9);
  elev.attach(3);
  stoper.attach(11);
  
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000); // 1000-1250
  shot.write(74);
  pitch.write(35);
  elev.write(180);
  stoper.write(10);
  delay(8000);
  leng = sizeof(e1) / sizeof(e1[0]);
  esc1.writeMicroseconds(e1[0]);
  esc2.writeMicroseconds(e2[0]);
  delay(1000);
}

void loop() {

   //impusca
   if (millis()-prev_t>400 && shot_mode==0) {
   shot.write(120);
   shot_number = random(0, leng);
   shot_mode=1;
   prev_t=millis();
  }
  
  //asteapta
   if (millis()-prev_t>t[shot_number] && shot_mode==1) {
   shot.write(74);   
   shot_mode=0;
   esc1.writeMicroseconds(e1[shot_number]);
   esc2.writeMicroseconds(e2[shot_number]);
   pitch.write(p[shot_number]);  
   prev_t=millis();
  }
  
  elevator();
}


void elevator(){ 
 if (micros()-elev_t>150){
  digitalWrite(2,HIGH);
  delayMicroseconds(600);
  digitalWrite(2,LOW);
  elev_t = micros();
 }
}
