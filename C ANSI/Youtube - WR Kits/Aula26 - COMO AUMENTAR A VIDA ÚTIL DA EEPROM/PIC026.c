/**
FUSE Bits do PIC16F628A
Oscillator           - INTOSC: I/O on RA6/OSC2/CLOUT, I/O on RA7/OSC1/CLKIN    (Cristal oscilador interno de 4MHz)
Watchdog Timer       - Off                                                     (Watch Dog Timer desabilitado)
Power Up Timer       - Enabled                                                 (Power Up Timer habilitado)
Master Clear Enable  - Enabled                                                 (Master Clear habilitado)
Brow Out Detect      - Disabled                                                (Brown Out desabilitado)
Low Voltage Program  - Disabled                                                (Sem programa��o em baixa tens�o)
Data EE Read Protect - Disabled                                                (Sem prote��o da mem�ria de dados)
Code Protect         - Off                                                     (Sem prote��o de c�digo)
*/

// --- Hardware Paradoxus 9 ---
#define  S2     RA0_bit
#define  S1     RA1_bit
#define  led2   RA2_bit
#define  led1   RA3_bit

#define INIT_ADRESS  0x01              //Vari�vel conter� o valor inicial do endere�o
#define MAX_ADRESS   0x7F              //Endere�o 127 de mem�ria

void changeCont();                    //Muda o valor do contador

void gravaEEPROM();                   //Grava o valor atual do display da EEPROM
void leEEPROM();                      //Fun��o para leitura da EEPROM

unsigned short cont = 0x00;           //cont incremento/decremento do display (dado a ser armazenado na EEPROM)

unsigned char catodo;                 //catodo recebe o valor de cont
unsigned char segmento[10] = {0x3f,   // display '0'
                              0x06,   // display '1'
                              0x5b,   // display '2'
                              0x4f,   // display '3'
                              0x66,   // display '4'
                              0x6d,   // display '5'
                              0x7d,   // display '6'
                              0x07,   // display '7'
                              0x7f,   // display '8'
                              0x67};  // display '9'

void main() {
    CMCON = 0x07;             //desabilita comparadores

    TRISA = 0x03;
    TRISB = 0x00;             //todo PORTB configurado como sa�da
    PORTA = 0x03;
    PORTB = 0x00;             //PORTB inicia em n�vel baixo

    leEEPROM();

    while(1) {
        changeCont();            //Muda o valor de Count

        if(!S2)                  //Se pressionar S2, grava valor atual de Count na EEPROM
           gravaEEPROM();

        catodo = segmento[cont]; //numero a ser enviado para PORTB
        PORTB = catodo;          //envia pattern para PORT B
        delay_ms(100);
    } //end while
} //end void main

void changeCont() {
   if(!S1) {
     cont++;
     led1 = 0x01;
     delay_ms(100);
     led1 = 0x00;

     if(cont > 0x09) 
        cont = 0x00;
   }
} //end changeCont

void gravaEEPROM() {
   unsigned short eeprom_adress;          //Vari�vel auxiliar local
   eeprom_adress = EEPROM_Read(0x00);     //Lemos o valor do endere�o zero e salvamos em eeprom_adress
   EEPROM_Write(eeprom_adress, cont);     //Salvamos o valor atual de cont no endere�o de n�mero "eeprom_adress"
   delay_ms(10);
   

   
   if(eeprom_adress == MAX_ADRESS)        //Teste se eeprom_adress � igual a 127
      eeprom_adress = INIT_ADRESS;
   else
      eeprom_adress++;                    //Incrementa eeprom_adress
   
   EEPROM_Write(0x00, eeprom_adress);     //Salva o valor de aux2 no endere�o 00 da eeprom
   
   delay_ms(10);

   led2 = 0x01;
   delay_ms(500);
   led2 = 0x00;
}

void leEEPROM(){
   unsigned short eeprom_adress;          //Vari�vel auxiliar local
   eeprom_adress = EEPROM_Read(0x00);     //Lemos o endere�o 0h da EEPROM e armazemos o valor em eeprom_adress

   if(eeprom_adress == 0xFF) {            //Testando eeprom_adress, EEPROM limpa? Sim...
      EEPROM_Write(0x00, INIT_ADRESS);    //For�a escrita do valor 1 no endere�o 0x00;
      delay_ms(10);                       //Tempo para escrita
   } else if(eeprom_adress == INIT_ADRESS)
      cont = EEPROM_Read(MAX_ADRESS);
   else
      cont = EEPROM_Read(eeprom_adress - 0x01);
}