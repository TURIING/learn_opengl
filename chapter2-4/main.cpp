//
// Created by TURIING on 2023/10/14.
//


#include <iostream>
#include "GLObject.h"

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT= 600;

int main() {
    auto gl = GLObject(SCR_WIDTH, SCR_HEIGHT);
    gl.loop();
}



