void main()
{
     ANSEL = 0x00;
     CMCON = 0x07;//00000 0111
     TRISIO = 0xFC;//1111 1100
     GPIO = 0x00;//0000 0000
     
     while(1)
     {
      GPIO = 0b00000001;
      delay_ms(100);
      GPIO = 0b00000010;
      delay_ms(100);
     }
     
}