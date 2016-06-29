; Sequencial de LEDs
; MCU: PIC16F84A   Clock: 4MHz
; Autor: C�sar Magalh�es

 list p=16f84A									; Microcontrolador utilizado PIC16F84A
  
  
; --- Arquivos inclu�dos no projeto ---
 #include <p16f84a.inc>							; Inclui o arquivo do PIC16F84A

; --- FUSE Bits ---
; Cristal oscilador externo 4MHz, sem watchdog timer, com power up timer, sem prote��o do c�digo
 __config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF

; --- Pagina��o de Mem�ria ---
 #define		bank0		bcf STATUS,RP0		; Cria um mnem�nico para o banco 0 de mem�ria
 #define		bank1		bsf STATUS,RP0		; Cria um mnem�nico para o banco 1 de mem�ria

; --- Registradores de Uso Geral ---
 cblock			H'0C'							; In�cio da mem�ria do usu�rio 
 cont1											; Contador usado na subrotina de delay
 cont2											; Contador usado na subrotina de delay 
 endc

; --- Vetor de RESET ---
				org		H'0000'					; Origem no endere�o 0000h de mem�ria
				goto		init				; Desvia do vetor de interrup��o
				
; --- Vetor de Interrup��o ---
				org		H'0004'					; Todas as interrup��es apontam para este endere�o
				retfie							; Retorna de interrup��o

; --- Programa Principal ---
init:
				bank1							; Seleciona o banco 1 de mem�ria
				movlw		H'00'				; W = B'00000000'
				movwf		TRISB				; Configura RB0,RB1,RB2,RB3,RB4,RB5,RB6 e RB7 como sa�da
				bank0							; Seleciona o banco 0 de mem�ria (padr�o do RESET)
				movlw		H'01'				; W = B'00000000'
				movwf		PORTB				; Incializa com todos leds desligados

main:
				call		delay500ms			; Chama a subrotina 'delay500ms'
				rlf			PORTB,F				; Rotaciona um bit � direita
				goto		main				; Volta para o label 'main'

delay500ms:
				movlw		D'200'				; Move o valor D'200' para W
				movwf		cont1				; Inicializa o 1� contador
												; Gasta 4 ciclos de m�quina

delay1:
				movlw		D'249'				; Move o valor D'249' para W
				movwf		cont2				; Inicializa o 2� contador
				nop
				nop
				nop
				nop
				nop
				nop
												; Gasta  8 ciclos de m�quina								

delay2:
				nop								
				nop
				nop
				nop
				nop
				nop
				nop
												; Gasta 7 ciclos de m�quina

				decfsz		cont2,F				; Decrementa o valor do 2� contador at� que seja igual a zero
				goto		delay2				; Vai para a label 'delay2' 

												; (249 x 10 - 1) + 8 = 2497 ciclos de m�quina

				decfsz		cont1,F				; Decrementa o valor do 1� contador at� que seja igual a zero
				goto		delay1				; Vai para a label 'delay1'

												; Gasta 3 ciclos de m�quina

												; (200 x 2497) + (200 x 3 - 1) = 499999 ciclos de m�quina
												; 499999 + 4 + 2 =  500005 ciclos de m�quina
												
												; d = ((10a - 1) + 8)b + (3b - 1) + 4 + 2
												; d = (10a + 7)b + 3b + 5
												; d = 10ab + 7b + 3b + 5
												; d = 10ab + 10b + 5
												; d = (10a + 10)b + 5
												; delay = (10 x cont2 + 10) x cont1 + 5
												; delay = (10 x 249 + 10) x 200 + 5 = 500005

				return							;Retorna ap�s a chamada da sub rotina
				
				end