#include <bits/stdc++.h>
using namespace std;

string binary(string bin){
    int bina = stoi(bin);
    int ans = 0;
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

      cout <<inst.length()<<":" << ans << ":" << inst  << endl;
      inst = "0" + inst;
      ans--;
    }

    return inst;
}



int main()
{

  cout << binary("2");

  return 0;
}
