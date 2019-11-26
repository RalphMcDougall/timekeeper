#include <bits/stdc++.h>

#include "timekeeper.h"

using namespace std;

void f1()
{
    Tracker t("f1");
    int c = 0;
    for (int i = 0; i < 10000; ++i)
    {
        c += 1;
    }
}
void f2()
{
    Tracker t("f2");
    int c = 0;
    for (int i = 0; i < 1000000; ++i)
    {
        c += 1;
    }
}

void exec_function()
{
    Tracker t("exec_function");
    
    f1();
    f2();

}

int main()
{
    cout << "Starting test program" << endl;
    exec_function();

    cout << "Ending test program" << endl;

    return 0;
}