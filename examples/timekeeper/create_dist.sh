echo "Compiling C++ library"
g++ -c timekeeper.cpp -std=c++11 -O2

echo "Copying into examples folder"
cp ./* ./examples/timekeeper/

echo "Done"