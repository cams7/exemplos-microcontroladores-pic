;
; Curso de Assembly para PIC Aula 014
;
; Instru��es de Desvio Condicional
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


 list		p=16f84A							;Microcontrolador utilizado PIC16F84A
  
  
; --- Arquivos inclu�dos no projeto ---
 #include <p16f84a.inc>							; Inclui o arquivo do PIC16F84A  
  
  
; --- FUSE Bits ---
; Cristal oscilador externo 4MHz, sem watchdog timer, com power up timer, sem prote��o do c�digo
 __config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF
	
 

; --- Pagina��o de Mem�ria ---
 #define		bank0		bcf	STATUS,RP0		;Cria um mnem�nico para o banco 0 de mem�ria
 #define		bank1		bsf STATUS,RP0		;Cria um mnem�nico para o banco 1 de mem�ria
  
 
; --- Registradores de Uso Geral ---
 cblock			H'0C'							;In�cio da mem�ria do usu�rio 
 registro1										;Registrador auxiliar
 registro2										;Registrador auxiliar
 cont1											;Contador
 cont2											;Contador 
 endc  											;Final da mem�ria do usu�rio
                             
; --- Vetor de RESET ---
				org			H'0000'				;Origem no endere�o 0000h de mem�ria
				goto		init				;Desvia do vetor de interrup��o
				
; --- Vetor de Interrup��o ---
				org			H'0004'				;Todas as interrup��es apontam para este endere�o
				retfie							;Retorna de interrup��o
				

; --- Programa Principal ---				
init:
				bank1							;Seleciona o banco 1 de mem�ria
				movlw		H'FF'				;W = B'11111111'
				movwf		TRISA				;TRISA = H'FF' (todos os bits s�o entrada)
				movlw		H'FF'				;W = B'11111111'
				movwf		TRISB				;TRISB = H'F5' configura RB1 e RB3 como sa�da
				bank0							;Seleciona o banco 0 de mem�ria (padr�o do RESET)
				
				movlw		B'11111011'			;W = H'FB'
				movwf		registro1			;registro1 = H'FB'
				movlw		B'00000100'			;W = H'04'
				movwf		registro2			;registro2 = H'04'

				movlw		D'5'				;W = D'5'
				movwf		cont1				;cont1 = D'5'
				movlw		D'250'				;W = D'250'
				movwf		cont2				;cont2 = D'250'

teste1:											;Loop infinito
				decfsz		cont1,F				;Decrementa o contador e verifica se o valor desse � igual a zero. Contador igual a zero?
				goto		teste1				;N�o. Devia para 'teste1'
				
				movlw		D'5'				;W = D'5'
				movwf		cont1				;cont1 = D'5'

teste2:
				incfsz		cont2,F				;Incrementa o contador e verifica se o valor desse � igual a zero. Contador igual a zero?
				goto		teste2				;N�o. Devia para 'teste2'

				movlw		D'250'				;W = D'250'
				movwf		cont2				;cont2 = D'250'			
				
teste3:											
				btfsc		registro1,2			;Bit 2 do 'registro1' � igual a zero?
				goto		altera_registro1	;N�o. Desvia para 'altera_registro1'

				movlw		B'00000100'			;W = H'04'
				movwf		registro1			;registro1 = H'04'

teste4:
				btfss		registro2,2			;Bit 2 do 'registrado2' � igual a um?
				goto		altera_registro2	;N�o. Desvia para 'altera_registro2'

				movlw		B'11111011'			;W = H'FB'
				movwf		registro2			;registro2 = H'FB'											

				goto		teste1				;Sim. Desvia para 'teste1'

altera_registro1:
				movlw		B'11111011'			;W = H'FB'
				movwf		registro1			;registro1 = H'FB'
				goto 		teste4

altera_registro2:
				movlw		B'00000100'			;W = H'04'
				movwf		registro2			;registro2 = H'04'
				goto 		teste1	 
				
				end								;Final do programa
				

;     INSTRU��ES:
; ----------------------------------
;
; DECFSZ	f,d
;
; Decrementa f (d = f - 1) e desvia* se f = 0
;
; d = 0 (W) ou d = 1 (f)
;
; ----------------------------------
;
; INCFSZ	f,d
;
; Incrementa f (d = f + 1) e desvia* se f = 0
;
; d = 0 (W) ou d = 1 (f)
;
; ----------------------------------
;
; BTFSC		f,b
;
; Testa bit do registrador f, desvia* se igual a zero
;
;  
;
; ----------------------------------
;
; BTFSS		f,b
;
; Testa bit do registrador f, desvia* se igual a um
;
;  
;
; ----------------------------------
;
;
;
; * O mesmo que saltar a pr�xima linha
;
;