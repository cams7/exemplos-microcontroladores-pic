;
; Curso de Assembly para PIC Aula 006
;
; Aciona um LED ligado em RB7, a partir de um botão ligado em RB0
;
; OBS.:
;
; LED ligado no modo current sourcing:
;
; '0' - apaga
; '1' - acende
;
; Botão ligado com resistor de pull-up:
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

; --- Os Termos do Set de Instruções ---
;W: work (registrador de trabalho ou acumulador)
;F: file (registradores especiais e/ou de uso geral | posição de memória)
;L: literal (constante, número qualquer) Utilizado como L nas instruções e k nos argumentos 
;D:  destination  (local  onde  o  resultado  de  uma  operação  será armazenado). 
;B: bit (bits dentro dos registradores, dentro de um byte, flags) 
;T: test (utilizado para teste de bits) 
;S: skip (pulo, salto, desvio condicional) 
;S: set (setar, tornar nível alto) 
;C: clear (limpar, tornar nível baixo) 
;Z: zero (testar se é equivalente a zero)	

; --- Paginação de Memória ---
 #define		bank0		bcf	STATUS,RP0		;Cria um mnemônico para o banco 0 de memória
 #define		bank1		bsf STATUS,RP0		;Cria um mnemônico para o banco 1 de memória
 

; --- Entradas ---
 #define		botao1		PORTB,RB0			;botão 1 ligado em RB0
 
 
; --- Saídas ---
 #define		led1		PORTB,RB7			;LED 1 ligado em RB7
                       
                             
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
				movlw		H'7F'				;W = B'01111111'
				movwf		TRISB				;TRISB = H'7F' (apenas o RB7 como saída)
				bank0							;seleciona o banco 0 de memória (padrão do RESET)
				movlw		H'FF'				;W = B'11111111'
				movwf		PORTB				;RB7 (configurado como saída) inicia em HIGH
				
loop:

				btfsc		botao1				;Botão foi pressionado?
				goto		apaga_led1			;Não, desvia para label apaga_led1
				bsf			led1				;Sim, liga led1
				goto		loop				;volta para label loop
				
apaga_led1:
				bcf			led1				;apaga led1
				goto		loop				;volta para label loop
				
				
				end								;Final do programa