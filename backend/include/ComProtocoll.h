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

    static constexpr std::array<uint8_t, 3> START_BYTES = {0xAA, 0xAA, 0xAA};
    static constexpr std::array<uint8_t, 2> END_BYTES   = {0x0D, 0x0A}; // CR,LF
    static constexpr uint8_t                NOP         = 0x00;         // no operation

    static constexpr uint8_t CMD_JOG         = 0x01;
    static constexpr uint8_t CMD_MOVE_TO_POS = 0x02;
}
#endif // COMPROTOCOLL_H