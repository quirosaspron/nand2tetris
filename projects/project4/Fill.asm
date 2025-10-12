// Reads the keyboard, if a key is pressed it fills the screes

// Read keybord
(START)
@24576
D=M
@BLACK
D;JGT // If Ram[24576] > 0 go to BLACK
@WHITE
D;JEQ // Else go to WHITE

(BLACK)
@SCREEN
D=A
@addr
M=D // Initial adress Ram[SCREEN]

@8191 // We need to paint 8192 16bit registers
D=A
@n
M=D
@i
M=0
(BLOOP) // Black loop
@i
D=M
@n
D=D-M
@START // if i > n go to start
D;JGT

@addr
A=M
M=-1 //Turn the register black
@i
M=M+1 // i = i + 1
@1
D=A
@addr
M=D+M // addr = addr + 1
@BLOOP
0;JMP

(WHITE)
@SCREEN //Ram[16384]
D=A
@addr
M=D // Initial adress Ram[SCREEN]

@8191 // We need to paint 8192 16bit registers
D=A
@n
M=D
@i
M=0
(WLOOP)
@i
D=M
@n
D=D-M
@START // if i > n go to start
D;JGT

@addr
A=M
M=0 //Turn the register white
@i
M=M+1 // i = i + 1
@1
D=A
@addr
M=D+M // addr = addr + 1
@WLOOP // Go to white loop
0;JMP