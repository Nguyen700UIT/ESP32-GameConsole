#include "Tetris/tetris.h"
#include "Tetris/display.h"

namespace tetris {

uint16_t tetrisBoard[BOARD_NUM_ROW][BOARD_NUM_COL];
int prevScore = 0;
int score = 0;
Piece currPiece;
Piece ghostPiece;
uint8_t nextPieceShape[4][4];
uint16_t nextPieceColor;
bool touchGround = false;
unsigned long lastTouchTime = 0;
unsigned long lastDelayedFall = 0;
unsigned long lastGameOverTime = 0;
bool erasedDeathScreen = false;
bool erasedPlayingUI = false;
bool redrawUI = false;

u_int8_t pixelCordToCellCord(int cord)
{
    return (cord - BOARD_OFFSET) / BLOCK_SIZE;
}

void initBoard()
{
  for (int y = 0; y < BOARD_NUM_ROW; y++)
  {
    for (int x = 0; x < BOARD_NUM_COL; x++)
    {
      tetrisBoard[y][x] = 0;
    }
  }
}

void drawBoard()
{
  for (int y = 0; y < BOARD_NUM_ROW; y++)
  {
    for (int x = 0; x < BOARD_NUM_COL; x++)
    {
      if (tetrisBoard[y][x])
      {
        tetrisCanvas.fillRect(BOARD_OFFSET + x * BLOCK_SIZE, BOARD_OFFSET + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, tetrisBoard[y][x]);
        tetrisCanvas.drawRect(BOARD_OFFSET + x * BLOCK_SIZE, BOARD_OFFSET + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, TFT_WHITE);
      }
    }
  }
}

void gameOver()
{
  if (checkCollision(currPiece.shape, currPiece.x, currPiece.y))
  {
    gameOverFlag = true;
    score = 0;
    lastGameOverTime = millis();
  }
}

void spawnPiece()
{
  currPiece.x = BOARD_OFFSET + (BOARD_PIXEL_WIDTH / 2) - (BLOCK_SIZE * 2);
  currPiece.y = BOARD_OFFSET;
  memcpy(currPiece.shape, nextPieceShape, sizeof(currPiece.shape));
  currPiece.pieceColor = nextPieceColor;

  int nextType = random(0, 7);
  memcpy(nextPieceShape, piecesShape[nextType], sizeof(nextPieceShape));
  nextPieceColor = pieceColor[nextType];
}

void calculateGhostPiece()
{
  ghostPiece = currPiece;
  while (!checkCollision(ghostPiece.shape, ghostPiece.x, ghostPiece.y + BLOCK_SIZE))
  {
    ghostPiece.y += GRAVITY;
  }
  ghostPiece.pieceColor = TFT_BLACK;
}

void initPiece()
{
  int type = random(0, 7);
  currPiece.pieceColor = pieceColor[type];
  currPiece.x = BOARD_OFFSET + (BOARD_PIXEL_WIDTH / 2) - (BLOCK_SIZE * 2);
  currPiece.y = BOARD_OFFSET;
  memcpy(currPiece.shape, piecesShape[type], sizeof(currPiece.shape));

  int nextType = random(0, 7);
  memcpy(nextPieceShape, piecesShape[nextType], sizeof(nextPieceShape));
  nextPieceColor = pieceColor[nextType];
}

bool checkCollision(u_int8_t shape[4][4], int x, int y)
{
  for (int tempY = 0; tempY < 4; ++tempY)
  {
    for (int tempX = 0; tempX < 4; ++tempX)
    {
      if (!shape[tempY][tempX]) continue;

      int px = x + tempX * BLOCK_SIZE;
      int py = y + tempY * BLOCK_SIZE;
      if (px < BOARD_OFFSET || px + BLOCK_SIZE > BOARD_OFFSET + BOARD_PIXEL_WIDTH)
      {
        return true;
      }

      if (py < BOARD_OFFSET || py + BLOCK_SIZE > BOARD_OFFSET + BOARD_PIXEL_HEIGHT)
      {
        return true;
      }

      int boardX = pixelCordToCellCord(px);
      int boardY = pixelCordToCellCord(py);

      if (boardY >= 0 && tetrisBoard[boardY][boardX])
      {
        return true;
      }
    }
  }
  return false;
}

void movePiece()
{
    if (isUp)
    {
      rotatePiece();
      isUp = false;
    }
    else if (isLeft)
    {
      if (!checkCollision(currPiece.shape, currPiece.x - BLOCK_SIZE, currPiece.y))
      {
        currPiece.x -= BLOCK_SIZE;
      }
      isLeft = false;
    }
    else if (isRight)
    {
      if (!checkCollision(currPiece.shape, currPiece.x + BLOCK_SIZE, currPiece.y))
      {
        currPiece.x += BLOCK_SIZE;
      }
      isRight = false;
    }
    else if (isDown)
    {
      if (digitalRead(DOWN) == HIGH) isDown = false;
      if (!checkCollision(currPiece.shape, currPiece.x, currPiece.y + BLOCK_SIZE))
      {
        currPiece.y += BLOCK_SIZE;
      }
    }
}

void lockPiece()
{
  for (int tempY = 0; tempY < 4; ++tempY)
  {
    for (int tempX = 0; tempX < 4; ++tempX)
    {
      if (!currPiece.shape[tempY][tempX]) continue;

      int px = currPiece.x + tempX * BLOCK_SIZE;
      int py = currPiece.y + tempY * BLOCK_SIZE;
      int boardX = pixelCordToCellCord(px);
      int boardY = pixelCordToCellCord(py);

      if (boardY >= 0)
      {
        tetrisBoard[boardY][boardX] = currPiece.pieceColor;
      }
    }
  }
}

int clearLine()
{
  int cleared = 0;
  for (int y = BOARD_NUM_ROW - 1; y > 0; --y)
  {
    bool full = true;
    for (int x = 0; x < BOARD_NUM_COL; ++x)
    {
      if (tetrisBoard[y][x] == 0)
      {
        full = false;
        break;
      }
    }
    if (full)
    {
      for (int i = y; i > 0; --i)
      {
        memcpy(tetrisBoard[i], tetrisBoard[i - 1], sizeof(tetrisBoard[i]));
      }
      memset(tetrisBoard[0], 0, sizeof(tetrisBoard[0]));
      y++;
      cleared++;
    }
  }
  return cleared;
}

void rotatePiece()
{
  u_int8_t temp[4][4];
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      temp[i][j] = currPiece.shape[j][i];
    }
  }

  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 2; ++j)
    {
      u_int8_t swap = temp[j][i];
      temp[j][i] = temp[3 - j][i];
      temp[3 - j][i] = swap;
    }
  }

  if (!checkCollision(temp, currPiece.x, currPiece.y))
  {
    memcpy(currPiece.shape, temp, sizeof(currPiece.shape));
  }
}

bool delayedFallAndLogic()
{
  bool flag = false;
  unsigned long now = millis();
  bool willCollide = checkCollision(currPiece.shape, currPiece.x, currPiece.y + BLOCK_SIZE);
  if (now - lastDelayedFall > 1500 && !willCollide)
  {
    currPiece.y += GRAVITY;
    touchGround = false;
    lastDelayedFall = now;
  }
  else if (willCollide)
  {
    if (!touchGround)
    {
      touchGround = true;
      lastTouchTime = now;
    }
    if (now - lastTouchTime > 500)
    {
      lockPiece();
      int temp = clearLine();
      prevScore = score;
      switch (temp)
      {
        case 1:
          score += 100;
          break;
        case 2:
          score += 300;
          break;
        case 3:
          score += 500;
          break;
        case 4:
          score += 800;
          break;
      }
      spawnPiece();
      gameOver();
      flag = true;
    }
  }
  return flag;
}

void drawPiece()
{
  for (int tempY = 0; tempY < 4; tempY++)
  {
    for (int tempX = 0; tempX < 4; tempX++)
    {
      if (currPiece.shape[tempY][tempX])
      {
        tetrisCanvas.fillRect(currPiece.x + tempX * BLOCK_SIZE, currPiece.y + tempY * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, currPiece.pieceColor);
        tetrisCanvas.drawRect(currPiece.x + tempX * BLOCK_SIZE, currPiece.y + tempY * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, TFT_WHITE);
      }
    }
  }
}

void drawGhostPiece()
{
  for (int tempY = 0; tempY < 4; tempY++)
  {
    for (int tempX = 0; tempX < 4; tempX++)
    {
      if (ghostPiece.shape[tempY][tempX])
      {
        tetrisCanvas.fillRect(ghostPiece.x + tempX * BLOCK_SIZE, ghostPiece.y + tempY * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, TFT_BLACK);
        tetrisCanvas.drawRect(ghostPiece.x + tempX * BLOCK_SIZE, ghostPiece.y + tempY * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, TFT_WHITE);
      }
    }
  }
}

void eraseScreen()
{
  tft.fillScreen(TFT_BLACK);
}

void drawScore()
{
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(BOARD_PIXEL_WIDTH + 5, 20);
  tft.println("Score: ");
  tft.setCursor(BOARD_PIXEL_WIDTH + 5, 45);
  tft.printf("%05d" , score);
}

void drawPlayingUI()
{
  tft.drawFastVLine(BOARD_PIXEL_WIDTH, 0, BOARD_PIXEL_HEIGHT, TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(BOARD_PIXEL_WIDTH + 5, 70);
  tft.println("Next");
  tft.setCursor(BOARD_PIXEL_WIDTH + 5, 95);
  tft.println("piece:");
}

void drawDeathUI()
{
  tft.drawFastVLine(BOARD_PIXEL_WIDTH, 0, BOARD_PIXEL_HEIGHT, TFT_WHITE);
  for (int tempY = 0; tempY < 4; tempY++)
  {
    for (int tempX = 0; tempX < 4; tempX++)
    {
      if (currPiece.shape[tempY][tempX])
      {
        tft.fillRect(currPiece.x + tempX * BLOCK_SIZE, currPiece.y + tempY * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, currPiece.pieceColor);
        tft.drawRect(currPiece.x + tempX * BLOCK_SIZE, currPiece.y + tempY * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, TFT_WHITE);
      }
    }
  }
  drawScore();
}

void drawGameOverUI()
{
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2);
  tft.println("GAME OVER");
}

void eraseNextPiece()
{
  tft.fillRect(225, 120, 4 * BLOCK_SIZE, 4 * BLOCK_SIZE, TFT_BLACK);
}

void drawNextPiece()
{
  for (int tempY = 0; tempY < 4; tempY++)
  {
    for (int tempX = 0; tempX < 4; tempX++)
    {
      if (nextPieceShape[tempY][tempX])
      {
        tft.fillRect(225 + tempX * BLOCK_SIZE, 120 + tempY * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, nextPieceColor);
        tft.drawRect(225 + tempX * BLOCK_SIZE, 120 + tempY * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, TFT_WHITE);
      }
    }
  }
}

void gameReset()
{
  score = 0;
  initBoard();
  initPiece();
  gameOverFlag = false;
  tft.fillScreen(TFT_BLACK);
}

void renderBoard()
{
  tetrisCanvas.fillSprite(TFT_BLACK);
  drawGhostPiece();
  drawPiece();
  drawBoard();
  tetrisCanvas.pushSprite(BOARD_OFFSET, BOARD_OFFSET);
}

}
