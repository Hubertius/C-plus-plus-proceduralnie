#include <string>
#include <algorithm>
#include <iostream>
std::string disemvowel(std::string str)
{
    char  vowels[] = "AEIOUaeiou";
    for(char * vowel = vowels; *vowel != '\0'; vowel++)
    {
  
        str.erase(std::remove(str.begin(), str.end(), *vowel), str.end());
        
    }
    return str;
}

int main()
{
    std::string str = "This website is for losers LOL!";
    std::cout << "String before erase: " << str << std::endl;
    std::cout << "String after erase: " << disemvowel(str) << std::endl;
    return 0;
}