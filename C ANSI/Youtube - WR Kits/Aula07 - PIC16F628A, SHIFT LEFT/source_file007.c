
unsigned int control = 0xFF;//1111 1111

void main()
{
  TRISB = 0x00;//0000 0000
  PORTB = 0xFF;//1111 1111
  
  while(1)
  {
     PORTB = control;
     delay_ms(300);

     control <<= 1;
     
     if(control == 0x00)
      control = 0xFF;
  }
}