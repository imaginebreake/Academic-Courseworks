        .data
space:  .asciiz " "   
newline:.asciiz "\n"
        .text
        .globl main
main:   
        li $v0, 5
        syscall
        move $s0, $v0
        li $t0, 0
loop1:  
        beq $t0, $s0, exit              # exit when loop1 to end
        addi $t0, $t0, 1                # t0++ if not to end, print number form 1 to t0 by loop2
        li $t1, 1               
        j loop2                         # start loop2 and print one line
        j loop1                         # excute loop1 again in a newline
loop2:
        move $a0, $t1
        li $v0, 1
        syscall
        la $a0, space
        li $v0, 4
        syscall
        beq $t0, $t1, printnewline      # jump to printnewline when loop2 to end
        addi $t1, $t1, 1                # if loop2 not to end, $t1++
        j loop2                         # loop2 excute again
printnewline:
        la $a0, newline
        li $v0, 4
        syscall
        j loop1                         # print a /n for newline
exit:
        li $v0, 10
        syscall

        
        
        