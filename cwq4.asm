        .data
msg_error:
        .asciiz "Invalid Input !\n"
msg_input:
        .asciiz "Please enter a number bigger or equal to 0 : "
output: .asciiz "Result : "
        .text
        .globl main
        
main:
        la $a0, msg_input
        li $v0, 4
        syscall
        
        li $v0, 6               # load n
        syscall
        
        li.s $f8, 0.0
        c.le.s $f0, $f8
        bc1t error_output
        
        li.s $f9, 0.5
        li.s $f10 0.000001
        mov.s $f1, $f0          # let $f1(x0) = n
        mul.s $f2, $f1, $f9     # let $f2(x1) = n * 0.5

loop:
        sub.s $f1, $f1, $f2
        abs.s $f1, $f1          # calculate abs(x0-x1) to $f0
        c.le.s $f1, $f10        # check if abs(x0-x1) > 1e-6
        bc1t success_output
        
next:
        mov.s $f1, $f2          # run x0 = x1; x1 = 0.5*(x0 + n/x0);
        div.s $f2, $f0, $f1
        add.s $f2, $f1, $f2
        mul.s $f2, $f9, $f2
        j loop

error_output:
        la $a0, msg_error
        li $v0, 4
        syscall
        j exit
     
success_output:
        la $a0, output
        li $v0, 4
        syscall
        mov.s $f12, $f2         # print square root
        li $v0, 2
        syscall
        j exit
        
exit:
        li $v0, 10
        syscall