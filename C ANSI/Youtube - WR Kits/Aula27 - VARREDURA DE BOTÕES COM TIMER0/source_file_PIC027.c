/*
        Curso de PIC WR Kits
        
        Aula 27 - Varredura de botões com Timer0
        
        Eng. Wagner Rambo, Maio de 2015
*/

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

// --- Hardware ---
#define S2      RA0_bit
#define S1      RA1_bit
#define led2    RA2_bit
#define led1    RA3_bit

//1/(4MHz/4) * (prescaler 1:128 TMR0) * (2^8 - TMR0) = 1us * 128 * 148 = 18,944ms
void interrupt() {
    if(T0IF_bit) {                        //Houve estouro do Timer0? Sim...
      T0IF_bit = 0x00;                    //Limpa a flag
      TMR0     = 0x6C;                    //Reinicia o timer0 com 108
      
      if(!S1) {                           //S1 pressionado? Sim...
           led1 = 0x01;                   //Acende led1
      } else if(!S2) {                      //S2 pressionado? Sim...
           led1 = 0x00;                   //Apaga led1
      } //end else if
    }
} //end interrupt


// --- Função Principal
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
     CMCON      = 0x07;                   //0b0000 0111 - Desabilita os comparadores
     
/**
Página 23 do datasheet
OPTION_REG – OPTION REGISTER (ADDRESS: 81h, 181h)
R/W-1 R/W-1  R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1
RBPU  INTEDG T0CS  T0SE  PSA   PS2   PS1   PS0
1     0      0     0     0     1     1     0
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
     OPTION_REG = 0x86;                   //0b1000 0110 - Timer0 incrementa com ciclo de instrução, prescaler 1:128
     
/**
Página 24 do datasheet
INTCON – INTERRUPT CONTROL REGISTER (ADDRESS: 0Bh, 8Bh, 10Bh, 18Bh)
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-x
GIE   PEIE  T0IE  INTE  RBIE  T0IF  INTF  RBIF
1     1     1     0     0     0     0     x
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
     
     TMR0       = 0x6C;                   //Inicia o timer0 com 108

     TRISA = 0xF3;                        //0b1111 0011 - Entrada em RA0 e RA1
     PORTA = 0x03;                        //0b0000 0011 - RA0 e RA1 iniciam em high

     while(1) {                           //Loop Infinito
        led2 = 0x00;                      //led2 apagado
        delay_ms(500);                    //aguarda meio segundo
        led2 = 0x01;                      //led2 aceso
        delay_ms(500);                    //aguarda meio segundo
     } //end while
} //end main