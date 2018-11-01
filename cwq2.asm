        .data
overflow:
        .asciiz "Overflow!\n"
        .text
        .globl main
        
main:   
        li $v0, 5
        syscall
        move $s6, $v0                 # load x in $s6
        li $v0, 5
        syscall
        move $s7, $v0                 # load y in $s7
        
        move $s1, $s6
        move $s2, $s7
        jal add_with_check            # get (x+y) in $s0
        move $s3, $s0                 # load (x+y) in $s3
        
        move $s1, $s0
        move $s2, $s0
        jal mul_with_check            # get (x+y)(x+y) in $s0
        
        move $s1, $s0                 # move (x+y)(x+y) to $s1
        move $s2, $s3                 # move (x+y) to $s2
        jal mul_with_check            # get (x+y)(x+y)(x+y) in $s0
        move $s5, $s0                 # load (x+y)(x+y)(x+y) in $s5
        
        move $s1, $s7
        move $s2, $s7
        jal mul_with_check            # get (y)(y) in $s0
        
        move $s1, $s0
        move $s2, $s7
        jal mul_with_check            # get (y)(y)(y) in $s0
        
        move $s1, $s0
        li $s2, 8
        jal mul_with_check            # get 8(y)(y)(y) in $s0
        
        move $s1, $s0
        move $s2, $s5
        jal add_with_check
        
        # print the result
        move $a0, $s0
        li $v0, 1
        syscall
        j exit
        
add_with_check:                       # there are two condition leads to a overflow in add (a+b=c):
        addu $s0, $s1, $s2            # 1. sign of a and b are same && 2. sign of a or b and c are different
        xor $t0, $s1, $s2             # if sign of $s1 and $s2 are diff then $t0 is less than 0
        blt $t0, $zero, no_overflow   # if $t0 less than 0 then there is no overflow
        xor $t0, $s0, $s1             # if sign of $s0 and $s1 are same then $t0 is greater than 0
        bge $t0, $zero, no_overflow   # if $t0 greater than 0 (means sign of c and a have same sign), no overflow
        j print_overflow              # if does not match all no_overflow condition, then there is a overflow

mul_with_check:
        mult $s1, $s2                 # result is in lo. when result<0, hi = FFFFFFFF;result>=0, hi = 00000000; if overflow, 0 and 1 will flow in to hi.
        mfhi $t0
        mflo $s0
        sra $t1, $s0, 31              # $t1 = 00000000 or FFFFFFFF after sra
        beq $t0, $t1, no_overflow     # if highest bit in lo duplicate to 32bit == bits in ho, no overflow. 
        j print_overflow              # overflow

print_overflow:
        la $a0, overflow
        li $v0, 4
        syscall
        j exit                        # jump back to main
        
no_overflow:
        jr $ra
        
exit:
        # end of program
        li $v0, 10
        syscall
