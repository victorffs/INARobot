#include "DualVNH5019MotorShield.h"
#include "Arduino.h"
#include "Magoo.h"

#define enc1PinA  19
#define enc2PinA  18
#define enc1PinB  17
#define enc2PinB  16

#define WAITING_COMMAND 0
#define MOVING 1
#define PREPARING_ROTATION 2
#define ROTATING 3

#define ID 1
 
DualVNH5019MotorShield md;

int led = 13; // Pino no qual o LED está conectado, no caso 13
int sensorR = A4; // Pino sensor da direita
int sensorL = A3; // Pino sensor da esquerda
// m1 é o motor da esquerda
// m2 é o motor da direita
unsigned int encoderR, encoderL;
float rotate90degrees;
char instruction = 'N';
int state;

void rotateRight();
void rotateLeft();

// A rotina setup é executada uma vez quando pressionado o reset:
void setup() {
  Serial.begin(9600);
  md.init();
  pinMode(led, OUTPUT);     // inicializa o pino do LED como saída.
  pinMode(sensorR,INPUT);
  pinMode(sensorL, INPUT);
  inicializa_encoder(enc1PinA, enc1PinB, enc2PinA, enc2PinB);
  //uma volta eh 24
  rotate90degrees =  20;
  state = MOVING;
}
//0 claro, 1 escuro
// A rotina loop é executada infinitas vezes até o Arduino ser desligado
void loop() {
  //Serial.print("SensorR ");
  //Serial.println(digitalRead(sensorR));
  if(state == WAITING_COMMAND){
        char id = Serial.read();
                
        while (id != ID + '0'){
          Serial.println(ID);
          delay(100);
          id = Serial.read();
        }
        if (id == ID + '0'){
          instruction = Serial.read();
          if(instruction == 'F' || instruction == 'L' || instruction == 'R')
            state = PREPARING_ROTATION;
        }
  }
  if(state == MOVING){
    if (!digitalRead(sensorR) && !digitalRead(sensorL)){
  	md.setM1Speed(0);//define a potencia do motor 1
  	md.setM2Speed(0);//define a potencia do motor 2
        Serial.println(ID);
        delay(1000);
        encoderR = retorna_posicao_encoder_1();
        encoderL = retorna_posicao_encoder_2();

        Serial.println(ID);
        state = WAITING_COMMAND;
    }
  
    else if (!digitalRead(sensorR) && digitalRead(sensorL)){
  	// acelerar motor direita
  	md.setM1Speed(-25);//define a potencia do motor 1
  	md.setM2Speed(-0);//define a potencia do moto2  
        delay(100);
    }
    else if (digitalRead(sensorR) && !
    digitalRead(sensorL)){
  	// acelera motor esquerda
  	md.setM1Speed(-0); //define a potencia do motor 1
  	md.setM2Speed(-25);//define a potencia do motor 2
        delay(100);
  
    }
    else {
          // Branco nos dois
  	// acelera os dois motores
  	md.setM1Speed(-26);//define a potencia do motor 1
  	md.setM2Speed(-28);//define a potencia do motor 2
    }
  }
  else if(state == PREPARING_ROTATION){
        md.setM1Speed(-26);
  	md.setM2Speed(-26);
        unsigned int auxL = retorna_posicao_encoder_2();
        int result = auxL - encoderL;
        if(result<0) result*=-1;
        if(result > 2){
          md.setM1Speed(0);
          md.setM2Speed(0);
          delay(1000);
          
          encoderR = retorna_posicao_encoder_1();
          encoderL = retorna_posicao_encoder_2();
        
          state = ROTATING;
        }  
  }
  else if(state == ROTATING){     
        if(instruction == 'L')   
          rotateLeft();
        else if(instruction == 'R')
          rotateRight();
        else{          
          md.setM1Speed(-26);
          md.setM2Speed(-26);
          delay(100);
          state = MOVING;
        }
  }
 }

void rotateRight(){
    md.setM1Speed(-30);//define a potencia do motor 1
    md.setM2Speed(0);//define a potencia do motor 2
    unsigned int auxL = retorna_posicao_encoder_2();
    int result = auxL - encoderL;
    if(result<0) result*=-1;
    if(result>= rotate90degrees && digitalRead(sensorR)){
      md.setM1Speed(0);//define a potencia do motor 1
      md.setM2Speed(0);//define a potencia do motor 2
      state = MOVING;
      delay(1000);
    }
}

void rotateLeft()
{
    md.setM1Speed(0);//define a potencia do motor 1
    md.setM2Speed(-30);//define a potencia do motor 2    
    unsigned int auxR = retorna_posicao_encoder_1();
    int result = auxR - encoderR;
    if(result<0) result*=-1;
    if(result >= rotate90degrees && digitalRead(sensorL)){
      md.setM1Speed(0);//define a potencia do motor 1
      md.setM2Speed(0);//define a potencia do motor 2
      state = MOVING;
      delay(1000);
    }
}
