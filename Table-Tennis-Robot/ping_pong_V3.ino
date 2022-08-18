#include <Servo.h>
Servo esc1,esc2,shot,pitch,elev,stoper;

int e1[]= {1053,1000,1057}; //backspin
int e2[]= {1000,1065,1025}; //topspin
int p[] = {30,30,30};     //pitch
int t[] = {900,900,900}; //time

int val,esc,shot_mode=1,state=1,leng,shot_number=0;
unsigned long prev_t=0,elev_t=0;
void setup() {
  pinMode(A0,INPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(2,OUTPUT);
  Serial.begin(9600);
  if (analogRead(A0)>500){esc1.attach(4); esc2.attach(6); shot.attach(10); pitch.attach(9); }
  else { esc1.attach(5); esc2.attach(5); shot.attach(5); pitch.attach(5); }
  elev.attach(3);
  
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000); // 1000-1250
  shot.write(74);
  pitch.write(30);
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
   if (millis()-prev_t>t[shot_number] && shot_mode==0) {
   shot.write(120);
   if (shot_number<leng-1) { shot_number++;}
            else  { shot_number=0; }; 
   shot_mode=1;
   prev_t=millis();
  }
  
  //asteapta
   if (millis()-prev_t>400 && shot_mode==1) {
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
