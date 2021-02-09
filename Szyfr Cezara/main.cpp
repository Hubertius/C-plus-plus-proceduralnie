#include <iostream>
#include <string>
using namespace std;

string encryptionCeasar(string);
string decryptionCeasar(string);

const int HOW_MANY = 3;

int main()
{
    string word;
    cout << "Write a word for encryption: ";
    getline(cin, word);

    cout << "Encrypted word: " << encryptionCeasar(word) << endl;
    cout << "Decrypted word: " << decryptionCeasar(encryptionCeasar(word)) << endl;
    return 0;
}


string encryptionCeasar(string word)
{
    int temp;
    for(int i = 0; i < word.length(); i++)
    {
        temp = word[i] + HOW_MANY;
        if( temp < 0)
        {
            word[i] += (255 + HOW_MANY);
        }
        else if( temp > 255 )
        {
            word[i] -= (255 - HOW_MANY);
        }
        else
        {
            word[i] += HOW_MANY;
        }
    }
    return word;
}

string decryptionCeasar(string word)
{
    int temp;
    for(int i = 0; i < word.length(); i++)
    {
        temp = word[i] - HOW_MANY;
        if( temp < 0 )
        {
            word[i] += (255 - HOW_MANY);
        }
        else if( temp > 255 )
        {
            word[i] -= (255 + HOW_MANY);
        }
        else
        {
            word[i] -= HOW_MANY;
        }
    }
    return word;
}