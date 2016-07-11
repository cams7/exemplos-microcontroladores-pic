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

#define butt1 RB1_bit
#define butt2 RB2_bit
#define butt3 RB3_bit
#define butt4 RB4_bit


unsigned short duty1, duty2;

void main()
{
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
     CMCON = 0x07;              //0b0000 0111 - Desabilitar os comparadores do PIC
     
     TRISB = 0xFF;              //0b1111 1111 - Todo PORTB será entrada
     TRISC = 0xF9;              //0b1111 1001 - Todo PORTC como saída
     PORTB = 0x1E;              //0b0001 1110 - PORTB inicia em high
     PORTC = 0x00;              //0b0000 0000 - PORTC inicia em low
     
     PWM1_Init(5000);           //p = 1/5000 = 200us
     PWM2_Init(1000);            //p = 1/1000 = 1ms
     
     duty1 = 0x7F;              //Inicia em 127
     duty2 = 0x7F;
     
     PWM1_Start();
     PWM2_Start();
     
     PWM1_Set_Duty(duty1);
     PWM2_Set_Duty(duty2);
     
     while(1)
     {
         if(!butt1)
         {
            delay_ms(40);
            duty1++;
            PWM1_Set_Duty(duty1);
         }else if(!butt2)
         {
            delay_ms(40);
            duty1--;
            PWM1_Set_Duty(duty1);
         }
         
         if(!butt3)
         {
            delay_ms(40);
            duty2++;
            PWM2_Set_Duty(duty2);
         } else if(!butt4)
         {
            delay_ms(40);
            duty2--;
            PWM2_Set_Duty(duty2);
         }
     } //end while
}