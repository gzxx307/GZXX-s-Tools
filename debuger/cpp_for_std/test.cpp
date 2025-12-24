#include <iostream>
#include "Debug.h"

Debug debug;

int main()
{
    debug.setIsEnabled(true);
    debug.setWithTimeStamp(true);

    debug.log(DebugRank::INFO, "This is an info message.");
    debug.log(DebugRank::WARNING, "This is a warning message.");
    debug.log(DebugRank::ERROR, "This is an error message.");
    debug.log(DebugRank::SUCCESS, "This is a success message.", "Initialization");
    debug.log(DebugRank::INFO, "This is another info message with a tag.", "Initialization");
    debug.log(DebugRank::INFO, std::to_string(debug.getTagDebugTimes("Initialization")));

    debug.logSpendTime([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }, "Simulated Task");
    debug.log(DebugRank::SUCCESS, "Task completed successfully.");

    debug.changeLevel(1);
    debug.log(DebugRank::INFO, "This is a debug message at level 1.");

    return 0;
}