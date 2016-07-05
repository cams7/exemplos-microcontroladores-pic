/**
--- FUSE Bits ---
Oscillator - HS
Watchdog Timer - Off
Power Up Timer - On
Brown Out Detect - On
Low Voltage Program - Disabled
Data EE Read Protect - Off
Flash Program Write - Write Protection Off
Background Debug - Disabled
Code Protect - Off
*/

#define  yellow  RB0_bit
#define  red     RB1_bit
#define  green   RB2_bit
#define  butt    RB7_bit

#define LF 0x0A //Alimentação de linha; mudança de linha; nova linha (inglês Line Feed)
#define CR 0x0D //Retorno do carro; retorno ao início da linha (inglês Carriage Return)

char uart_rd;
char txt[7];
long int valorAD = 0;

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
      111      Comparators Off   (POR Default Value)
      010      Two Independent Comparators
      011      Two Independent Comparators with Outputs
      100      Two Common Reference Comparators
      101      Two Common Reference Comparators with Outputs
      001      One Independent Comparator with Output
      110      Four Inputs Multiplexed to Two Comparators
*/
   CMCON = 0x07;              //0b0000 0111 - Desabilita os comparadores
/**
Página 127
ADCON0 REGISTER (ADDRESS 1Fh)
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0   U-0 R/W-0
ADCS1 ADCS0 CHS2  CHS1  CHS0  GO/DONE  -  ADON
0     0     0     0     0     0        0  1
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
   ADCON0 = 0x01;             //0b0000 0001 - Ativa o conversor AD
   
/**
Página 128
ADCON1 REGISTER (ADDRESS 9Fh)
R/W-0 R/W-0 U-0 U-0 R/W-0 R/W-0 R/W-0 R/W-0
ADFM  ADCS2 -   -   PCFG3 PCFG2 PCFG1 PCFG0
0     0     0   0   1     1     1     0
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
   ADCON1 = 0x0E;             //0b0000 1110 - Seleciona a entrada AN0

   TRISB = 0xF8;              //0b1111 1000 - RB7 como entrada digital
   PORTB = 0x00;              //0b0000 0000

   UART1_Init(9600);          // Initialize UART module at 9600 bps
   Delay_ms(100);             // Wait for UART module to stabilize

   UART1_Write_Text("Start"); // Escreve o texto Start
   UART1_Write(LF);           // Realimentação de linha
   UART1_Write(CR);           // Quebra de linha

   while (1) {                // Endless loop
      if (UART1_Data_Ready()) {  // If data is received,
         uart_rd = UART1_Read(); // read the received data,
         UART1_Write(uart_rd);   // and send data via UART

         switch(uart_rd) {
            case 'y': yellow = 0x01; break;
            case 't': yellow = 0x00; break;
            case 'r': red    = 0x01; break;
            case 'e': red    = 0x00; break;
            case 'g': green  = 0x01; break;
            case 'f': green  = 0x00; break;
         } //end switch
      } // end if

      if(butt) {
         valorAD = ADC_Read(0);        // Lê entrada analógica e atribui o valor ao valorAD
         valorAD *= 500;               // Ajuste o valor para o sensor LM35
         valorAD /= 1023;              // Ajusta o valor conforme conversor AD de 10 bits
         IntToStr(valorAD, txt);       // Converte o inteiro para string
      
         UART1_Write_Text(txt);        //Imprime o valor do ADC
         UART1_Write_Text(" Celsius");
         UART1_Write(LF);              // Realimentação de linha
         UART1_Write(CR);              // Quebra de linha
         delay_ms(100);
      }
   }
}