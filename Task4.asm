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

(CheckFirstIsNumber)
// KBD < 48, CheckFirstIsNumber
@KBD
D = M
@48
D = D - A
@CheckFirstIsNumber
D;JLT

// KBD > 57, CheckFirstIsNumber
@KBD
D = M
@57
D = D - A
@CheckFirstIsNumber
D;JGT

(GetInput)
// Get KeyBoard Value
@KBD
D = M
// If Value is NULL then loop
@GetInput
D;JEQ
// Check Equal Operator
@61
D = D - A
@FormatInput
D;JEQ
// Assign KBD value to Register
@KBD
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

@Exit

0;JMP

(Divide)
// Format R1
@4
D = A
@R1
M = D
@Exit
0;JMP

(Exit)
@5
D = A
@R3
M = D
@LOOP
0;JMP
