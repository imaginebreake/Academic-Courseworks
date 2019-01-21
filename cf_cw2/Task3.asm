// Task3
//
// this program output the round-off value of RAM[0]/RAM[1]
// and store in RAM[1]
//
//
// if y = 0 then {R3=-1; exit;}
// sign = 1
// if x<0 {x=-x; sign=-sign;}
// if y<0 {y=-y; sign=-sign;}
// z = -1
// while (x>=0) {x = x - y; z++}
// z = z*sign
// if (y >= -2x and z>0) then z = z + 1
// if (y > -2x and z<0) then z = z - 1

// create variable x = RAM[0]
@R0
D = M
@x
M = D
// create variable y = RAM[1]
@R1
D = M
@y
M = D
// create variable z = -1
@z
M = -1
// create variable sign = 1
@sign
M = 1

// check y==0
@y
D = M
@yIsZero
D;JEQ
@xAbs
0;JMP
// if y==0 then give R3 value and exit
(yIsZero)
@R3
M = -1
@Exit
0;JMP

// absolute x
(xAbs)
@x
D = M
@xLessZero
D;JLT
@yAbs
0;JMP
(xLessZero)
@x
M = -M
// negate sign at the same time
@sign
M = -M

// absolute y
(yAbs)
@y
D = M
@yLessZero
D;JLT
@LOOP
0;JMP
(yLessZero)
@y
M = -M
// negate sign at the same time
@sign
M = -M

// start the while loop to divide
(LOOP)
// if x<0 than jump out the loop
@x
D = M
@AssignSign
D;JLT
// x = x - y
@y
D = M
@x
M = M - D
// z++
@z
M = M + 1
// loop again
@LOOP
0;JMP

// assign sign to z
(AssignSign)
@sign
D = M
@zNegate
D;JLT
@RoundPos
0;JMP
(zNegate)
@z
M = -M
@RoundNeg
0;JMP

// check round-up for positive number
(RoundPos)
// if y+2x>=s0 then z++
@x
D = M
D = D + M
@y
D = D + M
@STORE
D;JLT
@z
M = M + 1
@STORE
0;JMP

// check round-up for negative number
(RoundNeg)
// if y+2x>s0 then z--
@x
D = M
D = D + M
@y
D = D + M
@STORE
D;JLE
@z
M = M - 1
@STORE
0;JMP

// store data in z to RAM[2]
(STORE)
@z
D = M
@R2
M = D
@R3
M = 1
@Exit
0;JMP

// end of program
(Exit)
@Exit
0;JMP