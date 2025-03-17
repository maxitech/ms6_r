#ifndef COMMANDDISPTATCHER_H
#define COMMANDDISPTATCHER_H

#include <Arduino.h>


class CommandDispatcher {
public: 
    CommandDispatcher();
    void dispatch(const std::vector<String> cmdParts);


private: 
    // classes to forward the cmd to
};


#endif //COMMANDDISPATCHER_H