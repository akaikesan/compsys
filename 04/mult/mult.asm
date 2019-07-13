// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

    @R1
    D=M
    @counter
    M=D

(LOOP)
    @counter
    M=M-1
    D=M
    @END
    D;JLT
    @R0
    D=M
    @sum
    M=M+D
    @LOOP
    0;JMP
(END)
    @sum
    D=M
    M=0
    @R2
    M=D
