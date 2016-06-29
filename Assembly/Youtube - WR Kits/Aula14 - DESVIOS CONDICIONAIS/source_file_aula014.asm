;
; Curso de Assembly para PIC Aula 014
;
; Instruções de Desvio Condicional
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
  
  
; --- Arquivos incluídos no projeto ---
 #include <p16f84a.inc>							; Inclui o arquivo do PIC16F84A  
  
  
; --- FUSE Bits ---
; Cristal oscilador externo 4MHz, sem watchdog timer, com power up timer, sem proteção do código
 __config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF
	
 

; --- Paginação de Memória ---
 #define		bank0		bcf	STATUS,RP0		;Cria um mnemônico para o banco 0 de memória
 #define		bank1		bsf STATUS,RP0		;Cria um mnemônico para o banco 1 de memória
  
 
; --- Registradores de Uso Geral ---
 cblock			H'0C'							;Início da memória do usuário 
 registro1										;Registrador auxiliar
 registro2										;Registrador auxiliar
 cont1											;Contador
 cont2											;Contador 
 endc  											;Final da memória do usuário
                             
; --- Vetor de RESET ---
				org			H'0000'				;Origem no endereço 0000h de memória
				goto		init				;Desvia do vetor de interrupção
				
; --- Vetor de Interrupção ---
				org			H'0004'				;Todas as interrupções apontam para este endereço
				retfie							;Retorna de interrupção
				

; --- Programa Principal ---				
init:
				bank1							;Seleciona o banco 1 de memória
				movlw		H'FF'				;W = B'11111111'
				movwf		TRISA				;TRISA = H'FF' (todos os bits são entrada)
				movlw		H'FF'				;W = B'11111111'
				movwf		TRISB				;TRISB = H'F5' configura RB1 e RB3 como saída
				bank0							;Seleciona o banco 0 de memória (padrão do RESET)
				
				movlw		B'11111011'			;W = H'FB'
				movwf		registro1			;registro1 = H'FB'
				movlw		B'00000100'			;W = H'04'
				movwf		registro2			;registro2 = H'04'

				movlw		D'5'				;W = D'5'
				movwf		cont1				;cont1 = D'5'
				movlw		D'250'				;W = D'250'
				movwf		cont2				;cont2 = D'250'

teste1:											;Loop infinito
				decfsz		cont1,F				;Decrementa o contador e verifica se o valor desse é igual a zero. Contador igual a zero?
				goto		teste1				;Não. Devia para 'teste1'
				
				movlw		D'5'				;W = D'5'
				movwf		cont1				;cont1 = D'5'

teste2:
				incfsz		cont2,F				;Incrementa o contador e verifica se o valor desse é igual a zero. Contador igual a zero?
				goto		teste2				;Não. Devia para 'teste2'

				movlw		D'250'				;W = D'250'
				movwf		cont2				;cont2 = D'250'			
				
teste3:											
				btfsc		registro1,2			;Bit 2 do 'registro1' é igual a zero?
				goto		altera_registro1	;Não. Desvia para 'altera_registro1'

				movlw		B'00000100'			;W = H'04'
				movwf		registro1			;registro1 = H'04'

teste4:
				btfss		registro2,2			;Bit 2 do 'registrado2' é igual a um?
				goto		altera_registro2	;Não. Desvia para 'altera_registro2'

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
				

;     INSTRUÇÕES:
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
; * O mesmo que saltar a próxima linha
;
;