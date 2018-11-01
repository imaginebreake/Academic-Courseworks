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
        la $a0, replace
        li $a1, 20
        syscall
        
        la $s0, origin
        lb $s0, 0($s0)        # get the byte of origin character
        la $s1, replace
        lb $s1, 0($s1)        # get the byte of new character
        la $a0, string        # point $a0 to the start of string
        
loop:
        lb $t0, ($a0)         # load the signal character in string to $t0
        bne $t0, $s0, next    # check if this character != origin character. if yes, jump to next
        sb $s1, ($a0)         # store byte in $s1 (new character) to $s1

next:  
        addi $a0, $a0, 1      # point $a0 to next character
        lb $t0, ($a0)         # load the signal character in string to $t0
        bne $t0, $zero, loop  # if $t0 is not null then do loop
        j exit                # to the string end, exit the program
        
exit:
        la $a0, string
        li $v0, 4
        syscall
        li $v0, 10
        syscall