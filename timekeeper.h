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

enum EventType {TRACKING_START, TRACKING_END};

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
    bool started = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> ns_start_time; // Used to calculate how long after the start each event occurs

    void addEvent(EventType, std::string);

};

class Tracker
{
    std::string function_name;
    void stopTracking();
    void recordExecution();
    bool is_root;

    public:
    Tracker(std::string);
    ~Tracker();
    void stop();
};


#endif