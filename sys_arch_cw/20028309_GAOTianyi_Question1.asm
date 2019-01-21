        .data
space:  .asciiz " "   
newline:.asciiz "\n"
msg_error:
        .asciiz "Invalid Input !"
msg_input:
        .asciiz "Please enter a positive number : "
        .text
        .globl main
        
main:   
        la $a0, msg_input           
        li $v0, 4
        syscall
        li $v0, 5
        syscall		                  # input the number
        ble $v0, $zero, error       # check number
        move $a0, $v0
        jal printlines
exit:
        li $v0, 10
        syscall
        
printlines:
        move $t1, $a0               # load $a0
        li $t0, 0                   # set the t0 to 0 for loop
        j printlines_loop
printlines_loop:
        addi $t0 ,$t0, 1
printlines_cond:
        addi $sp, $sp, -12          # allocate space on the stack
        sw $ra, 8($sp)              # save $ra -- push
        sw $t1, 4($sp)              # save $t1 -- push
        sw $t0, 0($sp)              # save $t0 -- push
        move $a0, $t0
        jal printline               # print singel line
        lw $ra, 8($sp)              # restore $ra -- push
        lw $t1, 4($sp)              # restore $t1 -- push
        lw $t0, 0($sp)              # restore $t0 -- push
        addi $sp, $sp, 12           # free space on the stack
        blt $t0, $t1, printlines_loop
        jr $ra
        
printline:
        move $t1, $a0
        li $t0, 0
        j printline_loop
printline_loop:
        addi $t0 ,$t0, 1
printline_cond:
        move $a0, $t0
        li $v0, 1
        syscall                         # print number
        la $a0, space
        li $v0, 4
        syscall                         # print space
        blt $t0, $t1, printline_loop
        la $a0, newline
        li $v0, 4
        syscall                         # print a /n for newline when the loop reaches to the end
        jr $ra
        
error:
        la $a0, msg_error
        li $v0, 4
        syscall
        j exit

        

        
        
        