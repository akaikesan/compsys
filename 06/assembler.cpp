#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <cctype>
using namespace std;

enum command {
  A_COMMAND,C_COMMAND,L_COMMAND,NO_COMMAND
};


class Parser{
private:
  ifstream reading_file;
  string reading_line_buffer;


public:





  Parser(string file){

    reading_file.open(file,ios::in);

  }


  bool hasMoreCommands()
  {
    return reading_file.eof();
  }


  void advance()
  {
    getline(reading_file,reading_line_buffer);
  }

  command commandType()
  {
    regex reC(R"(\w+=.*+)");
    regex reCj(R"(\w+;\w+)");
    regex reA(R"(@)");
    regex reL(R"(\(\w+\))");
    regex co(R"(//.*+)");//Commentout
    regex space(R"(\s+)");
    if (regex_search(reading_line_buffer,co))
    {
      reading_line_buffer = regex_replace(reading_line_buffer,co,"");
    }
    if(regex_search(reading_line_buffer,space))
    {
      reading_line_buffer = regex_replace(reading_line_buffer,space,"");


    }




    if(regex_search(reading_line_buffer,reA))
    {
      return A_COMMAND;
    }
    else if(regex_search(reading_line_buffer,reC) || regex_search(reading_line_buffer,reCj))
    {
      return C_COMMAND;
    }
    else if(regex_search(reading_line_buffer,reL))
    {
      return L_COMMAND;
    }
    return NO_COMMAND;
  }









  string symbol()
  {
    regex re(R"(\w+)");
    smatch results;
    if(commandType() == A_COMMAND || commandType() == L_COMMAND)
    {
      regex_search(reading_line_buffer,results,re);
      return results.str();
    }
    return "";
  }

  string dest()
  {
    if(commandType()==C_COMMAND)
    {
      regex re(R"((\w+)(=))");
      smatch results;

      regex_search(reading_line_buffer,results,re);
      return results[1].str();
    }
    return "";
  }

  string jmp()
  {
    if(commandType() == C_COMMAND)
    {
      regex re(R"((\w+)(;)(\w+))");
      smatch results;

      regex_search(reading_line_buffer,results,re);
      return results[3].str();
    }
    return "";
  }

  string comp()
  {
    if(commandType()==C_COMMAND)
    {
      regex re_dest(R"((=)(.*+))");
      regex re_jmp(R"((\w+)(;))");
      smatch results;

      if(regex_search(reading_line_buffer,results,re_dest))
      {
        return results[2].str();
      }
      if(regex_search(reading_line_buffer,results,re_jmp))
      {
        return results[1].str();
      }
    }
    return "";
  }

  void to_the_first_line()
  {
    reading_file.clear();
    reading_file.seekg(0,ios_base::beg);
  }


};



class Code
{
public:
  string dest(string neh)
  {
    if(neh == "")
    {
      return "000";
    }
    else if (neh == "M")
    {
      return "001";
    }
    else if(neh == "D")
    {
      return "010";
    }
    else if(neh == "MD")
    {
      return "011";
    }
    else if(neh == "A")
    {
      return "100";
    }
    else if (neh == "AM")
    {
      return "101";
    }
    else if(neh == "AD")
    {
      return "110";
    }
    else if(neh == "AMD")
    {
      return "111";
    }

    return "";
  }

  string jump(string neh)
  {
    if(neh == "")
    {
      return "000";
    }
    else if (neh == "JGT")
    {
      return "001";
    }
    else if(neh == "JEQ")
    {
      return "010";
    }
    else if(neh == "JGE")
    {
      return "011";
    }
    else if(neh == "JLT")
    {
      return "100";
    }
    else if (neh == "JNE")
    {
      return "101";
    }
    else if(neh == "JLE")
    {
      return "110";
    }
    else if(neh == "JMP")
    {
      return "111";
    }
    return "";
  }

  string comp(string neh)
  {
    if(neh == "0")
    {
      return "0101010";
    }
    else if (neh == "1")
    {
      return "0111111";
    }
    else if(neh == "-1")
    {
      return "0111010";
    }
    else if(neh == "D")
    {
      return "0001100";
    }
    else if(neh == "A")
    {
      return "0110000";
    }
    else if (neh == "!D")
    {
      return "0001101";
    }
    else if(neh == "!A")
    {
      return "0110001";
    }
    else if(neh == "-D")
    {
      return "0001111";
    }
    else if(neh =="-A")
    {
      return "0110011";
    }
    else if(neh == "D+1")
    {
      return "0011111";
    }
    else if(neh == "A+1")
    {
      return "0110111";
    }
    else if(neh == "D-1")
    {
      return "0001110";
    }
    else if(neh == "A-1")
    {
      return "0110010";
    }
    else if(neh == "D+A")
    {
      return "0000010";
    }
    else if(neh == "D-A")
    {
      return "0010011";
    }
    else if (neh =="A-D")
    {
      return "0000111";
    }
    else if(neh == "D&A")
    {
      return "0000000";
    }
    else if(neh=="D|A")
    {
      return "0010101";
    }
    else if(neh == "M")
    {
      return "1110000";
    }
    else if(neh == "!M")
    {
      return "1110001";
    }
    else if(neh == "-M")
    {
      return "1110011";
    }
    else if (neh == "M+1")
    {
      return "1110111";
    }
    else if(neh=="M-1")
    {
      return "1110010";
    }
    else if(neh == "D+M")
    {
      return "1000010";
    }
    else if (neh == "D-M")
    {
      return "1010011";
    }
    else if(neh == "M-D")
    {
      return "1000111";
    }
    else if(neh == "D&M")
    {
      return "1000000";
    }
    else if (neh == "D|M")
    {
      return "1010101";
    }
    return "";
  }




};


class SymbolTable
{
public:


  map<string,unsigned long int> v;
  SymbolTable(){
    for (int i=0;i<16;i++)
    {
      string s = 'R' + to_string(i);
      v[s] = i;
    }

    v["SCREEN"] = 16384;
    v["KBD"] = 24576;
    v["SP"] = 0;
    v["LCL"] = 1;
    v["ARG"] = 2;
    v["THIS"] = 3;
    v["THAT"] = 4;

  }




  void addEntry(string symbol, int address)
  {

      v[symbol] = address;
  }

  bool contains(string symbol)
  {

    auto iter = v.find(symbol);
    if (iter != end(v))
    {
      return true;
    }
    else
    {
      return false;
    }

  }

  int getAddress(string symbol)
  {
    return v[symbol];
  }
};


string binary(string bin){
    unsigned long int bina = stoi(bin);
    unsigned long int ans = 0;
    for (int i = 0; bina>0 ; i++)
    {
        ans = ans+(bina%2)*pow(10,i);
        bina = bina/2;
    }

    string inst = to_string(ans);

    ans = 16 - inst.length();

    int l = inst.length();

    while(ans != 0)
    {


      inst = "0" + inst;
      ans--;
    }

    return inst;
}

string binary(int bina){
    unsigned long int ans = 0;
    for (int i = 0; bina>0 ; i++)
    {
        ans = ans+(bina%2)*pow(10,i);
        bina = bina/2;
    }

    string inst = to_string(ans);

    ans = 16 - inst.length();

    int l = inst.length();

    while(ans != 0)
    {


      inst = "0" + inst;
      ans--;
    }

    return inst;
}


int main(int argc,char* argv[])
{
  Parser ps(argv[1]);
  Code converter;
  ofstream outputfile("Prog.hack");
  SymbolTable st;


  int counter = 1;


  while(!ps.hasMoreCommands())
  {
    ps.advance();
    if(ps.commandType() == L_COMMAND)
    {
      counter--;
      st.addEntry(ps.symbol(),counter);
    }
    if(ps.commandType() != NO_COMMAND)
    {
      counter++;
    }
  }

  ps.to_the_first_line();



  int address = 16;



  regex re(R"(\d+)");

  while(!ps.hasMoreCommands())
  {
    ps.advance();
    if (ps.commandType() == A_COMMAND)
    {

      if(!st.contains(ps.symbol()))
      {

        if (regex_match(ps.symbol(),re))
        {
          st.v[ps.symbol()] = stoi(ps.symbol());

        }
        else
        {
          st.addEntry(ps.symbol(),address);
          address++;

        }

      }
    }

    counter++;
  }



  ps.to_the_first_line();


  while(!ps.hasMoreCommands())
  {
    ps.advance();
    if (ps.commandType() == A_COMMAND)

    {
      outputfile << binary(st.getAddress(ps.symbol())) << endl;
    }
    if (ps.commandType() == C_COMMAND)
    {

      outputfile << 111 << converter.comp(ps.comp()) << converter.dest(ps.dest()) << converter.jump(ps.jmp()) << endl;

    }
  }



  return 0;
}
