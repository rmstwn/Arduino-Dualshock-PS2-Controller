#include <TimerOne.h>
#include <PS2X_lib.h>                        
#include <string.h>

PS2X ps2x;                                    /* create PS2 Controller Class*/
byte Type = 0;
byte vibrate = 0;
byte TxData[] = {0,0,127,127,127,127};
byte TxLeft;
byte TxRight;
signed int LX,LY,RX,RY;
int buttonState = 0; 

// pinout : SCK >> 13, SS >> 10, MOSI >> 11,MISO >> 12
void setup(){
   Serial.begin(115200);
   pinMode(3,INPUT);
   ps2x.config_gamepad(13,11,10,12, true, true); /* setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error*/
   Type = ps2x.readType();              

   signed int LX,LY,RX,RY;

   Timer1.initialize(10000);
   Timer1.attachInterrupt(timer);
}

void center_LX(){ if(LX>-5 && LX<5) {LX=0;} }
void center_LY(){ if(LY>-5 && LY<5) {LY=0;} }
void center_RX(){ if(RX>-5 && RX<5) {RX=0;} }
void center_RY(){ if(RY>-5 && RY<5) {RY=0;} }

void data_LX(){
  if(LX < 0 ) { Serial.write(0x2D); Serial.write(abs(LX)); }
  else { Serial.write(0x2B); Serial.write(abs(LX)); }
 }

void data_LY(){
  if(LY < 0 ) { Serial.write(0x2D); Serial.write(abs(LY)); }
  else { Serial.write(0x2B); Serial.write(abs(LY)); }
 }

void data_RX(){
  if(RX < 0 ) { Serial.write(0x2D); Serial.write(abs(RX)); }
  else { Serial.write(0x2B); Serial.write(abs(RX)); }
 }

void data_RY(){ 
  if(RY < 0 ) { Serial.write(0x2D); Serial.write(abs(RY)); }
  else { Serial.write(0x2B); Serial.write(abs(RY)); } 
  }

void timer(){
  int dleft[8]={0,0,0,0,0,0,0,0};
  int dright[8]={0,0,0,0,0,0,0,0};

  ps2x.read_gamepad(false, vibrate);   /* read controller and set large motor to spin at 'vibrate' speed */

  if (ps2x.Button(PSB_L3))        dleft[0] = 1;
  if (ps2x.Button(PSB_L2))        dleft[1] = 1;
  if (ps2x.Button(PSB_L1))        dleft[2] = 1;
  if (ps2x.Button(PSB_PAD_UP))    dleft[3] = 1;
  if (ps2x.Button(PSB_PAD_DOWN))  dleft[4] = 1;
  if (ps2x.Button(PSB_PAD_LEFT))  dleft[5] = 1;
  if (ps2x.Button(PSB_PAD_RIGHT)) dleft[6] = 1;
  if (ps2x.Button(PSB_SELECT))    dleft[7] = 1;

  if (ps2x.Button(PSB_R3))        dright[0] = 1;
  if (ps2x.Button(PSB_R2))        dright[1] = 1;
  if (ps2x.Button(PSB_R1))        dright[2] = 1;
  if (ps2x.Button(PSB_TRIANGLE))  dright[3] = 1;
  if (ps2x.Button(PSB_CROSS))     dright[4] = 1;
  if (ps2x.Button(PSB_CIRCLE))    dright[5] = 1;
  if (ps2x.Button(PSB_SQUARE))    dright[6] = 1;
  if (ps2x.Button(PSB_START))     dright[7] = 1;

  
  TxData[0] = (dleft[7] << 0) | 
              (dleft[6] << 1) | 
              (dleft[5] << 2) | 
              (dleft[4] << 3) | 
              (dleft[3] << 4) | 
              (dleft[2] << 5) | 
              (dleft[1] << 6) | 
              (dleft[0] << 7);
  TxData[1] = (dright[7] << 0) | 
              (dright[6] << 1) | 
              (dright[5] << 2) | 
              (dright[4] << 3) | 
              (dright[3] << 4) | 
              (dright[2] << 5) | 
              (dright[1] << 6) | 
              (dright[0] << 7);
              
                          
  TxData[2] = ps2x.Analog(PSS_LX);
  LX = map(TxData[2],0,255,-100,100);
  TxData[3] = ps2x.Analog(PSS_LY);
  LY = map(TxData[3],0,255,100,-100);
  TxData[4] = ps2x.Analog(PSS_RX);
  RX = map(TxData[4],0,255,-100,100);
  TxData[5] = ps2x.Analog(PSS_RY);
  RY = map(TxData[5],0,255,100,-100);
  
  TxLeft=dleft[0]*1+dleft[1]*2+dleft[2]*4+dleft[3]*8+dleft[4]*16+dleft[5]*32+dleft[6]*64+dleft[7]*128;
  TxRight=dright[0]*1+dright[1]*2+dright[2]*4+dright[3]*8+dright[4]*16+dright[5]*32+dright[6]*64+dright[7]*128;
  
  center_LX();
  center_LY();
  center_RX();
  center_RY();
  
  Serial.write(TxLeft);
  Serial.write(TxRight);
  data_LX();
  data_LY();
  data_RX();
  data_RY();
}

void loop(){

}
