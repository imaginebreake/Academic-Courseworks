//load Task2.hack,
load Task2.asm,
output-file Task2.out,
compare-to Task2.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2;

set RAM[0] 10,   // Set test arguments
set RAM[1] 3,
set RAM[2] -1;  // 3
repeat 200 {
  ticktock;
}
output;

set PC 0,
set RAM[0] 13,   // Set test arguments
set RAM[1] 5,
set RAM[2] -1;  // 3
repeat 200 {
  ticktock;
}
output;

set PC 0,
set RAM[0] 4,   // Set test arguments
set RAM[1] 10,
set RAM[2] -1;  // 0
repeat 200 {
  ticktock;
}
output;
