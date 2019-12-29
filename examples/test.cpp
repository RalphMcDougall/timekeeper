#include <bits/stdc++.h>

#include "timekeeper/timekeeper.h"

using namespace std;


int main()
{
    // Generate all of the primes up to 10^7
    Tracker t("main", "prime_search", "naive");

    vector<int> primes;
    for (int i = 2; i <= 1E2; ++i)
    {
        bool found = false;
        Tracker loop("loop");
        for (int j = 2; j * j <= i; ++j)
        {
            if (i % j == 0)
            {
                found = true;
                break;
            }
        }
        loop.stop();
        if (!found)
        {
            Tracker pb("pb");
            primes.push_back(i);
            pb.stop();
        }
    }

    cout << "Finished" << endl;
    return 0;
}