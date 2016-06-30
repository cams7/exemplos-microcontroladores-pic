;
; Curso de Assembly para PIC WR Kits Aula 26
;
;
; Como inverter o nível lógico de pinos de saída ou bits
;
; Sistema Embarcado Sugerido: PARADOXUS PEPTO
;
; Disponível em https://wrkits.com.br/catalog/show/141 
;
; Clock: 4MHz    Ciclo de máquina = 1µs
;
; Autor: Eng. Wagner Rambo   Data: Abril de 2016
;
;

; --- Listagem do Processador Utilizado ---
   	list	p=16F628A					;Utilizado PIC16F628A
	
	
; --- Arquivos Inclusos no Projeto ---
   	#include <p16F628a.inc>				;inclui o arquivo do PIC 16F628A
	
	
; --- FUSE Bits ---
; - Cristal de 4MHz
; - Desabilitamos o Watch Dog Timer
; - Habilitamos o Power Up Timer
; - Brown Out desabilitado
; - Sem programação em baixa tensão, sem proteção de código, sem proteção da memória EEPROM
  	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CP_OFF & _CPD_OFF & _MCLRE_ON
	
	
; --- Paginação de Memória ---
   	#define		bank0	bcf	STATUS,RP0	;Cria um mnemônico para selecionar o banco 0 de memória
   	#define		bank1	bsf	STATUS,RP0	;Cria um mnemônico para selecionar o banco 1 de memória

; --- Constantes ---
   	MASK		equ		B'00001100'		;Cria máscara para inverter estado de RA2 e RA3
	

; --- Registradores de Uso Geral ---
   	cblock		H'20'					;Início da memória disponível para o usuário	
   	t0									;Registrador auxiliar para temporização
 	t1									;Registrador auxiliar para temporização	
	endc								;Final da memória do usuário	
	
; --- Vetor de RESET ---
	org			H'0000'					;Origem no endereço 00h de memória
	goto		inicio					;Desvia para a label início
	

; --- Vetor de Interrupção ---
	org			H'0004'					;As interrupções deste processador apontam para este endereço
	retfie								;Retorna da interrupção	
	
inicio:
 
	bank1								;Seleciona o banco 1 de memória
	movlw		H'00'					;w = 00h
	movwf		TRISA					;Configura todo o PORTA como saída
	bank0								;Seleciona o banco 0 de memória
	movlw		H'07'					;w = 7h
	movwf		CMCON					;CMCON = 7h (apenas I/Os digitais) desabilita os comparadores internos
	movlw		B'00001000'				;w = 08h
	movwf		PORTA					;Inicializa PORTA
	
	;
	; MASK  = 00001100b
	;     W = 00001100b
	; PORTA = PORTA xor W
	; PORTA = 00001000b
	;	  xor 00001100b
	; PORTA = 00000100b
	;	  xor 00001100b
	; PORTA = 00001000b
	
loop:
	movlw		MASK					;Carrega máscara do pino do buzzer em Work
	xorwf		PORTA,F					;PORTA = PORTA xor MASK (inverte estado da saída)
	call		delay500ms				;aguarda 500ms

	goto		loop					;volta para loop	
	
; --- Desenvolvimento das Sub-Rotinas --- 							
delay500ms:								;Rotina para atraso de 500ms

	movlw		D'200'					;Move o valor para W
	movwf		t0						;Inicializa tempo0 

										; 4 ciclos de máquina

delay1:
	movlw		D'249'					;Move o valor para W
	movwf		t1						;Inicializa tempo1
	nop
	nop
	nop
	nop
	nop
	nop										
										; 8 ciclos de máquina

delay2:
	nop									;no operation | 1 ciclo de máquina
	nop									;no operation | 1 ciclo de máquina
	nop									;no operation | 1 ciclo de máquina
	nop									;no operation | 1 ciclo de máquina
	nop									;no operation | 1 ciclo de máquina
	nop									;no operation | 1 ciclo de máquina
	nop									;no operation | 1 ciclo de máquina
										; 7 ciclos de máquina

	decfsz		t1						;Decrementa tempo1 até que seja igual a zero
	goto		delay2					;Vai para a label aux2 

										; (249 x 10 - 1) + 8 = 2497 ciclos de máquina

	decfsz		t0						;Decrementa tempo0 até que seja igual a zero
	goto		delay1					;Vai para a label aux1

										; 3 ciclos de máquina

										; (200 x 2497) + (200 x 3 - 1) = 499999 ciclos de máquina
										; 499999 + 4 + 2 =  500005 ciclos de máquina

										; d = ((10a - 1) + 8)b + (3b - 1) + 4 + 2
										; d = (10a + 7)b + 3b + 5
										; d = 10ab + 7b + 3b + 5
										; d = 10ab + 10b + 5
										; d = (10a + 10)b + 5
										; delay = (10 x t1 + 10) x t0 + 5
										; delay = (10 x 249 + 10) x 200 + 5 = 500005

	return								;Retorna após a chamada da sub rotina
	
	end									;Final do Programa	