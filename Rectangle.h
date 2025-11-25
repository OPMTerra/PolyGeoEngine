#pragma once
#include <iostream>
#include <string>
#include "Shape.h"
using namespace std;

class Rectangle : public Shape {
    private:
    int x_coords;
    int y_coords;
    int width;
    int height;

    public:
    Rectangle(int x, int y, int w, int h) : x_coords(x), y_coords(y), width(w), height(h) {}

    string toSVG() override {
        return "<rect x=\"" + to_string(x_coords) + "\" y=\"" + to_string(y_coords) + "\" width=\"" + to_string(width) + "\" height=\"" + to_string(height) + "\" fill=\"blue\"/>";
    }
};