#include <bits/stdc++.h>
using namespace std;

bool check_int(std::string str)
{
  regex alpha(R"(\D)");

  if (regex_search(str,alpha))
  {
    return false;
  }
  return true;
}

int main()
{
  if(check_int("2"))
  {
    cout << "ysy" << endl;
  }

  cout << check_int("1111")<< stoi("32") <<endl;

  return 0;
}


//for( auto i = st.v.begin(); i != st.v.end() ; ++i ) {
//     std::cout << i->first << " " << i->second << "\n";
// }
