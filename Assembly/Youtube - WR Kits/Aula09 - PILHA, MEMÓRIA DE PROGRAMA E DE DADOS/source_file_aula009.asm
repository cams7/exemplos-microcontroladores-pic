;
; Curso de Assembly para PIC Aula 009
;
; Aciona LED1 ligado em RB1, a partir de botão1 ligado em RB0 e
; aciona LED2 ligado em RB3, a partir de botão2 ligado em RB2.
;
; OBS.:
;
; LEDs ligados no modo current sourcing:
;
; '0' - apaga
; '1' - acende
;
; Botões ligados com resistor de pull-up:
;
; '0' - botão acionado
; '1' - botão liberado
;  
;
; MCU: PIC16F84A   Clock: 4MHz
;
; www.wrkits.com.br | facebook.com/wrkits | youtube.com/canalwrkits
;
; Autor: Eng. Wagner Rambo   Data: Dezembro 2015
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
 

; --- Entradas ---
 #define		botao1		PORTB,RB0			;botão 1 ligado em RB0
 #define		botao2		PORTB,RB2			;botão 2 ligado em RB2
 
 
; --- Saídas ---
 #define		led1		PORTB,RB1			;LED 1 ligado em RB1
 #define		led2		PORTB,RB3			;LED 2 ligado em RB3
                       
                             
; --- Vetor de RESET ---
				org			H'0000'				;Origem no endereço 0000h de memória
				goto		inicio				;Desvia do vetor de interrupção
				
; --- Vetor de Interrupção ---
				org			H'0004'				;Todas as interrupções apontam para este endereço
				retfie							;Retorna de interrupção
				

; --- Programa Principal ---				
inicio:
				bank1							;seleciona o banco 1 de memória
				movlw		H'FF'				;W = B'11111111'
				movwf		TRISA				;TRISA = H'FF' (todos os bits são entrada)
				movlw		H'F5'				;W = B'11110101'
				movwf		TRISB				;TRISB = H'F5' configura RB1 e RB3 como saída
				bank0							;seleciona o banco 0 de memória (padrão do RESET)
				movlw		H'F5'				;W = B'11110101'
				movwf		PORTB				;LEDs iniciam desligados
				
loop:											;Loop infinito

				call		trata_but1			;Chama a sub-rotina para tratar botão1
				call		trata_but2			;Chama a sub-rotina para tratar botão2

				goto		loop				;volta para label loop
				
				
				
; --- Desenvolvimento das Sub-Rotinas ---

trata_but1:										;Sub-Rotina para tratar botão 1. 
												;O endereço dessa subrotina e armazenado no nível 1 da pilha
                call		pilha_nivel2
				btfsc		botao1				;Botão foi pressionado?
				goto		apaga_led1			;Não, desvia para label apaga_led1
				bsf			led1				;Sim, liga led1
				return							;retorna da sub-rotina
				
apaga_led1:
				bcf			led1				;apaga led1
				return							;retorna da sub-rotina
	
				
trata_but2:										;Sub-Rotina para tratar botão 2. 
												;O endereço dessa subrotina e armazenado no nível 1 da pilha
				btfsc		botao2				;Botão foi pressionado?
				goto		apaga_led2			;Não, desvia para label apaga_led2
				bsf			led2				;Sim, liga led2
				return							;retorna da sub-rotina
				
apaga_led2:
				bcf			led2				;apaga led2
				return							;retorna da sub-rotina
				
pilha_nivel2:									;Endereço da subrotina armazenado no nível 2 da pilha
				call		pilha_nivel3
				return

pilha_nivel3:									;Endereço da subrotina armazenado no nível 3 da pilha
				call		pilha_nivel4
				return

pilha_nivel4:									;Endereço da subrotina armazenado no nível 4 da pilha
				call		pilha_nivel5
				return

pilha_nivel5:									;Endereço da subrotina armazenado no nível 5 da pilha
				call		pilha_nivel6
				return
pilha_nivel6:									;Endereço da subrotina armazenado no nível 6 da pilha
				call		pilha_nivel7
				return

pilha_nivel7:									;Endereço da subrotina armazenado no nível 7 da pilha
				call		pilha_nivel8
				return

pilha_nivel8:									;Endereço da subrotina armazenado no nível 8 da pilha
				nop
				return
				
				
				end								;Final do programa