; Sequencial de LEDs
; MCU: PIC16F84A   Clock: 4MHz
; Autor: César Magalhães

 list p=16f84A									; Microcontrolador utilizado PIC16F84A
  
  
; --- Arquivos incluídos no projeto ---
 #include <p16f84a.inc>							; Inclui o arquivo do PIC16F84A

; --- FUSE Bits ---
; Cristal oscilador externo 4MHz, sem watchdog timer, com power up timer, sem proteção do código
 __config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF

; --- Paginação de Memória ---
 #define		bank0		bcf STATUS,RP0		; Cria um mnemônico para o banco 0 de memória
 #define		bank1		bsf STATUS,RP0		; Cria um mnemônico para o banco 1 de memória

; --- Registradores de Uso Geral ---
 cblock			H'0C'							; Início da memória do usuário 
 cont1											; Contador usado na subrotina de delay
 cont2											; Contador usado na subrotina de delay 
 endc

; --- Vetor de RESET ---
				org		H'0000'					; Origem no endereço 0000h de memória
				goto		init				; Desvia do vetor de interrupção
				
; --- Vetor de Interrupção ---
				org		H'0004'					; Todas as interrupções apontam para este endereço
				retfie							; Retorna de interrupção

; --- Programa Principal ---
init:
				bank1							; Seleciona o banco 1 de memória
				movlw		H'00'				; W = B'00000000'
				movwf		TRISB				; Configura RB0,RB1,RB2,RB3,RB4,RB5,RB6 e RB7 como saída
				bank0							; Seleciona o banco 0 de memória (padrão do RESET)
				movlw		H'01'				; W = B'00000000'
				movwf		PORTB				; Incializa com todos leds desligados

main:
				call		delay500ms			; Chama a subrotina 'delay500ms'
				rlf			PORTB,F				; Rotaciona um bit à direita
				goto		main				; Volta para o label 'main'

delay500ms:
				movlw		D'200'				; Move o valor D'200' para W
				movwf		cont1				; Inicializa o 1º contador
												; Gasta 4 ciclos de máquina

delay1:
				movlw		D'249'				; Move o valor D'249' para W
				movwf		cont2				; Inicializa o 2º contador
				nop
				nop
				nop
				nop
				nop
				nop
												; Gasta  8 ciclos de máquina								

delay2:
				nop								
				nop
				nop
				nop
				nop
				nop
				nop
												; Gasta 7 ciclos de máquina

				decfsz		cont2,F				; Decrementa o valor do 2º contador até que seja igual a zero
				goto		delay2				; Vai para a label 'delay2' 

												; (249 x 10 - 1) + 8 = 2497 ciclos de máquina

				decfsz		cont1,F				; Decrementa o valor do 1º contador até que seja igual a zero
				goto		delay1				; Vai para a label 'delay1'

												; Gasta 3 ciclos de máquina

												; (200 x 2497) + (200 x 3 - 1) = 499999 ciclos de máquina
												; 499999 + 4 + 2 =  500005 ciclos de máquina
												
												; d = ((10a - 1) + 8)b + (3b - 1) + 4 + 2
												; d = (10a + 7)b + 3b + 5
												; d = 10ab + 7b + 3b + 5
												; d = 10ab + 10b + 5
												; d = (10a + 10)b + 5
												; delay = (10 x cont2 + 10) x cont1 + 5
												; delay = (10 x 249 + 10) x 200 + 5 = 500005

				return							;Retorna após a chamada da sub rotina
				
				end