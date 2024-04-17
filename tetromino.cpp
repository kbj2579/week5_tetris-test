#include "tetromino.h"
#include "console/console.h"
#include <iostream>
#include <string>
#define SHADOW_STRING "⛶"
#define BLOCK_STRING "■"




    // 사이즈와 shape 문자열을 p/;통해 생성한다.
    // 문자열은 size * size 길이의 문자열이 주어진다.

    // 블록이 존재한다면 O를 통해 표시하고 아니라면 X를 통해 표시한다.
    // 문자열은 개행 없이 일렬로 나열되어 주어진다.
    // 예를 들어 T 미노의 모양과 생성은 다음과 같다.
    // XOX
    // OOO
    // XXX
    //
    // Tetromino T("T", 3, "XOXOOOXXX")

    // I 미노의 경우 다음과 같다.
    // XXXX
    // OOOO
    // XXXX
    // XXXX
    //
    // Tetromino I("I", 4, "XXXXOOOOXXXXXXXX")
    Tetromino::Tetromino(std::string name, int size, std::string shape) {
        name_ = name;
        size_ = size;
        width = -1;
        for(int i = 0; i < MAX_SIZE; i++){
            for(int j = 0; j < MAX_SIZE; j++){
                shape_[i][j] = false;
            }
        }

        if(size_ == 2){
            shape_[1][1] = true;
            shape_[1][2] = true;
            shape_[2][1] = true;
            shape_[2][2] = true;
        }

        else if(size_ == 3){
            for (int i = 0; i < size * size; i++) { 
                if (i % size == 0) width++;
                if (shape[i] == 'O') {
                    shape_[width][i % size] = true;
                }
            }
        }
        else if(size_ == 4){
            shape_[0][1] = true;
            shape_[1][1] = true;
            shape_[2][1] = true;
            shape_[3][1] = true;
        }
        original_ = this;
    }

    // 시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
    Tetromino Tetromino::rotatedCW() {
        if (size_ == 3) {
            temp = shape_[0][2];
            temp2 = shape_[2][2];
            shape_[0][2] = shape_[0][0];
            shape_[2][2] = temp;
            temp = shape_[2][0];
            shape_[2][0] = temp2;
            shape_[0][0] = temp;

            //
            temp = shape_[0][1];
            temp2 = shape_[1][2];
            shape_[0][1] = shape_[1][0];
            shape_[1][2] = temp;
            temp = shape_[2][1];
            shape_[2][1] = temp2;
            shape_[1][0] = temp;
        }
        else if(size_ == 4) {
            if(shape_[0][1] == true){
                for(int i = 0; i < MAX_SIZE; i++){
                    for(int j = 0; j < MAX_SIZE; j++){
                        shape_[i][j] = false;
                    }
                }
                shape_[1][0] = true;
                shape_[1][1] = true;
                shape_[1][2] = true;
                shape_[1][3] = true;
            }
            else if(shape_[1][0] == true){
                for(int i = 0; i < MAX_SIZE; i++){
                    for(int j = 0; j < MAX_SIZE; j++){
                        shape_[i][j] = false;
                    }
                }
                shape_[0][1] = true;
                shape_[1][1] = true;
                shape_[2][1] = true;
                shape_[3][1] = true;
            }
        }
        
        return *this;
    }
    // 반시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
    Tetromino Tetromino::rotatedCCW() {
            if (size_ == 3) {
                temp = shape_[0][0];
                temp2 = shape_[0][2];
                shape_[0][2] = shape_[2][2];
                shape_[0][0] = temp2;
                temp2 = shape_[2][0];
                shape_[2][0] = temp;
                shape_[2][2] = temp2;

                //
                temp = shape_[0][1];
                temp2 = shape_[1][2];
                shape_[1][2] = shape_[2][1];
                shape_[0][1] = temp2;
                temp2 = shape_[1][0];
                shape_[1][0] = temp;
                shape_[2][1] = temp2;
            }

            else if(size_ == 4) {
                if(shape_[0][1] == true){
                    for(int i = 0; i < MAX_SIZE; i++){
                        for(int j = 0; j < MAX_SIZE; j++){
                            shape_[i][j] = false;
                        }
                    }
                    shape_[1][0] = true;
                    shape_[1][1] = true;
                    shape_[1][2] = true;
                    shape_[1][3] = true;
                }
                else if(shape_[1][0] == true){
                    for(int i = 0; i < MAX_SIZE; i++){
                        for(int j = 0; j < MAX_SIZE; j++){
                            shape_[i][j] = false;
                        }
                    }
                    shape_[0][1] = true;
                    shape_[1][1] = true;
                    shape_[2][1] = true;
                    shape_[3][1] = true;
                }
            }
                return *this;
    }

    // 화면의 x, y 위치에 s 문자열로  테트로미노를 그린다
    void Tetromino::drawAt(std::string s, int x, int y) {
        console::draw(x, y, s);
    }