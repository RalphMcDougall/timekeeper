#include <bits/stdc++.h>

#include "timekeeper/timekeeper.h"

using namespace std;

int main(int argc, char** argv)
{
    int MAX_N = stoi(argv[1]);
    TimeKeeper::Tracker t("main", "prime_count", "sieve", true, MAX_N);

    int ans = 0;

    vector<bool> isPrime(MAX_N + 1, true);

    for (int i = 2; i <= MAX_N; ++i)
    {
        if (!isPrime[i]) continue;
        ans++;
        for (int j = i + i; j < isPrime.size(); j += i)
        {
            isPrime[j] = false;
        }
    }

    cout << "Found: " << ans << endl;

    return 0;
}