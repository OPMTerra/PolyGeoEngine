#pragma once
#include <iostream>
#include <string>
#include "Shape.h"
using namespace std;

class Circle : public Shape {
    private:
    int x_coords;
    int y_coords;
    int radius;

    public:
    Circle(int x, int y, int r) : x_coords(x), y_coords(y), radius(r) {}

    string toSVG() override {
        return "<circle cx=\"" + to_string(x_coords) + "\" cy=\"" + to_string(y_coords) + "\" r=\"" + to_string(radius) + "\" fill=\"red\"/>";
    }
};