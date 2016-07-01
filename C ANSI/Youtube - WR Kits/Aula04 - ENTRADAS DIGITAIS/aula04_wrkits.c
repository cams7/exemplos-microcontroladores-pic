void main()
{
     ANSEL = 0x00;      //0000 0000 - Desliga as entradas AD
     CMCON = 0x07;      //0000 0111 - Desliga os comparadores

     TRISIO.F4 = 1;     //Define o pino GP0 como entrada digital
     TRISIO.F5 = 0;     //Define o pino GP1 como saída digital

     GPIO = 0x00;       //0000 0000 - Coloca todos o pinos em nível lógico baixo

     while(1)
     {
       if(GPIO.F4)
       {
        GPIO.F5 = 1;
        delay_ms(5000);
       }
       else
        GPIO.F5 = 0;
     }

}