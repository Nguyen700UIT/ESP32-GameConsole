#pragma once
#include <Arduino.h>
#include "Tetris/config.h"
#include "Tetris/pieces.h"
#include "console/fps_counter.h"

//Tetris board matrix 200x240 pixel
#define BOARD_NUM_ROW 12 
#define BOARD_NUM_COL 10
#define BOARD_PIXEL_WIDTH  (BOARD_NUM_COL * BLOCK_SIZE)
#define BOARD_PIXEL_HEIGHT (BOARD_NUM_ROW * BLOCK_SIZE)
#define BOARD_OFFSET 0 // Bỏ 0 pixel đầu và  pixel cuối với mỗi trục X và Y

namespace tetris {

extern uint16_t tetrisBoard[BOARD_NUM_ROW][BOARD_NUM_COL];

struct Piece
{
    int x;
    int y;
    uint8_t shape[4][4];
    uint16_t pieceColor;
};

extern int prevScore;
extern int score;
extern Piece currPiece;
extern Piece ghostPiece;
extern uint8_t nextPieceShape[4][4];
extern uint16_t nextPieceColor;
extern bool touchGround;
extern unsigned long lastTouchTime;
extern unsigned long lastDelayedFall;
extern unsigned long lastHoldFall;
extern unsigned long lastGameOverTime;
extern bool erasedDeathScreen;
extern bool erasedPlayingUI;
extern bool redrawUI;

u_int8_t pixelCordToCellCord(int cord); //Can be x or y
bool checkCollision(u_int8_t shape[4][4], int x, int y);
void gameReset();
void initBoard();
void gameOver();
void drawBoard();
void spawnPiece();
void calculateGhostPiece();
void initPiece();
int clearLine();
void movePiece();
bool delayedFallAndLogic();
void rotatePiece();
void lockPiece();
void eraseScreen();
void drawPlayingUI();
void drawGameOverUI();
void eraseNextPiece();
void drawNextPiece();
void drawPiece();
void drawGhostPiece();
void drawDeathUI();
void drawScore();
void drawFpsStats(const console::FpsStats& stats);
void renderBoard();

}
