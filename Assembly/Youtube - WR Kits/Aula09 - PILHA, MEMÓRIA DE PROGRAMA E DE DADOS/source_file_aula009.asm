;
; Curso de Assembly para PIC Aula 009
;
; Aciona LED1 ligado em RB1, a partir de bot�o1 ligado em RB0 e
; aciona LED2 ligado em RB3, a partir de bot�o2 ligado em RB2.
;
; OBS.:
;
; LEDs ligados no modo current sourcing:
;
; '0' - apaga
; '1' - acende
;
; Bot�es ligados com resistor de pull-up:
;
; '0' - bot�o acionado
; '1' - bot�o liberado
;  
;
; MCU: PIC16F84A   Clock: 4MHz
;
; www.wrkits.com.br | facebook.com/wrkits | youtube.com/canalwrkits
;
; Autor: Eng. Wagner Rambo   Data: Dezembro 2015
;


 list		p=16f84A							; microcontrolador utilizado PIC16F84A
  
  
; --- Arquivos inclu�dos no projeto ---
 #include <p16f84a.inc>							; inclui o arquivo do PIC16F84A  
  
  
; --- FUSE Bits ---
; Cristal oscilador externo 4MHz, sem watchdog timer, com power up timer, sem prote��o do c�digo
 __config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF
	

; --- Pagina��o de Mem�ria ---
 #define		bank0		bcf	STATUS,RP0		;Cria um mnem�nico para o banco 0 de mem�ria
 #define		bank1		bsf STATUS,RP0		;Cria um mnem�nico para o banco 1 de mem�ria
 

; --- Entradas ---
 #define		botao1		PORTB,RB0			;bot�o 1 ligado em RB0
 #define		botao2		PORTB,RB2			;bot�o 2 ligado em RB2
 
 
; --- Sa�das ---
 #define		led1		PORTB,RB1			;LED 1 ligado em RB1
 #define		led2		PORTB,RB3			;LED 2 ligado em RB3
                       
                             
; --- Vetor de RESET ---
				org			H'0000'				;Origem no endere�o 0000h de mem�ria
				goto		inicio				;Desvia do vetor de interrup��o
				
; --- Vetor de Interrup��o ---
				org			H'0004'				;Todas as interrup��es apontam para este endere�o
				retfie							;Retorna de interrup��o
				

; --- Programa Principal ---				
inicio:
				bank1							;seleciona o banco 1 de mem�ria
				movlw		H'FF'				;W = B'11111111'
				movwf		TRISA				;TRISA = H'FF' (todos os bits s�o entrada)
				movlw		H'F5'				;W = B'11110101'
				movwf		TRISB				;TRISB = H'F5' configura RB1 e RB3 como sa�da
				bank0							;seleciona o banco 0 de mem�ria (padr�o do RESET)
				movlw		H'F5'				;W = B'11110101'
				movwf		PORTB				;LEDs iniciam desligados
				
loop:											;Loop infinito

				call		trata_but1			;Chama a sub-rotina para tratar bot�o1
				call		trata_but2			;Chama a sub-rotina para tratar bot�o2

				goto		loop				;volta para label loop
				
				
				
; --- Desenvolvimento das Sub-Rotinas ---

trata_but1:										;Sub-Rotina para tratar bot�o 1. 
												;O endere�o dessa subrotina e armazenado no n�vel 1 da pilha
                call		pilha_nivel2
				btfsc		botao1				;Bot�o foi pressionado?
				goto		apaga_led1			;N�o, desvia para label apaga_led1
				bsf			led1				;Sim, liga led1
				return							;retorna da sub-rotina
				
apaga_led1:
				bcf			led1				;apaga led1
				return							;retorna da sub-rotina
	
				
trata_but2:										;Sub-Rotina para tratar bot�o 2. 
												;O endere�o dessa subrotina e armazenado no n�vel 1 da pilha
				btfsc		botao2				;Bot�o foi pressionado?
				goto		apaga_led2			;N�o, desvia para label apaga_led2
				bsf			led2				;Sim, liga led2
				return							;retorna da sub-rotina
				
apaga_led2:
				bcf			led2				;apaga led2
				return							;retorna da sub-rotina
				
pilha_nivel2:									;Endere�o da subrotina armazenado no n�vel 2 da pilha
				call		pilha_nivel3
				return

pilha_nivel3:									;Endere�o da subrotina armazenado no n�vel 3 da pilha
				call		pilha_nivel4
				return

pilha_nivel4:									;Endere�o da subrotina armazenado no n�vel 4 da pilha
				call		pilha_nivel5
				return

pilha_nivel5:									;Endere�o da subrotina armazenado no n�vel 5 da pilha
				call		pilha_nivel6
				return
pilha_nivel6:									;Endere�o da subrotina armazenado no n�vel 6 da pilha
				call		pilha_nivel7
				return

pilha_nivel7:									;Endere�o da subrotina armazenado no n�vel 7 da pilha
				call		pilha_nivel8
				return

pilha_nivel8:									;Endere�o da subrotina armazenado no n�vel 8 da pilha
				nop
				return
				
				
				end								;Final do programa