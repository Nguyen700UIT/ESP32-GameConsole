#include "console/fps_counter.h"

namespace console {

void FpsCounter::reset(uint32_t nowMs)
{
    lastFrameAtMs_ = nowMs;
    frameCount_ = 0;
    fpsTotal_ = 0;
    stats_ = FpsStats{};
    hasLastFrame_ = false;
    hasStats_ = false;
}

void FpsCounter::recordFrame(uint32_t nowMs)
{
    if (!hasLastFrame_)
    {
        lastFrameAtMs_ = nowMs;
        hasLastFrame_ = true;
        return;
    }

    uint32_t deltaMs = nowMs - lastFrameAtMs_;
    lastFrameAtMs_ = nowMs;

    if (deltaMs == 0)
    {
        return;
    }

    uint16_t fps = static_cast<uint16_t>(1000U / deltaMs);
    stats_.current = fps;

    if (!hasStats_)
    {
        stats_.minimum = fps;
        stats_.maximum = fps;
        fpsTotal_ = fps;
        frameCount_ = 1;
        stats_.average = fps;
        hasStats_ = true;
        return;
    }

    if (fps < stats_.minimum)
    {
        stats_.minimum = fps;
    }

    if (fps > stats_.maximum)
    {
        stats_.maximum = fps;
    }

    fpsTotal_ += fps;
    frameCount_++;
    stats_.average = static_cast<uint16_t>(fpsTotal_ / frameCount_);
}

bool FpsCounter::hasStats() const
{
    return hasStats_;
}

FpsStats FpsCounter::stats() const
{
    return stats_;
}

}
