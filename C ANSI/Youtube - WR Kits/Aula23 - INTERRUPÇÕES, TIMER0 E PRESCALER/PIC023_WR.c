/**
FUSE Bits do PIC16F628A

Oscillator           - INTOSC: I/O on RA6/OSC2/CLOUT, I/O on RA7/OSC1/CLKIN    (Cristal oscilador interno de 4MHz)
Watchdog Timer       - Off                                                     (Watch Dog Timer desabilitado)
Power Up Timer       - Enabled                                                 (Power Up Timer habilitado)
Master Clear Enable  - Disabled                                                (Master Clear desabilitado)
Brow Out Detect      - Disabled                                                (Brown Out desabilitado)
Low Voltage Program  - Disabled                                                (Sem programação em baixa tensão)
Data EE Read Protect - Disabled                                                (Sem proteção da memória de dados)
Code Protect         - Off                                                     (Sem proteção de código)
*/
int counter = 0;       //Variável de contagem auxiliar

void interrupt() {     //Vetor de interrupção , endereço 04h de memória
   if(T0IF_bit) {      //Houve o estouro do TIMER0?
      counter++;       //Incrementa o counter
      TMR0  = 0x83;    //Reinicia o registrador TMR0
      T0IF_bit = 0x00; //Limpa a flag para a próxima interrupção
   }
} //end interrupt



void main() {
/**
Página 23 do datasheet
OPTION_REG – OPTION REGISTER (ADDRESS: 81h, 181h)

R/W-1 R/W-1  R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1
RBPU  INTEDG T0CS  T0SE  PSA   PS2   PS1   PS0
1     0      0     0     0     0     1     0

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
   OPTION_REG = 0x82;                //0b1000 0010 - Desabilita os resistores de pull-up internos
                                     //Configura o prescaler para 1:8 associado ao TMR0
                                     
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
   GIE_bit    = 0x01;                //Habilita a interrupção global
   PEIE_bit   = 0x01;                //Habilita a interrupção por periféricos
   T0IE_bit   = 0x01;                //Habilita a interrupção por estouro do TMR0
   
   TMR0       = 0x83;                //Inicia a contagem em 131
   
   TRISB.RB4  = 0x00;                //Configura o RB4 como saída digital
   RB4_bit    = 0x00;                //Inicia RB4 em LOW
   
   while(1) {
      if(counter == 500) {           //1/(4MHz/4) * (prescaler 1:256 TMR0) * (2^8 - TMR0) * counter = 1us * 8 * 125 * 500 = 0,5s
         RB4_bit = ~RB4_bit;
         counter = 0;
      }
   } //end while

} //end main