#pragma once

#include <cstdint>

namespace console {

struct FpsStats {
    uint16_t current = 0;
    uint16_t minimum = 0;
    uint16_t maximum = 0;
    uint16_t average = 0;
};

class FpsCounter {
public:
    void reset(uint32_t nowMs = 0);
    void recordFrame(uint32_t nowMs);
    bool hasStats() const;
    FpsStats stats() const;

private:
    uint32_t lastFrameAtMs_ = 0;
    uint32_t frameCount_ = 0;
    uint32_t fpsTotal_ = 0;
    FpsStats stats_;
    bool hasLastFrame_ = false;
    bool hasStats_ = false;
};

}