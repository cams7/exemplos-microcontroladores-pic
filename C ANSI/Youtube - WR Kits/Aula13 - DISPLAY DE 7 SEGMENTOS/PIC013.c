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

#define display7_ponto PORTB.F7

void main() {
   unsigned char catodo, cont = 0x00;  //0x00
   unsigned char segmento[] = {
      0x3f, //0b00111111 - número 0 no display
      0x06, //0b00000110 - número 1 no display
      0x5b, //0b01011011 - número 2 no display
      0x4f, //0b01001111 - número 3 no display
      0x66, //0b01100110 - número 4 no display
      0x6d, //0b01101101 - número 5 no display
      0x7d, //0b01111101 - número 6 no display
      0x07, //0b00000111 - número 7 no display
      0x7f, //0b01111111 - número 8 no display
      0x67  //0b01100111 - número 9 no display
   };
 
   CMCON = 0x07; //0b00000111 - desabilita comparadores
   TRISB = 0x00; //0b00000000 - todo PORTB configurado como saída
   PORTB = 0x00; //0b00000000 - PORTB inicia em nível baixo

   while(1) {
      catodo = segmento[cont]; //numero a ser enviado para PORTB

      PORTB = catodo;          //envia pattern para PORT B
      cont++;                  //cont = cont + 1

      if(cont==10){
         cont=0;
         display7_ponto = ~display7_ponto;
      }                        //reinicia contagem
      delay_ms(1000);          //delay de 1 segundo
   } //end while
} //end void main