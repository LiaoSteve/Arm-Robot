#include <Servo.h>
Servo base,tArm,dArm,claw,flag;
const int baseMax=178,baseMin=5;
const int dArmMax=178,dArmMin=5;
const int tArmMax=178,tArmMin=5;
const int clawMax=178,clawMin=5;

const double DSD=0;//the delay time of robot in order to control velocity

const int basePin=11;
const int dArmPin=6;
const int tArmPin=7;
const int clawPin=8;
int flagMax,flagMin;

int moveStep;

int counter;

int toPos;
int fromPos;

int recordArray[100][4]={
  
  };

void setup() {  
  base.attach(basePin);
  delay(200);  
  dArm.attach(dArmPin);
  delay(200);
  tArm.attach(tArmPin);
  delay(200);
  claw.attach(clawPin);
  delay(200);

  base.write(35);//起始動作
  delay(500);
  dArm.write(115);
  delay(500);
  tArm.write(140);
  delay(500);
  claw.write(80);
  delay(500);

  
  
  Serial.begin(9600);
  Serial1.begin(38400);//HC-05
  Serial.println("Arm robot is ready!");  
  
//given initial state of parameter:
  flag=tArm;
  moveStep=5;
  counter=-1;
  //toPos=70;
 // fromPos=60;
}

void loop() {
  if(Serial1.available()>0) {
    char catched=Serial1.read();    
    
    if(catched=='Q'){      
        moveStep=15;
        Serial.println("");
        Serial.print("moveStep=: ");
        Serial.println(moveStep);
     }
    else if(catched=='A'){
        moveStep=5;
        Serial.println("");
        Serial.print("moveStep=: ");
        Serial.println(moveStep);
     } 
    else if( catched=='C' || catched=='T' || catched=='D' || catched=='B'){
      Plant(catched);
      Serial.println("");
        Serial.print("Your plant is: ");
        Serial.println(catched);
     }
    else if(catched=='J'){
      toPos=flag.read()+moveStep;
      if (toPos>=flagMin && toPos<=flagMax){
           fromPos=flag.read();
           Move();
      }else {Serial.println("angle is in the critical range");}
      
    }
    else if(catched=='K'){
      toPos=flag.read()-moveStep;
      if (toPos>=flagMin && toPos<=flagMax){
           fromPos=flag.read();
           Move();
        }else {Serial.println("angle is in the critical range");}
      }
    else if(catched=='R'){
      counter++;
      Record();
     }
    else if(catched=='P'){
      RePlay();
     }  
   }
}
void Plant(char catched){//受控體            
    switch(catched){
      case 'B':           
          flag=base;
          flagMax=baseMax;
          flagMin=baseMin;                                                      
          break;
      case 'C':  
          flag=claw;  
          flagMax=clawMax;
          flagMin=clawMin;         
          break;
      case 'T': 
          flag=tArm; 
          flagMax=tArmMax;
          flagMin=tArmMin;                             
          break;
      case 'D':         
          flag=dArm;   
          flagMax=dArmMax;
          flagMin=dArmMin;        
          break;
      }         
  }
  void Move(){//驅動受控體    
          
      if (fromPos<=toPos){//角度變大
        for (int i=fromPos;i<=toPos;i++){
          flag.write(i);          
          delay(DSD);
          }
        }else {
          for (int i=fromPos;i>=toPos;i--){//角度變小
          flag.write(i);         
          delay(DSD);} 
        }  
     Serial.println(toPos);
  } 
 void Record(){
  recordArray[counter][0]=base.read();  
  recordArray[counter][1]=dArm.read();
  recordArray[counter][2]=tArm.read();
  recordArray[counter][3]=claw.read();
  ;}
 
 void RePlay(){
  for(int p=0;p<=counter;p++){      
               
     flag=base;
     fromPos =flag.read();  
     toPos=recordArray[p][0];         
     Move();
     delay(50);
     
     flag=dArm;
     fromPos =flag.read();  
     toPos=recordArray[p][1];
     Move();
     delay(50);
     
     flag=tArm;
     fromPos =flag.read();  
     toPos=recordArray[p][2];
     Move();
     delay(50);
     
     flag=claw;
     fromPos =flag.read();  
     toPos=recordArray[p][3];
     Move();  
     delay(50);  
         }  
  }
  
