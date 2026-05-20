#include "pong/ai_interference.h"

#include <cstdint>


namespace pong {
    uint8_t* tensorArena = nullptr;
    const tflite::Model* model = nullptr;
    tflite::MicroMutableOpResolver<5> resolver;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* input = nullptr;
    TfLiteTensor* output = nullptr;

    namespace {
        alignas(16) uint8_t tensorArenaStorage[TENSOR_ARENA_SIZE];

        bool ensureTensorArena()
        {
            if (tensorArena != nullptr) return true;
            tensorArena = tensorArenaStorage;
            return true;
        }
    }

    
    void initAI() 
    {
        if (interpreter != nullptr) return;
        if (!ensureTensorArena()) return;

        //Initialize global model and resolver only once
        model = tflite::GetModel(pong_modelHeavy_int8_tflite);
        static bool resolver_ready = false;
        if (!resolver_ready) {
            resolver.AddFullyConnected();
            resolver.AddRelu();
            resolver.AddSoftmax();
            resolver_ready = true;
        }
        
        //Interpreter
        static tflite::MicroErrorReporter error_reporter;

        interpreter = new tflite::MicroInterpreter
        (
            model,
            resolver,
            tensorArena,
            TENSOR_ARENA_SIZE,
            &error_reporter
        );

        
        ///Allocate tensors for interpreter
        if (interpreter->AllocateTensors() != kTfLiteOk) 
        {
            delete interpreter;
            interpreter = nullptr;
            return;
        }
        
        input = interpreter->input(0);
        output = interpreter->output(0);
    }

    void releaseAI()
    {
        if (interpreter != nullptr)
        {
        delete interpreter;
        interpreter = nullptr;
        }
        input = nullptr;
        output = nullptr;
        model = nullptr;
        tensorArena = nullptr;
    }

    inline float clamp(float x)
    {
        if (x > 1) return 1;
        if (x < 0) return 0;

        return x;
    }


    void normalize(float &ballX, float &ballY, float &vy, float &vx, float &paddleY, float &distance, float &deltaY)
    {
        ballX = (ballX - MIN_BALL_X) / (MAX_BALL_X - MIN_BALL_X); 
        ballY = (ballY - MIN_BALL_Y) / (MAX_BALL_Y - MIN_BALL_Y);
        vy = (vy - MIN_VY) / (MAX_VY - MIN_VY);
        vx = (vx - MIN_VX) / (MAX_VX - MIN_VX);
        paddleY = (paddleY - MIN_PADDLE_Y) / (MAX_PADDLE_Y - MIN_PADDLE_Y);
        distance = (distance - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE);
        deltaY = (deltaY - MIN_DELTA_Y) / (MAX_DELTA_Y - MIN_DELTA_Y);


        ballX = clamp(ballX);
        ballY = clamp(ballY);
        vy = clamp(vy);
        vx = clamp(vx);
        paddleY = clamp(paddleY);
        distance = clamp(distance);
        deltaY = clamp(deltaY);
    }

    int predict(float ballX, float ballY, float vy, float vx, float paddleY, float distance, float deltaY)
    {
        // Normalize
        normalize(ballX, ballY, vy, vx, paddleY, distance, deltaY);

        
        if (!input || !interpreter) return 2; // Default stay 

        //Quantize
        //quantized_value = (float_value / scale) + zero_point
        float features[7] = {ballX, ballY, vy, vx, paddleY, distance, deltaY};
        for (int i = 0; i < 7; i++) {
            int32_t q = (int32_t)(features[i] / input->params.scale + input->params.zero_point); 

            if (q > 127) q = 127;
            if (q < -128) q = -128;
            input->data.int8[i] = (int8_t)q; //Nap input vao neural network
        }

        // Run inference
        if (interpreter->Invoke() != kTfLiteOk) return 2;

        //Lay output la gia tri du doan
        int8_t* results = output->data.int8;
        int action = 0;
        int8_t max_val = results[0];

        if (results[1] > max_val) { max_val = results[1]; action = 1; }
        if (results[2] > max_val) { action = 2; }

        return action;
    }
}
