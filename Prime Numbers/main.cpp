#include <iostream>
using namespace std;

bool checkNumberForPrime(int number);

int main()
{
    cout << "Write prime number: ";
    int prime;
    cin >> prime;
    cin.ignore();
    bool check = checkNumberForPrime(prime);
    if( check == false )
    {
        cout << "Your number isn't a prime number!" << endl;
    }
    else
    {
        cout << "Your number is a prime number!" << endl;
    }
    return 0;
}

bool checkNumberForPrime(int number)
{
    int count = 0;
    if( number <= 2 )
    {
        return false;
    }
    else
    {
        for(unsigned int i = 2; i < number; i++)
        {
            if( (number % i) == 0 )
            {
                count++;
            }
        }
    }
    if( count == 0 )
    {
        return true;
    }
    return false;
}