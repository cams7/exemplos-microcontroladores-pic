;
; Curso de Assembly para PIC Aula 007
;
; Aciona um LED ligado em RB1, a partir de um bot�o ligado em RB0
;
; OBS.:
;
; LED ligado no modo current sourcing:
;
; '0' - apaga
; '1' - acende
;
; Bot�o ligado com resistor de pull-up:
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
 
 
; --- Sa�das ---
 #define		led1		PORTB,RB1			;LED 1 ligado em RB1
                       
                             
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
				movlw		H'FD'				;W = B'11111101'
				movwf		TRISB				;TRISB = H'FD' (apenas o RB1 como sa�da)
				bank0							;seleciona o banco 0 de mem�ria (padr�o do RESET)
				movlw		H'FD'				;W = B'11111101'
				movwf		PORTB				;RB0 (configurado como sa�da) inicia em LOW
				
loop:

				btfsc		botao1				;Bot�o foi pressionado?
				goto		apaga_led1			;N�o, desvia para label apaga_led1
				bsf			led1				;Sim, liga led1
				goto		loop				;volta para label loop
				
apaga_led1:
				bcf			led1				;apaga led1
				goto		loop				;volta para label loop
				
				
				end								;Final do programa