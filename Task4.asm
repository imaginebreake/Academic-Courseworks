// Task4
//
// this program implement the single-digit arithmetic calculation
// output the calculation result
//
// 

(LOOP)

// i is a iterator
@i
M = 0
// store previous keyboard input
@PreKBDtmp
// store the KBD value for following constructions
@KBDtmp
M = 0

(GetInput)
// Get KeyBoard Value
@KBD
D = M
@KBDtmp
M = D
// If Value is NULL then loop
@GetInput
D;JEQ
// Check Same Input
@PreKBDtmp
D = M
@KBDtmp
D = D - M
@GetInput
D;JEQ
// Assign KBDtmp value to PreKBDtmp
@KBDtmp
D = M
@PreKBDtmp
M = D
// Check Equal Operator
@61
D = D - A
@FormatInput
D;JEQ
// Assign KBDtmp value to Register
@KBDtmp
D = M
M = 0
@i
A = M
M = D
@i
M = M + 1
// loop to receive next value
@GetInput
0;JMP

(FormatInput)
// Clear Data
@R3
M = 0
@R4
M = 0
// Change number from ASCII to actual value
// first number
@R0
D = M
@48
D = D - A
@R0
M = D
// second number
@R2
D = M
@48
D = D - A
@R2
M = D

// Formant and Choose Operator
// Add
@R1
D = M
@43
D = D - A
@Add
D;JEQ
// Minus
@R1
D = M
@45
D = D - A
@Minus
D;JEQ
// Multiply
@R1
D = M
@42
D = D - A
@Multiply
D;JEQ
// Divide
@R1
D = M
@47
D = D - A
@Divide
D;JEQ
// Not operator matched, restart program
@LOOP
0;JMP

(Add)
// Format R1
@1
D = A
@R1
M = D
// add
@R0
D = M
@R2
D = D + M
@R4
M = D
@Exit
0;JMP

(Minus)
// Format R1
@2
D = A
@R1
M = D
// minus
@R0
D = M
@R2
D = D - M
@R4
M = D
@Exit
0;JMP


(Multiply)
// Format R1
@3
D = A
@R1
M = D
// multiply
// create variable x = RAM[0]
@R0
D = M
@x
M = D
// create variable y = RAM[2]
@R2
D = M
@y
M = D
// Loop of multiply
(MulLoop)
// y--
@y
M = M - 1
// R4 = R4 + x
@x
D = M
@R4
M = D + M
// if y==0 break
@y
D = M
@Exit
D;JLE
@MulLoop
0;JMP //GOTO MulLoop


(Divide)
// Format R1
@4
D = A
@R1
M = D
// divide
// create variable x = RAM[0]
@R0
D = M
@x
M = D
// create variable y = RAM[2]
@R2
D = M
@y
M = D
// create variable RAM[4] = -1
@R4
M = -1
// start loop for divide
(DivLOOP)
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
// RAM[4]++
@R4
M = M + 1
// loop again
@DivLOOP
0;JMP
// check round-up
(ROUNDUP)
// if y+2x>0 then RAM[4]++
@x
D = M
D = D + M
@y
D = D + M
@Exit
D;JLT
@z
M = M + 1
@Exit
0;JMP

(Exit)
@5
D = A
@R3
M = D
@LOOP
0;JMP