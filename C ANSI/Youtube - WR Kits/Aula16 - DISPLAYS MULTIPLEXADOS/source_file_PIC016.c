/*
*  --- FUSE Bits ---
*  Oscillator           - _XT_OSC
*  Watchdog Timer       - _WDT_OFF
*  Power Up Timer       - _PWRTE_ON
*  Master Clear Enable  - _MCLRE_ON
*  Brown Out Detect     - _BOREN_OFF
*  Low Voltage Program  - _LVP_OFF
*  Data EE Read Protect - _DATA_CP_OFF
*  Code Protect         - _CP_OFF
*/

#define DIGIT1 PORTB.F7
#define DIGIT2 PORTA.F2
#define DIGIT3 PORTA.F3
#define MAIS   PORTA.F1
#define MENOS  PORTA.F0

unsigned int cont = 0;
unsigned char unidade, dezena, centena;

unsigned char numero_display(unsigned char digito);
void muda_contagem();

//Inicio programa principal
//5e-3 x 3 = 15e-3 -> 1/15e-3 = 66,67Hz
//A frequ�ncia da mudan�a de estado dos leds de 7 segmentos � 66,67Hz.
//E essa est� acima de 50Hz(percistencia m�xima da vis�o humana)
void main() {
   CMCON = 0x07;             //0b00000111 - Desabilita os comparadores
   TRISA=0xF3;               //0b11110011 - RA0 e RA1 s�o configurados como entrada
   TRISB=0x00;               //0b00000000 - Todo o PORTB foi configurado como sa�da
   DIGIT1=0;                 //Desabilita Digito1
   DIGIT2=0;                 //Desabilita Digito2
   DIGIT3=0;                 //Desabilita Digito3

   while(1) {
      muda_contagem();
      
      // 0/100 - (0%100)/100 = 0 - 0/100 = 0 - 0 = 0
      // 1/100 - (1%100)/100 = 0,01 - 1/100 = 0,01 - 0,01 = 0
      // 10/100 - (10%100)/100 = 0,1 - 10/100 = 0,1 - 0,1 = 0
      // 100/100 - (100%100)/100 = 1 - 0/100 = 1 - 0 = 1
      // 999/100 - (999%100)/100 = 9,99 - 99/100 = 9,99 - 0,99 = 9
      centena = (cont/100)-((cont%100)/100); //D�gito mais significativo ou centena
      PORTB = numero_display(centena);       //Envia para o PORTB
      DIGIT1 = 1;                            //Habilita o dig�to 1 (centena)
      delay_ms(5);                           //Aguarda 5 milisegundos
      DIGIT1 = 0;                            //Desabilita o dig�to 1 (centena)

      // 0%100 = 0
      // 10%100  = 10
      // 999%100 = 99
      dezena = (cont%100);
      // 0/10 - (0%10)/10 = 0 - 0/10 = 0
      // 10/10 - (10%10)/10 = 1 - 0/10 = 1
      // 99/10 - (99%10)/10 = 9,9 - 9/10 = 9,9 - 0,9 = 9
      dezena = (dezena/10) - ((dezena%10)/10); //Dezena
      PORTB = numero_display(dezena);          //Envia para PORTB
      DIGIT2=1;                                //Habilita o d�gito 2(dezena)
      delay_ms(5);                             //Aguarda 5 milisegundos
      DIGIT2=0;                                //Desabilita d�gito 2(dezena)
      
      // 0%10 = 0
      // 10%10 = 0
      // 999%10 = 9
      unidade=cont%10;               //D�gito menos significativo ou unidade
      PORTB=numero_display(unidade); //Envia para PORTB
      DIGIT3=1;                      //Habilita o d�gito 3(unidade)
      delay_ms(5);                   //Aguarda 5 milisegundos
      DIGIT3=0;                      //Desabilita o d�gito 3(unidade)
   }
}

//Esta fun��o retorna o byte(o n�mero do display de 7 segmentos) a ser enviado ao PORTB e recebe como parametro o n�mero que ser� exibido no display.
unsigned char numero_display(unsigned char digito) {
   unsigned char num;
   unsigned char SEGMENTO[] = {
      0x3f, //0b00111111 - n�mero 0 no display
      0x06, //0b00000110 - n�mero 1 no display
      0x5b, //0b01011011 - n�mero 2 no display
      0x4f, //0b01001111 - n�mero 3 no display
      0x66, //0b01100110 - n�mero 4 no display
      0x6d, //0b01101101 - n�mero 5 no display
      0x7d, //0b01111101 - n�mero 6 no display
      0x07, //0b00000111 - n�mero 7 no display
      0x7f, //0b01111111 - n�mero 8 no display
      0x67  //0b01100111 - n�mero 9 no display
   };
   num = SEGMENTO[digito];
   return num;
}

//Esta fun�ao mudar� o valor da v�riavel 'cont', se o bot�o MAIS ou MENOS for pressionado.
void muda_contagem() {
   while(MAIS) {
      cont++;
      delay_ms(180);
      if(cont > 999)
         cont = 0;
   }
   while(MENOS) {
      cont--;
      delay_ms(180);
      if(cont < 1)
         cont = 999;
   }
}