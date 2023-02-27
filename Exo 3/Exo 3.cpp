#include <iostream>

using namespace std;

struct essai
{
    int n;
    float x;
};

void raz(essai* ptr)
{
    ptr->n = 0;
    ptr->x = 0;
}

void raz(essai& ref)
{
    ref.n = 0;
    ref.x = 0;
}

int main()
{
    essai test;
    test.n = 3;
    test.x = 6;
    cout << "valeur de n : " << test.n << endl;
    cout << "valeur de x : " << test.x << endl;
    raz(&test);
    cout << "valeur de n : " << test.n << endl;
    cout << "valeur de x : " << test.x << endl;
    test.n = 3;
    test.x = 6;
    cout << "valeur de n : " << test.n << endl;
    cout << "valeur de x : " << test.x << endl;
    raz(test);
    cout << "valeur de n : " << test.n << endl;
    cout << "valeur de x : " << test.x << endl;


}

