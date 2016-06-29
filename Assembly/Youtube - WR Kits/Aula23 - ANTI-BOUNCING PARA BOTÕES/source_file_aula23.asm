;
; Curso de Assembly para PIC WR Kits Aula 23
;
;
; Aula prática - Anti-Bouncing para botões utilizando software
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
	list	p=16F628A						;Utilizado PIC16F628A
	
	
; --- Arquivos Inclusos no Projeto ---
	#include <p16F628a.inc>					;inclui o arquivo do PIC 16F628A
	
	
; --- FUSE Bits ---
; - Cristal de 4MHz
; - Desabilitamos o Watch Dog Timer
; - Habilitamos o Power Up Timer
; - Brown Out desabilitado
; - Sem programação em baixa tensão, sem proteção de código, sem proteção da memória EEPROM
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CP_OFF & _CPD_OFF & _MCLRE_ON
	
	
; --- Paginação de Memória ---
	#define		bank0	bcf	STATUS,RP0		;Cria um mnemônico para selecionar o banco 0 de memória
	#define		bank1	bsf	STATUS,RP0		;Cria um mnemônico para selecionar o banco 1 de memória
	
	
; --- Saídas (Dispositivos onboard PEPTO) ---
	#define     led1	PORTA,3				;LED1 onboard ligado ao pino RA3 (modo current sourcing) 0 - desliga 1 - liga
	#define		led2	PORTA,2				;LED2 onboard ligado ao pino RA2 (modo current sourcing) 0 - desliga 1 - liga
	
	
; --- Entrada (Dispositivo onboard PEPTO) ---
	#define		S1		PORTB,0				;Botão S1 ligado em RB0 (botão pressionado - nível low)
	
	
; --- Registradores de Uso Geral ---
	cblock		H'20'						;Início da memória disponível para o usuário
	
	bouncing								;"anti trepidação" para o botão
 
	
	
	endc									;Final da memória do usuário
	

; --- Constantes ---
	v_bouncing		equ		D'250'			;Valor do bouncing
 
	
	
	
; --- Vetor de RESET ---
	org			H'0000'						;Origem no endereço 00h de memória
	goto		inicio						;Desvia para a label início
	

; --- Vetor de Interrupção ---
	org			H'0004'						;As interrupções deste processador apontam para este endereço
	retfie									;Retorna da interrupção
	
	
inicio:
	bank0									;Seleciona o banco 0 de memória
	movlw		H'07'						;w = 7h
	movwf		CMCON						;CMCON = 7h (apenas I/Os digitais) desabilita os comparadores internos
	bank1									;Seleciona o banco 1 de memória
	movlw		H'00'						;w = 00h
	movwf		OPTION_REG					;Habilita resistores de pull-up internos
	movlw		H'13'						;w = 03h
	movwf		TRISA						;Configura saídas para os leds onboard
	movlw		H'FF'						;w = FFh
	movwf		TRISB						;Configura todo o PORTB como entrada
	bank0									;Seleciona o banco 0 de memória
	
	bsf			led1						;LED1 inicia ligado
	bcf			led2						;LED2 inicia desligado
	
	
 
	
	
loop:

	movlw		v_bouncing					;Move o valor de bouncing para work
	movwf		bouncing					;Registrador bouncing recebe o conteúdo de v_bouncing


test_butt:

	btfsc		S1							;Botão pressionado?
	goto		loop						;Não, volta para loop e continua aguardando
	decfsz		bouncing,F					;Sim. Decrementa bouncing. bouncing igual a zero?
	goto		test_butt					;Não, volta para aux
	
	comf		PORTA						;Sim, inverte o estado de PORTA (leds onboard)
	

	goto		loop						;volta para loop
	

	
	end										;Final do Programa
	
	