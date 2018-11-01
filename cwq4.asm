        .data

        .text
        .globl main
        
main:
        li $v0, 6               # load n
        syscall
        li.s $f9, 0.5
        li.s $f10 0.000001
        mov.s $f1, $f0          # let $f1(x0) = n
        mul.s $f2, $f1, $f9     # let $f2(x1) = n * 0.5

loop:
        sub.s $f1, $f1, $f2
        abs.s $f1, $f1          # calculate abs(x0-x1) to $f0
        c.le.s $f1, $f10        # check if abs(x0-x1) > 1e-6
        bc1t exit
        
next:
        mov.s $f1, $f2          # run x0 = x1; x1 = 0.5*(x0 + n/x0);
        div.s $f2, $f0, $f1
        add.s $f2, $f1, $f2
        mul.s $f2, $f9, $f2
        j loop

exit:
        mov.s $f12, $f2         # print square root
        li $v0, 2
        syscall
        li $v0, 10
        syscall