g++ -c prime_brute.cpp -std=c++11 -O2
g++ prime_brute.o timekeeper/timekeeper.o -std=c++11 -O2 -o prime_brute

g++ -c prime_mr.cpp -std=c++11 -O2
g++ prime_mr.o timekeeper/timekeeper.o -std=c++11 -O2 -o prime_mr

g++ -c prime_root.cpp -std=c++11 -O2
g++ prime_root.o timekeeper/timekeeper.o -std=c++11 -O2 -o prime_root

g++ -c prime_sieve.cpp -std=c++11 -O2
g++ prime_sieve.o timekeeper/timekeeper.o -std=c++11 -O2 -o prime_sieve
