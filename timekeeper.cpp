#include "timekeeper.h"

void RecordBook::addEvent(EventType event_type, std::string event_issuer)
{
    auto add_time = std::chrono::high_resolution_clock::now();
    tkll exec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(add_time - RecordBook::ns_start_time).count();

    TKEvent* ne = new TKEvent();
    ne->time_stamp = exec_time;
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

    RecordBook::addEvent(TRACKING_START, function_name);
    RecordBook::tracking_depth++;
}

Tracker::~Tracker() {stopTracking();}
void Tracker::stop() {stopTracking();}

void Tracker::stopTracking()
{

    RecordBook::addEvent(TRACKING_END, function_name);

    RecordBook::tracking_depth--;
    if (RecordBook::tracking_depth == 0) recordExecution();
}

void Tracker::recordExecution()
{
    // Create the CSV File for the report
    std::string run_file = std::to_string(RecordBook::record_start) + ".csv";
    std::ofstream csv_file(run_file, std::ofstream::out);
    csv_file << "time,type,issuer\n";

    for (int i = 0; i < RecordBook::event_order.size(); ++i)
    {
        csv_file << RecordBook::event_order[i]->time_stamp << "," << RecordBook::event_order[i]->event_type << "," << RecordBook::event_order[i]->event_issuer << "\n";
        free( RecordBook::event_order[i] ); // Free the events stored
    }
    csv_file.close();
}