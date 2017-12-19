#ifndef MAGOO_H
#define MAGOO_H

#include <Arduino.h>

char inicializa_encoder(unsigned int encoder_1a, unsigned int encoder_1b, unsigned int encoder_2a, unsigned int encoder_2b);
unsigned int retorna_posicao_encoder_1();
unsigned int retorna_posicao_encoder_2();
char retorna_versao();
void ISR_Encoder1();
void ISR_Encoder2();

#endif

