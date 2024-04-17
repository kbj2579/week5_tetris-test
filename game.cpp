#include "game.h"
#include "console/console.h"
#include <iostream>
#include <cstdlib>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60


Tetromino Tetromino::I("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino Tetromino::O("O", 2, "OOOO");
Tetromino Tetromino::T("T", 3, "XOXOOOXXX");
Tetromino Tetromino::S("S", 3, "XOOOOXXXX");
Tetromino Tetromino::Z("Z", 3, "OOXXOOXXX");
Tetromino Tetromino::J("J", 3, "OXXOOOXXX");
Tetromino Tetromino::L("L", 3, "XXOOOOXXX");


// board 3개를 그린다.
void Game::drawBoard(){
  console::drawBox(0,0,11,21); // board 칸을 그린다
  console::drawBox(12,0,17,5); // Next 칸을 그린다
  console::drawBox(18,0,23,5); // Hold 칸을 그린다
}
void Game::random() {
  randNum = (int)rand() % 7 + 1;
  switch(randNum){
    case 1:
      nextT = Tetromino("I", 4, "XXXXOOOOXXXXXXXX");
      break;
    case 2:
      nextT = Tetromino("O", 2, "OOOO");
      break;
    case 3:
      nextT = Tetromino("T", 3, "XOXOOOXXX");
      break;
    case 4:
      nextT = Tetromino("S", 3, "XOOOOXXXX");
      break;
    case 5:
      nextT = Tetromino("Z", 3, "OOXXOOXXX");
      break;
    case 6:
      nextT = Tetromino("J", 3, "OXXOOOXXX");
      break;
    case 7:
      nextT = Tetromino("L", 3, "XXOOOOXXX");
      break;
  }
}

void Game::firstRandom() {
  randNum = (int)rand() % 7 + 1;
  switch(randNum){
    case 1:
      curT = Tetromino("I", 4, "XXXXOOOOXXXXXXXX");
      break;
    case 2:
      curT = Tetromino("O", 2, "OOOO");
      break;
    case 3:
      curT = Tetromino("T", 3, "XOXOOOXXX");
      break;
    case 4:
      curT = Tetromino("S", 3, "XOOOOXXXX");
      break;
    case 5:
      curT = Tetromino("Z", 3, "OOXXOOXXX");
      break;
    case 6:
      curT = Tetromino("J", 3, "OXXOOOXXX");
      break;
    case 7:
      curT = Tetromino("L", 3, "XXOOOOXXX");
      break;
  }
}

void Game::downTetro() {
  dropTimer -= 4;
  if(dropTimer == 0){
    dropTimer = DROP_DELAY;
    curY++;
  }
}

void Game::handleTetroInput() {
  if(console::key(console::K_X)){
    cw++;
    curT.rotatedCW();
  }
  if(console::key(console::K_Z)){
    ccw++;
    curT.rotatedCCW();
  }
  if(console::key(console::K_LEFT)){
    if(!leftWall) {curX--;}
  }
  if(console::key(console::K_RIGHT)){
    if(!rightWall){curX++;}
  }
  if(console::key(console::K_SPACE)){
    spaceTouch += 1;
    if(spaceTouch == 1){
      holdT = curT;
      if(cw >= ccw){
        for(int i = 0; i < cw - ccw; i++){
          holdT.rotatedCCW();
        }
      }
      else{
        for(int i = 0; i < ccw - cw; i++){
          holdT.rotatedCW();
        }
      }
      floor = true;
    }
    else if(canSpace == true){
      swapT = holdT;
      holdT = curT;
      curT = swapT;
      if(cw >= ccw){
        for(int i = 0; i < cw - ccw; i++){
          holdT.rotatedCCW();
        }
      }
      else{
        for(int i = 0; i < ccw - cw; i++){
          holdT.rotatedCW();
        }
      }
      canSpace = false;
    }
  }
  if(console::key(console::K_DOWN)){
    curY++;
  }
}

 void Game::hitWall() { 
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(curT.check(i, j) == true && (curX + j == 1 || board_[curX + j - 2][curY + i] == true)){
        leftWall = true;
        return;
      }
    }
    for(int j = 3; j >= 0; j--){
      if(curT.check(i, j) == true && (curX + j == 10 || board_[curX + j - 2][curY + i] == true)){
        rightWall = true;
        return;
      }
    }
  }
  leftWall = false;
  rightWall = false;
}

void Game::hitFloor(){
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(curT.check(i, j) == true && (board_[curX + j - 1][curY + i] == true || curY + i == 20)){
        floor = true;
        for(int k = 0; k < 4; k++){
          for(int m = 0; m < 4; m++){
            if(curT.check(k, m)) {
              board_[curX + m - 1][curY + k - 1] = true;
            }
          }
        }
        return;
      }
    }
  }
}

  // 게임의 한 프레임을 처리한다.
void Game::update() {
  handleTetroInput();
  downTetro();
  hitWall();
  hitFloor();
  lineCheck();
  if(floor){ // 현재 테트로미노가 바닥에 닿았을경우 다음 테트로미노가 현재 테트로미노가 되어 이동한다.
    curT = nextT;
    random();
    curX = BOARD_WIDTH/2 - 1;
    curY = 1;
    floor = false;
    canSpace = true;
    cw = 0;
    ccw = 0;
  }
}   
// 게임 화면을 그린다.
void Game::draw(){
  drawBoard(); // 보드판을 그린다.
  console::draw(13,0,"Next");
  console::draw(19,0,"Hold");
   
   int minH = 21;
   int minX = 0;
   int minY = 0;
   for(int i = 3; i >= 0; i--){
    for(int j = 3; j >= 0; j--){
      if(curT.check(i, j)){
        for(int m = curY + i - 1; m < BOARD_HEIGHT; m++){
            if(board_[curX + j - 1][m] == true){
              if(m + 1 - (curY + i) < minH){
                minH = m + 1 - (curY + i);
                minX = curX;
                minY = m - i;
              }
            }
        }
      }
    }
  }
    
    if(minH == 21){
      bool check = true;
      int floorY = 20;
      for(int i = 3; i >= 0; i--){
        for(int j = 3; j >= 0; j--){
          if(curT.check(i, j)){
            if(check){
              minY = i;
              floorY -= i;
              check = false;
            }
            curT.drawAt(SHADOW_STRING, curX + j, floorY + i);
          }
        }
      }
    }
    else{
      for(int i = 3; i >= 0; i--){
        for(int j = 3; j >= 0; j--){
          if(curT.check(i, j)){
            curT.drawAt(SHADOW_STRING, minX + j, minY + i);
          }
        }   
      }
    }

    // 보드판에서 움직이고 있는 테트로미노를 그린다.
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(curT.check(i, j)){
        curT.drawAt(BLOCK_STRING, curX + j, curY + i);
      }
    }
  }

  // 보드판에 쌓이는 블록을 그린다.
  for(int i = 0; i < BOARD_WIDTH; i++){
    for(int j = 0; j < BOARD_HEIGHT; j++){
      if(board_[i][j] == true){
        console::draw(i + 1, j + 1, BLOCK_STRING);
      }
    }
  }

  // Next 칸에 nextT를 그린다.
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(nextT.check(i, j)){
        nextT.drawAt(BLOCK_STRING, 13 + j, 1 + i);
      }
    }
  }
  // Hold 칸에 holdT를 그린다.  
  if(spaceTouch >= 1){
    for(int i = 0; i < 4; i++){
      for(int j = 0; j < 4; j++){
        if(holdT.check(i, j)){
          holdT.drawAt(BLOCK_STRING, 19 + j, 1 + i);
        }
      }
    }
  } 
}

void Game::lineCheck(){
  int count;
  for(int i = BOARD_HEIGHT - 1; i >= 0; i--){
    count = 0;
    for(int j = 0; j < BOARD_WIDTH; j++){
      if(board_[j][i] == true){
        count++;
      }
    }
    
    if(count == 10){
      count_line++;
      for(int k = 0; k < BOARD_WIDTH; k++){
        board_[k][i] = false;
      }
      for(int m = i; m > 0; m--){
        for(int n = 0; n < BOARD_WIDTH; n++){
          board_[n][m] = board_[n][m - 1];
        }
      }
      for(int p = 0; p < BOARD_WIDTH; p++){
        board_[p][0] = false;
      }
      i -= 1;
    }
  }
}




// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit(){
  // ESC 키를 누르면 게임종료
  if(console::key(console::K_ESC)){
    return true;
  }
  if(count_line == 40){
    return true;
  }
  return false;
}

Game::Game() {
  for(int i = 0; i < BOARD_WIDTH; i++){
    for(int j = 0; j < BOARD_HEIGHT; j++){
      board_[i][j] = false;
    }
  }
  count_line = 0;   
  randNum = 0;
  firstRandom();
  random();
}