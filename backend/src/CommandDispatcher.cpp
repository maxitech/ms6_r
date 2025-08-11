#include "CommandDispatcher.h"
#include "ComProtocoll.h"
#include "Setup.h"

using namespace CommunicationProtocoll;

CommandDispatcher::CommandDispatcher(ProgramLoader& programLoader)
    : _programLoader(programLoader) {};

void CommandDispatcher::dispatch(const ProcessedData& processedData)
{
    if (processedData.cmdId == NOP)
    {
        return;
    }

    bool isValidCmd = std::find(CMD_IDS.begin(), CMD_IDS.end(), processedData.cmdId) != CMD_IDS.end();

    if (isValidCmd)
    {
        _programLoader.handleCommand(processedData);

        // * Take as reference if setup is needed.
        // if (processedData.cmdId == <setup>)
        // {
        //     Setup::getInstance().update();
        // }
        // else
        // {
        //     _programLoader.handleCommand(processedData);
        // }
    }
}