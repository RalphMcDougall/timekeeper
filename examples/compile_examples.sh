echo "Compiling prime_root"
g++ -c prime_root.cpp -std=c++11 -O2
g++ prime_root.o timekeeper/timekeeper.o -std=c++11 -O2 -o prime_root

echo "Compiling prime_sieve"
g++ -c prime_sieve.cpp -std=c++11 -O2
g++ prime_sieve.o timekeeper/timekeeper.o -std=c++11 -O2 -o prime_sieve

echo "Done"