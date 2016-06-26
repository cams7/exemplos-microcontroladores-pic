;
; Curso de Assembly para PIC Aula 002
;
; MCU: PIC16F84A   Clock: 4MHz
;
; Autor: Eng. Wagner Rambo   Data: Novembro 2015
;


  list		p=16f84A				; microcontrolador utilizado PIC16F84A
  
  
; --- Arquivos incluídos no projeto ---
  #include <p16f84a.inc>			; inclui o arquivo do PIC16F84A  
  
  
; --- FUSE Bits ---
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF
