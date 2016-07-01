void main()
{
  ANSEL = 0x00;
  CMCON = 0x07;
  
  TRISIO = 0b11111100;
  
  while(1)
  {
    GPIO.F0 = 1;
    delay_ms(10);
    GPIO.F0 = 0;
    delay_ms(10);
    
    GPIO.F1 = 1;
    delay_ms(1);
    GPIO.F1 = 0;
    delay_ms(1);
  
  }


}