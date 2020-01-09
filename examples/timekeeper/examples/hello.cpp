#include <bits/stdc++.h>
#include "timekeeper/timekeeper.h"

using namespace std;

void sum()
{
    TimeKeeper::Tracker t("sum");

    cout << "The sum of the first 100 values is: ";

    int ans = 0;
    for (int i = 1; i <= 100; ++i)
    {
        ans += i;
    }
    cout << ans << endl;
}


int main()
{
    TimeKeeper::Tracker t("main", "hello", "hello");

    cout << "Hello!" << endl;

    sum();


    return 0;
}