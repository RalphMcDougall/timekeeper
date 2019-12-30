#include <bits/stdc++.h>

#include "timekeeper/timekeeper.h"

using namespace std;

int main(int argc, char **argv)
{
    int MAX_N = stoi(argv[1]);
    // Generate all of the primes up to MAX_N
    TimeKeeper::Tracker t("main", "prime_count", "sqrt", true, MAX_N);

    int ans = 0;
    for (int i = 2; i <= MAX_N; ++i)
    {
        bool found = false;
        for (int j = 2; j * j <= i; ++j)
        {
            if (i % j == 0)
            {
                found = true;
                break;
            }
        }
        if (!found) ans++;
    }
    cout << "Found: " << ans << std::endl;
    return 0;
}