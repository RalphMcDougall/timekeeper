#include "timekeeper.h"

// Set the initial function depth
int RecordBook::function_depth = 0;

std::vector<TKEvent*> RecordBook::event_order;
int RecordBook::record_start;
bool RecordBook::started = false;
std::chrono::time_point<std::chrono::high_resolution_clock> RecordBook::ns_start_time;


void RecordBook::addEvent(tkll event_time, EventType event_type, std::string event_issuer)
{
    TKEvent* ne = new TKEvent();
    ne->time_stamp = event_time;
    ne->event_type = event_type;
    ne->event_issuer = event_issuer;

    RecordBook::event_order.push_back(ne);
}


Tracker::Tracker(std::string _function_name)
{
    if (!RecordBook::started)
    {
        // First time a Tracker is being started
        RecordBook::record_start = time(0);
        RecordBook::started = true;
        RecordBook::ns_start_time = std::chrono::high_resolution_clock::now();
    }

    // Constructor that starts timing of function
    function_name = _function_name;
    tkll exec_time = ( std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - RecordBook::ns_start_time) ).count();

    RecordBook::addEvent(exec_time, FUNCTION_START, function_name);

    RecordBook::function_depth++;
}

Tracker::~Tracker()
{
    // Destructor that indicates that function has ended
    auto finish_tp = std::chrono::high_resolution_clock::now();
    tkll exec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tp - RecordBook::ns_start_time).count();

    RecordBook::addEvent(exec_time, FUNCTION_END, function_name);

    RecordBook::function_depth--;
    if (RecordBook::function_depth == 0)
    {
        // All functions exited and program ending
        // Create the CSV File for the report
        std::string run_file = std::to_string(RecordBook::record_start) + ".csv";
        std::ofstream csv_file(run_file, std::ofstream::out);
        csv_file << "time,type,issuer\n";

        for (int i = 0; i < RecordBook::event_order.size(); ++i)
        {
            csv_file << RecordBook::event_order[i]->time_stamp << "," << RecordBook::event_order[i]->event_type << "," << RecordBook::event_order[i]->event_issuer << "\n";
            free( RecordBook::event_order[i] );
        }
        csv_file.close();
    }
}