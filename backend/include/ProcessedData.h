#ifndef PROCESSEDDATA_H
#define PROCESSEDDATA_H

#include <array>
#include <cstdint>
#include <optional>
#include <tuple>
#include <vector>

struct ProcessedData
{
    uint8_t                                                                                       cmdId;
    std::optional<uint8_t>                                                                        program;
    std::optional<std::vector<int32_t>>                                                           jogSpeeds;
    std::optional<std::vector<int32_t>>                                                           targetPositions;
    std::optional<std::tuple<uint32_t, uint32_t>>                                                 trajStartData; // (numPoints, totalTimeMs)
    std::optional<std::tuple<uint8_t, std::vector<std::tuple<uint32_t, std::array<int32_t, 6>>>>> trajChunkData;
    std::optional<bool>                                                                           trajExecute;
    std::optional<bool>                                                                           trajCancel;
    // std::optional<bool>                 is_requestTelemetry;
};

#endif // PROCESSEDDATA_H