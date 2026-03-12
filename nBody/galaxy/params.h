#ifndef PARAMS_H
#define PARAMS_H

static constexpr bool OMP_OK = true;
static constexpr unsigned N = 1000;
static constexpr double soft = 1;
static constexpr double G = 1;
static constexpr double FPS = 60;
static constexpr double updatesPerFrame = 1;
static constexpr double tau = 1 / (FPS * updatesPerFrame);
static constexpr double mToPx = 1;

static constexpr unsigned W = 1800, H = 1000;
static constexpr double PI = 3.14159265358979323846;

#endif