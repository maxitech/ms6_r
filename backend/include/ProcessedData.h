#ifndef PROCESSEDDATA_H
#define PROCESSEDDATA_H

#include <cstdint>
#include <optional>
#include <vector>

struct ProcessedData
{
    uint8_t                             cmdId;
    std::optional<std::vector<int32_t>> jogSpeeds;
    std::optional<bool>                 is_requestTelemetry;
};

#endif // PROCESSEDDATA_H