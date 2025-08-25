#ifndef COMPROTOCOLL_H
#define COMPROTOCOLL_H
#include <array>
#include <cstdint>

namespace CommunicationProtocoll
{
    // *******************Request******************
    static constexpr int NUM_CMDS = 6;

    static constexpr std::array<uint8_t, 3> START_BYTES = {0xAA, 0xAA, 0xAA};
    static constexpr std::array<uint8_t, 2> END_BYTES   = {0x0D, 0x0A}; // CR,LF

    // Global Control 0x01 - 0x0F(Start, Stop, Idle, Pause, Resume, etc.)
    static constexpr uint8_t NOP       = 0x00; // no operation
    static constexpr uint8_t CMD_START = 0x01;
    static constexpr uint8_t CMD_STOP  = 0x02;
    static constexpr uint8_t CMD_IDLE  = 0x03;
    static constexpr uint8_t CMD_SETUP = 0x04; // only used for errors while processing setup data

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

    // Reserved 0xA0 - 0xDF & 0xE2 - 0xFF

    // *******************Response******************
    // Status Codes
    static constexpr uint8_t STATUS_OK    = 0xE0;
    static constexpr uint8_t STATUS_ERROR = 0xE1;

    // Detail Codes when STATUS_OK
    // Data range: 0x00–0x1F
    static constexpr uint8_t DATA_NONE  = 0x00;
    static constexpr uint8_t DATA_STEPS = 0x01;

    // Info range: 0x20–0x3F
    static constexpr uint8_t INFO_LOADED_PROGRAM      = 0x20;
    static constexpr uint8_t INFO_RELOADED_PROGRAM    = 0x21;
    static constexpr uint8_t INFO_ALREADY_RUNNING     = 0x22;
    static constexpr uint8_t INFO_HOMING_ALREADY_DONE = 0x23;
    // static constexpr uint8_t INFO_FINISHED            = 0x23;

    // Warn range: 0x40–0x5F
    static constexpr uint8_t WARN_NOP_IGNORED       = 0x40;
    static constexpr uint8_t WARN_NO_PROGRAM_LOADED = 0x41;
    static constexpr uint8_t WARN_LIMIT_HIT         = 0x42;
    static constexpr uint8_t WARN_ARM_NOT_HOMED     = 0x43;

    // Detail Codes when STATUS_ERROR
    // Error range: 0x60–0x7F
    static constexpr uint8_t ERR_UNKNOWN        = 0x60;
    static constexpr uint8_t ERR_INVALID_PACKET = 0x61;
    static constexpr uint8_t ERR_TIMEOUT        = 0x62;
    static constexpr uint8_t ERR_CHECKSUM       = 0x63;

    static constexpr uint8_t ERR_NO_PROCESSOR           = 0x64;
    static constexpr uint8_t ERR_BUFFER_OVERFLOW        = 0x65;
    static constexpr uint8_t ERR_INVALID_CMD            = 0x66;
    static constexpr uint8_t ERR_INVALID_TELEMETRY_FLAG = 0x67;
    static constexpr uint8_t ERR_UNKNOWN_PROGRAM        = 0x68;

    static constexpr uint8_t ERR_INDEX_OOB   = 0x69;
    static constexpr uint8_t ERR_INVALID_DIR = 0x6A;
    static constexpr uint8_t ERR_JSON_PARSE  = 0x6B;
    static constexpr uint8_t ERR_ALLOC_FAIL  = 0x6C;

    static constexpr std::array<uint8_t, NUM_CMDS>
        CMD_IDS {CMD_JOG, CMD_MOVE_TO_POS, CMD_LOAD, CMD_START, CMD_STOP, CMD_IDLE};
}
#endif // COMPROTOCOLL_H