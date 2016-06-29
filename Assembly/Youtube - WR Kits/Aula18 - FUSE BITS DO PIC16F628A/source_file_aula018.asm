;
; Curso de Assembly para PIC Aula 018
;
; FUSE Bits do PIC16F628A
;
; Deseja-se configurar os FUSE Bits do PIC16F628A para as seguintes características:
;
; - Cristal oscilador externo 4MHz
; - Watch Dog Timer desabilitado
; - Power Up Timer habilitado
; - Master Clear habilitado
; - Brown Out desabilitado
; - Sem programação em baixa tensão
; - Sem proteção da memória de dados
; - Sem proteção de código
;  
;
; MCU: PIC16F628A   Clock: 4MHz
;
;
; Sistema Embarcado Sugerido: PARADOXUS PEPTO
;
; Disponível em https://wrkits.com.br/catalog/show/141 
;
; www.wrkits.com.br | facebook.com/wrkits | youtube.com/canalwrkits
;
; Autor: Eng. Wagner Rambo   Data: Março de 2016
;

	list	p=16f628a						;listagem do processador utilizado
	
	
	#include <p16f628a.inc>					;inclusão do arquivo com os registradores do PIC16F628A
	
	
; --- FUSE Bits ---

	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _MCLRE_ON & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _CP_OFF