#ifndef _TIMEKEEPER
#define _TIMEKEEPER

#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

typedef long long tkll;

enum EventType {FUNCTION_START, FUNCTION_END};

struct TKEvent
{
    tkll time_stamp;            // How many nanoseconds after the program started did this event get issued
    EventType event_type;       // What is this event for
    std::string event_issuer;   // Which function created the event
};

namespace RecordBook
{
    std::vector<TKEvent*> event_order;
    int record_start;
    bool started;
    std::chrono::time_point<std::chrono::high_resolution_clock> ns_start_time;
    int function_depth;

    void addEvent(tkll, EventType, std::string);

};

class Tracker
{
    std::string function_name;

    public:
    Tracker(std::string function_name);
    ~Tracker();
};


#endif