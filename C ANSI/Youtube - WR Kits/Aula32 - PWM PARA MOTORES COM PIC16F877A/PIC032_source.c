/*
    Curso de PIC Aula 32
    
    PWM para Motores com PIC16F877A
    
    Clock: 16HMz
    
    Ciclo de máquina: ???
    
    Autor: Eng. Wagner Rambo
    
    Data: Junho de 2015

*/

/**
FUSE Bits do PIC16F877A
Oscillator           - HS                                                      (Cristal oscilador interno de 16MHz)
Watchdog Timer       - Off                                                     (Watch Dog Timer desabilitado)
Power Up Timer       - On                                                      (Power Up Timer habilitado)
Brow Out Detect      - Off                                                     (Brown Out desabilitado)
Low Voltage Program  - Disabled                                                (Sem programação em baixa tensão)
Data EE Read Protect - Off                                                     (Sem proteção da memória de dados)
Flash Program Write  - Write Protection Off
Background Debug     - Disabled
Code Protect         - Off                                                     (Sem proteção de código)
*/

unsigned short percent_duty = 50; //duty cycle de 50%

unsigned ADC = 0;

void main() {
/**
Página 135
CMCON REGISTER
R-0   R-0   R/W-0 R/W-0 R/W-0 R/W-1 R/W-1 R/W-1
C2OUT C1OUT C2INV C1INV CIS   CM2   CM1   CM0
0     0     0     0     0     1     1     1

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
      1 = C2 output inverted
      0 = C2 output not inverted
bit 4 C1INV: Comparator 1 Output Inversion bit
      1 = C1 output inverted
      0 = C1 output not inverted
bit 3 CIS: Comparator Input Switch bit
      When CM2:CM0 = 110:
           1 = C1 VIN- connects to RA3/AN3
           C2 VIN- connects to RA2/AN2
           0 = C1 VIN- connects to RA0/AN0
           C2 VIN- connects to RA1/AN1
bit 2 CM2:CM0: Comparator Mode bits
      000      Comparators Reset
      111      Comparators Off (POR Default Value)
      010      Two Independent Comparators
      011      Two Independent Comparators with Outputs
      100      Two Common Reference Comparators
      101      Two Common Reference Comparators with Outputs
      001      One Independent Comparator with Output
      110      Four Inputs Multiplexed to Two Comparators
*/
      CMCON =  0x07;                //0b0000 0111 - Desabilita os comparadores
      
/**
Página 127
ADCON0 REGISTER (ADDRESS 1Fh)
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0   U-0 R/W-0
ADCS1 ADCS0 CHS2  CHS1  CHS0  GO/DONE  -  ADON
0     0     0     0     0     0        x  1

bit 7-6 ADCS1:ADCS0: A/D Conversion Clock Select bits (ADCON0 bits in bold)
        ADCON1<ADCS2>  ADCON0<ADCS1:ADCS0>  Clock Conversion
        0              00                   FOSC/2
        0              01                   FOSC/8
        0              10                   FOSC/32
        0              11                   FRC (clock derived from the internal A/D RC oscillator)
        1              00                   FOSC/4
        1              01                   FOSC/16
        1              10                   FOSC/64
        1              11                   FRC (clock derived from the internal A/D RC oscillator)
bit 5-3 CHS2:CHS0: Analog Channel Select bits
        000 = Channel 0 (AN0)
        001 = Channel 1 (AN1)
        010 = Channel 2 (AN2)
        011 = Channel 3 (AN3)
        100 = Channel 4 (AN4)
        101 = Channel 5 (AN5)
        110 = Channel 6 (AN6)
        111 = Channel 7 (AN7)
bit 2 GO/DONE: A/D Conversion Status bit
      When ADON = 1:
           1 =  A/D conversion in progress (setting this bit starts the A/D conversion which is automatically
           cleared by hardware when the A/D conversion is complete)
           0 =  A/D conversion not in progress
bit 1 Unimplemented: Read as '0'
bit 0 ADON: A/D On bit
      1 = A/D converter module is powered up
      0 = A/D converter module is shut-off and consumes no operating current
*/
      ADCON0 = 0x01;                //0b0000 0001 - Liga o módulo conversor AD e habilita o canal AN0
      
/**
Página 128
ADCON1 REGISTER (ADDRESS 9Fh)
R/W-0 R/W-0 U-0 U-0 R/W-0 R/W-0 R/W-0 R/W-0
ADFM  ADCS2 -   -   PCFG3 PCFG2 PCFG1 PCFG0
0     0     x   x   1     1     1     0

bit 7 ADFM: A/D Result Format Select bit
      1 = Right justified. Six (6) Most Significant bits of ADRESH are read as '0'.
      0 = Left justified. Six (6) Least Significant bits of ADRESL are read as '0'.
bit 6 ADCS2: A/D Conversion Clock Select bit (ADCON1 bits in shaded area and in bold)
      ADCON1<ADCS2>  ADCON0<ADCS1:ADCS0>  Clock Conversion
      0              00                   FOSC/2
      0              01                   FOSC/8
      0              10                   FOSC/32
      0              11                   FRC (clock derived from the internal A/D RC oscillator)
      1              00                   FOSC/4
      1              01                   FOSC/16
      1              10                   FOSC/64
      1              11                   FRC (clock derived from the internal A/D RC oscillator)
bit 5-4 Unimplemented: Read as '0'
bit 3-0 PCFG3:PCFG0: A/D Port Configuration Control bits
        PCFG<3:0> AN7 AN6 AN5 AN4 AN3   AN2   AN1 AN0 VREF+ VREF- C/R
        0000      A   A   A   A   A     A     A   A   VDD   VSS   8/0
        0001      A   A   A   A   VREF+ A     A   A   AN3   VSS   7/1
        0010      D   D   D   A   A     A     A   A   VDD   VSS   5/0
        0011      D   D   D   A   VREF+ A     A   A   AN3   VSS   4/1
        0100      D   D   D   D   A     D     A   A   VDD   VSS   3/0
        0101      D   D   D   D   VREF+ D     A   A   AN3   VSS   2/1
        011x      D   D   D   D   D     D     D   D   -     -     0/0
        1000      A   A   A   A   VREF+ VREF- A   A   AN3   AN2   6/2
        1001      D   D   A   A   A     A     A   A   VDD   VSS   6/0
        1010      D   D   A   A   VREF+ A     A   A   AN3   VSS   5/1
        1011      D   D   A   A   VREF+ VREF- A   A   AN3   AN2   4/2
        1100      D   D   D   A   VREF+ VREF- A   A   AN3   AN2   3/2
        1101      D   D   D   D   VREF+ VREF- A   A   AN3   AN2   2/2
        1110      D   D   D   D   D     D     D   A   VDD   VSS   1/0
        1111      D   D   D   D   VREF+ VREF- D   A   AN3   AN2   1/2
*/
      ADCON1 = 0x0E;                //0b0000 1110 - Configura apenas o AN0 como analógico
      
      PWM1_Init(1000);              //Inicia o PWM1 com a frequência de 1kHz
      PWM1_Start();
      PWM1_Set_Duty((percent_duty*255)/100);
      
      while(1) {
              ADC = ADC_Read(0);
              PWM1_Set_Duty((percent_duty*255)/100);
              
              //1023 / (5v/2v) = 409,2
              if(ADC < 400) {          //Se o motor for forçado, haverá uma queda de tensão
                 percent_duty++;
                 delay_ms(50);
                 
                 if(percent_duty > 90) 
                    percent_duty = 90; //O "duty cycle" será 90% e então aumentará o torque do motor
              } else
                 percent_duty = 80;    //Quando o motor estiver funcionando normalmente, a tensão estabilizará e então o "duty cycle" será 80%
      }
}