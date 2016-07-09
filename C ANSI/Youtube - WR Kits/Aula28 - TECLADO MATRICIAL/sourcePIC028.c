/*
        Curso de PIC WR Kits

        Aula 28 - Teclado Matricial

        Eng. Wagner Rambo, Maio de 2015
*/

/**
FUSE Bits do PIC16F628A
Oscillator           - XT                                                      (Cristal oscilador externo de 4MHz)
Watchdog Timer       - Off                                                     (Watch Dog Timer desabilitado)
Power Up Timer       - Enabled                                                 (Power Up Timer habilitado)
Master Clear Enable  - Enabled                                                 (Master Clear habilitado)
Brow Out Detect      - Disabled                                                (Brown Out desabilitado)
Low Voltage Program  - Disabled                                                (Sem programação em baixa tensão)
Data EE Read Protect - Disabled                                                (Sem proteção da memória de dados)
Code Protect         - Off                                                     (Sem proteção de código)
*/

#define TRUE 0x01
#define FALSE 0x00

// --- Hardware ---
#define COL_1   RB3_bit
#define COL_2   RB4_bit
#define COL_3   RB5_bit
#define ROW_A   RA4_bit
#define ROW_B   RB0_bit
#define ROW_C   RB1_bit
#define ROW_D   RB2_bit

#define DISPLAY1_ON RB6_bit
#define DISPLAY2_ON RB7_bit

#define INDEX_ADRESS 0x00                    //O índice será armazenado nesse endereço
#define INIT_ADRESS 0x01                     //Primeiro endereço ultilizado para armazenar os valores
#define MAX_ADRESS 0x7F                      //Endereço 127 de memória

#define DISPLAY1 0x00
#define DISPLAY2 0x01

void altera_display7(unsigned short numero); //Função que altera o número no display de 7 segmentos
void grava_eeprom();                         //Grava o valor atual do display na EEPROM
void grava_indice_eeprom();                  //Grava o endereço na EEPROM
void le_eeprom();                            //Função para leitura da EEPROM

unsigned short display_numero[] = {0x00, 0x00};

unsigned short eeprom_adress;                //Endereço na EEPROM
unsigned short grava_numero_eeprom  = FALSE;

//1/(4MHz/4) * (prescaler 1:256 TMR0) * (2^8 - TMR0) = 1/1MHz * 256 * (256 - 0) = 1us * 256 * 256 = 65,536ms
void interrupt() {
    if(T0IF_bit) {                        //Houve estouro do Timer0?
      T0IF_bit = 0x00;                    //Limpa a flag
      TMR0     = 0x00;                    //Reinicia o timer0 com o valor 6

      if(!COL_1) {                        //Coluna 1 em nível low?
         //PORTB = 0b??11 0??? & 0b1100 0111 = 0b??00 0??? | 0b0010 1000
         PORTB &= 0xC7;
         PORTB |= 0x28;                   //Apenas a coluna 2 em nível baixo
         
         if(!(ROW_A && ROW_B && ROW_C && ROW_D)) {
            display_numero[DISPLAY2] = display_numero[DISPLAY1];

            if(!ROW_A)
               display_numero[DISPLAY1] = 0x02;
            else if(!ROW_B)
               display_numero[DISPLAY1] = 0x05;
            else if(!ROW_C)
               display_numero[DISPLAY1] = 0x08;
            else
               display_numero[DISPLAY1] = 0x00;
         }
      } else if(!COL_2) {                 //Coluna 2 em nível low?
         //PORTB = 0b??10 1??? & 0b1100 0111 = 0b??00 0??? | 0b0001 1000
         PORTB &= 0xC7;
         PORTB |= 0x18;                   //Apenas a coluna 3 em nível baixo

         if(!(ROW_A && ROW_B && ROW_C)) {
            display_numero[DISPLAY2] = display_numero[DISPLAY1];
           
            if(!ROW_A)
               display_numero[DISPLAY1] = 0x03;
            else if(!ROW_B)
               display_numero[DISPLAY1] = 0x06;
            else
               display_numero[DISPLAY1] = 0x09;
         } else if(!ROW_D)
            grava_numero_eeprom  = TRUE;
      } else if(!COL_3) {                 //Coluna 3 em nível low?
         //PORTB = 0b??01 1??? & 0b1100 0111 = 0b??00 0??? | 0b0011 0000
         PORTB &= 0xC7;
         PORTB |= 0x30;                   //Apenas a coluna 1 em nível baixo

         if(!(ROW_A && ROW_B && ROW_C)) {
            display_numero[DISPLAY2] = display_numero[DISPLAY1];
           
            if(!ROW_A)
               display_numero[DISPLAY1] = 0x01;
            else if(!ROW_B)
               display_numero[DISPLAY1] = 0x04;
            else
               display_numero[DISPLAY1] = 0x07;
         } else if(!ROW_D) {
            display_numero[DISPLAY2] = 0x00;
            display_numero[DISPLAY1] = 0x00;
         }
      }
    }
} //end interrupt

// --- Função Principal
void main(){
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
     CMCON      = 0x07;                   //0b0000 0111 - Desabilita os comparadores

/**
Página 23 do datasheet
OPTION_REG – OPTION REGISTER (ADDRESS: 81h, 181h)
R/W-1 R/W-1  R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1
RBPU  INTEDG T0CS  T0SE  PSA   PS2   PS1   PS0
1     0      0     0     0     1     1     1
bit 7 RBPU: PORTB Pull-up Enable bit
      1 = PORTB pull-ups are disabled
      0 = PORTB pull-ups are enabled by individual port latch values
bit 6 INTEDG: Interrupt Edge Select bit
      1 = Interrupt on rising edge of RB0/INT pin
      0 = Interrupt on falling edge of RB0/INT pin
bit 5 T0CS: TMR0 Clock Source Select bit
      1 = Transition on RA4/T0CKI/CMP2 pin
      0 = Internal instruction cycle clock (CLKOUT)
bit 4 T0SE: TMR0 Source Edge Select bit
      1 = Increment on high-to-low transition on RA4/T0CKI/CMP2 pin
      0 = Increment on low-to-high transition on RA4/T0CKI/CMP2 pin
bit 3 PSA: Prescaler Assignment bit
      1 = Prescaler is assigned to the WDT
      0 = Prescaler is assigned to the Timer0 module
bit 2-0 PS<2:0>: Prescaler Rate Select bits
        Bit Value  TMR0 Rate  WDT Rate
        000        1 : 2      1 : 1
        001        1 : 4      1 : 2
        010        1 : 8      1 : 4
        011        1 : 16     1 : 8
        100        1 : 32     1 : 16
        101        1 : 64     1 : 32
        110        1 : 128    1 : 64
        111        1 : 256    1 : 128
*/
     OPTION_REG = 0x87;                   //0b1000 0111 - Timer0 incrementa com ciclo de instrução, prescaler 1:8
     
/**
Página 24 do datasheet
INTCON – INTERRUPT CONTROL REGISTER (ADDRESS: 0Bh, 8Bh, 10Bh, 18Bh)
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-x
GIE   PEIE  T0IE  INTE  RBIE  T0IF  INTF  RBIF
1     1     1     0     0     ?     0     x
bit 7 GIE: Global Interrupt Enable bit
      1 = Enables all un-masked interrupts
      0 = Disables all interrupts
bit 6 PEIE: Peripheral Interrupt Enable bit
      1 = Enables all un-masked peripheral interrupts
      0 = Disables all peripheral interrupts
bit 5 T0IE: TMR0 Overflow Interrupt Enable bit
      1 = Enables the TMR0 interrupt
      0 = Disables the TMR0 interrupt
bit 4 INTE: RB0/INT External Interrupt Enable bit
      1 = Enables the RB0/INT external interrupt
      0 = Disables the RB0/INT external interrupt
bit 3 RBIE: RB Port Change Interrupt Enable bit
      1 = Enables the RB port change interrupt
      0 = Disables the RB port change interrupt
bit 2 T0IF: TMR0 Overflow Interrupt Flag bit
      1 = TMR0 register has overflowed (must be cleared in software)
      0 = TMR0 register did not overflow
bit 1 INTF: RB0/INT External Interrupt Flag bit
      1 = The RB0/INT external interrupt occurred (must be cleared in software)
      0 = The RB0/INT external interrupt did not occur
bit 0 RBIF: RB Port Change Interrupt Flag bit
      1 = When at least one of the RB<7:4> pins changes state (must be cleared in software)
      0 = None of the RB<7:4> pins have changed state
*/
     GIE_bit    = 0x01;                   //Habilita interrupção global
     PEIE_bit   = 0x01;                   //Habilita interrupção por periféricos
     T0IE_bit   = 0x01;                   //Habilita interrução do Timer0

     TMR0       = 0x00;                   //Inicia o timer0 com o valor 0

     TRISA = 0xF0;                        //0b1111 0000 - Saída para o display de 7 segmentos
     TRISB = 0x07;                        //0b0000 0111 - Os 3 bits mais significativos do nibble mais significativo do PORTB será entrada
     PORTA = 0x10;                        //0b0001 0000 - O display inicializa em zero
     PORTB = 0x37;                        //0b0011 0111 - Alguns bits são iniciados em high.
     
     le_eeprom();
     
     //Loop Infinito
     //5e-3 x 2 = 10e-3 -> 1/10e-3 = 100Hz
     //A frequência da mudança de estado dos leds de 7 segmentos é 100Hz.
     //E essa está acima de 50Hz(percistencia máxima da visão humana)
     while(1) {
        if(grava_numero_eeprom  == TRUE)
           grava_eeprom();

        altera_display7(display_numero[DISPLAY1]);
        DISPLAY1_ON = TRUE;                //Habilita o 1º display
        delay_ms(5);                       //Aguarda 10 milisegundos
        DISPLAY1_ON = FALSE;               //Desabilita o 1º display

        if(grava_numero_eeprom  == TRUE){
           grava_indice_eeprom();
           grava_numero_eeprom  = FALSE;
        }
        
        altera_display7(display_numero[DISPLAY2]);
        DISPLAY2_ON = TRUE;                //Habilita o 2º display
        delay_ms(5);                       //Aguarda 10 milisegundos
        DISPLAY2_ON = FALSE;               //Desabilita o 2º display
     } //end while
} //end main

void altera_display7(unsigned short numero){
   //Zera o nibble menos significativo
   //PORTA = 0b???????? & 0b11110000 = 0b????0000
   PORTA &= 0xF0;

   //number = 0 -> number = 0b00000000 -> PORTA = 0b????0000 | 0b00000000 = 0b????0000
   //number = 1 -> number = 0b00000001 -> PORTA = 0b????0000 | 0b00000001 = 0b????0001
   //number = 2 -> number = 0b00000010 -> PORTA = 0b????0000 | 0b00000010 = 0b????0010
   //number = 3 -> number = 0b00000011 -> PORTA = 0b????0000 | 0b00000011 = 0b????0011
   //number = 4 -> number = 0b00000100 -> PORTA = 0b????0000 | 0b00000100 = 0b????0100
   //number = 5 -> number = 0b00000101 -> PORTA = 0b????0000 | 0b00000101 = 0b????0101
   //number = 6 -> number = 0b00000110 -> PORTA = 0b????0000 | 0b00000110 = 0b????0110
   //number = 7 -> number = 0b00000111 -> PORTA = 0b????0000 | 0b00000111 = 0b????0111
   //number = 8 -> number = 0b00001000 -> PORTA = 0b????0000 | 0b00001000 = 0b????1000
   //number = 9 -> number = 0b00001001 -> PORTA = 0b????0000 | 0b00001001 = 0b????1001
   PORTA |= numero;
}

void grava_eeprom() {
   unsigned short eeprom_value;
   
   //0x10 + 0 = 0 + 0 = 0
   //1x10 + 2 = 10 + 2 = 12
   //9x10 + 5 = 90 + 5 = 95
   eeprom_value = display_numero[DISPLAY2] * 10 + display_numero[DISPLAY1];
   
   eeprom_adress = EEPROM_Read(INDEX_ADRESS);     //Lemos o valor do endereço zero e salvamos em eeprom_adress
   EEPROM_Write(eeprom_adress, eeprom_value);     //Salvamos o valor atual de cont no endereço de número "eeprom_adress"
}

void grava_indice_eeprom() {
   if(eeprom_adress == MAX_ADRESS)                //Teste se eeprom_adress é igual a 127
      eeprom_adress = INIT_ADRESS;
   else
      eeprom_adress++;                            //Incrementa eeprom_adress

   EEPROM_Write(INDEX_ADRESS, eeprom_adress);     //Salva o valor de aux2 no endereço 00 da eeprom
}

void le_eeprom(){
   eeprom_adress = EEPROM_Read(INDEX_ADRESS);     //Lemos o endereço 0h da EEPROM e armazemos o valor em eeprom_adress

   if(eeprom_adress == 0xFF) {                    //Testando eeprom_adress, EEPROM limpa? Sim...
      EEPROM_Write(INDEX_ADRESS, INIT_ADRESS);    //Força escrita do valor 1 no endereço 0x00;
      delay_ms(5);                                //Tempo para escrita
   } else {
      unsigned short eeprom_value;                //Valor na EEPROM
      
      if(eeprom_adress == INIT_ADRESS)
         eeprom_value = EEPROM_Read(MAX_ADRESS);
      else
         eeprom_value = EEPROM_Read(eeprom_adress - 0x01);


      // 0/10 - (0%10)/10 = 0 - 0/10 = 0
      // 0%10 = 0
      
      // 12/10 - (12%10)/10 = 1,2 - 2/10 = 1,2 - 0,2 = 1
      // 12%10 = 2
      
      // 95/10 - (95%10)/10 = 9,5 - 5/10 = 9,5 - 0,5 = 9
      // 95%10 = 5
      display_numero[DISPLAY2] = (eeprom_value/10) - ((eeprom_value%10)/10);
      display_numero[DISPLAY1] = eeprom_value%10;
   }
}