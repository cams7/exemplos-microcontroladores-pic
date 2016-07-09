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

void interrupt() {
/**
Página 26 do datasheet
PIR1 – PERIPHERAL INTERRUPT REGISTER 1 (ADDRESS: 0Ch)
R/W-0 R/W-0 R-0  R-0  U-0 R/W-0  R/W-0  R/W-0
EEIF  CMIF  RCIF TXIF -   CCP1IF TMR2IF TMR1IF
0     0     0    0    0   0      ?      0

bit 7 EEIF: EEPROM Write Operation Interrupt Flag bit
      1 = The write operation completed (must be cleared in software)
      0 = The write operation has not completed or has not been started
bit 6 CMIF: Comparator Interrupt Flag bit
      1 = Comparator output has changed
      0 = Comparator output has not changed
bit 5 RCIF: USART Receive Interrupt Flag bit
      1 = The USART receive buffer is full
      0 = The USART receive buffer is empty
bit 4 TXIF: USART Transmit Interrupt Flag bit
      1 = The USART transmit buffer is empty
      0 = The USART transmit buffer is full
bit 3 Unimplemented: Read as '0'
bit 2 CCP1IF: CCP1 Interrupt Flag bit
      Capture Mode
         1 = A TMR1 register capture occurred (must be cleared in software)
         0 = No TMR1 register capture occurred
      Compare Mode
         1 = A TMR1 register compare match occurred (must be cleared in software)
         0 = No TMR1 register compare match occurred
      PWM Mode
         Unused in this mode
bit 1 TMR2IF: TMR2 to PR2 Match Interrupt Flag bit
      1 = TMR2 to PR2 match occurred (must be cleared in software)
      0 = No TMR2 to PR2 match occurred
bit 0 TMR1IF: TMR1 Overflow Interrupt Flag bit
      1 = TMR1 register overflowed (must be cleared in software)
      0 = TMR1 register did not overflow
*/
   if(TMR2IF_bit) {              //Houve o overflow do timer2?
        PORTB = ~PORTB;         //Inverte o valor do PORTB
        TMR2IF_bit = 0x00;      //Limpa a flag
   }
}

void main() {
/**
Página 24 do datasheet
INTCON – INTERRUPT CONTROL REGISTER (ADDRESS: 0Bh, 8Bh, 10Bh, 18Bh)
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-x
GIE   PEIE  T0IE  INTE  RBIE  T0IF  INTF  RBIF
1     1     0     0     0     ?     0     x
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
     GIE_bit =  0x01;            //Habilita a interrupção global
     PEIE_bit = 0x01;            //Habilita a interrupção por periféricos

/**
Página 25 do datasheet
PIE1 – PERIPHERAL INTERRUPT ENABLE REGISTER 1 (ADDRESS: 8Ch)
R/W-0 R/W-0 R/W-0 R/W-0 U-0 R/W-0  R/W-0  R/W-0
EEIE  CMIE  RCIE  TXIE  -   CCP1IE TMR2IE TMR1IE
0     0     0     0     0   0      1      0
bit 7 EEIE: EE Write Complete Interrupt Enable Bit
      1 = Enables the EE write complete interrupt
      0 = Disables the EE write complete interrupt
bit 6 CMIE: Comparator Interrupt Enable bit
      1 = Enables the comparator interrupt
      0 = Disables the comparator interrupt
bit 5 RCIE: USART Receive Interrupt Enable bit
      1 = Enables the USART receive interrupt
      0 = Disables the USART receive interrupt
bit 4 TXIE: USART Transmit Interrupt Enable bit
      1 = Enables the USART transmit interrupt
      0 = Disables the USART transmit interrupt
bit 3 Unimplemented: Read as '0'
bit 2 CCP1IE: CCP1 Interrupt Enable bit
      1 = Enables the CCP1 interrupt
      0 = Disables the CCP1 interrupt
bit 1 TMR2IE: TMR2 to PR2 Match Interrupt Enable bit
      1 = Enables the TMR2 to PR2 match interrupt
      0 = Disables the TMR2 to PR2 match interrupt
bit 0 TMR1IE: TMR1 Overflow Interrupt Enable bit
      1 = Enables the TMR1 overflow interrupt
      0 = Disables the TMR1 overflow interrupt
*/
     TMR2IE_bit = 0x01;          //0b0000 0001 - Habilita a interrupção do timer2
     
/**
Página 53 do datasheet
T2CON – TIMER2 CONTROL REGISTER (ADDRESS: 12h)
U-0 R/W-0   R/W-0   R/W-0   R/W-0   R/W-0  R/W-0   R/W-0
-   TOUTPS3 TOUTPS2 TOUTPS1 TOUTPS0 TMR2ON T2CKPS1 T2CKPS0
0   1       1       1       1       1      1       0

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
     T2CON = 0x7E;               //0b0111 1110 - Liga o timer2, postscaler 1:16, prescaler 1:16
     
     PR2   = 0xFF;                //Compara o TMR2 com PR2, PR2 igual a 256

     TRISB = 0xFE;               //0b1111 1110
     PORTB = 0x00;               //0b0000 0000
     
     // Ciclo de máquina x PR2 x prescaler x postscaler
     // 1us x 256 x 16 x 16 =  65,536ms
     // Frequência = 1/(65,536e-3 * 2) = 7,63Hz
     
     while(1) {
          //
     }
}