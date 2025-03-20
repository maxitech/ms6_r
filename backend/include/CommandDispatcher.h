#ifndef COMMANDDISPTATCHER_H
#define COMMANDDISPTATCHER_H

#include <Arduino.h>
#include "ProgramLoader.h"


class CommandDispatcher {
public: 
    explicit CommandDispatcher(ProgramLoader& programLoader);
    void dispatch(const std::vector<String>& cmdParts);
    
private: 
    ProgramLoader& _programLoader;
};


#endif //COMMANDDISPATCHER_H