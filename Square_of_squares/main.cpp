#include <iostream>
#include <cmath>
using namespace std;

bool is_square(int n)
{
  if(n < 0)
  {
      return false;
  }
  return (static_cast<int>(sqrt(n)) * static_cast<int>(sqrt(n))) == n;

}

int main()
{
    if(is_square(26))
    {
        cout << "True";
    }
    else
    {
        cout << "False";
    }
    return 0;
}