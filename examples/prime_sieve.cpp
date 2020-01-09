#include <bits/stdc++.h>

#include "timekeeper/timekeeper.h"

using namespace std;

int main(int argc, char** argv)
{
    int MAX_N = stoi(argv[1]);
    TimeKeeper::Tracker t("main", "prime_sum", "sieve", true, MAX_N);

    int cnt = 0;

    vector<bool> isPrime(MAX_N + 1, true);

    for (int i = 2; i <= MAX_N; ++i)
    {
        if (!isPrime[i]) continue;
        cnt++;
        for (int j = i + i; j < isPrime.size(); j += i)
        {
            isPrime[j] = false;
        }
    }

    TimeKeeper::Tracker s("sum", true, cnt);
    long long ans = 0;
    for (int i = 2; i < isPrime.size(); ++i)
    {
        ans += (isPrime[i] ? i : 0);
    }

    cout << "Answer: " << ans << std::endl;

    return 0;
}