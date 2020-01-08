echo "Compiling C++ library"
g++ -c timekeeper.cpp -std=c++11 -O2
echo "Copying header and object to distribution folder"
cp timekeeper.o ../timekeeper
cp timekeeper.h ../timekeeper
echo "Copying analyzer to distribution folder"
cp analyzer.py ../timekeeper
echo "Done"