        .data
string: .space 20
origin: .space 20
replace:.space 20
msg1:   .asciiz "Input string: "
msg2:   .asciiz "Input character: "
msg3:   .asciiz "Output: "

        .text
        .globl main
        
main:
        li $v0, 6
        syscall
        li.s $f9, 0.5
        li.s $f10 0.000001
        mov.s $f1, $f0
        mul.s $f2, $f1, $f9

loop:
        sub.s $f1, $f1, $f2
        abs.s $f1, $f1
        c.le.s $f1, $f10
        bc1t exit
        
next:
        mov.s $f1, $f2
        div.s $f2, $f0, $f1
        add.s $f2, $f1, $f2
        mul.s $f2, $f9, $f2
        j loop

exit:
        mov.s $f12, $f2 
        li $v0, 2
        syscall
        li $v0, 10
        syscall