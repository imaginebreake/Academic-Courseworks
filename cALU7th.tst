load calu.hdl,
output-file calu7th.out,
output-list time%S1.4.1 x%B1.16.1 y%B1.16.1 nx%B1.1.1 zy%B1.1.1 
            ny%B1.1.1 no%B1.1.1 cx%B1.1.1 re%B1.1.1 
            op%B1.1.1 out%B1.16.1 zr%B1.1.1 ng%B1.1.1 of%B1.1.1;      
//2*4
set x %B0000000000000010,
set y %B0000000000000100,
set nx 0,
set zy 0,
set ny 0,
set no 0,
set cx 0,
set re 0,
set op 1,
eval,
tick,
output;
tock,
output;

// 38*36
set x %B0000000000100110,
set y %B0000000000100100,
set nx 0,
set zy 0,
set ny 0,
set no 0,
set cx 0,
set re 0,
set op 1,
eval,
tick,
output;
tock,
output;

// -55*55
set x %B1111111111001001,
set y %B0000000000110111,
set nx 0,
set zy 0,
set ny 0,
set no 0,
set cx 0,
set re 0,
set op 1,
eval,
tick,
output;
tock,
output;

// 450*400 (overflow)
set x %B0000000111000010,
set y %B0000000110010000,
set nx 0,
set zy 0,
set ny 0,
set no 0,
set cx 0,
set re 0,
set op 1,
eval,
tick,
output;
tock,
output;
