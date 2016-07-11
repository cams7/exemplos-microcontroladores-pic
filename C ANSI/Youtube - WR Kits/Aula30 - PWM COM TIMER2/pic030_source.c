/*
        Curso de PIC WR Kits

        Aula 30 - PWM configurado a partir do Timer 2

        Eng. Wagner Rambo, Junho de 2015
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

//1/(4MHz/4) * (prescaler 1:1256 TMR0) * (2^8 - TMR0) = 1/1MHz * 256 * (256 - 0) = 1us * 256 * 256 = 65,536ms
void interrupt()
{
    if(T0IF_bit)                          //Houve estouro do Timer0?
    {                                     //Sim...
      T0IF_bit = 0x00;                    //Limpa a flag
      TMR0     = 0x00;                    //Reinicia o timer0 com o valor 0

      if(!S1)                           //S1 pressionado?
      {                                 //Sim...
       CCPR1L++;
      } //end if
      else if(!S2)                      //S2 pressionado?
      {                                 //Sim...
        CCPR1L--;
      } //end else if
    }
} //end interrupt

// --- Função Principal
void main()
{
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
     OPTION_REG = 0x87;                   //0b1000 0111 - Timer0 incrementa com ciclo de instrução, prescaler 1:256
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

     TMR0       = 0x00;                   //Inicia o timer0 com o valor 0
     
/**
Página 53 do datasheet
T2CON – TIMER2 CONTROL REGISTER (ADDRESS: 12h)
U-0 R/W-0   R/W-0   R/W-0   R/W-0   R/W-0  R/W-0   R/W-0
-   TOUTPS3 TOUTPS2 TOUTPS1 TOUTPS0 TMR2ON T2CKPS1 T2CKPS0
x   0       0       0       0       1      1       0
bit 7 Unimplemented: Read as '0'
bit 6-3 TOUTPS<3:0>: Timer2 Output Postscale Select bits
        0000 = 1:1 Postscale Value
        0001 = 1:2 Postscale Value
        0010 = 1:3 Postscale Value
        0011 = 1:4 Postscale Value
        0100 = 1:5 Postscale Value
        0101 = 1:6 Postscale Value
        0110 = 1:7 Postscale Value
        0111 = 1:8 Postscale Value
        1000 = 1:9 Postscale Value
        1001 = 1:10 Postscale Value
        1010 = 1:11 Postscale Value
        1011 = 1:12 Postscale Value
        1100 = 1:13 Postscale Value
        1101 = 1:14 Postscale Value
        1110 = 1:15 Postscale Value
        1111 = 1:16 Postscale
bit 2 TMR2ON: Timer2 On bit
      1 = Timer2 is on
      0 = Timer2 is off
bit 1-0 T2CKPS<1:0>: Timer2 Clock Prescale Select bits
        00 = 1:1 Prescaler Value
        01 = 1:4 Prescaler Value
        1x = 1:16 Prescaler Value
*/
     T2CON      = 0x06;                   //0b0000 0110 - Liga o timer2 e prescaler 1:16
     
     // periodo = (PR2 + 1) x ciclo de máquina x prescaler do Timer2
     // periodo = 256 x 1us x 16 = 4,096ms
     // frequencia = 1/periodo = 244,14Hz

     // PR2+1 = TMR2 (quando há o estouro do Timer2, low para high) / CCPR1L:CCP1CON<5:4>
     PR2        = 0xFF;                   //Inicializa o registrador de controle do timer2 em 255

/**
Página 55 do datasheet
CCP1CON – CCP OPERATION REGISTER (ADDRESS: 17h)
U-0 U-0 R/W-0 R/W-0 R/W-0  R/W-0  R/W-0  R/W-0
-   -   CCP1X CCP1Y CCP1M3 CCP1M2 CCP1M1 CCP1M0
x   x   0     0     1      1      0      0

bit 7-6 Unimplemented: Read as '0'
bit 5-4 CCP1X:CCP1Y: PWM Least Significant bits
        Capture Mode - Unused
        Compare Mode - Unused
        PWM Mode - These bits are the two LSbs of the PWM duty cycle. The eight MSbs are found in CCPRxL.
bit 3-0 CCP1M<3:0>: CCPx Mode Select bits
        0000 = Capture/Compare/PWM off (resets CCP1 module)
        0100 = Capture mode, every falling edge
        0101 = Capture mode, every rising edge
        0110 = Capture mode, every 4th rising edge
        0111 = Capture mode, every 16th rising edge
        1000 = Compare mode, set output on match (CCP1IF bit is set)
        1001 = Compare mode, clear output on match (CCP1IF bit is set)
        1010 = Compare mode, generate software interrupt on match (CCP1IF bit is set, CCP1 pin is unaffected)
        1011 = Compare mode, trigger special event (CCP1IF bit is set; CCP1 resets TMR1
        11xx = PWM mode
*/
     CCP1CON    = 0x0C;                   //0b0000 1100 - Habilita o modo PWM
     
     CCPR1L     = 0x00;                   //LED iniciará desligado

     TRISA = 0xF3;                        //0b1111 0011 - Entrada em RA0 e RA1
     TRISB = 0xF7;                        //0b1111 0111 - O RB3 será saída
     PORTA = 0x03;                        //0b0000 0011 - RA0 e RA1 iniciam em high
     PORTB = 0x00;                        //0b0000 0000 - Todo PORTB inicia em nível low

     while(1)                             //Loop Infinito
    {
         //
     } //end while

} //end main