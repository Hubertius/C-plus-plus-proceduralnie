#include <iostream>
#include <cmath>
using namespace std;


int main()
{
    string number;
    int score = 0;
    cout << "Write your number: ";
    cin >> number;
    cin.ignore();

    for(short i = 0; i < number.length(); i++)
    {
        if( number[i]  == '1' )
        {
            score = score * 2 + 1;
        }
        else if( number[i] == '0' )
        {
            score *= 2;
        }
        else
        {
            cout << "You entered wrong character (another than 1 or 0). Program will end now.";
            return 1;
        }
    }
    cout << "Your number in decimal system: " << score;
    return 0;
}