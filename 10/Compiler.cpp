#include<bits/stdc++.h>
#include<regex>


enum TokenType{
  KEYWORD,SYMBOL,IDENTIFIER,INT_CONST,STRING_CONST
};

enum KeyWord{
  CLASS,METHOD,FUNCTION,CONSTRUCTOR,INT,BOOLEAN,CHAR,VOID,VAR,STATIC,FIELD,LET,DO,IF,ELSE,WHILE,RETURN,TTRUE,FFALSE,NNULL,THIS
};


static std::regex clskw(R"(class)");
static std::regex methkw(R"(method)");
static std::regex funckw(R"(function)");
static std::regex conkw(R"(constructor)");
static std::regex intkw(R"(int)");
static std::regex boolkw(R"(boolean)");
static std::regex chrkw(R"(char)");
static std::regex voidkw(R"(void)");
static std::regex varkw(R"(var)");
static std::regex statkw(R"(static)");
static std::regex fiekw(R"(field)");
static std::regex letkw(R"(let)");
static std::regex dokw(R"(do)");
static std::regex ifkw(R"(if)");
static std::regex elsekw(R"(else)");
static std::regex whilkw(R"(while)");
static std::regex retkw(R"(return)");
static std::regex truekw(R"(true)");
static std::regex falskw(R"(false)");
static std::regex nullkw(R"(null)");
static std::regex thiskw(R"(this)");
static std::regex kw(R"((class|constructor|function|method|field|static|var|int|char|boolean|void|true|false|null|this|let|do|if|else|while|return)(($|\s)*))");
static std::regex symbRegex(R"(\||~|>|<|&|/|\*|\+|;|,|\.|\)|\(|\]|\[|\}|=|-|\{)");
static std::regex space(R"(\s+)");
static std::regex closingbracket(R"(\}|\]|\))");
static std::regex openingbracket(R"(\{|\[|\()");
static std::regex regSemicolon(R"(;)");

static std::regex opRegex(R"(>|<|&|/|\*|\+|\||=|-)");


static std::regex quotation(R"((\"))");
static std::regex integ(R"(\d+)");
static std::regex strconst(R"((\")((.)*)(\"))");
static std::regex ident(R"(\w+)");
static std::regex lineRegex(R"((<\w+>)(.+)(</\w+>))");


class JackTokenizer{
private:

  std::ifstream reading_file;
  std::ofstream outputfile;
  std::string reading_line_buffer;
  std::smatch m;
  std::string buffer;

  bool not_permitted = false;




  void lineTokenizer(std::string line){

    //order of if-sentences in the next while-sentence cannot be changed.

    std::regex commentout(R"(//.*)");//Commentout
    std::regex commentoutRange(R"(/\*.*\*/)");//Commentout
    std::regex commentoutRangeLeft(R"(/\*.*)");//Commentout
    std::regex commentoutRangeRight(R"(.*\*/)");//Commentout

    if (std::regex_search(line,commentout)) line = std::regex_replace(line,commentout,"");
    if (std::regex_search(line,commentoutRange)) line = std::regex_replace(line,commentoutRange,"");
    if (std::regex_search(line,commentoutRangeRight))
    {
      line = std::regex_replace(line,commentoutRangeRight,"");
      not_permitted = false;
    }

    if(not_permitted) return;

    if (std::regex_search(line,commentoutRangeLeft))
    {
      line = std::regex_replace(line,commentoutRangeLeft,"");
      not_permitted = true;
    }


    std::cout << line << std::endl;

    while(line != ""){
      std::smatch m;

      if(std::regex_search(line,m,space) && m.str() == line.substr(0,m.str().length())){
        line = m.suffix();
      }

      else if (std::regex_search(line,m,strconst) && m.str() == line.substr(0,m.str().length())){
        outputfile << "<stringConstant> " << m[2].str() << " </stringConstant>" << std::endl;
        line = m.suffix();
      }

      else if (std::regex_search(line,m,symbRegex) && m.str() == line.substr(0,m.str().length())){
        std::string symboltemp = m.str();
        if(symboltemp == "<") symboltemp = "&lt;";
        else if(symboltemp == ">") symboltemp = "&gt;";
        else if(symboltemp == "&") symboltemp = "&amp;";

        outputfile << "<symbol> " << symboltemp << " </symbol>" << std::endl;
        line = m.suffix();
      }

      else if(std::regex_search(line,m,kw) && (m.str() == line.substr(0,m.str().length()))){
        outputfile << "<keyword> " << m[1].str() << " </keyword>" << std::endl;
        line = m.suffix();

      }

      else if(std::regex_search(line,m,integ) && m.str() == line.substr(0,m.str().length())){
        outputfile << "<integerConstant> " << m.str() << " </integerConstant>" << std::endl;
        line = m.suffix();

      }

      else if(std::regex_search(line,m,ident) && m.str() == line.substr(0,m.str().length())){
        outputfile << "<identifier> " << m.str() <<  " </identifier>" << std::endl;
        line = m.suffix();
      }


    }
  }
public:

  JackTokenizer(std::string inputfilename){

    reading_file.open(inputfilename,std::ios::in);
    outputfile.open("tokens.xml",std::ios::out | std::ios::ate);


    while(!reading_file.eof()){
      lineTokenizer(reading_line_buffer);
      getline(reading_file,reading_line_buffer);
    }


    reading_file.close();
    reading_file.clear();
    reading_file.open("tokens.xml",std::ios::in);

  }

  std::string readbuffer()
  {
    return reading_line_buffer;
  }

  bool hasMoreTokens()
  {
    return !reading_file.eof();
  }

  void advance()
  {
    getline(reading_file,reading_line_buffer);//if you read the line, end your function after advance();
  }



  TokenType tokenType(){
    std::regex kwToken(R"(<keyword>)");
    std::regex symToken(R"(<symbol>)");
    std::regex idToken(R"(<identifier>)");
    std::regex intToken(R"(<integerConstant>)");
    std::regex strToken(R"(<stirngConstant>)");


    if(std::regex_search(reading_line_buffer,kwToken)) return KEYWORD;

    else if(std::regex_search(reading_line_buffer,symToken)) return SYMBOL;

    else if(std::regex_search(reading_line_buffer,idToken)) return IDENTIFIER;

    else if(std::regex_search(reading_line_buffer,intToken)) return INT_CONST;

    else if(std::regex_search(reading_line_buffer,strToken)) return STRING_CONST;

  }


  KeyWord keyword(){

    std::regex_search(reading_line_buffer,m,lineRegex);

    std::string content = m[2].str();


    if (std::regex_search(content,clskw)) return CLASS;
    else if (std::regex_search(content,methkw)) return METHOD;
    else if (std::regex_search(content,funckw)) return FUNCTION;
    else if (std::regex_search(content,conkw)) return CONSTRUCTOR;
    else if (std::regex_search(content,intkw)) return INT;
    else if (std::regex_search(content,boolkw)) return BOOLEAN;
    else if (std::regex_search(content,chrkw)) return CHAR;
    else if (std::regex_search(content,voidkw)) return VOID;
    else if (std::regex_search(content,varkw)) return VAR;
    else if (std::regex_search(content,statkw)) return STATIC;
    else if (std::regex_search(content,fiekw)) return FIELD;
    else if (std::regex_search(content,letkw)) return LET;
    else if (std::regex_search(content,dokw)) return DO;
    else if (std::regex_search(content,ifkw)) return IF;
    else if (std::regex_search(content,elsekw)) return ELSE;
    else if (std::regex_search(content,whilkw)) return WHILE;
    else if (std::regex_search(content,retkw)) return RETURN;
    else if (std::regex_search(content,truekw)) return TTRUE;
    else if (std::regex_search(content,falskw)) return FFALSE;
    else if (std::regex_search(content,nullkw)) return NNULL;
    else if (std::regex_search(content,thiskw)) return THIS;
  }

  std::string symbol(){

    std::regex_search(reading_line_buffer,m,lineRegex);
    buffer = m[2].str();

    if(buffer == "&lt;") buffer = "<";
    else if(buffer == "&gt;") buffer = ">";
    else if(buffer == "&amp;") buffer = "&";

    return buffer;
  }

};


class CompilationEngine{

private:
  std::ofstream outputfile;
  int indentcounter = 0;
  std::string indent = "";

public:
  std::vector<std::string> middlebracketStack;
  std::vector<std::string> semicolonStack;






  CompilationEngine()
  {
    outputfile.open("analyzedTokens.xml",std::ios::out | std::ios::ate);
  }








  void increaseIndent()
  {
    indentcounter++;
    indent += "  ";
  }










  void decreaseIndent()
  {
    indentcounter--;
    indent = "";
    for(int i=0;i<indentcounter;i++) indent += "  ";
  }











  void compileClass(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<class>" << std::endl;

    increaseIndent();

    for(int i=0;i<3;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();
    }



    while(tokenizer->tokenType() != SYMBOL)
    {
      if(tokenizer->keyword() == STATIC || tokenizer->keyword() == FIELD) compileclassVarDec(tokenizer);

      else if(tokenizer->keyword() == CONSTRUCTOR || tokenizer->keyword() == FUNCTION || tokenizer->keyword() ==METHOD) compileSubroutine(tokenizer);

      else
      {
        std::cout << "Syntaxerror! wrong definition in class" << std::endl;
        break;
      }
    }

    outputfile << indent << tokenizer->readbuffer() << std::endl;

    decreaseIndent();

    outputfile << indent << "</class>" << std::endl;

  }




  void compileSubroutine(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<subroutineDec>" << std::endl;
    increaseIndent();



    for(int i=0;i<4;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();
    }




    compileParameterList(tokenizer);





    outputfile << indent << "<subroutineBody>" << std::endl;
    increaseIndent();



    outputfile << indent << tokenizer->readbuffer() << std::endl;// output "{"
    tokenizer->advance();




    while(tokenizer->tokenType() != SYMBOL)
    {
      if(tokenizer->keyword() == VAR) compileVarDec(tokenizer);

      else if(tokenizer->keyword() == LET || tokenizer->keyword() == IF || tokenizer->keyword() == WHILE || tokenizer->keyword() ==DO || tokenizer->keyword() ==RETURN) compileStatements(tokenizer);

      else{
        std::cout << "Syntaxerror! wrong definition in SubroutineBody" << std::endl;
        break;
      }
    }




    outputfile << indent << tokenizer->readbuffer() << std::endl;//output "}"
    tokenizer->advance();
    decreaseIndent();
    outputfile << indent << "</subroutineBody>" << std::endl;
    decreaseIndent();
    outputfile << indent << "</subroutineDec>" << std::endl;







  }










  void compileStatements(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<statements>" << std::endl;
    increaseIndent();
    bool line_is_statement = true;
    while(line_is_statement)
    {

      switch (tokenizer->keyword())
      {
        case LET:
          compileLet(tokenizer);break;
        case DO:
          compileDo(tokenizer);break;
        case WHILE:
          compileWhile(tokenizer);break;
        case RETURN:
          compileReturn(tokenizer);break;
        case IF:
          compileIf(tokenizer);break;
        default:
          line_is_statement = false;


      }


    }
    decreaseIndent();
    outputfile << indent << "</statements>" << std::endl;
  }









  void compileLet(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<letStatement>" << std::endl;
    increaseIndent();
    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();
    }
    if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\[)")))
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output "["
      tokenizer->advance();
      compileExpression(tokenizer);
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output "]"
      tokenizer->advance();
    }
    outputfile << indent << tokenizer->readbuffer() << std::endl;//output "="
    tokenizer->advance();

    compileExpression(tokenizer);

    outputfile << indent << tokenizer->readbuffer() << std::endl;//output ";"
    tokenizer->advance();
    decreaseIndent();
    outputfile << indent << "</letStatement>" << std::endl;


  }




  void compileDo(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<doStatement>" << std::endl;
    increaseIndent();
    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();
    }

    if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\.)")))
    {
      for(int i=0;i<3;i++)
      {
        outputfile << indent << tokenizer->readbuffer() << std::endl;//output .subroutinName(
        tokenizer->advance();
      }
      compileExpressionList(tokenizer);

      outputfile << indent << tokenizer->readbuffer() << std::endl;//output )
      tokenizer->advance();
    }
    else if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\()"))){

      outputfile << indent << tokenizer->readbuffer() << std::endl;//output (
      tokenizer->advance();

      compileExpressionList(tokenizer);

      outputfile << indent << tokenizer->readbuffer() << std::endl; //output )
      tokenizer->advance();
    }

    outputfile << indent << tokenizer->readbuffer() << std::endl; //output ;
    tokenizer->advance();

    decreaseIndent();
    outputfile << indent << "</doStatement>" << std::endl;

  }




  void compileWhile(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<whileStatement>" << std::endl;
    increaseIndent();
    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output "("
      tokenizer->advance();
    }
    compileExpression(tokenizer);

    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output ")" "{"
      tokenizer->advance();
    }

    compileStatements(tokenizer);

    outputfile << indent << tokenizer->readbuffer() << std::endl;//output "}"
    tokenizer->advance();

    decreaseIndent();
    outputfile << indent << "</whileStatement>" << std::endl;
  }








  void compileIf(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<ifStatement>" << std::endl;
    increaseIndent();

    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output "if" "("
      tokenizer->advance();
    }

    compileExpression(tokenizer);//expressions

    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output ")" "{"
      tokenizer->advance();
    }

    compileStatements(tokenizer);//statements

    outputfile << indent << tokenizer->readbuffer() << std::endl;//output "}"
    tokenizer->advance();

    if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(else)")))
    {
      for(int i=0;i<2;i++)
      {
        outputfile << indent << tokenizer->readbuffer() << std::endl;//output "else" "{"
        tokenizer->advance();
      }

      compileStatements(tokenizer);

      outputfile << indent << tokenizer->readbuffer() << std::endl;//output "}"
      tokenizer->advance();
    }




    decreaseIndent();
    outputfile << indent << "</ifStatement>" << std::endl;

  }


  void compileReturn(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<returnStatement>" << std::endl;
    increaseIndent();

    outputfile << indent << tokenizer->readbuffer() << std::endl;//output "return"
    tokenizer->advance();
    if(tokenizer->readbuffer() != "<symbol> ; </symbol>")
    {
      compileExpression(tokenizer);
    }

    outputfile << indent << tokenizer->readbuffer() << std::endl;//output ";"
    tokenizer->advance();

    decreaseIndent();
    outputfile << indent << "</returnStatement>" << std::endl;

  }



  void compileExpression(JackTokenizer *tokenizer)
  {

    outputfile << indent << "<expression>" << std::endl;
    increaseIndent();
    compileTerm(tokenizer);

    while(tokenizer->tokenType() == SYMBOL)
    {
      if(std::regex_search(tokenizer->symbol(),opRegex))
      {
        std::string symboltemp = tokenizer->symbol();
        if(symboltemp == "<") symboltemp = "&lt;";
        else if(symboltemp == ">") symboltemp = "&gt;";
        else if(symboltemp == "&") symboltemp = "&amp;";
        outputfile << indent << "<symbol>" << symboltemp << "</symbol>" << std::endl;
        tokenizer->advance();
        compileTerm(tokenizer);
      }
      else break;
    }
    decreaseIndent();
    outputfile << indent << "</expression>" << std::endl;

  }


















  void compileParameterList(JackTokenizer *tokenizer){
    outputfile << indent << "<parameterList>" << std::endl;
    increaseIndent();

    while (!std::regex_search(tokenizer->readbuffer(),std::regex(R"(\))")))
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();
    }

    decreaseIndent();
    outputfile << indent << "</parameterList>" << std::endl;
    outputfile << indent << tokenizer->readbuffer() << std::endl;//output ")"
    tokenizer->advance();
  }











  void compileVarDec(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<varDec>" << std::endl;
    increaseIndent();

    outputfile << indent << tokenizer->readbuffer() << std::endl;
    while (!std::regex_search(tokenizer->readbuffer(),std::regex(R"(;)"))){
      tokenizer->advance();
      outputfile << indent << tokenizer->readbuffer() << std::endl;
    }

    tokenizer->advance();
    decreaseIndent();
    outputfile << indent << "</varDec>" << std::endl;
  }








  void compileclassVarDec(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<classVarDec>" << std::endl;
    increaseIndent();

    outputfile << indent << tokenizer->readbuffer() << std::endl;
    while (!std::regex_search(tokenizer->readbuffer(),std::regex(R"(;)"))){
      tokenizer->advance();
      outputfile << indent << tokenizer->readbuffer() << std::endl;
    }

    tokenizer->advance();
    decreaseIndent();
    outputfile << indent << "</classVarDec>" << std::endl;
  }






  void compileExpressionList(JackTokenizer *tokenizer)
  {
    outputfile << indent << "<expressionList>" << std::endl;
    increaseIndent();
    while (!std::regex_search(tokenizer->readbuffer(),std::regex(R"(\))")))
    {
      compileExpression(tokenizer);
      if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(,)"))){
        outputfile << indent << tokenizer->readbuffer() << std::endl;
        tokenizer->advance();
      }
    }
    decreaseIndent();
    outputfile << indent << "</expressionList>" << std::endl;
  }



  void compileTerm(JackTokenizer *tokenizer)
  {

    outputfile << indent << "<term>" << std::endl;
    increaseIndent();

    if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\()")) )
    {

      outputfile << indent << tokenizer->readbuffer() << std::endl;//output "["
      tokenizer->advance();
      compileExpression(tokenizer);
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output "]"
      tokenizer->advance();

      decreaseIndent();
      outputfile << indent << "</term>" << std::endl;
      return;

    }
    else if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(-)")) || std::regex_search(tokenizer->readbuffer(),std::regex(R"(~)")))
    {

      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();

      compileTerm(tokenizer);

      decreaseIndent();
      outputfile << indent << "</term>" << std::endl;
      return;

    }

    else outputfile << indent << tokenizer->readbuffer() << std::endl;
    tokenizer->advance();

    if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\.)")))
    {
      for(int i=0;i<3;i++)
      {
        outputfile << indent << tokenizer->readbuffer() << std::endl;
        tokenizer->advance();
      }
      compileExpressionList(tokenizer);

      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();
    }
    else if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\()"))){

      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();

      compileExpressionList(tokenizer);

      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();
    }
    else if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\[)")))
    {

      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();

      compileExpression(tokenizer);

      outputfile << indent << tokenizer->readbuffer() << std::endl;
      tokenizer->advance();
    }

    decreaseIndent();
    outputfile << indent << "</term>" << std::endl;
  }

};







int main(int argc,char* argv[])
{

  JackTokenizer tokenizer(argv[1]);
  CompilationEngine ce;

  while(tokenizer.hasMoreTokens())
  {
    tokenizer.advance();
    if(tokenizer.tokenType() == KEYWORD){
      if(tokenizer.keyword() == CLASS) ce.compileClass(&tokenizer);
    }

  }


  return 0;
}
