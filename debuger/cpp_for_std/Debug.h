#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <unordered_map>

enum class DebugRank
{
    DEBUG,
    SUCCESS,
    INFO,
    WARNING,
    ERROR
};
enum class DebugColor
{
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

class Debug
{
private:
    bool isEnabled = false;
    bool withTimeStamp = false;

    int num;
    int lengthOfSpace = 15;
    int currentLevel = 0;
    int lengthOfLine = 80;

    // Default Debug Colors
    DebugColor successColor = DebugColor::GREEN;
    DebugColor infoColor = DebugColor::WHITE;
    DebugColor warningColor = DebugColor::YELLOW;
    DebugColor errorColor = DebugColor::RED;
    // Debug Status Colors Only for status changed info
    DebugColor statusChangedColor = DebugColor::CYAN;
    // Debug times
    int successTimes = 0;
    int infoTimes = 0;
    int warningTimes = 0;
    int errorTimes = 0;
    int totalTimes = 0;

    // Provide tag-based time tracking for personal use
    std::unordered_map<std::string, int> tagTimes;
    
    std::string SwitchColorCode(const DebugColor& color);
    std::string GetColorCode(const DebugRank& rank);

    std::string GetCurrentTimeString();
    
    // Calculate spaces for [RANK] part
    std::string CalculateSpaces(const std::string& str);
    // Calculate '-' for line part
    std::string GetLineWithMessage(const std::string& str);

    // Calculate level string
    std::string GetLevelString(int level);

    // Update TagTimes
    void HandleTagDebugTimes(const std::string& tag);

public:
    Debug();
    Debug(const bool& enable);
    // You need to enable debug before using timestamp
    Debug(const bool& enable, const bool& withTimeStamp);
    
    // Getters and Setters

    // Get whether debug is enabled
    bool getIsEnabled() const;
    // Enable or disable debug, return current status
    bool setIsEnabled(const bool& enable);
    // Get whether timestamp is enabled
    bool getWithTimeStamp() const;
    // Enable or disable timestamp, return current status
    bool setWithTimeStamp(const bool& enable);

    // Use this function to get debug times for a specific rank
    int getDebugTimes(const DebugRank& rank);
    // Use this function to get total debug times
    // To prevent update TotalDebugTimes every time logging, we just calculate it when you call this function.
    // If you get it privately, it may be inaccurate.
    int getTotalDebugTimes();
    // Reset debug times for a specific rank
    // Note: If you reset a specific rank debug times, total debug times will not be changed until you call getTotalDebugTimes().
    void resetDebugTimes(const DebugRank& rank);
    // Reset total debug times
    // If you call this function, all specific rank debug times will also be reset.
    void resetTotalDebugTimes();
    // Get debug times for a specific tag
    void resetTagDebugTimes(const std::string& tag);
    // Reset all tag debug times
    void resetAllTagDebugTimes();

    // Get current debug level
    int getCurrentLevel() const;
    // Handle current level. You can only plus or minus 1 each time.
    // For example, if you input 2, it will only plus 1 to current level.
    bool changeLevel(int level);

    // LengthOfSpace is used to calculate spaces between [RANK] and message for better alignment.
    // You can adjust it according to your console width, but we suggest to keep it longer than any category of rank and your debug level.
    int getLengthOfSpace() const;
    // LengthOfSpace is used to calculate spaces between [RANK] and message for better alignment.
    // You can adjust it according to your console width, but we suggest to keep it longer than any category of rank and your debug level.
    bool setLengthOfSpace(int length);

    // Basic log function
    /*
    * @param rank: DebugRank enum value indicating the severity level of the message.
    * @param message: The message string to be logged.
    * @param tag: An optional string tag to categorize your log message.
    * @return bool: Returns true if the message was logged successfully, false otherwise.
    */
    bool log(const DebugRank& rank, const std::string& message, const std::string& tag = "");
    
    // Print a line with color and message
    void line(const DebugColor& color = DebugColor::WHITE, const std::string& message = "");

    // You can customize the color for each rank.
    bool setColor(const DebugRank& rank, const DebugColor& color);

    // Log time spent in a function
    // This function will execute the provided function and log the time taken to complete it.
    // So it can just take a function with no parameters and no return value.
    /*
    * @param func: A std::function<void()> representing the function to be executed and timed.
    * @param taskName: An optional string representing the name of the task being timed.
    */
    void logSpendTime(const std::function<void()>& func, const std::string& taskName);

    // Get the debug times for a specific tag if you used tags in your log messages.
    int getTagDebugTimes(const std::string& tag);


};

