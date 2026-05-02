#include "pong/pong.h"


namespace pong {

int playerScore = 0;
int aiScore = 0;
bool lost = false;
int aiSpeed = 3;
int aiReaction = 14;
unsigned long lastPredictionTime = 0;

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

void ballBehavior()
{
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
        if (ballY + BALL_SIZE >= paddleLeftY && ballY <= paddleLeftY + PADDLE_HEIGHT)
        {
            ballX = PADDLE_WIDTH; // Prevent ball from going behind paddle and triggering a score
            float relative = (ballY + (BALL_SIZE / 2.0) - (paddleLeftY + PADDLE_HEIGHT / 2.0)) / (PADDLE_HEIGHT / 2.0);
            ballSpeedY = constrain(relative, -1.0, 1.0) * BALL_MAX_SPEED; // Use BALL_MAX_SPEED for vertical component
            // Ensure minimum horizontal speed after paddle hit
            if (abs(ballSpeedX) < MIN_BALL_SPEED_X) {
                ballSpeedX = (ballSpeedX > 0) ? MIN_BALL_SPEED_X : -MIN_BALL_SPEED_X;
            }
            ballSpeedX = abs(ballSpeedX); 

            float speed = sqrt(ballSpeedX * ballSpeedX + ballSpeedY * ballSpeedY);
            ballSpeedX = (ballSpeedX / speed) * BALL_BASE_SPEED;
            ballSpeedY = (ballSpeedY / speed) * BALL_BASE_SPEED;
        }
    }

    // Right Paddle collision - Only check if ball is moving right
    if (ballSpeedX > 0 && ballX >= SCREEN_WIDTH - PADDLE_WIDTH - BALL_SIZE)
    {
        if (ballY + BALL_SIZE >= paddleRightY && ballY <= paddleRightY + PADDLE_HEIGHT)
        {
            ballX = SCREEN_WIDTH - PADDLE_WIDTH - BALL_SIZE; // Prevent ball from getting stuck
            float relative = (ballY + (BALL_SIZE / 2.0) - (paddleRightY + PADDLE_HEIGHT / 2.0)) / (PADDLE_HEIGHT / 2.0);
            ballSpeedY = constrain(relative, -1.0, 1.0) * BALL_MAX_SPEED; // Use BALL_MAX_SPEED for vertical component
            // Ensure minimum horizontal speed after paddle hit
            if (abs(ballSpeedX) < MIN_BALL_SPEED_X) {
                ballSpeedX = (ballSpeedX > 0) ? MIN_BALL_SPEED_X : -MIN_BALL_SPEED_X;
            }
            ballSpeedX = -abs(ballSpeedX);

            float speed = sqrt(ballSpeedX * ballSpeedX + ballSpeedY * ballSpeedY);
            ballSpeedX = (ballSpeedX / speed) * BALL_BASE_SPEED;
            ballSpeedY = (ballSpeedY / speed) * BALL_BASE_SPEED;
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
        if (playerScore > 0 && playerScore % 5 == 0)
        {
            ballSpeedX *= 1.2;
            ballSpeedY *= 1.2;
            ballSpeedX = constrain(ballSpeedX, -BALL_MAX_SPEED, BALL_MAX_SPEED);
            ballSpeedY = constrain(ballSpeedY, -BALL_MAX_SPEED, BALL_MAX_SPEED);
        }

        aiReaction--;
        aiSpeed++;
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
    ballSpeedY = BALL_BASE_SPEED;
    aiSpeed = 3;
    aiReaction = 14;
    lastPredictionTime = 0;
    resetBall();
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

    tft.fillRect(0, GAME_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - GAME_HEIGHT, TFT_DARKGREY);
    tft.drawFastHLine(0, GAME_HEIGHT, SCREEN_WIDTH, TFT_WHITE);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.setCursor(10, GAME_HEIGHT + 12);
    tft.printf("PLAYER: %d", playerScore);
    tft.setCursor(180, GAME_HEIGHT + 12);
    tft.printf("AI: %d", aiScore);
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
