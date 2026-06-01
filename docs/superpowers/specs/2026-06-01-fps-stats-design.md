# FPS Stats Design

## Goal

Show live FPS statistics on the TFT screen for every game: current, minimum, maximum, and average FPS.

## Approach

Use one shared FPS counter in `include/console/fps_counter.h` and `src/fps_counter.cpp`. Each game owns one counter instance, resets it on `enter()` and game reset, records one frame in `tick()` after the visible frame has been rendered, and draws the stats in the existing UI region.

## Display

The compact TFT text is:

```text
FPS 32 MN 28 MX 41 AV 34
```

Flappy Bird, Snake, and Pong draw the text on the bottom UI band. Tetris draws it in the right-side panel under the next-piece area. The stats update at most four times per second to avoid extra flicker and display overhead.

## Calculation

The first frame after reset seeds timing and is not included in min, max, or average. After that, each frame computes `fps = 1000 / deltaMs` using `millis()` timestamps. Minimum and maximum track observed FPS values. Average is a running average across all measured frames.

## Testing

The FPS counter is pure C++ logic and tested with a host-side C++ test that injects timestamps. Game integration is verified by building the PlatformIO firmware.
