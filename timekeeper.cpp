#include "timekeeper.h"

namespace TimeKeeper
{
    std::string program_name = "";
    std::queue<TKEvent*> event_order;
    int record_start;
    bool started = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> ns_start_time;
}

void TimeKeeper::addEvent(EventType event_type, std::string event_issuer)
{
    auto add_time = std::chrono::high_resolution_clock::now();
    tkll exec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(add_time - ns_start_time).count();

    TKEvent* ne = new TKEvent();
    ne->time_stamp = exec_time;
    ne->event_type = event_type;
    ne->event_issuer = event_issuer;

    event_order.push(ne);
}

void TimeKeeper::setName(std::string name)
{
    program_name = name;
}


Tracker::Tracker(std::string _tracker_name)
{
    is_root = false;
    if (!TimeKeeper::started)
    {
        // First time a Tracker is being started
        TimeKeeper::record_start = time(0);
        TimeKeeper::started = true;
        TimeKeeper::ns_start_time = std::chrono::high_resolution_clock::now();
        is_root = true;
    }
    tracker_name = _tracker_name;
    recorded = false;
    TimeKeeper::addEvent(TRACKING_START, tracker_name);
}

Tracker::~Tracker()
{
    if(!recorded) stopTracking();
}
void Tracker::stop()
{
    if(!recorded) stopTracking();
}

void Tracker::stopTracking()
{
    TimeKeeper::addEvent(TRACKING_END, tracker_name);
    if (is_root) recordExecution();
    recorded = true;
}

void Tracker::recordExecution()
{
    // Create the CSV File for the report
    std::string run_file = std::to_string(TimeKeeper::record_start) + ".csv";
    if (TimeKeeper::program_name != "")
    {
        run_file = TimeKeeper::program_name + "_" + run_file;
    }
    std::ofstream csv_file(run_file, std::ofstream::out);
    csv_file << "time,type,issuer\n";

    while (TimeKeeper::event_order.size() > 0)
    {
        TKEvent* top = TimeKeeper::event_order.front();
        csv_file << top->time_stamp << "," << top->event_type << "," << top->event_issuer << "\n";
        free( top ); // Free the event stored
        TimeKeeper::event_order.pop();
    }
    csv_file.close();
}