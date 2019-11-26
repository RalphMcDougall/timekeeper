#ifndef _TIMEKEEPER
#define _TIMEKEEPER

#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <chrono>

typedef long long tkll;


class RecordBook
{
    public:
    // Total execution times of all functions
    static std::unordered_map<std::string, tkll> function_records;
    static std::set<std::string> function_name_set;

};



class Tracker
{
    static int function_depth;

    std::string function_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_tp;

    public:

    Tracker(std::string function_name);
    ~Tracker();
};


#endif