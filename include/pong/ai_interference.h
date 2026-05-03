#pragma once
#include "pong/model.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#define TENSOR_ARENA_SIZE 4096 //4KB RAM
#define MAX_BALL_X 319.96
#define MIN_BALL_X 0.53
#define MAX_BALL_Y 192.00
#define MIN_BALL_Y 0.00
#define MAX_VY 6.00
#define MIN_VY -6.00
#define MIN_VX 2.00
#define MAX_VX 6.00
#define MIN_PADDLE_Y 0.00
#define MAX_PADDLE_Y 170.00
#define MIN_DELTA_Y -155.90
#define MAX_DELTA_Y 162.42

namespace pong {

    extern uint8_t tensorArena[TENSOR_ARENA_SIZE];
    extern const tflite::Model* model;
    extern tflite::MicroMutableOpResolver<5> resolver;
    extern tflite::MicroInterpreter* interpreter; // Changed to pointer due to non-default constructor
    extern TfLiteTensor* input;
    extern TfLiteTensor* output;



    void initAI();
    void releaseAI();
    void normalize(float &ballX, float &ballY, float &vy, float &vx, float &paddleY, float &deltaY);
    inline float clamp(float x);
    int predict(float ballX, float ballY, float vy, float vx, float paddleY, float deltaY);

}