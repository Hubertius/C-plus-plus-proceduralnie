#include <iostream>
#include <string>
#include <cmath>
using namespace std;

void initMenuBox();
void menuDecision(int choice);
double fieldCircle(double);
double fieldSquare(double);
double fieldRectangle(double a, double b);
double fieldTriangle(double a, double h);
bool validation(string);
bool validation();


int main()
{
    char character = 't';
    int choice;

    do
    {
        system("cls");
        initMenuBox();
        while( !(cin >> choice) )
        {
            cin.clear(); // removes flag of error from cin
            cin.ignore(1024, '\n');
            initMenuBox();
            cout << "You wrote wrong character, try again!" << endl;
        }

        cin.ignore(1024, '\n');

        menuDecision(choice);
        do
        {
            cout << "Do you want to continue (T/N)? ";
            cin >> character;
            cin.ignore(1024, '\n');
        } while (character != 'n' && character != 'N' && character != 't' && character != 'T');
        

    } while (character == 't' || character ==  'T');  // continue the program until character == 'T" or character == 't'
    

    cout << "Hello!";
    return 0;
}

void initMenuBox()
{
    cout << "Choose your option." << endl;
    cout << "1. Field of the circle." << endl;
    cout << "2. Field of the square." << endl;
    cout << "3. Field of the rectangle." << endl;
    cout << "4. Field of the triangle." << endl;

}

void menuDecision(int choice)
{
    system("cls");
    initMenuBox();
    double firstSide, secondSide, radius, height;
    switch(choice)
    {
        case 1:
            do
            {
                cout << "Write value of radius of the circle: ";
                cin >> radius;
            } while ( !validation() );
            cout << "Field of the cicle: " << fieldCircle(radius) << endl;
            break;
        case 2:
            do
            {
                cout << "Write value of side of the square: ";
                cin >> firstSide;
            } while ( !validation("Eror! You wrote wong data!") );
            cout << "Field of the square: " << fieldSquare(firstSide) << endl;
            break;
        case 3:
            do
            {
                cout << "Write value of first side and second side of the rectangle: ";
                cin >> firstSide >> secondSide ;
            } while ( !validation("Eror! You wrote wong data!") );
            cout << "Field of the rectangle: " << fieldRectangle(firstSide, secondSide) << endl;
            break;
        case 4:
            do
            {
                cout << "Write value of first side and height of the rectangle: ";
                cin >> firstSide >> height;
            } while ( !validation("Eror! You wrote wong data!") );
            cout << "Field of the triangle: " << fieldTriangle(firstSide, height) << endl;
            break;
        default:
            cout << "Wrong option (nothing from the menu)." << endl;
    }
}

double fieldCircle(double r)
{
    return  r * r * M_PI;
}

double fieldSquare(double a)
{
    return a * a;
}

double fieldRectangle(double a, double b)
{
    return a * b;
}

double fieldTriangle(double a, double h)
{
    return 0,5 * a  * h;
}

bool validation(string error_msg)
{
    if( cin.rdstate() ) // Returns the current internal error state flags of the stream (goodbit, eofbit, failbit, badbit).
    {
        cin.clear(); // removes flag of error from cin
        cin.ignore(1024, '\n'); // ignores characters in the buffer
        system("cls"); // clears screen == "cls" shortcut
        initMenuBox();
        cout << error_msg;
        return false;
    }
    return true;
}

bool validation()
{
    if (cin.rdstate())
     {
        cin.clear();
        cin.ignore(1024, '\n');
        
        system("cls");
        initMenuBox();

        return false;
     } 
     return true;       
}