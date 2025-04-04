#ifndef COMMANDDISPTATCHER_H
#define COMMANDDISPTATCHER_H

#include "ProgramLoader.h"
#include <Arduino.h>

class CommandDispatcher
{
public:
    explicit CommandDispatcher(ProgramLoader& programLoader);
    void dispatch(const std::vector<String>& cmdParts);

private:
    ProgramLoader& _programLoader;
};

#endif // COMMANDDISPATCHER_H