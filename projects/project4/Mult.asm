// Computes Ram[2] = Ram[0] * Ram[1]
// i = 1
@i
M=1
// sum = 0
@sum
M=0
(LOOP)
// if (i > Ram[1]) go to stop
@i 
D=M
@R1
D=D-M
@STOP
D;JGT
// sum = sum + Ram[0]
@sum
D=M
@R0
D=D+M
@sum
M=D

// i = i + 1
@i
M=M+1

// go to loop
@LOOP
0;JMP

(STOP)
@sum
D=M

// Ram[2] = sum
@R2
M=D

// go to end for infinity
(END)
@END
0;JMP