







(Sys.init)

@6
D=A
@SP
A=M
M=D
@SP
M=M+1

@8
D=A
@SP
A=M
M=D
@SP
M=M+1

@Sys.init$.1
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
@2
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D

@Class1.set
0;JMP

(Sys.init$.1)

@5
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

@23
D=A
@SP
A=M
M=D
@SP
M=M+1

@15
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
@2
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D

@Class2.set
0;JMP

(Sys.init$.2)

@5
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

@Sys.init$.3
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

@Class1.get
0;JMP

(Sys.init$.3)

@Sys.init$.4
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

@Class2.get
0;JMP

(Sys.init$.4)

(END)

@END
0;JMP







(Class1.set)

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

@StaticsTest/Class1.0
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
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

@StaticsTest/Class1.1
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

@0
D=A
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



(Class1.get)

@StaticsTest/Class1.0
D=M
@SP
A=M
M=D
@SP
M=M+1

@StaticsTest/Class1.1
D=M
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







(Class2.set)

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

@StaticsTest/Class2.0
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
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

@StaticsTest/Class2.1
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

@0
D=A
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



(Class2.get)

@StaticsTest/Class2.0
D=M
@SP
A=M
M=D
@SP
M=M+1

@StaticsTest/Class2.1
D=M
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

