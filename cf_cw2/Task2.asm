// Task2
//
// this program output the round-off value of RAM[0]/RAM[1]
// and store in RAM[1]
//
// z = -1
// while (x>=0) {x = x - y; z++}
// if (y > -2x) then z = z + 1

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

// start the while loop
(LOOP)
// if x<0 than jump out the loop
@x
D = M
@ROUNDUP
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

// check round-up
(ROUNDUP)
// if y+2x>0 then z++
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


// store data in z to RAM[2]
(STORE)
@z
D = M
@R2
M = D
@Exit
0;JMP

// end of program
(Exit)
@Exit
0;JMP