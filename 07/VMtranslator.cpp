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

    else if (std::regex_search(reading_line_buffer,gt)) return C_GOTO;

    else if (std::regex_search(reading_line_buffer,igoto)) return C_IF;

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
        std::regex splitter(R"((\w+)(\s)(\w+)(\s*))");
        std::smatch results;
        std::regex_search(reading_line_buffer,results,splitter);
        return results[3].str();
      }
    }

    else{
      return  "";
    }


  }




  std::string arg2(){
    if((commandType() == C_PUSH || commandType() == C_POP) || (commandType() == C_FUNCTION||commandType() == C_CALL)){
      std::regex splitter(R"((\w+)(\s)(\w+)(\s)(\d+))");
      std::smatch results;
      if(std::regex_search(reading_line_buffer,results,splitter)) return results[5].str();
      else return "";
    }
    else{
      return "";
    }

  }




};



class CodeWriter{
private:
  int lctr = 0;
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

    outputfile.open(fn,std::ios::out);
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
        << "@SP" <<std::endl    //this 2 lines implies M refers to
        << "A=M" << std::endl   //the highest stack
        << "M=D" << std::endl
        << "@SP" <<std::endl       //this 2 lines implies M refers to
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

};





int main(int argc,char* argv[])
{
  Parser psr(argv[1]);
  CodeWriter cw;

  cw.setFileName(argv[2]);


  while(!psr.hasMoreCommands())
  {
    psr.advance();


    if(psr.commandType() != C_NO){
      if(psr.commandType() == C_ARITHMETIC){



        cw.writeArithmetic(psr.arg1());
      }
      else if(psr.commandType() == C_PUSH || psr.commandType() == C_POP){


        cw.writePushPop(psr.commandType(),psr.arg1(),stoi(psr.arg2()));
      }
    }
  }
  return 0;
}
