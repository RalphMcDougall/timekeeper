#include "timekeeper.h"

namespace TimeKeeper
{
    std::string project_name;
    std::string program_name;
    std::queue<TKEvent*> event_order;
    int record_start;
    bool registered = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> ns_start_time;


    void addEvent(const EventType event_type, const std::string event_issuer, const tkll flag)
    {
        auto add_time = std::chrono::high_resolution_clock::now();
        tkll exec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(add_time - ns_start_time).count();

        TKEvent* ne = new TKEvent();
        ne->time_stamp = exec_time;
        ne->event_type = event_type;
        ne->event_issuer = event_issuer;
        ne->flag = flag;

        event_order.push(ne);
    }

    void sign_up(const std::string _project_name, const std::string _program_name)
    {
        if (registered) std::cerr << "TimeKeeper has already been registered" << std::endl;
        assert(!registered); // Registration should only occur once
        project_name = _project_name;
        program_name = _program_name;

        record_start = time(0);
        ns_start_time = std::chrono::high_resolution_clock::now();

        registered = true;
    }

    void complete()
    {
        // Create the CSV File for the report
        std::string run_file = "timekeeper/" + program_name + "_" + std::to_string(record_start) + ".csv";
        std::ofstream csv_file(run_file, std::ofstream::out);
        csv_file << "project_name," << project_name << "\n";
        csv_file << "program_name," << program_name << "\n";
        csv_file << "time,type,issuer,flag\n";

        while (event_order.size() > 0)
        {
            TKEvent* top = event_order.front();
            csv_file << top->time_stamp << "," << top->event_type << "," << top->event_issuer << "," << top->flag << "\n";
            free( top ); // Free the event stored
            event_order.pop();
        }
        csv_file.close();
    }

    Tracker::Tracker(const std::string _tracker_name, const bool _project_events, const tkll _flag)
    {
        if (!registered) std::cerr << "TimeKeeper must be registered before trackers are initialised" << std::endl;
        assert(registered); // The TimeKeeper must be registered in order for execution to continue
        
        covers_timekeeper_scope = false;
        tracker_name = _tracker_name;
        recorded = false;
        project_events = _project_events;
        flag = _flag;
        addEvent(project_events ? PROJECT_TRACKING_START : TRACKING_START, tracker_name, flag);
    }

    Tracker::Tracker(const std::string _tracker_name, const std::string _project_name, const std::string _program_name, const bool _project_events, const tkll _flag)
    {
        sign_up(_project_name, _program_name);
        
        covers_timekeeper_scope = true;
        tracker_name = _tracker_name;
        recorded = false;
        project_events = _project_events;
        flag = _flag;
        addEvent(project_events ? PROJECT_TRACKING_START : TRACKING_START, tracker_name, flag);
    }

    Tracker::~Tracker()
    {
        stop();

        if (covers_timekeeper_scope) complete();
    }
    void Tracker::stop()
    {
        if (recorded) return;
        addEvent(project_events ? PROJECT_TRACKING_END : TRACKING_END, tracker_name, flag);
        recorded = true;
    }
}