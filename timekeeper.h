#ifndef _TIMEKEEPER
#define _TIMEKEEPER

#include <string>


class Tracker
{
    static int function_depth;

    std::string function_name;

    public:

    Tracker(std::string function_name);
    ~Tracker();
};


#endif