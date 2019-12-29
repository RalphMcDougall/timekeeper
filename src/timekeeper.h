#ifndef _TIMEKEEPER
#define _TIMEKEEPER

#include <assert.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>

typedef long long tkll;

enum EventType 
{
    TRACKING_START = 0, 
    TRACKING_END = 1,
    PROJECT_TRACKING_START = 2,
    PROJECT_TRACKING_END = 3
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

    void addEvent(const EventType, const std::string);  // Add a new event to the queue
    void sign_up(const std::string, const std::string); // Register the name of the project and program and establish a basis for timing
    void complete();    // Write to the CSV file and clear the event queue

};

class Tracker
{
    std::string tracker_name;       // Name that will be used to refer to this tracker
    bool recorded;                  // Whether or not this tracker has already been recorded. Don't want the same one recorded multiple times.
    bool covers_timekeeper_scope;   // If the whole program is being timed, it is not necessary to sign up the timekeeper and register a tracker
    bool project_events;            // Whether or not the events that get issued are of importance to the project tracking

    public:
    Tracker(const std::string, const bool);     // Standard constructor where just the tracker name is passed
    Tracker(const std::string, const std::string, const std::string, const bool);   // Special constructor where the project name and program name are given too so that the TimeKeeper registering can be performed in the same step, simplifying the library usage
    ~Tracker();     // Destructor calls stop() so that trackers do not explicitly need to be stopped when tracking a function
    void stop();    // Creates a TRACKING_END event
};


#endif