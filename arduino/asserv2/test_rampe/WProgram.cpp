#include "WProgram.h"

#include <iostream>

using namespace std;

_Serial Serial;

void _Serial::println(const char* m)
{
    cout << m << endl;
}

void _Serial::println(const int m)
{
    cout << m << endl;
}

void _Serial::println(const double m)
{
    cout << m << endl;
}

void _Serial::print(const char* m)
{
    cout << m;
}
void _Serial::print(const int m)
{
    cout << m;
}

void _Serial::print(const double m)
{
    cout << m;
}
