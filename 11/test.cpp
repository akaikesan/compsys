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


  SymbolRow(const SymbolRow& symbolRow):
  name(symbolRow.name),type(symbolRow.type),kind(symbolRow.kind),indexNumber(symbolRow.indexNumber){

  }
  ~SymbolRow(){}

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
    subroutineSymbolTable.clear();
  }

  void define(std::string name,std::string type,Kind kind)
  {





    if(kind = STATIC_K)
    {
      staticCounter++;
    }
    else if (kind = FIELD_K)
    {

      fieldCounter++;
    }
    else if (kind = ARG_K)
    {
      argCounter++;
    }
    else if (kind = VAR_K)
    {
      varCounter++;
    }

    classSymbolTable.push_back(SymbolRow(name,type,kind,varCount(kind)));

    std::cout << kindOf("go") <<std::endl;
    std::cout << typeOf("go") <<std::endl;
    std::cout << indexOf("go") << std::endl;

  }








  int varCount(Kind kind)
  {
    if(kind = STATIC_K)
    {
      return staticCounter;
    }
    else if (kind = FIELD_K)
    {

      return fieldCounter;
    }
    else if (kind = ARG_K)
    {

      return argCounter;
    }
    else if (kind = VAR_K)
    {
      return varCounter;
    }
    else
    {
      std::cout << "compiler Error!" << std::endl;
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

  }






};
