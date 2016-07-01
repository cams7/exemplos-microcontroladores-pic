
/*
* --- FUSE Bits ---
* Cristal oscilador externo 4MHz, sem watchdog timer, com power up timer, sem proteção do código
* __config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF
*/

void main() 
{
//       RB  76543210
   TRISB = 0b00000000;
   PORTB = 0b00000000;
   
   while(1)
   {
     PORTB++;  //PORTB = PORTB + 1
     delay_ms(50);
   }


}