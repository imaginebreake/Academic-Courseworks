        .data
string: .space 256
origin: .space 20
new:.space 20
msg1:   .asciiz "Input string: "
msg2:   .asciiz "Input character: "
msg3:   .asciiz "Output: "

        .text
        .globl main
        
main:
        la $a0, msg1          # print input string
        li $v0, 4
        syscall
        
        li $v0, 8             # read input string
        la $a0, string
        li $a1, 256
        syscall
        
        la $a0, msg2          # print input origin character
        li $v0, 4
        syscall
        
        li $v0, 8             # read origin character
        la $a0, origin
        li $a1, 20
        syscall
        
        la $a0, msg2          # print input new character
        li $v0, 4
        syscall
        
        li $v0, 8             # read new character
        la $a0, new
        li $a1, 20
        syscall
        
        la $t1, origin
        lb $a1, 0($t1)        # get the byte of origin character
        la $t2, new
        lb $a2, 0($t2)        # get the byte of new character
        la $a0, string        # point $a0 to the start of string
        
        jal replace
        
exit:
        la $a0, string
        li $v0, 4
        syscall
        li $v0, 10
        syscall
        
replace:
        move $t1, $a1
        move $t2, $a2
replace_loop:
        addi $a0, $a0, 1      # point $a0 to next character
replace_cond:
        lb $t0, ($a0)         # load the single character in string to $t0
        beq $t0, $zero, replace_exit  # if $t0 is null then do exit
        bne $t0, $t1, replace_loop    # check if this character != origin character. if yes, jump to next
        sb $t2, ($a0)         # store byte in $t2 (new character) to $a0
        j replace_loop
replace_exit:
        jr $ra                # jump back

        
