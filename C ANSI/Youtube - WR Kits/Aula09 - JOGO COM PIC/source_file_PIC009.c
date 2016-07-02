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

#define S2   PORTA.F0
#define S1   PORTA.F1
#define led2 PORTA.F2
#define led1 PORTA.F3

int numbers;

void main() 
{
   CMCON = 0x07;     //0000 0111 - Desabilitar os comparadores
   TRISA = 0xF3;     //1111 0011 - Indica que led1 e led2 são saídas digitais
   PORTA = 0x03;     //0000 0011 - Inicializam os leds apagados

   while(1)
   {
      numbers = rand();
      
      led1 = numbers;
      
      led2 = ~led1;
      
      while(led1)  //enquanto led1 = 1
      {
          if(S1 == 0) led1 = 0;
          delay_ms(70);
      }
      
      while(led2)
      {
         if(S2 == 0) led2 = 0;
         delay_ms(70);
      }
      
      delay_ms(50);
   
   
   }


}