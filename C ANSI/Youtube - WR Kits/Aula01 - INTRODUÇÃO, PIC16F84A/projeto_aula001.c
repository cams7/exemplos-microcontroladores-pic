
void main()
{
     /**
     Saída digital
     0: OUTPUT
     1: INPUT
     **/
     TRISB = 0b00000000;

     /**
     Os LEDs inicializarão apagados
     Porta 1: Current source
     Porta 2: Current sink
     **/
     //Portas: 76543210
     PORTB = 0b00000100;

     
     while(1)
     {
      PORTB = 0b00000110;
      //RB1_bit = 1;
      //RB2_bit = 1;
      delay_ms(1000);
      PORTB = 0b00000000;
      //RB1_bit = 0;
      //RB2_bit = 0;
      delay_ms(1000);
     }
}