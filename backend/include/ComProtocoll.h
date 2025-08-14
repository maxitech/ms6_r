#ifndef COMPROTOCOLL_H
#define COMPROTOCOLL_H
#include <array>
#include <cstdint>

namespace CommunicationProtocoll
{
    // /**
    //  * @brief Maximum size of the serial data buffer.
    //  */
    // static constexpr size_t MAX_PACKAGE_SIZE = 256;
    static constexpr int NUM_CMDS = 6;

    static constexpr std::array<uint8_t, 3> START_BYTES = {0xAA, 0xAA, 0xAA};
    static constexpr std::array<uint8_t, 2> END_BYTES   = {0x0D, 0x0A}; // CR,LF
    static constexpr uint8_t                NOP         = 0x00;         // no operation

    static constexpr uint8_t CMD_JOG         = 0x01;
    static constexpr uint8_t CMD_MOVE_TO_POS = 0x02;
    static constexpr uint8_t CMD_LOAD        = 0x03;
    static constexpr uint8_t CMD_START       = 0x04;
    static constexpr uint8_t CMD_STOP        = 0x05;
    static constexpr uint8_t CMD_IDLE        = 0x07;

    static constexpr uint8_t PRG_PING          = 0x06;
    static constexpr uint8_t PRG_TEST_SWITCHES = 0x08;
    static constexpr uint8_t PRG_HOME          = 0x09;
    static constexpr uint8_t PRG_MAIN          = 0x10;
    static constexpr uint8_t PRG_PONG          = 0x11;

    static constexpr std::array<uint8_t, NUM_CMDS> CMD_IDS {CMD_JOG, CMD_MOVE_TO_POS, CMD_LOAD, CMD_START, CMD_STOP, CMD_IDLE};
}
#endif // COMPROTOCOLL_H