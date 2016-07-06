/**
FUSE Bits do PIC16F628A
Oscillator           - INTOSC: I/O on RA6/OSC2/CLOUT, I/O on RA7/OSC1/CLKIN    (Cristal oscilador interno de 4MHz)
Watchdog Timer       - Off                                                     (Watch Dog Timer desabilitado)
Power Up Timer       - Enabled                                                 (Power Up Timer habilitado)
Master Clear Enable  - Enabled                                                 (Master Clear habilitado)
Brow Out Detect      - Disabled                                                (Brown Out desabilitado)
Low Voltage Program  - Disabled                                                (Sem programação em baixa tensão)
Data EE Read Protect - Disabled                                                (Sem proteção da memória de dados)
Code Protect         - Off                                                     (Sem proteção de código)
*/

#define  S2     RA0_bit
#define  S1     RA1_bit
#define  led2   RA2_bit
#define  led1   RA3_bit

void changeCont();                    //Muda o valor do contador
void gravaEEPROM();                   //Grava o valor atual do display da EEPROM


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
/**
Página 61 do datasheet
CMCON – COMPARATOR CONFIGURATION REGISTER (ADDRESS: 01Fh)

R-0   R-0   R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
C2OUT C1OUT C2INV C1INV CIS   CM2   CM1   CM0
0     0     0     0     x     1     1     1

bit 7 C2OUT: Comparator 2 Output bit
      When C2INV = 0:
           1 = C2 VIN+ > C2 VIN-
           0 = C2 VIN+ < C2 VIN-
      When C2INV = 1:
           1 = C2 VIN+ < C2 VIN-
           0 = C2 VIN+ > C2 VIN-
bit 6 C1OUT: Comparator 1 Output bit
      When C1INV = 0:
           1 = C1 VIN+ > C1 VIN-
           0 = C1 VIN+ < C1 VIN-
      When C1INV = 1:
           1 = C1 VIN+ < C1 VIN-
           0 = C1 VIN+ > C1 VIN-
bit 5 C2INV: Comparator 2 Output Inversion bit
      1 = C2 Output inverted
      0 = C2 Output not inverted
bit 4 C1INV: Comparator 1 Output Inversion bit
      1 = C1 Output inverted
      0 = C1 Output not inverted
bit 3 CIS: Comparator Input Switch bit
      When CM<2:0>: = 001
      Then:
           1 = C1 VIN- connects to RA3
           0 = C1 VIN- connects to RA0
      When CM<2:0> = 010
      Then:
           1 = C1 VIN- connects to RA3
           C2 VIN- connects to RA2
           0 = C1 VIN- connects to RA0
           C2 VIN- connects to RA1
bit 2-0 CM<2:0>: Comparator Mode bits
        000      Comparators Reset   (POR Default Value)
        111      Comparators Off
        100      Two Independent Comparators
        010      Four Inputs Multiplexed to Two Comparators
        011      Two Common Reference Comparators
        110      Two Common Reference Comparators with Outputs
        101      One Independent Comparator
        001      Three Inputs Multiplexed to Two Comparators
*/
    CMCON = 0x07; //0b0000 0111 - desabilita comparadores

    TRISA = 0xF3; //0b1111 0011
    TRISB = 0x80; //0b1000 0000 - todo PORTB configurado como saída
    PORTA = 0x03; //0b0000 0011
    PORTB = 0x00; //0b0000 0000 - PORTB inicia em nível baixo

    cont = EEPROM_Read(0x00);

    while(1) {
        changeCont();
        
        if(!S2) 
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

void gravaEEPROM(){
   EEPROM_Write(0x00, cont);
   delay_ms(10);
   led2 = 0x01;
   delay_ms(500);
   led2 = 0x00;
}