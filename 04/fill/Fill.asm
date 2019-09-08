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
