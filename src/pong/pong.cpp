#include "pong/pong.h"
#include "pong/ai_interference.h"


namespace pong {


int playerScore = 0;
int aiScore = 0;
bool lost = false;
int aiSpeed = 2;
int aiReaction = 14;
unsigned long lastPredictionTime = 0;
float currentBallSpeedMagnitude = BALL_BASE_SPEED; //global variable to track dynamic base speed
unsigned long lastLogTime = 0;
float ballMultiplier = 0;
int prevPlayerScore = -1;
int prevAiScore = -1;




void playerMove()
{
    if (isUp && paddleLeftY > 0)
    {
        paddleLeftY = max(0, paddleLeftY - PADDLE_SPEED);
    }

    if (isDown && paddleLeftY < GAME_HEIGHT - PADDLE_HEIGHT)
    {
        paddleLeftY = min(GAME_HEIGHT - PADDLE_HEIGHT, paddleLeftY + PADDLE_SPEED);
    }
}

int playerMoveForML()
{
    int action = 2; // 0=up, 1=down, 2=stay
    if (isUp && paddleLeftY > 0)
    {
        paddleLeftY = max(0, paddleLeftY - PADDLE_SPEED);
        action = 0;
    }

    if (isDown && paddleLeftY < GAME_HEIGHT - PADDLE_HEIGHT)
    {
        paddleLeftY = min(GAME_HEIGHT - PADDLE_HEIGHT, paddleLeftY + PADDLE_SPEED);
        action = 1;
    }

    return action;
}

void logPlayer(int action)
{
    Serial.printf("%f,%f,%f,%f,%f,%d\n",
        ballX,
        ballY,
        ballSpeedY,
        ballSpeedX,
        (float)paddleLeftY,
        action
    );
}

void logAI(int action)
{
    Serial.printf("%f,%f,%f,%f,%f,%d\n",
        ballX,
        ballY,
        ballSpeedY,
        ballSpeedX,
        (float)paddleRightY,
        action
    );
}

void aiMove()
{
    unsigned long currentPredictionTime = millis();
    int paddleCenter = paddleRightY + PADDLE_HEIGHT / 2;

    if (currentPredictionTime - lastPredictionTime >= AI_PREDICTION_INTERVAL)
    {
        if (ballY > paddleCenter + aiReaction)
        {
            paddleRightY += aiSpeed;
        }
        else if (ballY < paddleCenter - aiReaction)
        {
            paddleRightY -= aiSpeed;
        }

        lastPredictionTime = currentPredictionTime;
    }

    paddleRightY = constrain(paddleRightY, 0, GAME_HEIGHT - PADDLE_HEIGHT);
}

int aiMoveForML()
{
    int paddleCenter = paddleRightY + PADDLE_HEIGHT / 2;
    int action = 2; // 0=up, 1=down, 2=stay

    //Move when ball is on the AI side
    if (ballSpeedX > 0)
    {
        int threshold = 3; 

        if (ballY > paddleCenter + threshold)
        {
            paddleRightY += aiSpeed;
            action = 1;
        }
        else if (ballY < paddleCenter - threshold)
        {
            paddleRightY -= aiSpeed;
            action = 0;
        }
        else
        {
            action = 2; // stay
        }
    }

    paddleRightY = constrain(paddleRightY, 0, GAME_HEIGHT - PADDLE_HEIGHT);

    return action;
}

void aiMoveUsingML()
{
    static int lastAction = 2; // Default to 'Stay'
    unsigned long now = millis();

    // Chi predict sau khoang thoi giang interval
    if (now - lastPredictionTime >= AI_PREDICTION_INTERVAL)
    {
        float currentPaddleY = (float)paddleRightY;
        float paddleCenterY = currentPaddleY + (PADDLE_HEIGHT / 2.0); 
        float aiPaddleX = SCREEN_WIDTH - PADDLE_WIDTH;
        float distance = sqrt((aiPaddleX - ballX) * (aiPaddleX - ballX) +
                            (currentPaddleY - ballY) * (currentPaddleY - ballY));
        float deltaY = paddleCenterY - ballY;

        lastAction = predict(ballX, ballY, ballSpeedY, ballSpeedX, currentPaddleY, distance, deltaY);
        lastPredictionTime = now;
    }

    //Thuc hien hanh dong cuoi cung
    if (lastAction == 0) { // UP
        paddleRightY -= aiSpeed;
    } else if (lastAction == 1) { // DOWN
        paddleRightY += aiSpeed;
    }

    paddleRightY = constrain(paddleRightY, 0, GAME_HEIGHT - PADDLE_HEIGHT);
}

void ballBehavior()
{
    ballSpeedX += ballSpeedX*ballMultiplier;
    ballSpeedY += ballSpeedY*ballMultiplier;
    ballSpeedX = constrain(ballSpeedX, -BALL_MAX_SPEED, BALL_MAX_SPEED);
    ballSpeedY = constrain(ballSpeedY, -BALL_MAX_SPEED, BALL_MAX_SPEED);

    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Bounce off top wall
    if (ballY <= 0)
    {
        ballY = 0;
        ballSpeedY = -ballSpeedY;
        //minimum horizontal speed after wall bounce
        if (abs(ballSpeedX) < MIN_BALL_SPEED_X) {
            ballSpeedX = (ballSpeedX > 0) ? MIN_BALL_SPEED_X : -MIN_BALL_SPEED_X;
        }
    }
    // Bounce off bottom wall
    else if (ballY >= GAME_HEIGHT - BALL_SIZE)
    {
        ballY = GAME_HEIGHT - BALL_SIZE;
        ballSpeedY = -ballSpeedY;
        //minimum horizontal speed after wall bounce
        if (abs(ballSpeedX) < MIN_BALL_SPEED_X) {
            ballSpeedX = (ballSpeedX > 0) ? MIN_BALL_SPEED_X : -MIN_BALL_SPEED_X;
        }
    }

    // Left Paddle collision - Only check if ball is moving left
    if (ballSpeedX < 0 && ballX <= PADDLE_WIDTH)
    {
        if (ballY + BALL_SIZE >= paddleLeftY && ballY <= paddleLeftY + PADDLE_HEIGHT) //Speed is max when hit at top or bottom
        {
            queueSend(SOUND_HIT);
            ballX = PADDLE_WIDTH; // Prevent ball from going behind paddle and triggering a score
            float relative = (ballY + (BALL_SIZE / 2.0) - (paddleLeftY + PADDLE_HEIGHT / 2.0)) / (PADDLE_HEIGHT / 2.0); // negative top half, positive bottom half
            ballSpeedY = constrain(relative, -1.0, 1.0) * BALL_MAX_SPEED; // Use BALL_MAX_SPEED for vertical component
            //minimum horizontal speed after paddle hit
            if (abs(ballSpeedX) < MIN_BALL_SPEED_X) 
            {
                ballSpeedX = (ballSpeedX > 0) ? MIN_BALL_SPEED_X : -MIN_BALL_SPEED_X;
            }
            ballSpeedX = abs(ballSpeedX); 

            float speed = sqrt(ballSpeedX * ballSpeedX + ballSpeedY * ballSpeedY);
            ballSpeedX = (ballSpeedX / speed) * currentBallSpeedMagnitude; // Use dynamic base speed
            ballSpeedY = (ballSpeedY / speed) * currentBallSpeedMagnitude; // Use dynamic base speed
        }
    }

    // Right Paddle collision - Only check if ball is moving right
    if (ballSpeedX > 0 && ballX >= SCREEN_WIDTH - PADDLE_WIDTH - BALL_SIZE)
    {
        if (ballY + BALL_SIZE >= paddleRightY && ballY <= paddleRightY + PADDLE_HEIGHT) //Speed is max when hit at top or bottom
        {
            queueSend(SOUND_HIT);
            ballX = SCREEN_WIDTH - PADDLE_WIDTH - BALL_SIZE; // Prevent ball from getting stuck
            float relative = (ballY + (BALL_SIZE / 2.0) - (paddleRightY + PADDLE_HEIGHT / 2.0)) / (PADDLE_HEIGHT / 2.0);
            ballSpeedY = constrain(relative, -1.0, 1.0) * BALL_MAX_SPEED; // Use BALL_MAX_SPEED for vertical component
            //minimum horizontal speed after paddle hit
            if (abs(ballSpeedX) < MIN_BALL_SPEED_X) 
            {
                ballSpeedX = (ballSpeedX > 0) ? MIN_BALL_SPEED_X : -MIN_BALL_SPEED_X;
            }
            ballSpeedX = -abs(ballSpeedX);

            float speed = sqrt(ballSpeedX * ballSpeedX + ballSpeedY * ballSpeedY);
            ballSpeedX = (ballSpeedX / speed) * currentBallSpeedMagnitude; // Use dynamic base speed
            ballSpeedY = (ballSpeedY / speed) * currentBallSpeedMagnitude; // Use dynamic base speed
        }
    }
}

void scoring()
{
    bool playerScored = false;

    if (ballX <= 0)
    {
        aiScore++;
        resetBall();
    }

    if (ballX + BALL_SIZE >= SCREEN_WIDTH)
    {
        playerScore++;
        playerScored = true;
        resetBall();
    }

    if (aiScore >= 10)
    {
        gameOverFlag = true;
    }

    if (playerScored)
    {
        if (playerScore > 0 && playerScore % 3 == 0)
        {
            ballMultiplier += 0.01;
            currentBallSpeedMagnitude = BALL_BASE_SPEED * (1.0 + ballMultiplier); // Increase the target speed
            aiSpeed++;
        }

        aiReaction--;

    }
}

void resetGame()
{
    playerScore = 0;
    aiScore = 0;
    lost = false;
    gameOverFlag = false;
    paddleLeftY = (GAME_HEIGHT - PADDLE_HEIGHT) / 2;
    paddleRightY = (GAME_HEIGHT - PADDLE_HEIGHT) / 2;
    ballX = (SCREEN_WIDTH - BALL_SIZE) / 2;
    ballY = (GAME_HEIGHT - BALL_SIZE) / 2;
    ballSpeedX = BALL_BASE_SPEED;
    ballMultiplier = 0;
    currentBallSpeedMagnitude = BALL_BASE_SPEED; // Reset to base speed
    ballSpeedY = BALL_BASE_SPEED;
    aiSpeed = 3;
    aiReaction = 14;
    lastPredictionTime = 0;
    resetBall();
    drawPlayingUI();
}

void drawPlayingUI()
{
    tft.drawFastHLine(0, GAME_HEIGHT, SCREEN_WIDTH, TFT_WHITE);
    tft.fillRect(0, GAME_HEIGHT + 1, SCREEN_WIDTH, SCREEN_HEIGHT - GAME_HEIGHT, TFT_DARKGREY);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.setCursor(10, GAME_HEIGHT + 12);
    tft.print("PLAYER: ");
    tft.setCursor(180, GAME_HEIGHT + 12);
    tft.print("AI: ");

    prevPlayerScore = -1;
    prevAiScore = -1;
    drawScore();
}

void drawScore()
{
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);

    if (playerScore != prevPlayerScore)
    {
        tft.fillRect(100, GAME_HEIGHT + 12, 55, 16, TFT_DARKGREY);
        tft.setCursor(100, GAME_HEIGHT + 12);
        tft.printf("%02d", playerScore);
        prevPlayerScore = playerScore;
    }

    if (aiScore != prevAiScore)
    {
        tft.fillRect(220, GAME_HEIGHT + 12, 45, 16, TFT_DARKGREY);
        tft.setCursor(220, GAME_HEIGHT + 12);
        tft.printf("%02d", aiScore);
        prevAiScore = aiScore;
    }
}

void gameLogicForAIML()
{
    unsigned long now = millis();
    // Capture state BEFORE movement
    float currentBallX = ballX;
    float currentBallY = ballY;
    float currentBallVy = ballSpeedY;
    float currentBallVx = ballSpeedX;
    float currentPaddleY = (float)paddleRightY;
    float aiPaddleX = SCREEN_WIDTH - PADDLE_WIDTH;
    float distance = sqrt((aiPaddleX - currentBallX) * (aiPaddleX - currentBallX) +
                            (currentPaddleY - currentBallY) * (currentPaddleY - currentBallY));

    float paddleCenterY = currentPaddleY + (PADDLE_HEIGHT / 2.0); 
    float deltaY = paddleCenterY - currentBallY;
    

    playerMove(); 
    aiMoveUsingML(); // Use the model to drive the paddle
    if (interpreter != nullptr) {
        Serial.println(interpreter->arena_used_bytes());
    }
    // //Log du lieu khi bong tien gan ben phai va bong da di qua nua san
    // if (now - lastLogTime >= LOG_INTERVAL && currentBallVx > 0 && currentBallX > ((float)SCREEN_WIDTH * 0.5))
    // {
    //     if (ballX > 0 && ballX < SCREEN_WIDTH) 
    //     {
    //         Serial.printf("%f,%f,%f,%f,%f, %f, %d\n",
    //                 currentBallX,
    //                 currentBallY,
    //                 currentBallVy,
    //                 currentBallVx,
    //                 currentPaddleY,
    //                 deltaY, 
    //                 2       
    //             );
    //         lastLogTime = now;
    //     }
    // }

    ballBehavior();
    scoring();
    
}

void gameLogicForHumanML()
{
    unsigned long now = millis();
    
    //Lay du lieu truoc khi di chuyen de model biet du doan action
    float currentBallX = ballX;
    float currentBallY = ballY;
    float currentBallVy = ballSpeedY;
    float currentBallVx = ballSpeedX;
    float currentPaddleY = (float)paddleLeftY;
    float distance = sqrt((PADDLE_WIDTH - currentBallX) * (PADDLE_WIDTH - currentBallX) +
                            (currentPaddleY - currentBallY) * (currentPaddleY - currentBallY));

    float paddleCenterY = currentPaddleY + (PADDLE_HEIGHT / 2.0); 
    float deltaY = paddleCenterY - currentBallY;
    //Tinh delta tu tam cua paddle
    int action = playerMoveForML(); //Action is output of neural network

    //Log du lieu khi da qua thoi gian interval va bong da chay lai nguoi choi
    if (now - lastLogTime >= LOG_INTERVAL && currentBallVx < 0)
    {
        //Filter action 2
        bool isMoving = action != 2;
        if ((!isMoving && random(0, 10) <= 1) || isMoving) //Luon log khi di chuyen hoac chi log 2/10 khi dung yen
        {
            if (ballX > 0 && ballX < SCREEN_WIDTH) 
            {
                    Serial.printf("%f,%f,%f,%f,%f, %f, %f, %d\n",
                        currentBallX,
                        currentBallY,
                        currentBallVy,
                        currentBallVx,
                        currentPaddleY,
                        distance,
                        deltaY,
                        action
                    );
                lastLogTime = now;
            }
        }
    }

    aiMoveUsingML();
    ballBehavior();
    scoring();
}

void gameLogic()
{
    playerMove();
    aiMove();
    ballBehavior();
    scoring();
}

void render()
{
    canvas.fillSprite(TFT_BLACK);
    canvas.fillRect(0, paddleLeftY, PADDLE_WIDTH, PADDLE_HEIGHT, TFT_BROWN);
    canvas.fillRect(SCREEN_WIDTH - PADDLE_WIDTH, paddleRightY, PADDLE_WIDTH, PADDLE_HEIGHT, TFT_BROWN);
    canvas.fillCircle((int)ballX + BALL_SIZE / 2, (int)ballY + BALL_SIZE / 2, BALL_SIZE / 2, TFT_RED);
    canvas.pushSprite(0, 0);
    drawScore();
}

void drawGameOver()
{
    if (!lost)
    {
        lost = true;
        tft.setTextSize(3);
        tft.setTextColor(TFT_RED);
        tft.setCursor(70, 80);
        tft.println("GAME OVER");
    }
}

}
