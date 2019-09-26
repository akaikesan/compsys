#include<bits/stdc++.h>
#include<map>
#include<fstream>
#include<iostream>

enum command{
  C_ARITHMETIC,C_PUSH,C_POP,C_LABEL,C_GOTO,C_IF,C_FUNCTION,C_RETURN,C_CALL,C_NO
};

int PS = 0;

class Parser{

private:
  std::ifstream reading_file;
  std::string reading_line_buffer;
public:
  Parser(std::string file){
    reading_file.open(file,std::ios::in);
  }

  bool hasMoreCommands()
  {
    return reading_file.eof();
  }

  void advance()
  {
    getline(reading_file,reading_line_buffer);
  }

  command commandType(){


    std::regex co(R"(//.*+)");//Commentout
    std::regex space(R"(\s\s+)");//Space
    if (std::regex_search(reading_line_buffer,co)) reading_line_buffer = std::regex_replace(reading_line_buffer,co,"");

    if (std::regex_search(reading_line_buffer,space)) reading_line_buffer = std::regex_replace(reading_line_buffer,space,"");

    //from here, Spaces and comments in reading_line_buffer is removed.

    std::regex arithmetic(R"(add|sub|neg|eq|gt|lt|and|or|not)");
    std::regex push(R"(push)");
    std::regex pop(R"(pop)");
    std::regex label(R"(label)");
    std::regex gt(R"(goto)");
    std::regex igoto(R"(if-goto)");
    std::regex function(R"(function)");
    std::regex call(R"(call)");
    std::regex rtn(R"(return)");

    if(std::regex_search(reading_line_buffer,arithmetic)) return C_ARITHMETIC;

    else if (std::regex_search(reading_line_buffer,push)) return C_PUSH;

    else if (std::regex_search(reading_line_buffer,pop)) return C_POP;

    else if (std::regex_search(reading_line_buffer,label)) return C_LABEL;

    else if (std::regex_search(reading_line_buffer,igoto)) return C_IF;

    else if (std::regex_search(reading_line_buffer,gt)) return C_GOTO;

    else if (std::regex_search(reading_line_buffer,function)) return C_FUNCTION;

    else if (std::regex_search(reading_line_buffer,call)) return C_CALL;

    else if (std::regex_search(reading_line_buffer,rtn)) return C_RETURN;

    else return C_NO;

  }



  std::string arg1(){

    if (commandType() != C_RETURN){

      if(commandType() == C_ARITHMETIC){
        std::regex arithmetic(R"((add|sub|neg|eq|gt|lt|and|or|not))");
        std::smatch results;
        std::regex_search(reading_line_buffer,results,arithmetic);
        return results[1].str();
      }

      else{
        std::regex splitter(R"((\w+)(\s)([\w\.\:]+)(\s*))");
        std::smatch results;
        std::regex_search(reading_line_buffer,results,splitter);
        return results[3].str();
      }
    }

    else{
      return  "";
    }


  }




  int arg2(){
    if((commandType() == C_PUSH || commandType() == C_POP) || (commandType() == C_FUNCTION||commandType() == C_CALL)){
      std::regex splitter(R"((\w+)(\s)([\w\.\:]+)(\s)(\d+))");
      std::smatch results;
      if(std::regex_search(reading_line_buffer,results,splitter)) return stoi(results[5]);

    }

  }




};



class CodeWriter{
private:
  int lctr = 0;
  int rtnc = 0;
  std::ofstream outputfile;


public:



  std::map<std::string,int> mp;
  CodeWriter(){
    mp["local"] = 1;
    mp["argument"] = 2;
    mp["this"] = 3;
    mp["that"] = 4;
    mp["temp"] = 5;
    mp["static"] = 16;
    mp["pointer"] = 3;

  }

  void setFileName(std::string fn){
    outputfile.open(fn,std::ios::out | std::ios::ate);
  }

  void writeArithmetic(std::string op){


    outputfile
    << "@SP" << std::endl;

    if(op != "neg" && op != "not" ){

      outputfile
      << "M=M-1"<<std::endl
      << "A=M" << std::endl
      << "D=M" << std::endl
      << "A=A-1" << std::endl;

      if(op =="add")outputfile << "M=M+D" << std::endl;
      else if(op == "sub") outputfile << "M=M-D" << std::endl;
      else if(op == "and") outputfile << "M=D&M" << std::endl;
      else if(op == "or") outputfile << "M=D|M" << std::endl;

      else{
        if(op == "eq") {
          outputfile << "D=M-D" << std::endl
          << "@P" << lctr << std::endl
          << "D;JEQ" << std::endl;

        }
        else if(op == "gt") {
          outputfile << "D=M-D" << std::endl
          << "@P" << lctr << std::endl
          << "D;JGT" << std::endl;
        }
        else if(op == "lt") {
          outputfile << "D=M-D" << std::endl
          << "@P" << lctr << std::endl
          << "D;JLT" << std::endl;

        }
        outputfile
        << "@SP" << std::endl
        << "A=M" << std::endl
        << "A=A-1" << std::endl
        << "M=0" << std::endl
        << "@P" << lctr+1 << std::endl
        << "0;JMP" <<std::endl
        << "(P" << lctr << ")"<< std::endl;
        lctr++;
        outputfile
        << "@SP" << std::endl
        << "A=M" << std::endl
        << "A=A-1" << std::endl
        << "M=-1" << std::endl
        << "(P" << lctr << ")"<< std::endl;
        lctr++;

      }


    }
    else{
      outputfile
      << "A=M" << std::endl
      << "A=A-1" << std::endl;
      if(op == "neg") {
        outputfile << "M=-M" << std::endl;
      }
      else{
        outputfile << "M=!M" << std::endl;
      }

    }

    outputfile.flush();



  }

  void writePushPop(command cmd,std::string segment,int index){

    if (cmd == C_PUSH){
      outputfile << "//PUSH"<< std::endl;
      if (segment != "constant"){
        if(segment == "this"){
          outputfile
          << "@THIS" << std::endl
          << "D=M" <<std::endl;
        }
        else if(segment == "that"){
          outputfile
          << "@THAT" << std::endl
          << "D=M" <<std::endl;
        }
        else if(segment == "local"){
          outputfile
          << "@LCL" << std::endl
          << "D=M" <<std::endl;
        }
        else if(segment == "argument"){
          outputfile
          << "@ARG" << std::endl
          << "D=M" <<std::endl;
        }
        else{
          outputfile
          << "@" << mp[segment] << std::endl
          << "D=A" << std::endl;
        }
        outputfile
        << "@" << index << std::endl
        << "A=D+A" << std::endl
        << "D=M" << std::endl
        << "@SP" << std::endl    //this 2 lines implie M refers to
        << "A=M" << std::endl    //the highest stack
        << "M=D" << std::endl
        << "@SP" << std::endl       //this 2 lines implies M refers to
        << "M=M+1" << std::endl;   //increment of the SP

      }
      else
      {
        outputfile
        << "@" << index << std::endl
        << "D=A" << std::endl
        << "@SP" << std::endl
        << "A=M" << std::endl
        << "M=D" << std::endl
        << "@SP" << std::endl
        << "M=M+1" << std::endl;
      }
    }
    else if(cmd == C_POP){
      outputfile << "//POP" << std::endl;
      if (segment != "constant"){
        if(segment == "this"){
          outputfile
          << "@THIS" << std::endl
          << "D=M" <<std::endl;
        }
        else if(segment == "that"){
          outputfile
          << "@THAT" << std::endl
          << "D=M" <<std::endl;
        }
        else if(segment == "local"){
          outputfile
          << "@LCL" << std::endl
          << "D=M" <<std::endl;
        }
        else if(segment == "argument"){
          outputfile
          << "@ARG" << std::endl
          << "D=M" <<std::endl;
        }
        else{
          outputfile
          << "@" << mp[segment] << std::endl
          << "D=A" << std::endl;
        }
        outputfile
        << "@" << index << std::endl
        << "D=D+A" << std::endl
        << "@R13" << std::endl
        << "M=D" << std::endl
        << "@SP" << std::endl
        << "M=M-1" <<std::endl
        << "A=M" << std::endl
        << "D=M" << std::endl
        << "@R13" <<std::endl
        << "A=M" << std::endl
        << "M=D" << std::endl;


      }
      else std::cout << "you pop to virtual segment named constant." << std::endl;

    }

    outputfile.flush();

  }


  void writeInit(){
    outputfile
    << "@256" << std::endl
    << "D=A" << std::endl
    << "@SP" << std::endl
    << "M=D" << std::endl;
    writeCall("Sys.init",0);
  }

  void writeLabel(std::string label){

    outputfile
    << "//LABEL" <<std::endl
    << "(" << label << ")" << std::endl;
  }

  void writeGoto(std::string label){
    outputfile
    << "//GOTO" << std::endl
    << "@" << label << std::endl
    << "0;JMP" << std::endl;
  }

  void writeIf(std::string label){
    outputfile
    << "//IF" <<std::endl
    << "@SP" << std::endl
    << "M=M-1" << std::endl
    << "A=M" << std::endl
    << "D=M" << std::endl
    << "@" << label << std::endl
    << "D;JNE" << std::endl;
  }

  void writeCall(std::string functionName,int numArgs)
  {

    outputfile
    << "//CALL" << std::endl
    << "@" << functionName << "_RETURN_" << rtnc << std::endl
    << "D=A" << std::endl
    << "@SP" << std::endl
    << "A=M" << std::endl
    << "M=D" << std::endl
    << "@SP" << std::endl
    << "M=M+1" << std::endl

    << "@LCL" << std::endl
    << "D=M" << std::endl
    << "@SP" << std::endl
    << "A=M" << std::endl
    << "M=D" << std::endl
    << "@SP" << std::endl
    << "M=M+1" << std::endl

    << "@ARG" << std::endl
    << "D=M" << std::endl
    << "@SP" << std::endl
    << "A=M" << std::endl
    << "M=D" << std::endl
    << "@SP" << std::endl
    << "M=M+1" << std::endl

    << "@THIS" << std::endl
    << "D=M" << std::endl
    << "@SP" << std::endl
    << "A=M" << std::endl
    << "M=D" << std::endl
    << "@SP" << std::endl
    << "M=M+1" << std::endl

    << "@THAT" << std::endl
    << "D=M" << std::endl
    << "@SP" << std::endl
    << "A=M" << std::endl
    << "M=D" << std::endl
    << "@SP" << std::endl
    << "M=M+1" << std::endl

    << "@" << numArgs+5 << std::endl
    << "D=A"<< std::endl
    << "@SP" << std::endl
    << "D=M-D"<< std::endl
    << "@ARG" << std::endl
    << "M=D" << std::endl

    << "@SP" << std::endl
    << "D=M" << std::endl
    << "@LCL" << std::endl
    << "M=D" << std::endl

    << "@"<< functionName << std::endl
    << "0;JMP" << std::endl


    << "(" << functionName << "_RETURN_" << rtnc << ")" << std::endl;


    rtnc++;

  }

  void writeReturn(){
    outputfile
    << "//RETURN" << std::endl
    << "@LCL" << std::endl
    << "D=M" << std::endl
    << "@R13" << std::endl
    << "M=D" << std::endl

    << "@R13" << std::endl
    << "D=M" << std::endl
    << "@5" << std::endl
    << "D=D-A" << std::endl
    << "A=D" << std::endl
    << "D=M" << std::endl
    << "@14" << std::endl
    << "M=D" << std::endl


    << "@SP" << std::endl
    << "A=M" << std::endl
    << "D=M" << std::endl
    << "@ARG" << std::endl
    << "A=M" << std::endl
    << "M=D" << std::endl


    << "@ARG" << std::endl
    << "M=M+1" << std::endl
    << "D=M" << std::endl
    << "@SP" << std::endl
    << "M=D" << std::endl

    << "@R13" << std::endl
    << "M=M-1" << std::endl
    << "A=M" << std::endl
    << "D=M" << std::endl
    << "@THAT" << std::endl
    << "M=D" << std::endl

    << "@R13" << std::endl
    << "M=M-1" << std::endl
    << "A=M" << std::endl
    << "D=M" << std::endl
    << "@THIS" << std::endl
    << "M=D" << std::endl

    << "@R13" << std::endl
    << "M=M-1" << std::endl
    << "A=M" << std::endl
    << "D=M" << std::endl
    << "@ARG" << std::endl
    << "M=D" << std::endl

    << "@R13" << std::endl
    << "M=M-1" << std::endl
    << "A=M" << std::endl
    << "D=M" << std::endl
    << "@LCL" << std::endl
    << "M=D" << std::endl

    << "@R14"<< std::endl
    << "0;JMP" << std::endl;



  }


  void writeFunction(std::string functionName,int numLocals){
    outputfile
    << "//Function" << std::endl
    << "(" << functionName << ")" << std::endl;
    outputfile
    << "@SP" <<std::endl;
    for(int i=0;i<numLocals;i++)
    outputfile
    << "A=M" << std::endl
    << "M=0" << std::endl
    << "@SP" << std::endl
    << "M=M+1" << std::endl;
  }

};





int main(int argc,char *argv[])
{
  std::remove(argv[1]); // delete file
  int n;
  std::cout << "How many files are compliled?" << std::endl << "Type number:";
  std::cin >> n;
  std::vector<std::string> files(n+2);

  std::cout << "input filenames in order you compile" << std::endl;

  for(int i=0;i<n;i++) std::cin >> files[i];

  CodeWriter cw;

  cw.setFileName(argv[1]);

  //cw.writeInit();

  for(int i=0;i<n;i++){


    Parser psr(files[i]);



    while(!psr.hasMoreCommands())
    {
      psr.advance();


      if(psr.commandType() != C_NO){

        if(psr.commandType() == C_ARITHMETIC) cw.writeArithmetic(psr.arg1());

        else if(psr.commandType() == C_PUSH || psr.commandType() == C_POP) cw.writePushPop(psr.commandType(),psr.arg1(),psr.arg2());

        else if (psr.commandType() == C_CALL) cw.writeCall(psr.arg1(),psr.arg2());

        else if(psr.commandType() == C_FUNCTION) cw.writeFunction(psr.arg1(),psr.arg2());

        else if(psr.commandType() == C_LABEL) cw.writeLabel(psr.arg1());

        else if(psr.commandType() == C_IF) cw.writeIf(psr.arg1());

        else if(psr.commandType() == C_GOTO) cw.writeGoto(psr.arg1());

        else if(psr.commandType() == C_RETURN) cw.writeReturn();


      }
    }
  }
  return 0;
}
