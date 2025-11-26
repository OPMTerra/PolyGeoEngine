#pragma once
#include <iostream>
#include <string>
#include "Shape.h"
using namespace std;

class Triangle : public Shape {
    private:
    int x1, y1;
    int x2, y2;
    int x3, y3;

    public:
    Triangle (int X1, int Y1, int X2, int Y2, int X3, int Y3)
    : x1(X1), y1(Y1), x2(X2), y2(Y2), x3(X3), y3(Y3) {}

    string toSVG() override {
        return "<polygon points=\"" + to_string(x1) + "," + to_string(y1) + " " + to_string(x2) + "," + to_string(y2) + " " + to_string(x3) + "," + to_string(y3) + "\" fill=\"yellow\"/>";
    }
};