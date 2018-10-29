        .data
string: .space 256
origin: .space 20
replace:.space 20
msg1:   .asciiz "Input string: "
msg2:   .asciiz "Input character: "
msg3:   .asciiz "Output: "

        .text
        .globl main
        
main:
        la $a0, msg1
        li $v0, 4
        syscall
        
        li $v0, 8
        la $a0, string
        li $a1, 256
        syscall
        
        la $a0, msg2
        li $v0, 4
        syscall
        
        li $v0, 8
        la $a0, origin
        li $a1, 20
        syscall
        
        la $a0, msg2
        li $v0, 4
        syscall
        
        li $v0, 8
        la $a0, replace
        li $a1, 20
        syscall
        
        la $s0, origin
        lb $s0, 0($s0)
        la $s1, replace
        lb $s1, 0($s1)
        la $a0, string
        
loop:
        lb $t0, ($a0)
        bne $t0, $s0, next
        sb $s1, ($a0)

next:  
        addi $a0, $a0, 1
        lb $t0, ($a0)
        bne $t0, $zero, loop
        j exit
        
exit:
        la $a0, string
        li $v0, 4
        syscall
        li $v0, 10
        syscall