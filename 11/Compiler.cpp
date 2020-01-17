#include<bits/stdc++.h>
#include<regex>



enum TokenType{
  KEYWORD,SYMBOL,IDENTIFIER,INT_CONST,STRING_CONST
};

enum KeyWord{
  CLASS,METHOD,FUNCTION,CONSTRUCTOR,INT,BOOLEAN,CHAR,VOID,VAR,STATIC,FIELD,LET,DO,IF,ELSE,WHILE,RETURN,TTRUE,FFALSE,NNULL,THIS
};

enum Kind{
  STATIC_K,FIELD_K,ARG_K,VAR_K,NO_K
};


enum Command{
  ADD,SUB,NEG,EQ,GT,LT,AND,OR,NOT
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
static std::regex kw(R"((class|constructor|function|method|field|static|var|int|char|boolean|void|true|false|null|this|let|do|if|else|while|return)(\W+))");
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




    while(line != ""){
      std::smatch m;

      if(std::regex_search(line,m,space) && m.str() == line.substr(0,m.str().length())){
        line = m.suffix();
      }

      else if (std::regex_search(line,m,strconst) && m.str() == line.substr(0,m.str().length())){
        outputfile << "<stringConstant>" << m[2].str() << "</stringConstant>" << std::endl;
        line = m.suffix();
      }

      else if (std::regex_search(line,m,symbRegex) && m.str() == line.substr(0,m.str().length())){
        std::string symboltemp = m.str();
        if(symboltemp == "<") symboltemp = "&lt;";
        else if(symboltemp == ">") symboltemp = "&gt;";
        else if(symboltemp == "&") symboltemp = "&amp;";

        outputfile << "<symbol>" << symboltemp << "</symbol>" << std::endl;
        line = m.suffix();
      }

      else if(std::regex_search(line,m,kw) && (m.str() == line.substr(0,m.str().length()))){
        outputfile << "<keyword>" << m[1].str() << "</keyword>" << std::endl;

        std::string temp = m[2].str();
        line = m.suffix();
        line.insert(0,temp);

      }

      else if(std::regex_search(line,m,integ) && m.str() == line.substr(0,m.str().length())){
        outputfile << "<integerConstant>" << m.str() << "</integerConstant>" << std::endl;
        line = m.suffix();

      }

      else if(std::regex_search(line,m,ident) && m.str() == line.substr(0,m.str().length())){
        outputfile << "<identifier>" << m.str() <<  "</identifier>" << std::endl;
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
    std::regex strToken(R"(<stringConstant>)");


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

  std::string content(){

    std::regex_search(reading_line_buffer,m,lineRegex);
    buffer = m[2].str();
    return buffer;
  }

};




class SymbolRow
{
private:
  std::string name;
  std::string type;
  Kind kind;
  int indexNumber;

public:

  SymbolRow(std::string nname,std::string ttype,Kind kkind,int iindexNumber):
  name(nname),type(ttype),kind(kkind),indexNumber(iindexNumber){}

  void setIndex(int index){
    indexNumber = index;
  }

  std::string getname(){return name;}
  std::string gettype(){return type;}
  Kind getkind(){return kind;}
  int getindexNumber(){return indexNumber;}


};



class SymbolTable
{
private:
  int staticCounter = 0;
  int fieldCounter = 0;
  int argCounter = 0;
  int varCounter = 0;


  std::vector<SymbolRow> classSymbolTable;
  std::vector<SymbolRow> subroutineSymbolTable;

public:


  void startSubroutine()
  {
    argCounter = 0;
    varCounter = 0;

    subroutineSymbolTable.clear();
  }

  void define(std::string name,std::string type,Kind kind)
  {


    if(kind == STATIC_K)
    {
      staticCounter++;
      classSymbolTable.push_back(SymbolRow(name,type,kind,varCount(kind) - 1));
    }
    else if (kind == FIELD_K)
    {
      fieldCounter++;
      classSymbolTable.push_back(SymbolRow(name,type,kind,varCount(kind) - 1));

    }
    else if (kind == ARG_K)
    {
      argCounter++;
      subroutineSymbolTable.push_back(SymbolRow(name,type,kind,varCount(kind) - 1));

    }
    else if (kind == VAR_K)
    {
      varCounter++;
      subroutineSymbolTable.push_back(SymbolRow(name,type,kind,varCount(kind) - 1));

    }


  }

  void showsymbolTable(){
    std::cout << "class-------------------------------------------" << std::endl;
    for(auto v : classSymbolTable) std::cout << v.getname() << "," << v.gettype() << "," << v.getkind() << "," << v.getindexNumber() << std::endl;

    std::cout <<  "              method-----------------------------------------------"<<std::endl;
    for(auto v : subroutineSymbolTable) std::cout << "                            " << v.getname() << "," << v.gettype() << "," << v.getkind() << "," << v.getindexNumber() << std::endl;
  }

  std::vector<SymbolRow> getclassSymbolTable(){
    return classSymbolTable;
  }

  std::vector<SymbolRow> getsubroutineSymbolTable(){
    return subroutineSymbolTable;
  }


  int varCount(Kind kind)
  {
    if(kind == STATIC_K)
    {
      return staticCounter;
    }
    else if (kind == FIELD_K)
    {
      return fieldCounter;
    }
    else if (kind == ARG_K)
    {

      return argCounter;
    }
    else if (kind == VAR_K)
    {
      return varCounter;
    }
    else
    {
      std::cout << "SymbolTable.varCount:  CompilationError! you use undefined variable." << std::endl;
    }


  }

  std::string typeOf(std::string name)
  {
    for(auto row : subroutineSymbolTable)
    {
      if(row.getname() == name)
      {
        return row.gettype();
      }
    }

    for(auto row : classSymbolTable)
    {
      if(row.getname() == name)
      {
        return row.gettype();
      }
    }
  }

  bool there_is(std::string name)
  {
    for(auto row : subroutineSymbolTable)
    {
      if(row.getname() == name)
      {
        return true;
      }
    }

    for(auto row : classSymbolTable)
    {
      if(row.getname() == name)
      {
        return true;
      }
    }
    return false;
  }





  Kind kindOf(std::string name)
  {
    for(auto row : subroutineSymbolTable)
    {
      if(row.getname() == name)
      {
        return row.getkind();
      }
    }

    for(auto row : classSymbolTable)
    {
      if(row.getname() == name)
      {
        return row.getkind();
      }
    }
    std::cout << "SymbolTable.kindOf" +name +  "  :  Compilation Error! you use undefined variable." << std::endl;
  }




  int indexOf(std::string name)
  {
    for(auto row : subroutineSymbolTable)
    {
      if(row.getname() == name)
      {
        return row.getindexNumber();
      }
    }

    for(auto row : classSymbolTable)
    {
      if(row.getname() == name)
      {
        return row.getindexNumber();
      }
    }

    std::cout << "SymbolTable.indexOf  :  Compilation Error! you use undefined variable." << std::endl;
  }






};


class VMwriter
{
private:
  std::ofstream outputVMfile;
public:
  VMwriter(std::string outputfileName){outputVMfile.open(outputfileName,std::ios::out | std::ios::ate);}

  void writepush(std::string segment,int index){outputVMfile << "push " << segment << " " << index << std::endl;}


  void writepop(std::string segment,int index){outputVMfile << "pop " << segment << " " << index << std::endl;}

  void writeArithmetic(Command c){
    if(c == ADD) outputVMfile << "add" << std::endl;
    else if(c == SUB) outputVMfile << "sub" << std::endl;
    else if(c==NEG)outputVMfile << "neg" << std::endl;
    else if(c==EQ)outputVMfile << "eq" << std::endl;
    else if(c==GT)outputVMfile << "gt" << std::endl;
    else if(c==LT)outputVMfile << "lt" << std::endl;
    else if(c==AND)outputVMfile << "and" << std::endl;
    else if(c==OR)outputVMfile << "or" << std::endl;
    else if(c==NOT)outputVMfile << "not" << std::endl;
  }


  void writeLabel(std::string label){outputVMfile << "label " << label << std::endl;}

  void writeGoto(std::string label){outputVMfile << "goto " << label << std::endl;}

  void writeIf(std::string label){outputVMfile << "if-goto " << label << std::endl;}

  void writeCall(std::string name,int nArgs){outputVMfile << "call " << name << " " << nArgs << std::endl;}

  void writeFunction(std::string name,int nLocals){outputVMfile << "function " << name << " " << nLocals << std::endl;}

  void writeReturn(){outputVMfile << "return" << std::endl;}


  void close(){outputVMfile.close();}
};



class CompilationEngine{

private:
  std::ofstream outputfile;
  int indentcounter = 0;
  int true_num = 0;
  int false_num = 0;
  int goto_num = 0;

  std::string indent = "";

  std::string className;

  std::vector<std::string> middlebracketStack;
  std::vector<std::string> semicolonStack;

  void writeAdvance(JackTokenizer *tokenizer){
    outputfile << indent << tokenizer->readbuffer() << std::endl;
    tokenizer->advance();
  }

public:






  CompilationEngine(){
    outputfile.open("analyzedTokens.xml",std::ios::out | std::ios::ate);
  }

  void increaseIndent(){
    indentcounter++;
    indent += "  ";
  }

  void decreaseIndent(){
    indentcounter--;
    indent = "";
    for(int i=0;i<indentcounter;i++) indent += "  ";
  }











  void compileClass(JackTokenizer *tokenizer,SymbolTable &symbolTable,VMwriter &vmwriter) {
    outputfile << indent << "<class>" << std::endl;

    increaseIndent();

    writeAdvance(tokenizer);//output "class"

    className = tokenizer->content();


    for(int i=0;i<2;i++) writeAdvance(tokenizer); //output className
                                                  //output {




    while(tokenizer->tokenType() != SYMBOL)
    {
      if(tokenizer->keyword() == STATIC || tokenizer->keyword() == FIELD) compileclassVarDec(tokenizer,symbolTable);

      else if(tokenizer->keyword() == CONSTRUCTOR || tokenizer->keyword() == FUNCTION || tokenizer->keyword() ==METHOD) compileSubroutine(tokenizer,&symbolTable,vmwriter);

      else
      {
        writeAdvance(tokenizer);
        std::cout << "Syntaxerror! wrong definition in class OR file to be compiled has end line that has letter.\n please put empty line in end of file. " << std::endl;
        break;
      }
    }

    outputfile << indent << tokenizer->readbuffer() << std::endl;

    decreaseIndent();

    outputfile << indent << "</class>" << std::endl;

  }




  void compileSubroutine(JackTokenizer *tokenizer,SymbolTable *symbolTable,VMwriter &vmwriter){
    symbolTable->startSubroutine();

    outputfile << indent << "<subroutineDec>" << std::endl;
    increaseIndent();
    std::string subType;
    std::string methodName;
    subType = tokenizer->content();
    writeAdvance(tokenizer);

    for(int i=0;i<3;i++)
    {
      if(tokenizer->tokenType() == IDENTIFIER) methodName = tokenizer->content();
      writeAdvance(tokenizer);
    }
    if(subType == "method")
    {
      symbolTable->define("this",className,ARG_K);
    }



    compileParameterList(tokenizer,symbolTable);





    outputfile << indent << "<subroutineBody>" << std::endl;
    increaseIndent();



    outputfile << indent << tokenizer->readbuffer() << std::endl;// output "{"
    tokenizer->advance();


    bool Var_is_uncompiled = true;

    while(tokenizer->tokenType() != SYMBOL)
    {
      if(tokenizer->keyword() == VAR){ compileVarDec(tokenizer,symbolTable);}
      else if(Var_is_uncompiled){
        if(subType == "method"){
          vmwriter.writeFunction(className + "." + methodName,symbolTable->varCount(VAR_K));
          vmwriter.writepush("argument",0);
          vmwriter.writepop("pointer",0);
        }
        else if(subType == "constructor"){
          vmwriter.writeFunction(className + "." + methodName,symbolTable->varCount(VAR_K));
          vmwriter.writepush("constant",symbolTable->varCount(FIELD_K));
          if(symbolTable->varCount(FIELD_K) == 0)
          std::cout << "you defined constructor in class without field." << std::endl
          << "this constructor must be function. or you forgot to define field." <<std::endl;
          vmwriter.writeCall("Memory.alloc",1);
          vmwriter.writepop("pointer",0);
        }
        else {
          vmwriter.writeFunction(className + "." + methodName,symbolTable->varCount(VAR_K));
        }


        Var_is_uncompiled = false;
      }
      else if(tokenizer->keyword() == LET
        || tokenizer->keyword() == IF
        || tokenizer->keyword() == WHILE
        || tokenizer->keyword() ==DO
        || tokenizer->keyword() ==RETURN){
        compileStatements(tokenizer,symbolTable,vmwriter);
      }

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


    // symbolTable->showsymbolTable();
    false_num = 0;
    true_num = 0;
    goto_num = 0;




    return;
  }




  void compileStatements(JackTokenizer *tokenizer,SymbolTable *symbolTable,VMwriter &vmwriter){
    outputfile << indent << "<statements>" << std::endl;
    increaseIndent();
    bool line_is_statement = true;
    while(line_is_statement)
    {

      switch (tokenizer->keyword())
      {
        case LET:
          compileLet(tokenizer,symbolTable,vmwriter);break;
        case DO:
          compileDo(tokenizer,symbolTable,vmwriter);break;
        case WHILE:
          compileWhile(tokenizer,symbolTable,vmwriter);break;
        case RETURN:
          compileReturn(tokenizer,symbolTable,vmwriter);break;
        case IF:
          compileIf(tokenizer,symbolTable,vmwriter);break;
        default:
          line_is_statement = false;


      }


    }
    decreaseIndent();
    outputfile << indent << "</statements>" << std::endl;
  }




  void compileLet(JackTokenizer *tokenizer,SymbolTable *symbolTable,VMwriter &vmwriter)
  {
    outputfile << indent << "<letStatement>" << std::endl;
    increaseIndent();
    std::string identifier;
    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;
      if(tokenizer->tokenType() == IDENTIFIER)  identifier = tokenizer->content();
      tokenizer->advance();
    }



    bool is_array = false;
    if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\[)")))
    {
      is_array = true;
      writeAdvance(tokenizer);//output "["
      compileExpression(tokenizer,symbolTable,vmwriter);
      writeAdvance(tokenizer);//output "]"
    }
    std::string segment;





    if(symbolTable->kindOf(identifier) == VAR_K) segment = "local";
    else if(symbolTable->kindOf(identifier) == ARG_K) segment = "argument";
    else if(symbolTable->kindOf(identifier) == STATIC_K) segment = "static";
    else if (symbolTable->kindOf(identifier) == FIELD_K) segment = "this";
    if(is_array){
      vmwriter.writepush(segment,symbolTable->indexOf(identifier));
      vmwriter.writeArithmetic(ADD);
      vmwriter.writepop("temp",1);

    }









    writeAdvance(tokenizer);//output "="

    compileExpression(tokenizer,symbolTable,vmwriter);

    writeAdvance(tokenizer);//output ";"

    decreaseIndent();
    outputfile << indent << "</letStatement>" << std::endl;


    if(is_array) {
      vmwriter.writepush("temp",1);
      vmwriter.writepop("pointer",1);
      vmwriter.writepop("that",0);
    }
    else vmwriter.writepop(segment,symbolTable->indexOf(identifier));
    return;

  }




  void compileDo(JackTokenizer *tokenizer,SymbolTable *symbolTable,VMwriter &vmwriter)
  {
    outputfile << indent << "<doStatement>" << std::endl;
    increaseIndent();
    writeAdvance(tokenizer);//output do

    std::string tempName = tokenizer->content();

    int nArgs = 0;

    writeAdvance(tokenizer);//output identifier


    if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\.)")))
    {
      writeAdvance(tokenizer);//output .
      if(symbolTable->there_is(tempName)){
        std::string segment;
        nArgs++;
        if(symbolTable->kindOf(tempName) == STATIC_K)segment = "static";
        else if(symbolTable->kindOf(tempName) == FIELD_K) segment = "this";
        else if(symbolTable->kindOf(tempName) == ARG_K) segment = "argument";
        else if(symbolTable->kindOf(tempName) == VAR_K) segment = "local";

        vmwriter.writepush(segment,symbolTable->indexOf(tempName));

        tempName = symbolTable->typeOf(tempName) + "." + tokenizer->content();
      }
      else {

        tempName += "." +  tokenizer->content();
      }
      for(int i=0;i<2;i++) writeAdvance(tokenizer);//output subroutinName(

      nArgs += compileExpressionList(tokenizer,symbolTable,vmwriter);

      writeAdvance(tokenizer);//output )
    }
    else if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\()")))
    {

      nArgs++;
      tempName = className + "." +  tempName;
      vmwriter.writepush("pointer",0);

      writeAdvance(tokenizer);//output (

      nArgs += compileExpressionList(tokenizer,symbolTable,vmwriter);

      writeAdvance(tokenizer);//output )
    }

    writeAdvance(tokenizer); //output ;

    vmwriter.writeCall(tempName,nArgs);

    vmwriter.writepop("temp",0);

    decreaseIndent();
    outputfile << indent << "</doStatement>" << std::endl;

  }




  void compileWhile(JackTokenizer *tokenizer,SymbolTable *symbolTable,VMwriter &vmwriter)
  {
    int t = true_num;
    int f = false_num;
    int go = goto_num;
    true_num++;
    goto_num++;
    false_num++;
    outputfile << indent << "<whileStatement>" << std::endl;
    increaseIndent();
    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output "("
      tokenizer->advance();
    }
    vmwriter.writeLabel("GLABEL" + std::to_string(go));
    compileExpression(tokenizer,symbolTable,vmwriter);
    vmwriter.writeIf("IF_TRUE" + std::to_string(t));
    vmwriter.writeGoto("IF_FALSE" + std::to_string(f));
    vmwriter.writeLabel("IF_TRUE" + std::to_string(t));


    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output ")" "{"
      tokenizer->advance();
    }

    compileStatements(tokenizer,symbolTable,vmwriter);

    vmwriter.writeGoto("GLABEL" + std::to_string(go));
    vmwriter.writeLabel("IF_FALSE"+ std::to_string(f));

    outputfile << indent << tokenizer->readbuffer() << std::endl;//output "}"
    tokenizer->advance();

    decreaseIndent();
    outputfile << indent << "</whileStatement>" << std::endl;
  }








  void compileIf(JackTokenizer *tokenizer,SymbolTable *symbolTable,VMwriter &vmwriter)
  {
    int t = true_num;
    int f = false_num;
    int go = goto_num;
    true_num++;
    goto_num++;
    false_num++;
    outputfile << indent << "<ifStatement>" << std::endl;
    increaseIndent();

    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output "if" "("
      tokenizer->advance();
    }

    compileExpression(tokenizer,symbolTable,vmwriter);//expressions

    for(int i=0;i<2;i++)
    {
      outputfile << indent << tokenizer->readbuffer() << std::endl;//output ")" "{"
      tokenizer->advance();
    }
    vmwriter.writeIf("IF_TRUE" + std::to_string(t));
    vmwriter.writeGoto("IF_FALSE" + std::to_string(f));
    vmwriter.writeLabel("IF_TRUE" + std::to_string(t));
    compileStatements(tokenizer,symbolTable,vmwriter);//statements

    outputfile << indent << tokenizer->readbuffer() << std::endl;//output "}"
    tokenizer->advance();

    if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(else)")))
    {
      vmwriter.writeGoto("GLABEL" + std::to_string(go));
      vmwriter.writeLabel("IF_FALSE" + std::to_string(f));
      for(int i=0;i<2;i++)
      {
        outputfile << indent << tokenizer->readbuffer() << std::endl;//output "else" "{"
        tokenizer->advance();
      }

      compileStatements(tokenizer,symbolTable,vmwriter);

      outputfile << indent << tokenizer->readbuffer() << std::endl;//output "}"
      tokenizer->advance();
      vmwriter.writeLabel("GLABEL" + std::to_string(go));
    }
    else{
      vmwriter.writeLabel("IF_FALSE" + std::to_string(f));
    }




    decreaseIndent();
    outputfile << indent << "</ifStatement>" << std::endl;

  }


  void compileReturn(JackTokenizer *tokenizer,SymbolTable *symbolTable,VMwriter &vmwriter)
  {
    outputfile << indent << "<returnStatement>" << std::endl;
    increaseIndent();

    writeAdvance(tokenizer);//output "return"

    if(tokenizer->readbuffer() != "<symbol>;</symbol>") compileExpression(tokenizer,symbolTable,vmwriter);
    else {
      vmwriter.writepush("constant",0);
    }
    writeAdvance(tokenizer);//output ";"

    decreaseIndent();
    outputfile << indent << "</returnStatement>" << std::endl;

    vmwriter.writeReturn();

    return;
  }



  void compileExpression(JackTokenizer *tokenizer,SymbolTable *symbolTable,VMwriter &vmwriter)
  {

    std::vector<Command> symbol;
    outputfile << indent << "<expression>" << std::endl;
    increaseIndent();
    if(tokenizer->symbol() == "-"){
      symbol.push_back(NEG);
    }
    else if(tokenizer->symbol() == "~") {
      symbol.push_back(NOT);
    }

    compileTerm(tokenizer,symbolTable,vmwriter);


    std::string symboltemp;

    while(tokenizer->tokenType() == SYMBOL)
    {
      if(std::regex_search(tokenizer->symbol(),opRegex))
      {
        symboltemp = tokenizer->symbol();
        if(symboltemp == "<") symboltemp = "&lt;";
        else if(symboltemp == ">") symboltemp = "&gt;";
        else if(symboltemp == "&") symboltemp = "&amp;";
        outputfile << indent << "<symbol>" << symboltemp << "</symbol>" << std::endl;
        tokenizer->advance();
        compileTerm(tokenizer,symbolTable,vmwriter);


        if(symboltemp == "+") symbol.push_back(ADD);
        else if(symboltemp == "-") symbol.push_back(SUB);
        else if(symboltemp == "=") symbol.push_back(EQ);
        else if(symboltemp == "&lt;") symbol.push_back(LT);
        else if(symboltemp == "&gt;") symbol.push_back(GT);
        else if(symboltemp == "&amp;") symbol.push_back(AND);
        else if(symboltemp == "|") symbol.push_back(OR);
        else if(symboltemp == "-") symbol.push_back(NEG);
        else if(symboltemp == "~") symbol.push_back(NOT);
        else if(symboltemp == "*") vmwriter.writeCall("Math.multiply",2);
        else if(symboltemp == "/") vmwriter.writeCall("Math.divide",2);


      }
      else break;
    }




    while(!symbol.empty()){
      Command c = symbol.back();
      symbol.pop_back();
      vmwriter.writeArithmetic(c);
    }

    decreaseIndent();
    outputfile << indent << "</expression>" << std::endl;
  }


















  void compileParameterList(JackTokenizer *tokenizer,SymbolTable *symbolTable){
    outputfile << indent << "<parameterList>" << std::endl;
    increaseIndent();

    Kind kind = ARG_K;

    while (!std::regex_search(tokenizer->readbuffer(),std::regex(R"(\))")))
    {

      outputfile << indent << tokenizer->readbuffer() << std::endl;
      std::string type = tokenizer->content();
      tokenizer->advance();



      outputfile << indent << tokenizer->readbuffer() << std::endl;
      std::string name = tokenizer->content();
      tokenizer->advance();

      symbolTable->define(name,type,kind);




      if(tokenizer->content() == ","){
        outputfile << indent << tokenizer->readbuffer() << std::endl;
        tokenizer->advance();
      }


    }

    decreaseIndent();
    outputfile << indent << "</parameterList>" << std::endl;
    outputfile << indent << tokenizer->readbuffer() << std::endl;//output ")"
    tokenizer->advance();
  }











  void compileVarDec(JackTokenizer *tokenizer,SymbolTable *symbolTable){
    outputfile << indent << "<varDec>" << std::endl;
    increaseIndent();

    outputfile << indent << tokenizer->readbuffer() << std::endl;



    Kind kind;
    if(tokenizer->keyword() == VAR) kind = VAR_K;
    else std::cout << "compilationError!!" << std::endl;

    tokenizer->advance();
    outputfile << indent << tokenizer->readbuffer() << std::endl;

    std::string type = tokenizer->content();


    while (!std::regex_search(tokenizer->readbuffer(),std::regex(R"(;)"))){
      tokenizer->advance();
      if(tokenizer->tokenType() == IDENTIFIER){

        symbolTable->define(tokenizer->content(),type,kind);
      }
      outputfile << indent << tokenizer->readbuffer() << std::endl;

    }

    tokenizer->advance();
    decreaseIndent();
    outputfile << indent << "</varDec>" << std::endl;
  }








  void compileclassVarDec(JackTokenizer *tokenizer,SymbolTable &symbolTable){
    outputfile << indent << "<classVarDec>" << std::endl;
    increaseIndent();

    outputfile << indent << tokenizer->readbuffer() << std::endl;

    Kind kind;
    if(tokenizer->keyword() == STATIC) kind = STATIC_K;
    else if(tokenizer->keyword() == FIELD) kind = FIELD_K;


    tokenizer->advance();
    outputfile << indent << tokenizer->readbuffer() << std::endl;


    std::string type = tokenizer->content();


    while (!std::regex_search(tokenizer->readbuffer(),std::regex(R"(;)")))
    {


      tokenizer->advance();
      if(tokenizer->tokenType() == IDENTIFIER){

        symbolTable.define(tokenizer->content(),type,kind);
      }
      outputfile << indent << tokenizer->readbuffer() << std::endl;




    }

    tokenizer->advance();
    decreaseIndent();
    outputfile << indent << "</classVarDec>" << std::endl;
  }






  int compileExpressionList(JackTokenizer *tokenizer,SymbolTable *symbolTable,VMwriter &vmwriter){
    outputfile << indent << "<expressionList>" << std::endl;
    int counter = 0;
    increaseIndent();

    while (!std::regex_search(tokenizer->readbuffer(),std::regex(R"(\))")))
    {
      compileExpression(tokenizer,symbolTable,vmwriter);
      counter++;
      if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(,)"))){
        outputfile << indent << tokenizer->readbuffer() << std::endl;
        tokenizer->advance();
      }

    }
    decreaseIndent();
    outputfile << indent << "</expressionList>" << std::endl;

    return counter;
  }





  void compileTerm(JackTokenizer *tokenizer,SymbolTable *symbolTable,VMwriter &vmwriter){

    outputfile << indent << "<term>" << std::endl;
    increaseIndent();


    if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\()")) )
    {





      writeAdvance(tokenizer);//output "("


      compileExpression(tokenizer,symbolTable,vmwriter);


      writeAdvance(tokenizer);//output ")"

      decreaseIndent();
      outputfile << indent << "</term>" << std::endl;


      return;
    }
    else if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(-)")) || std::regex_search(tokenizer->readbuffer(),std::regex(R"(~)")))
    {

      writeAdvance(tokenizer);

      compileTerm(tokenizer,symbolTable,vmwriter);

      decreaseIndent();
      outputfile << indent << "</term>" << std::endl;



      return;



    }


    else {
      outputfile << indent << tokenizer->readbuffer() << std::endl;
      //output just one letter term or varName or subroutinename or classname
    }
    bool terminal = true;
    TokenType tempType = tokenizer->tokenType();


    std::string tempconstant = tokenizer->content();

    tokenizer->advance();



    if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\.)")))
    {
      KeyWord kw;
      terminal = false;
      if(tempType != IDENTIFIER) std::cout << "compilationError. you use \".\" after something not identifier" << std::endl;
      std::string TypeName;

      if(symbolTable->there_is(tempconstant)){
        kw = METHOD;
        TypeName = symbolTable->typeOf(tempconstant);
        std::string segment;
        if(symbolTable->kindOf(tempconstant) == STATIC_K)segment = "static";
        else if(symbolTable->kindOf(tempconstant) == FIELD_K) segment = "this";
        else if(symbolTable->kindOf(tempconstant) == ARG_K) segment = "argument";
        else if(symbolTable->kindOf(tempconstant) == VAR_K) segment = "local";


        vmwriter.writepush(segment,symbolTable->indexOf(tempconstant));
      }

      writeAdvance(tokenizer);//output .

      std::string subName = tokenizer->content();

      for(int i=0;i<2;i++) writeAdvance(tokenizer);//output "methodName ("


      int nArgs = compileExpressionList(tokenizer,symbolTable,vmwriter);


      if(kw == METHOD) vmwriter.writeCall(TypeName + "." + subName,nArgs + 1);
      else vmwriter.writeCall(tempconstant + "." + subName,nArgs);

      writeAdvance(tokenizer);







    }
    else if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\()"))){




      terminal = false;

      if(tempType != IDENTIFIER) std::cout << "compilationError. you use \"(\" after something not identifier"<< std::endl;
      vmwriter.writepush("pointer",0); // address of object of parent method (this 0 is the field variable of the object)
      writeAdvance(tokenizer); //output(
      int nArgs = compileExpressionList(tokenizer,symbolTable,vmwriter);


      vmwriter.writeCall(className + "." + tempconstant,nArgs + 1);
      writeAdvance(tokenizer);






    }
    else if(std::regex_search(tokenizer->readbuffer(),std::regex(R"(\[)")))
    {






      terminal = false;
      std::string segment;
      if(symbolTable->kindOf(tempconstant) == VAR_K) segment = "local";
      else if(symbolTable->kindOf(tempconstant) == ARG_K) segment = "argument";
      else if(symbolTable->kindOf(tempconstant) == STATIC_K) segment = "static";
      else if(symbolTable->kindOf(tempconstant) == FIELD_K) segment = "this";

      vmwriter.writepush(segment,symbolTable->indexOf(tempconstant));
      writeAdvance(tokenizer);
      compileExpression(tokenizer,symbolTable,vmwriter);
      vmwriter.writeArithmetic(ADD);
      vmwriter.writepop("pointer",1);
      vmwriter.writepush("that",0);
      writeAdvance(tokenizer);









    }



    if(terminal){

      if(tempType == INT_CONST) vmwriter.writepush("constant",std::stoi(tempconstant));

      else if(tempType == STRING_CONST){
        vmwriter.writepush("constant",tempconstant.length());
        vmwriter.writeCall("String.new",1);
        for(int i=0;i<tempconstant.length();i++){
          char k = tempconstant[i];
          int j = k;
          vmwriter.writepush("constant",j);
          vmwriter.writeCall("String.appendChar",2);

        }
      }
      else if(tempType == IDENTIFIER){
        std::string segment;
        if(symbolTable->kindOf(tempconstant) == VAR_K) segment = "local";
        else if(symbolTable->kindOf(tempconstant) == ARG_K) segment = "argument";
        else if(symbolTable->kindOf(tempconstant) == STATIC_K) segment = "static";
        else if(symbolTable->kindOf(tempconstant) == FIELD_K){
          segment = "this";
        }


        vmwriter.writepush(segment,symbolTable->indexOf(tempconstant));

      }
      else if(tempconstant == "this"){
        vmwriter.writepush("pointer",0);
      }
      else if(tempconstant == "true"){
        vmwriter.writepush("constant",0);
        vmwriter.writeArithmetic(NOT);
      }
      else if(tempconstant == "false"|| tempconstant == "null"){
        vmwriter.writepush("constant",0);
      }

    }

    decreaseIndent();
    outputfile << indent << "</term>" << std::endl;
  }

};








int main(int argc,char* argv[]){
  std::smatch m;
  std::string s = argv[1];
  bool is_jackfile = std::regex_search(s,m,std::regex(R"((.*)(\.jack))")); //    if (std::regex_search(line,commentout)) line = std::regex_replace(line,commentout,"");


  JackTokenizer tokenizer(argv[1]);
  CompilationEngine ce;
  SymbolTable symbolTable;
  VMwriter vmwriter(m[1].str() + ".vm");

  while(tokenizer.hasMoreTokens())
  {
    tokenizer.advance();
    if(tokenizer.tokenType() == KEYWORD){
      if(tokenizer.keyword() == CLASS) ce.compileClass(&tokenizer,symbolTable,vmwriter);
    }

  }


  return 0;
}
