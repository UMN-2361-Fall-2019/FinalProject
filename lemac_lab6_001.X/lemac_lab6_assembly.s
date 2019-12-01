.include "xc.inc"
.text		    ;BP (put the following data in ROM(program memory))
; This is a library, thus it can *not* contain a _main function: the C file will
; deine main(). However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading
; underscore (_) and be included in a comment delimited list below.
.global  _lem_wait_100us, _lem_wait_1ms
    
 _lem_wait_1ms:      ;;2 calling
    repeat #15993    ;1 cycle to load and prep
    nop            ;15995+1 cycles to execute 
    return         ;3 cycles for the return
  ;16000 cycles 
    .end



