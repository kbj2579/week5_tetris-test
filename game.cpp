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

// 테트로미노를 내린다.
void Game::downTetro() {
  dropTimer -= 1;
  if(dropTimer == 0){
    dropTimer = DROP_DELAY;
    curY++;
  }
}

// 테트로미노를 회전시키거나 이동시킨다.
void Game::handleTetroInput() {
  if(console::key(console::K_X)){
    if(cwCheck()){
      cw++;
      curT.rotatedCW();
    }
  }
  if(console::key(console::K_Z)){
    if(cwCheck()){
      ccw++;
      curT.rotatedCCW();
    }
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
      curX = BOARD_WIDTH/2 - 1;
      curY = 1;
    }
  }
  if(console::key(console::K_UP)){ // 테트로미노를 즉시 바닥에 도착시킨다
    curY = shadowOrHard();
  }


  if(console::key(console::K_DOWN)){
    curY++;
  }
}


 void Game::hitWall() { 
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(curT.check(i, j) == true && (curX + j == 1 || board_[curX + j - 2][curY + i - 1] == true)){
        leftWall = true;
        rightWall = false;
        return;
      }
    }
    for(int j = 3; j >= 0; j--){
      if(curT.check(i, j) == true && (curX + j == 10 || board_[curX + j][curY + i - 1] == true)){
        rightWall = true;
        leftWall = false;
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

// 승리 패배 확인
void Game::winOrLose(){
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(board_[3][1] || board_[4][1] || board_[5][1]){ // 패배 데드라인 설정.
        lose = true;
      }
    }
  }
}

// 쉐도우, 하드드롭 위치 확인
int Game::shadowOrHard() {
  int dis = 19;
  int s = curT.size();
  if(s == 2) s++;

  for (int i = s - 1; i >= 0; i--){
    for (int j = 0; j < s; j++){
      if (curT.check(i, j)){
        for (int k = 19; k >= curY; k--)
          if (board_[curX + j - 1][k]) {
            if (k - i <= dis)
              dis =  k - i;
          }
      }
    }
  }


  if (s == 3){
    for (int j = 0; j < s; j++)
      if (curT.check(s - 1, j)){ 
        if (dis < 18) return dis;
        return 18;
      }
  }
  else if (s == 4)
  {
    for (int j = 0; j < s; j++)
    {
      if (curT.check(s - 1, j)){
        if (dis < 17) return dis;
        return 17;
      }
      if (curT.check(s - 2, j)){
        if (dis < 16) return dis;
        return 16;
      }
    }
  }
  return dis;
}

bool Game::cwCheck(){
  bool can = true;
  // I 블록이 바닥에 닿기직전 세로로 회전을 못하게 처리한다.
  if(curT.size() == 4){
    if(curY == 18 && curT.check(1, 0)){
      can = false;
      return can;
    }
  }

  // I 블록의 회전을 못하는 경우를 처리한다.
  if(curT.size() == 4){
      for(int i = 0; i < 4; i++){
        for(int j = 3; j >= 0; j--){
          if(curT.check(i, j) == true && (curX + j == 1 || board_[curX + j - 2][curY + i - 1] == true)){
            if(i == 0 && j == 1)
              can = false;
              return can;
          }
          else if(curT.check(i, j) == true && (curX + j == 10 || curX + j + 1 == 10 || board_[curX + j][curY + i - 1] == true || board_[curX + j + 1][curY + i - 1] == true)){
            if(i == 0 && j == 1)
              can = false;
              return can;
          }
        }
      }
      return can;
  }
  
  else{     // size가 3인 블록이 회전을 못하는 경우를 처리한다.
    for(int i = 0; i < 4; i++){
      for(int j = 3; j >= 0; j--){
        if(curT.check(i, j) == true && (curX + j == 1 || board_[curX + j - 2][curY + i - 1] == true)){
          if(j == 1) 
            can = false;
            return can;
          if(j == 2) 
            can = false;
            return can;
          
        }
      }
    }

    for(int i = 0; i < 4; i++){
      for(int j = 0; j <= 3; j++){
        if(curT.check(i, j) == true && (curX + j == 10 || board_[curX + j][curY + i - 1] == true)){
          if(j == 1) 
            can = false;
            return can;
          if(j == 2) 
            can = false;
            return can;
        }
      }
    }
    return can;
  }
}
  // 게임의 한 프레임을 처리한다.
void Game::update() {
  handleTetroInput();
  downTetro();
  hitWall();
  hitFloor();
  lineCheck();
  winOrLose();
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
  console::draw(0, 22, std::to_string(count_line) + " lines left");

  auto currentTime = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

  int minutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count() % 60;
  int seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count() % 60;
  int milliseconds = duration.count() % 1000;

  // 시간출력
  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(2) << minutes << ":"//분
      << std::setfill('0') << std::setw(2) << seconds << "."//초
      << std::setfill('0') << std::setw(2) << milliseconds / 10;
  console::draw(2, 23, oss.str());
  
  //shadaw string 을 그린다
  int s = curT.size();
  if(s == 2) s++;
  int shadowY = shadowOrHard();
  for (int i = 0; i < s; ++i){
    for (int j = 0; j < s; ++j){
      if (curT.check(i, j)){
        curT.drawAt(SHADOW_STRING, curX + j, shadowY + i);
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

  // 승리, 패배 될 경우를 그린다
  if (count_line == 0){
    console::draw(2, 10, "You Win");
    console::draw(2, 11, oss.str());
  }
  if(lose){
    console::draw(2, 10, "You Lost");
  }
}


// 줄이 채워졌는지 체크한다
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
      count_line--;
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
  if(count_line == 0){
    return true;
  }
  if(lose){
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
  count_line = 40;
  randNum = 0;
  firstRandom();
  random();
}