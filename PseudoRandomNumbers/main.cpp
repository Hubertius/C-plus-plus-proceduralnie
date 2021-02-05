#include <iostream>
#include <ctime>

void lotto(int total_balls, int balls_to_allot);

using std::cout;
using std::endl;

int main()
{
    lotto(49, 6);
    return 0;
}


void lotto(int total_balls, int balls_to_allot)
{
    if( total_balls < balls_to_allot )
    {
        cout << "Erorr! Wrong parameters in the function!" << endl;
        return;
    }
    srand(time(NULL));

    int * balls = new int[balls_to_allot];

    for(int i = 0; i < balls_to_allot; i++)
    {
        *(balls + i) = rand() % total_balls + 1;
        for(int j = 0; j <= i; j++)
        {
            if(balls[i] == balls[j] && i != j)
            {
                i--;
                break;
            }
            else if(i == j)
            {
                cout << "ball[" << i + 1 << "] = " << balls[i] << endl;
            }
        }
    }
}