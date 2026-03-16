@256
D=A
@SP
M=D

@$.1
D=A
@SP
A=M
M=D
@SP
M=M+1

@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@0
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D

@Sys.init
0;JMP

($.1)












(Sys.init)


@4
D=A
@SP
A=M
M=D
@SP
M=M+1


@Sys.init$.2
D=A
@SP
A=M
M=D
@SP
M=M+1

@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@1
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D

@Main.fibonacci
0;JMP

(Sys.init$.2)

(END)

@END
0;JMP







(Main.fibonacci)

@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

@2
D=A
@SP
A=M
M=D
@SP
M=M+1

@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@TRUE_0
D;JLT
@SP
A=M
M=0
@END_0
0;JMP
(TRUE_0)
@SP
A=M
M=-1
(END_0)
@SP
M=M+1

@SP
M=M-1
A=M
D=M
@N_LT_2
D;JNE

@N_GE_2
0;JMP

(N_LT_2)

@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

@LCL
D=M
@5
D=D-A
A=D
D=M
@R15
M=D

@ARG
D=M
@0
A=D+A
D=A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D

@ARG
D=M+1
@SP
M=D
@1
D=A
@LCL
D=M-D
A=D
D=M
@THAT
M=D
@2
D=A
@LCL
D=M-D
A=D
D=M
@THIS
M=D
@3
D=A
@LCL
D=M-D
A=D
D=M
@ARG
M=D
@4
D=A
@LCL
D=M-D
A=D
D=M
@LCL
M=D
@R15
A=M
0;JMP

(N_GE_2)

@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

@2
D=A
@SP
A=M
M=D
@SP
M=M+1

@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
M=M-D
@SP
M=M+1

@Main.fibonacci$.3
D=A
@SP
A=M
M=D
@SP
M=M+1

@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@1
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D

@Main.fibonacci
0;JMP

(Main.fibonacci$.3)

@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

@1
D=A
@SP
A=M
M=D
@SP
M=M+1

@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
M=M-D
@SP
M=M+1

@Main.fibonacci$.4
D=A
@SP
A=M
M=D
@SP
M=M+1

@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@1
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D

@Main.fibonacci
0;JMP

(Main.fibonacci$.4)

@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
M=D+M
@SP
M=M+1

@LCL
D=M
@5
D=D-A
A=D
D=M
@R15
M=D

@ARG
D=M
@0
A=D+A
D=A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D

@ARG
D=M+1
@SP
M=D
@1
D=A
@LCL
D=M-D
A=D
D=M
@THAT
M=D
@2
D=A
@LCL
D=M-D
A=D
D=M
@THIS
M=D
@3
D=A
@LCL
D=M-D
A=D
D=M
@ARG
M=D
@4
D=A
@LCL
D=M-D
A=D
D=M
@LCL
M=D
@R15
A=M
0;JMP


