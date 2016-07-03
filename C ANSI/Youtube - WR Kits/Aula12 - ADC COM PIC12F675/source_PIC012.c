/*
*  --- FUSE Bits ---
*  Oscillator           - _INTRC_OSC_NOCLKOUT
*  Watchdog Timer       - _WDT_OFF
*  Power Up Timer       - _PWRTE_OFF
*  Master Clear Enable  - _MCLRE_OFF
*  Brown Out Detect     - _BODEN_OFF
*  Code Protect         - _CP_OFF
*  Data EE Read Protect - _CPD_OFF
*/

#define D1 GPIO.F0
#define D2 GPIO.F1
#define D3 GPIO.F4
#define D4 GPIO.F5

unsigned int leitura = 0;

void main() {
/**
*  Página 44 do datasheet
*  ANSEL - ANALOG SELECT REGISTER (ADDRESS: 9Fh)
*  - ADCS2 ADCS1 ADCS0 ANS3 ANS2 ANS1 ANS0
*  0 0     0     0     0    1    0    0
*
*  bit 7   Unimplemented: Read as '0'
*  bit 6-4 ADCS<2:0>: A/D Conversion Clock Select bits
*          000 = FOSC/2
*          001 = FOSC/8
*          010 = FOSC/32
*          x11 = FRC (clock derived from a dedicated internal oscillator = 500 kHz max)
*          100 = FOSC/4
*          101 = FOSC/16
*          110 = FOSC/64
*
*  bit 3-0 ANS3:ANS0: Analog Select bits (Between analog or digital function on pins AN<3:0>, respectively.)
*          1 = Analog input; pin is assigned as analog input
*          0 = Digital I/O; pin is assigned to port or special function
*/
   ANSEL = 0x04;  // 0b00000100 - Seleciona o AN2 como entrada analógica
   
/**
*  Página 43 do datasheet
*  ADCON0 - A/D CONTROL REGISTER (ADDRESS: 1Fh)
*  ADFM VCFG - - CHS1 CHS0 GO/DONE ADON
*  0    0    0 0 1    0    0       1
*
*  bit 7 ADFM: A/D Result Formed Select bit
*        1 = Right justified
*        0 = Left justified
*
*  bit 6 VCFG: Voltage Reference bit
*        1 = VREF pin
*        0 = VDD
*
*  bit 5-4 Unimplemented: Read as zero
*  bit 3-2 CHS1:CHS0: Analog Channel Select bits
*          00 = Channel 00 (AN0)
*          01 = Channel 01 (AN1)
*          10 = Channel 02 (AN2)
*          11 = Channel 03 (AN3)
*
*  bit 1 GO/DONE: A/D Conversion Status bit
*        1 = A/D conversion cycle in progress. Setting this bit starts an A/D conversion cycle.
*        This bit is automatically cleared by hardware when the A/D conversion has completed.
*        0 = A/D conversion completed/not in progress
*
*  bit 0 ADON: A/D Conversion STATUS bit
*        1 = A/D converter module is operating
*        0 = A/D converter is shut-off and consumes no operating current
*/
   ADCON0 = 0x09; // 0b00001001 - Seleciona a entrada AN0
   
/**
*  Página 35 do datasheet
*  CMCON - COMPARATOR CONTROL REGISTER (ADDRESS: 19h)
*  - COUT - CINV CIS CM2 CM1 CM0
*  0 0    0 0    0   1   1   1
*
*  bit 7 Unimplemented: Read as '0'
*  bit 6 COUT: Comparator Output bit
*  When CINV = 0:
*       1 = VIN+ > VIN-
*       0 = VIN+ < VIN-
*  When CINV = 1:
*       1 = VIN+ < VIN-
*       0 = VIN+ > VIN-
*
*  bit 5 Unimplemented: Read as '0'
*  bit 4 CINV: Comparator Output Inversion bit
*      1 = Output inverted
*      0 = Output not inverted
*
*  bit 3 CIS: Comparator Input Switch bit
*  When CM2:CM0 = 110 or 101:
*       1 = VIN- connects to CIN+
*       0 = VIN- connects to CIN-
*
*  bit 2-0 CM2:CM0: Comparator Mode bits
*          000      Comparator Reset (POR Default Value - low power)
*          111      Comparator Off (Lowest power)
*          010      Comparator without Output
*          100      Comparator w/o Output and with Internal Reference
*          011      Comparator with Output and Internal Reference
*          101      Multiplexed Input with Internal Reference and Output
*          001      Comparator with Output
*          110      Multiplexed Input with Internal Reference
*/
   CMCON = 0x07;   // 0b00000011 - Desabilita os comparadores internos
  
   TRISIO = 0x04; // 0b00000100 - GPIO.F0 seja uma entrada
   GPIO = 0x00;   // 0b00000000 - Inicia tudo em low
  
   while(1) {
      leitura = ADC_Read(2);
      
      if(leitura > 816) {
         D1 = 1;
         D2 = 1;
         D3 = 1;
         D4 = 1;
      } else if(leitura > 612) {
         D1 = 1;
         D2 = 1;
         D3 = 1;
         D4 = 0;
      } else if(leitura > 408) {
         D1 = 1;
         D2 = 1;
         D3 = 0;
         D4 = 0;
      } else if(leitura > 204) {
         D1 = 1;
         D2 = 0;
         D3 = 0;
         D4 = 0;
      } else {
         D1 = 0;
         D2 = 0;
         D3 = 0;
         D4 = 0;
      }
  
      delay_ms(100); //taxa de atualização do ADC
   } //end while
} //end main