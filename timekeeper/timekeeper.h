#ifndef _TIMEKEEPER
#define _TIMEKEEPER

#include <assert.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <queue>

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
    extern std::string project_name;            // Used to group different programs together so that they can be compared.
    extern std::string program_name;            // The name given to identify this program.
    extern std::queue<TKEvent*> event_order;    // The order that all events get issued in. Added to CSV file at the end.
    extern int record_start;                    // The integer time that the program started. Used for name of CSV file.
    extern bool registered;                     // Used to make sure that a name has been given and timing has started.
    extern std::chrono::time_point<std::chrono::high_resolution_clock> ns_start_time; // Used to calculate how long after the start each event occurs

    void addEvent(EventType, std::string);
    void sign_up(std::string, std::string);
    void complete();

};

class Tracker
{
    std::string tracker_name;       // Name that will be used to refer to this tracker
    bool recorded;                  // Whether or not this tracker has already been recorded. Don't want the same one recorded multiple times.
    bool covers_timekeeper_scope;   // If the whole program is being timed, it is not necessary to sign up the timekeeper and register a tracker

    public:
    Tracker(std::string, std::string, std::string);
    Tracker(std::string);
    ~Tracker();
    void stop();
};


#endif