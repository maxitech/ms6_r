#ifndef COMPROTOCOLL_H
#define COMPROTOCOLL_H
#include <array>
#include <cstdint>

namespace CommunicationProtocoll
{
    static constexpr int NUM_CMDS = 6;

    static constexpr std::array<uint8_t, 3> START_BYTES = {0xAA, 0xAA, 0xAA};
    static constexpr std::array<uint8_t, 2> END_BYTES   = {0x0D, 0x0A}; // CR,LF

    // Global Control 0x01 - 0x0F(Start, Stop, Idle, Pause, Resume, etc.)
    static constexpr uint8_t NOP       = 0x00; // no operation
    static constexpr uint8_t CMD_START = 0x01;
    static constexpr uint8_t CMD_STOP  = 0x02;
    static constexpr uint8_t CMD_IDLE  = 0x03;
    // static constexpr uint8_t CMD_PAUSE  = 0x04;
    // static constexpr uint8_t CMD_RESUME = 0x05;

    // Movement 0x10 - 0x4F(Jog, MTP, MR, etc.)
    static constexpr uint8_t CMD_JOG         = 0x10;
    static constexpr uint8_t CMD_MOVE_TO_POS = 0x11;

    // Program Control 0x50 - 0x7F(Load, etc.)
    static constexpr uint8_t CMD_LOAD = 0x50;
    // CMD_SELECT_PRG = 0x51
    // CMD_RUN_PRG    = 0x52

    // Program Selection(CMD_LOAD Modifiers) * *0x80 - 0x8F(Information for CMD_LOAD - which predefined program should be loaded)
    static constexpr uint8_t PRG_PING          = 0x80;
    static constexpr uint8_t PRG_PONG          = 0x81;
    static constexpr uint8_t PRG_TEST_SWITCHES = 0x82;
    static constexpr uint8_t PRG_HOME          = 0x83;
    static constexpr uint8_t PRG_MAIN          = 0x84;

    // Fixed Payload System / Debug 0x90 - 0x9F(Diagnostics, DataRequest, etc.)
    static constexpr uint8_t SYS_DIAGNOSTICS  = 0x90;
    static constexpr uint8_t SYS_DATA_REQUEST = 0x91;

    // Reserve 0xA0 - 0xFF

    static constexpr std::array<uint8_t, NUM_CMDS>
        CMD_IDS {CMD_JOG, CMD_MOVE_TO_POS, CMD_LOAD, CMD_START, CMD_STOP, CMD_IDLE};
}
#endif // COMPROTOCOLL_H