// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed.
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
(START)
      @KBD
      D=M
      @i
      M=0
      @WHITE
      D;JEQ
(BLACK)
      @i
      D=M
      @SCREEN
      A=A+D //SCREEN+i
      M=-1   //kuroにする
      @i
      D=M   //D=i
      @8191
      D=D-A //D=i=8191になったら全部kuroということでSTARTに戻る
      @START

      D;JEQ
      @i
      M=M+1
      @BLACK
      0;JMP
(WHITE)
      @i
      D=M
      @SCREEN
      A=A+D //SCREEN+i
      M=0   //siroにする
      @i
      D=M   //D=i
      @8191
      D=D-A //D=i=8191になったら全部白ということでSTARTに戻る
      @START

      D;JEQ
      @i
      M=M+1
      @WHITE
      0;JMP
