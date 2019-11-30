#include "timekeeper.h"

// Set the initial function depth
int Tracker::function_depth = 0;

std::unordered_map<std::string, tkll> RecordBook::function_records;
std::set<std::string> RecordBook::function_name_set;


Tracker::Tracker(std::string _function_name)
{
    // Constructor that starts timing of function
    function_name = _function_name;
    
    start_tp = std::chrono::high_resolution_clock::now();
    RecordBook::function_name_set.insert(function_name);

    function_depth++;
}

Tracker::~Tracker()
{
    // Destructor that stops timing of function

    auto finish_tp = std::chrono::high_resolution_clock::now();
    tkll exec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tp - start_tp).count();

    RecordBook::function_records[function_name] += exec_time;

    function_depth--;

    if (function_depth == 0)
    {
        // All functions exited and program ending

        for (auto iter = RecordBook::function_name_set.begin(); iter != RecordBook::function_name_set.end(); ++iter)
        {
            // Output function executing times
            std::string fn = *iter;
            std::cout << fn << " : \t"<< RecordBook::function_records[fn] << "ns\n";
        }
    }
}