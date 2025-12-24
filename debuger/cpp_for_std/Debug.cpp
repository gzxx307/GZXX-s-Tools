#include "Debug.h"

Debug::Debug() : isEnabled(false), withTimeStamp(false) {}
Debug::Debug(const bool& enable) : isEnabled(enable), withTimeStamp(false)
{
    log(DebugRank::DEBUG, std::string("Debugging has been ") + (enable ? "enabled." : "disabled."));
}
Debug::Debug(const bool& enable, const bool& withTimeStamp) : isEnabled(enable), withTimeStamp(withTimeStamp)
{
    log(DebugRank::DEBUG, std::string("Debugging has been ") + (enable ? "enabled." : "disabled."));
    log(DebugRank::DEBUG, std::string("Timestamp has been ") + (withTimeStamp ? "enabled." : "disabled."));
}

// Private Helper

std::string Debug::SwitchColorCode(const DebugColor& color)
{
    switch (color)
    {
    case DebugColor::WHITE:
        return "\033[0m";
    case DebugColor::BLACK:
        return "\033[30m";
    case DebugColor::RED:
        return "\033[31m";
    case DebugColor::GREEN:
        return "\033[32m";
    case DebugColor::YELLOW:
        return "\033[33m";
    case DebugColor::BLUE:
        return "\033[34m";
    case DebugColor::MAGENTA:
        return "\033[35m";
    case DebugColor::CYAN:
        return "\033[36m";
    default:
        return "\033[0m";
    }
}
std::string Debug::GetColorCode(const DebugRank& rank)
{
    switch (rank)
    {
    case DebugRank::SUCCESS:
        return SwitchColorCode(successColor);
    case DebugRank::INFO:
        return SwitchColorCode(infoColor);
    case DebugRank::WARNING:
        return SwitchColorCode(warningColor);
    case DebugRank::ERROR:
        return SwitchColorCode(errorColor);
    case DebugRank::DEBUG:
        return SwitchColorCode(statusChangedColor);
    default:
        return SwitchColorCode(DebugColor::WHITE);
    }
}

std::string Debug::GetCurrentTimeString()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &now_time);
    buffer[24] = '\0';
    std::string str = std::string(buffer);

    str = "<" + str + ">";

    str += std::string(5, ' ');

    return str;
}

std::string Debug::CalculateSpaces(const std::string& str)
{
    int spacesNeeded = lengthOfSpace - static_cast<int>(str.length());
    if (spacesNeeded < 0) spacesNeeded = 0;
    return std::string(spacesNeeded, ' ');
}
std::string Debug::GetLineWithMessage(const std::string& str)
{
    std::string _str = "";
    int n = str.length();
    std::string str_line = std::string((lengthOfLine - n) / 2, '-');
    _str += str_line + " " + str + " " + str_line;
    if (_str.length() < lengthOfLine)
    {
        _str += "-";
    }
    
    return _str;
}

std::string Debug::GetLevelString(int level)
{
    std::string str = "";

    for (int i = 0; i < level; ++i)
    {
        str += " |";
    }
    if (level != 0) str += " ";

    return str;
}

void Debug::HandleTagDebugTimes(const std::string& tag)
{
    if (tag.empty()) return;

    if (tagTimes.find(tag) == tagTimes.end())
    {
        tagTimes[tag] = 1;
    }
    else
    {
        tagTimes[tag] += 1;
    }
}

// Get Debug Status / Private Members

bool Debug::getIsEnabled() const
{
    return isEnabled;
}
bool Debug::setIsEnabled(const bool& enable)
{
    isEnabled = enable;
    log(DebugRank::DEBUG, "Debugging has been " + std::string(enable ? "enabled." : "disabled."));
    return isEnabled;
}
bool Debug::getWithTimeStamp() const
{
    return withTimeStamp;
}
bool Debug::setWithTimeStamp(const bool& enable)
{
    withTimeStamp = enable;
    log(DebugRank::DEBUG, "Timestamp has been " + std::string(enable ? "enabled." : "disabled."));
    return withTimeStamp;
}
int Debug::getDebugTimes(const DebugRank& rank)
{
    switch (rank)
    {
    case DebugRank::SUCCESS:
        return successTimes;
    case DebugRank::INFO:
        return infoTimes;
    case DebugRank::WARNING:
        return warningTimes;
    case DebugRank::ERROR:
        return errorTimes;
    default:
        return 0;
    }
}

int Debug::getTotalDebugTimes()
{
    totalTimes = successTimes + infoTimes + warningTimes + errorTimes;
    return totalTimes;
}

void Debug::resetDebugTimes(const DebugRank& rank)
{
    switch (rank)
    {
    case DebugRank::SUCCESS:
        successTimes = 0;
        break;
    case DebugRank::INFO:
        infoTimes = 0;
        break;
    case DebugRank::WARNING:
        warningTimes = 0;
        break;
    case DebugRank::ERROR:
        errorTimes = 0;
        break;
    default:
        break;
    }
}
void Debug::resetTotalDebugTimes()
{
    successTimes = 0;
    infoTimes = 0;
    warningTimes = 0;
    errorTimes = 0;
    totalTimes = 0;
}
void Debug::resetTagDebugTimes(const std::string& tag)
{
    if (tag.empty()) return;
    tagTimes.erase(tag);
}
void Debug::resetAllTagDebugTimes()
{
    tagTimes.clear();
}

int Debug::getCurrentLevel() const
{
    return currentLevel;
}

bool Debug::changeLevel(int level)
{
    if (level == 0) return true;
    level = (level > 0) ? 1 : -1;
    if (currentLevel + level < 0)
    {
        log(DebugRank::WARNING, "Current level cannot be less than 0.");
        return false;
    }
    currentLevel += level;
    return true;
}

int Debug::getLengthOfSpace() const
{
    return lengthOfSpace;
}
bool Debug::setLengthOfSpace(int length)
{
    // Length of space cannot be negative
    if (length < 0)
    {
        log(DebugRank::WARNING, "Length of space cannot be negative.");
        return false;
    }
    lengthOfSpace = length;
    return true;
}

bool Debug::setColor(const DebugRank& rank, const DebugColor& color)
{
    if (!isEnabled)
    {
        log(DebugRank::WARNING, "Debugging is disabled so that color changing failed.");
        return false;
    }

    switch (rank)
    {
    case DebugRank::INFO:
        infoColor = color;
        break;
    case DebugRank::WARNING:
        warningColor = color;
        break;
    case DebugRank::ERROR:
        errorColor = color;
        break;
    default:
        return false;
    }
    return true;
}

bool Debug::log(const DebugRank& rank, const std::string& message, const std::string& tag)
{
    if (!isEnabled) return false;

    std::string str_rank = "";
    switch (rank)
    {
    case DebugRank::SUCCESS:
        str_rank += "[SUCCESS]";
        break;
    case DebugRank::INFO:
        str_rank += "[INFO]";
        break;
    case DebugRank::WARNING:
        str_rank += "[WARNING]";
        break;
    case DebugRank::ERROR:
        str_rank += "[ERROR]";
        break;
    case DebugRank::DEBUG:
        str_rank += "[DEBUG]";
        break;
    default:
        str_rank += "[UNKNOWN]";
        break;
    }
    std::string str_color = GetColorCode(rank);
    std::string str_level = GetLevelString(currentLevel);
    std::string str_level_spaces = CalculateSpaces(str_level + str_rank);
    std::string str_time = "";
    if (withTimeStamp) str_time += GetCurrentTimeString();
    std::string str_tag = "";
    if (!tag.empty()) str_tag += "(" + tag + ")";

    // "\RankColor" + " | [RANK] " + spaces + "<TIME>  (tag)message + "\033[0m"
    std::string str = str_color + str_level + str_rank + str_level_spaces + str_time + str_tag + message + SwitchColorCode(DebugColor::WHITE);

    // Update TagTimes
    HandleTagDebugTimes(tag);

    std::cout << str << std::endl;

    return true;
}

void Debug::line(const DebugColor& color, const std::string& message)
{
    if (!isEnabled) return;
    
    std::string str_color = SwitchColorCode(color);
    std::string str_line = GetLineWithMessage(message);
    std::string str = str_color + str_line + SwitchColorCode(DebugColor::WHITE);
    std::cout << str << std::endl;
}

void Debug::logSpendTime(const std::function<void()>& func, const std::string& taskName)
{
    if (!isEnabled) return;

    auto start = std::chrono::high_resolution_clock::now();

    func();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    log(DebugRank::INFO, taskName + " took " + std::to_string(duration.count()) + " ms.");
}

int Debug::getTagDebugTimes(const std::string& tag)
{
    if (tag.empty()) return 0;

    if (tagTimes.find(tag) != tagTimes.end())
    {
        return tagTimes[tag];
    }
    else
    {
        return 0;
    }
}