#include <iostream>
using namespace std;


int main()
{
    int number;
    int tab[100];
    int conditional;

    cout << "Write a number: ";
    cin >> number;
    cin.ignore();

    int i = 0;
    do
    {
       i++;
       tab[i] = number % 2;
       number /= 2;
       conditional = 2 * number + tab[i];
    } while ( conditional >= 2 );
    
    cout << "Your number in binary system : ";
    for(; i > 0; i--)
    {
        cout << tab[i];
    }
    cout << endl;

    return 0;
}