#include "timekeeper.h"

namespace TimeKeeper
{
    std::string project_name;
    std::string program_name;
    std::queue<TKEvent*> event_order;
    int record_start;
    bool registered = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> ns_start_time;
}

void TimeKeeper::addEvent(const EventType event_type, const std::string event_issuer)
{
    auto add_time = std::chrono::high_resolution_clock::now();
    tkll exec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(add_time - ns_start_time).count();

    TKEvent* ne = new TKEvent();
    ne->time_stamp = exec_time;
    ne->event_type = event_type;
    ne->event_issuer = event_issuer;

    event_order.push(ne);
}

void TimeKeeper::sign_up(const std::string _project_name, const std::string _program_name)
{
    if (registered) std::cerr << "TimeKeeper has already been registered" << std::endl;
    assert(!registered); // Registration should only occur once
    project_name = _project_name;
    program_name = _program_name;

    record_start = time(0);
    ns_start_time = std::chrono::high_resolution_clock::now();

    registered = true;
}

void TimeKeeper::complete()
{
    // Create the CSV File for the report
    std::string run_file = "timekeeper/" + TimeKeeper::program_name + "_" + std::to_string(TimeKeeper::record_start) + ".csv";
    std::ofstream csv_file(run_file, std::ofstream::out);
    csv_file << "project_name," << project_name << "\n";
    csv_file << "program_name," << program_name << "\n";
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

Tracker::Tracker(const std::string _tracker_name, const bool _project_events)
{
    if (!TimeKeeper::registered) std::cerr << "TimeKeeper must be registered before trackers are initialised" << std::endl;
    assert(TimeKeeper::registered); // The TimeKeeper must be registered in order for execution to continue
    
    covers_timekeeper_scope = false;
    tracker_name = _tracker_name;
    recorded = false;
    project_events = _project_events;
    TimeKeeper::addEvent(project_events ? PROJECT_TRACKING_START : TRACKING_START, tracker_name);
}

Tracker::Tracker(const std::string _tracker_name, const std::string _project_name, const std::string _program_name, const bool _project_events)
{
    TimeKeeper::sign_up(_project_name, _program_name);
    
    covers_timekeeper_scope = true;
    tracker_name = _tracker_name;
    recorded = false;
    project_events = _project_events;
    TimeKeeper::addEvent(project_events ? PROJECT_TRACKING_START : TRACKING_START, tracker_name);
}

Tracker::~Tracker()
{
    stop();

    if (covers_timekeeper_scope) TimeKeeper::complete();
}
void Tracker::stop()
{
    if (recorded) return;
    TimeKeeper::addEvent(project_events ? PROJECT_TRACKING_END : TRACKING_END, tracker_name);
    recorded = true;
}