#include "CommandDispatcher.h"
#include "ComProtocol.h"
#include "DebugLog.h"
#include "Setup.h"
#include "Utils.h"

#define LOG(level, msg) DebugLog::log(level, msg)

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
    }
}

void CommandDispatcher::dispatch(const String& cmd, const std::vector<String>& args)
{
    if (cmd == "" || args.empty())
    {
        Utils::createAndSendPacket(CMD_SETUP, STATUS_ERROR, ERR_INVALID_PACKET);
        LOG(LOG_ERROR, "No command or no arguments");
        return;
    }

    std::vector<String> cmds       = {"SETUP"};
    bool                isValidCmd = std::find(cmds.begin(), cmds.end(), cmd) != cmds.end();
    if (isValidCmd)
    {
        if (cmd == "SETUP")
        {
            Setup::getInstance().update(args[0]);
        }
    }
    else
    {
        Utils::createAndSendPacket(CMD_SETUP, STATUS_ERROR, ERR_INVALID_CMD);
        String debugMsg = "Unknown command or command invalide! Correct format <$cmd,[arg, arg, ...]>" + cmd;
        LOG(LOG_ERROR, debugMsg);
    }
}