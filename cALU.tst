load cALU.hdl,
output-file cALU.out,
compare-to cALU.cmp,
output-list time%S1.4.1 x%B1.16.1 y%B1.16.1 nx%B1.1.1 zy%B1.1.1 
            ny%B1.1.1 no%B1.1.1 cx%B1.1.1 re%B1.1.1 
            op%B1.1.1 out%B1.16.1 zr%B1.1.1 ng%B1.1.1 of%B1.1.1;      
//negate x
//-2
set x %B0000000000000010,
set y %B0000000000000100,
set nx 0,
set zy 1,
set ny 1,
set no 1,
set cx 0,
set re 0,
set op 0,
eval,
tick,
output;
tock,
output;

// Compute x + 1
//3
set nx 1,
set zy 1,
set ny 1,
set no 1,
set cx 0,
set re 0,
set op 0,
eval,
tick,
output;
tock,
output;

// Compute x - 1
//2
set nx 0,
set zy 1,
set ny 1,
set no 0,
set cx 0,
set re 0,
set op 0,
eval,
tick,
output;
tock,
output;

// Compute x + y
//6
set nx 0,
set zy 0,
set ny 0,
set no 0,
set cx 0,
set re 0,
set op 0,
eval,
tick,
output;
tock,
output;

// Compute x - y
//-2
set nx 1,
set zy 0,
set ny 0,
set no 1,
set cx 0,
set re 0,
set op 0,
eval,
tick,
output;
tock,
output;

// Compute y - x
//2
set nx 0,
set zy 0,
set ny 1,
set no 1,
set cx 0,
set re 0,
set op 0,
eval,
tick,
output;
tock,
output;

// Compute out+1
//3
set nx 1,
set zy 1,
set ny 1,
set no 1,
set cx 1,
set re 0,
set op 0,
eval,
tick,
output;
tock,
output;

// Compute out+1
//4
set nx 1,
set zy 1,
set ny 1,
set no 1,
set cx 1,
set re 0,
set op 0,
eval,
tick,
output;
tock,
output;

