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

enum EventType 
{
    TRACKING_START = 0, 
    TRACKING_END = 1
};

struct TKEvent
{
    tkll time_stamp;            // How many nanoseconds after the program started did this event get issued
    EventType event_type;       // What is this event for
    std::string event_issuer;   // Which function created the event
};

namespace TimeKeeper
{
    extern std::string program_name;      // The name given to identify this program
    extern std::vector<TKEvent*> event_order;  // The order that all events get issued in. Added to CSV file at the end.
    extern int record_start;                   // The integer time that the program started. Used for name of CSV file.
    extern bool started;               // Whether or not the recording has been started before
    extern std::chrono::time_point<std::chrono::high_resolution_clock> ns_start_time; // Used to calculate how long after the start each event occurs

    void addEvent(EventType, std::string);
    void setName(std::string);

};

class Tracker
{
    std::string tracker_name;   // Name that will be used to refer to this tracker
    bool is_root;               // Whether or not the tracker is the main program tracker
    bool recorded;              // Whether or not this tracker has already been recorded. Don't want the same one recorded multiple times.
    
    void stopTracking();
    void recordExecution();

    public:
    Tracker(std::string);
    ~Tracker();
    void stop();
};


#endif