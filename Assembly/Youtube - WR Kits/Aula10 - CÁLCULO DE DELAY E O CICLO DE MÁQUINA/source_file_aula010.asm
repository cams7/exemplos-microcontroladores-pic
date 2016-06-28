;
; Curso de Assembly para PIC Aula 10
;
; Aciona LED1 ligado em RB0 e apaga LED2 ligado em RB1
; aguarda 500 milissegundos
; Aciona LED2 ligado em RB1 e apaga LED1 ligado em RB0
; aguarda 500 milissegundos
;
;
; OBS.:
;
; LEDs ligados no modo current sourcing:
;
; '0' - apaga
; '1' - acende
;
; Cálculo do Ciclo de Máquina:
;
; Ciclo de máquina = 1/(Freq Cristal / 4) = 1us
;
;  
;  
;  
;
; MCU: PIC16F84A   Clock: 4MHz
;
; www.wrkits.com.br | facebook.com/wrkits | youtube.com/canalwrkits
;
; Autor: Eng. Wagner Rambo   Data: Janeiro de 2016
;


 list		p=16f84A							; microcontrolador utilizado PIC16F84A
  
  
; --- Arquivos incluídos no projeto ---
 #include <p16f84a.inc>							; inclui o arquivo do PIC16F84A  
  
  
; --- FUSE Bits ---
; Cristal oscilador externo 4MHz, sem watchdog timer, com power up timer, sem proteção do código
 __config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF
	

; --- Paginação de Memória ---
 #define		bank0		bcf	STATUS,RP0		;Cria um mnemônico para o banco 0 de memória
 #define		bank1		bsf STATUS,RP0		;Cria um mnemônico para o banco 1 de memória
 
; --- Saídas ---
 #define		led1		PORTB,RB0			;LED 1 ligado em RB1
 #define		led2		PORTB,RB1			;LED 2 ligado em RB3
                       
                             
; --- Vetor de RESET ---
				org			H'0000'				;Origem no endereço 0000h de memória
				goto		inicio				;Desvia do vetor de interrupção
				
; --- Vetor de Interrupção ---
				org			H'0004'				;Todas as interrupções apontam para este endereço
				retfie							;Retorna de interrupção
				

; --- Programa Principal ---				
inicio:
				bank1							;seleciona o banco 1 de memória
				movlw		H'FC'				;W = B'11111100'
				movwf		TRISB				;TRISB = H'F5' configura RB1 e RB3 como saída
				bank0							;seleciona o banco 0 de memória (padrão do RESET)
				movlw		H'FC'				;W = B'11111100'
				movwf		PORTB				;LEDs iniciam desligados
				
loop:											;Loop infinito

				bsf			led1				;liga LED1
				bcf			led2				;desliga LED2
				call		delay500ms			;chama sub rotina
				bcf			led1				;desliga LED1
				bsf			led2				;liga LED2
				call		delay500ms			;chama sub rotina

				goto		loop				;volta para label loop
				
				
				
; --- Desenvolvimento das Sub-Rotinas ---
 				
				
delay500ms:

				movlw		D'200'				;Move o valor para W
				movwf		H'0C'				;Inicializa a variável tempo0 

												; 4 ciclos de máquina

aux1:
				movlw		D'249'				;Move o valor para W
				movwf		H'0D'				;Inicializa a variável tempo1
				nop
				nop
				nop
				nop
				nop
				nop
	
												; Gasta 8 ciclos de máquina

aux2:
				nop								
				nop
				nop
				nop
				nop
				nop
				nop								;Gastar 7 ciclo de máquina

				decfsz		H'0D'				;Decrementa tempo1 até que seja igual a zero
				goto		aux2				;Vai para a label aux2 

												; (249 x 10 - 1) + 8 = 2497 ciclos de máquina												

				decfsz		H'0C'				;Decrementa tempo0 até que seja igual a zero
				goto		aux1				;Vai para a label aux1

												; 3 ciclos de máquina

												; (200 x 2497) + (200 x 3 - 1) = 499999 ciclos de máquina
												; 499999 + 4 + 2 =  500005 ciclos de máquina
												
												;d = ((10a - 1) + 8)b + (3b - 1) + 4 + 2
												;d = (10a + 7)b + 3b + 5
												;d = 10ab + 7b + 3b + 5
												;d = 10ab + 10b + 5
												;d = (10a + 10)b + 5
												;d = (10 x 249 + 10) x 200 + 5 = 500005

				return							;Retorna após a chamada da sub rotina
				
				
				end								;Final do programa