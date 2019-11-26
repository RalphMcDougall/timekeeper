#include "timekeeper.h"

// Set the initial function depth
int Tracker::function_depth = 0;

Tracker::Tracker(std::string _function_name)
{
    // Constructor that starts timing of function
    function_name = _function_name;


    function_depth++;
}

Tracker::~Tracker()
{
    // Destructor that stops timing of function
    function_depth--;
}