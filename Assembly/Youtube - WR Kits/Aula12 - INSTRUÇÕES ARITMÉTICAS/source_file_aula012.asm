;
; Curso de Assembly para PIC Aula 012
;
; Instru��es Aritm�ticas em Assembly
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
  
  
; --- Arquivos inclu�dos no projeto ---
 #include <p16f84a.inc>							; inclui o arquivo do PIC16F84A  
  
  
; --- FUSE Bits ---
; Cristal oscilador externo 4MHz, sem watchdog timer, com power up timer, sem prote��o do c�digo
 __config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF
	
 

; --- Pagina��o de Mem�ria ---
 #define		bank0		bcf	STATUS,RP0		;Cria um mnem�nico para o banco 0 de mem�ria
 #define		bank1		bsf STATUS,RP0		;Cria um mnem�nico para o banco 1 de mem�ria
  
 
; --- Registradores de Uso Geral ---
 cblock			H'0C'							;inicio da mem�ria do usu�rio
 
 regist1										;registrador auxiliar
 regist2										;registrador auxiliar
 
 endc  											;final da mem�ria do usu�rio
                             
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
				movlw		H'FF'				;W = B'11110101'
				movwf		TRISB				;TRISB = H'F5' configura RB1 e RB3 como sa�da
				bank0							;seleciona o banco 0 de mem�ria (padr�o do RESET)
 				clrf		regist1
 				clrf		regist2
				
loop:											;Loop infinito

				movlw		D'30'				;Move a constante D'30' no registrador W
				addlw		D'68'				;W = D'68' + W -> W = D'68' + D'30' = D'98'
				movwf 		regist2				;regist2 = D'98'
				
				movlw		H'AC'				;Move a constante H'AC' para W		
				
				addwf		regist2,W			;W = regist2 + W -> W = D'98' + D'172' = D'270' =  H'010E', W = H'0E' = D'14' 				
				movwf 		regist1				;regist1 = D'14'
						
				addwf		regist1,W			;W = regist1 + W -> W = D'14' + D'14' = D'28' 
				movwf 		regist2				;regist2 = D'28'

				rlf			regist2,W			;W = regist2 << 1 -> W = B'00011100' << 1 -> W = D'56'	
				movwf 		regist2				;regist2 = D'56'

				rrf			regist1,W			;W = regist1 >> 1 -> W = B'00001110' >> 1 -> W = D'7'	
				movwf 		regist1				;regist1 = D'7'

				movfw		regist2				;W = D'56'
				sublw		D'100'				;W = D'100' - W -> W = D'100' - D'56' = D'44'	
				movwf 		regist2				;regist2 = D'44'

				movfw		regist1				;W = D'7'
				subwf		regist2,W			;W = regist2 - W -> W = D'44' - D'7' = D'37'
				movwf 		regist1				;regist1 = D'37'
				
				goto		loop
				
				end								;Final do programa
				

;     INSTRU��ES:
; ----------------------------------
;
; ADDLW		k
;
; Opera��o:   W = W + k
;
; ----------------------------------
;
; ADDWF		f,d
;
; Opera��o:   d = W + f
;
; d = 0 (W) ou d = 1 (f)
;
; ----------------------------------
;
; RLF		f,d
;
; Opera��o:   d = f << 1 (rotaciona o registrador f um bit para esquerda 'multiplica')
;
; d = 0 (W) ou d = 1 (f)
;
; ----------------------------------
;
; RRF		f,d
;
; Opera��o:   d = f >> 1 (rotaciona o registrador f um bit para direita 'divide')
;
; d = 0 (W) ou d = 1 (f)
;
; ----------------------------------
;
; SUBLW		k
;
; Opera��o:   W = k - W
;
; ----------------------------------
;
; SUBWF		f,d
;
; Opera��o:   d = f - W
;
; d = 0 (W) ou d = 1 (f)
;
; ----------------------------------