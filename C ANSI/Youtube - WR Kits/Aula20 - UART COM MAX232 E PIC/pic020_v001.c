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

#define LF 0x0A //Alimentação de linha; mudança de linha; nova linha (inglês Line Feed)
#define CR 0x0D //Retorno do carro; retorno ao início da linha (inglês Carriage Return)

char uart_rd;

void main() {
   //ANSEL  = 0;     // Configure AN pins as digital
   //ANSELH = 0;
   ADCON0 = 0x00;    //0b00000000
   ADCON1 = 0x06;    //0b00000110
   CMCON = 0x07;     //0b00000111
   TRISB = 0x80;     //0b10000000
   PORTB = 0x00;     //0b00000000
   UART1_Init(9600); // Initialize UART module at 9600 bps
   Delay_ms(100);    // Wait for UART module to stabilize
   RB6_bit = 0x01;
   UART1_Write_Text("Start");
   UART1_Write(LF);
   UART1_Write(CR);

   while (1) {                   // Endless loop
      if (UART1_Data_Ready()) {  // If data is received,
         uart_rd = UART1_Read(); // read the received data,
         UART1_Write(uart_rd);   // and send data via UART
         switch(uart_rd) {
            case 'y': 
               RB0_bit = 0x01; 
               break;
            case 't': 
               RB0_bit = 0x00; 
               break;
            case 'r': 
               RB1_bit = 0x01; 
               break;
            case 'e': 
               RB1_bit = 0x00; 
               break;
            case 'g': 
               RB2_bit = 0x01; 
               break;
            case 'f': 
               RB2_bit = 0x00; 
               break;
            default:
               break;
         }
      }
    
      if(RB7_bit) {
         UART1_Write_Text("Alert!");
         UART1_Write(LF);
         UART1_Write(CR);
      }
  }
}