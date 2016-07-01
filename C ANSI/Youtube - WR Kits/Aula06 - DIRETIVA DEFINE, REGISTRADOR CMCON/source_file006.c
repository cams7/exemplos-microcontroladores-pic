/*
* --- FUSE Bits ---
* _MCLRE_OFF & _WDT_OFF & _INTRC_OSC_NOCLKOUT
*/

#define S1 GPIO.F0
#define S2 GPIO.F1
#define D1 GPIO.F4
#define D2 GPIO.F5

void main()
{
      ANSEL = 0x00;
      CMCON = 0x07;

      TRISIO.F0 = 1;
      TRISIO.F1 = 1;
      TRISIO.F4 = 0;
      TRISIO.F5 = 0;
      
      S1 = 1;
      S2 = 1;
      D1 = 0;
      D2 = 0;
      

      while(1)
      {
        if(S1 == 0)
        {
          D1 = ~D1;
          delay_ms(300);
        }
        if(S2 == 0)
        {
          D2 = ~D2;
          delay_ms(300);
        }

      }

}