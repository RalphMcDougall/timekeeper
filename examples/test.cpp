#include <bits/stdc++.h>

#include "timekeeper/timekeeper.h"

using namespace std;

const int MAX_N = 1E2;

int main()
{
    // Generate all of the primes up to MAX_N
    TimeKeeper::Tracker t("main", "prime_search", "naive", true, MAX_N);

    vector<int> primes;
    for (int i = 2; i <= MAX_N; ++i)
    {
        bool found = false;
        TimeKeeper::Tracker loop("loop");
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
            TimeKeeper::Tracker pb("pb");
            primes.push_back(i);
            pb.stop();
        }
    }

    cout << "Finished" << endl;
    return 0;
}