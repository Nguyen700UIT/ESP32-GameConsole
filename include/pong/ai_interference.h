#pragma once
// #include "pong/model.h"
// #include "pong/modelAdvanced.h"
#include "pong/heavyModel.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#define TENSOR_ARENA_SIZE 70*1024 //70KB RAM
#define MAX_BALL_X 319.02
#define MIN_BALL_X 18.00
#define MAX_BALL_Y 192.00
#define MIN_BALL_Y 0.00
#define MAX_VY 7.00
#define MIN_VY -7.00
#define MIN_VX 2.00
#define MAX_VX 7.00
#define MIN_PADDLE_Y 1.00
#define MAX_PADDLE_Y 170.00
#define MIN_DISTANCE 0.06
#define MAX_DISTANCE 313.46
#define MIN_DELTA_Y -149.00
#define MAX_DELTA_Y 147.00

namespace pong {

    extern uint8_t tensorArena[TENSOR_ARENA_SIZE];
    extern const tflite::Model* model;
    extern tflite::MicroMutableOpResolver<5> resolver;
    extern tflite::MicroInterpreter* interpreter; // Changed to pointer due to non-default constructor
    extern TfLiteTensor* input;
    extern TfLiteTensor* output;



    void initAI();
    void releaseAI();
    void normalize(float &ballX, float &ballY, float &vy, float &vx, float &paddleY, float &distance, float &deltaY);
    inline float clamp(float x);
    int predict(float ballX, float ballY, float vy, float vx, float paddleY, float distance, float deltaY);

}
