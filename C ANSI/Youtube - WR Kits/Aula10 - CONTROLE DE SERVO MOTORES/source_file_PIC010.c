/**
* FUSE Bits do PIC16F628A
*
* - Cristal oscilador externo 4MHz
* - Watch Dog Timer desabilitado
* - Power Up Timer habilitado
* - Master Clear habilitado
* - Brown Out desabilitado
* - Sem programação em baixa tensão
* - Sem proteção da memória de dados
* - Sem proteção de código
*
* __config _XT_OSC & _WDT_OFF & _PWRTE_ON & _MCLRE_ON & _BOREN_OFF & _LVP_OFF & DATA_CP_OFF & _CP_OFF
*/


#define S2     PORTA.F0       //hardware onboard do PDX9
#define S1     PORTA.F1
#define led2   PORTA.F2
#define led1   PORTA.F3       //...
#define servo1 PORTB.F0       //RB0 controla o servo1

//protótipos das funções de controle do servo
void servo1_horario();
void servo1_centro();
void servo1_antihorario();

void main(){
   CMCON = 0x07;         //0000 0111 - desliga comparadores do PIC
   TRISA = 0xF3;         //1111 0011 - RA0 e RA1 como entrada digital
   TRISB = 0xFE;         //1111 1110 - PORTB como saída digital
   PORTA = 0x03;         //0000 0011 - RA0 e RA1 iniciam em high
   PORTB = 0x00;         //0000 0000 - PORTB inicia todo em low
   while(1){
      led1 = ~S1;
      led2 = ~S2;
      
      //Se manter S1 solto e S2 pressionado,
      //servo move-se no sentido anti-horário
      if(S1 && !S2)
         servo1_antihorario();
      //Se manter S1 pressionado e S2 solto,
      //servo move-se no sentido anti horário
      else if(!S1 && S2)
         servo1_horario();
      //Senão...
      //...centraliza servo
      else
         servo1_centro();
   }
}

//função que faz o servo se move no sentido horário
//17,7ms + 2,3ms = 20ms
void servo1_horario(){
   servo1 = 0;
   delay_us(17700);
   servo1 = 1;
   delay_us(2300); //pulso de 2ms
}

//função que centraliza o servo
//18,5ms + 1,5ms = 20ms
void servo1_centro(){
   servo1 = 0;
   delay_us(18500);
   servo1 = 1;
   delay_us(1500); //pulso de 1,5ms
}

//função que faz o servo se move no sentido anti-horário
//19,2ms + 0,8ms = 20ms
void servo1_antihorario(){
   servo1 = 0;
   delay_us(19200);
   servo1 = 1;
   delay_us(800); //pulso de 1ms
}