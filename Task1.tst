//load Task1.hack,
load Task1.asm,
output-file Task1.out,
compare-to Task1.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2;

set RAM[0] 10,   // Set test arguments
set RAM[1] 3,
set RAM[2] -1;  // 3
repeat 200 {
  ticktock;
}
output;

set PC 0,
set RAM[0] 10,   // Set test arguments
set RAM[1] 2,
set RAM[2] -1;  // 5
repeat 200 {
  ticktock;
}
output;

set PC 0,
set RAM[0] 10,   // Set test arguments
set RAM[1] 20,
set RAM[2] -1;  // 0
repeat 200 {
  ticktock;
}
output;
