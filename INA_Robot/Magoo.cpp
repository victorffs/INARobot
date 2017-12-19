#include "Magoo.h"

unsigned int enc1PinA;
unsigned int enc1PinB;
unsigned int enc2PinA;
unsigned int enc2PinB;
unsigned int enc1Pos;
unsigned int enc2Pos;

char inicializa_encoder(unsigned int encoder_1a, unsigned int encoder_1b, unsigned int encoder_2a, unsigned int encoder_2b)
{
  enc1PinA = encoder_1a;  // definição dos pinos usados pelo encoder		
  enc1PinB = encoder_1b;  // definição dos pinos usados pelo encoder
  enc2PinA = encoder_2a;  // definição dos pinos usados pelo encoder
  enc2PinB = encoder_2b;  // definição dos pinos usados pelo encoder

  enc1Pos = 0;            // limpa variável de contagem
  enc1Pos = 0;            // limpa variável de contagem
  
  pinMode(enc1PinA, INPUT);           // pino configurado como entrada
  digitalWrite(enc1PinA, HIGH);       // liga resistor de pullup 
  pinMode(enc1PinB, INPUT);           // pino configurado como entrada 
  digitalWrite(enc1PinB, HIGH);       // liga resistor de pullup 
  pinMode(enc2PinA, INPUT);           // pino configurado como entrada 
  digitalWrite(enc2PinA, HIGH);       // liga resistor de pullup 
  pinMode(enc2PinB, INPUT);           // pino configurado como entrada 
  digitalWrite(enc2PinB, HIGH);       // liga resistor de pullup 

  if(enc1PinA == 19)
  {
    attachInterrupt(4, ISR_Encoder1, CHANGE);  // interrupção pino 19 encoder
  }
  else return(0);

  if(enc2PinA == 18)
  {
    attachInterrupt(5, ISR_Encoder2, CHANGE);  // interrupção pino 18 encoder
  }
  else return(0);

  return(1);
}

unsigned int retorna_posicao_encoder_1()
{
  return(enc1Pos);
}

unsigned int retorna_posicao_encoder_2()
{
  return(enc2Pos);
}

char retorna_versao()
{
  return(1);
}

void ISR_Encoder1()
{
  if (digitalRead(enc1PinA) == digitalRead(enc1PinB)) 
  {
    enc1Pos++;
    if(enc1Pos > 10000)
    {
      enc1Pos = 0;
    }
  }
  else
  {
    enc1Pos--;
    if(enc1Pos > 10000)
    {
      enc1Pos = 9999;
    }
  }
}

void ISR_Encoder2()
{
  if (digitalRead(enc2PinA) != digitalRead(enc2PinB)) 
  {
    enc2Pos++;
    if(enc2Pos > 10000)
    {
      enc2Pos = 0;
    }
  }
  else
  {
    enc2Pos--;
    if(enc2Pos > 10000)
    {
      enc2Pos = 9999;
    }
  }
}
