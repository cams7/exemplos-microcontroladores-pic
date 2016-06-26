;
; Curso de Assembly para PIC Aula 005
;
; Compilando e Simulando um C�digo no MPLAB v8.92
;
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

; --- Os Termos do Set de Instru��es ---
;W: work (registrador de trabalho ou acumulador)
;F: file (registradores especiais e/ou de uso geral | posi��o de mem�ria)
;L: literal (constante, n�mero qualquer) Utilizado como L nas instru��es e k nos argumentos 
;D:  destination  (local  onde  o  resultado  de  uma  opera��o  ser� armazenado). 
;B: bit (bits dentro dos registradores, dentro de um byte, flags) 
;T: test (utilizado para teste de bits) 
;S: skip (pulo, salto, desvio condicional) 
;S: set (setar, tornar n�vel alto) 
;C: clear (limpar, tornar n�vel baixo) 
;Z: zero (testar se � equivalente a zero)	

; --- Pagina��o de Mem�ria ---
 #define		bank0		bcf	STATUS,RP0		;Cria um mnem�nico para o banco 0 de mem�ria
 #define		bank1		bsf STATUS,RP0		;Cria um mnem�nico para o banco 1 de mem�ria
 

; --- Entradas ---
 #define		botao1		PORTB,RB0			;bot�o 1 ligado em RB0
 
 
; --- Sa�das ---
 #define		led1		PORTB,RB7			;LED 1 ligado em RB7
                       
                             
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
												;TRISA = 11111111
												;TRISA = 00011111											
				movlw		H'7F'				;W = B'01111111'
				movwf		TRISB				;TRISB = H'7F' (apenas o RB7 como sa�da)
				bank0							;seleciona o banco 0 de mem�ria (padr�o do RESET)
				movlw		H'FF'				;W = B'11111111'
				movwf		PORTB				;RB7 (configurado como sa�da) inicia em HIGH
												;TRISB = 01111111
												;PORTB = 11111111
												;PORTB = 10000000
				
				goto		$					;Segura o c�digo nesta linha
				
				
				end								;Final do programa