#define S1   PORTA.F1
#define S2   PORTA.F0
#define led1 PORTA.F3
#define led2 PORTA.F2


void main()
{
  CMCON = 0x07;      //0000 0111
  TRISA = 0xF3;      //1111 0011
  PORTA = 0x03;      //0000 0011
  
  while(1)
  {
    if(S1 == 0)
    {
      while(1) //programa 1
      {
        led1 = 1;
        led2 = 0;
        delay_ms(500);
        led1 = 0;
        led2 = 1;
        delay_ms(500);
      }
    }
    if(S2 == 0)
    {
      while(1) //programa 2
      {
        led1 = 1;
        led2 = 1;
        delay_ms(500);
        led1 = 0;
        led2 = 0;
        delay_ms(500);
      }
    }
  
  
  }   //end loop principal


}   //end main