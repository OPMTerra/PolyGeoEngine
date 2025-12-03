#pragma once
#include <iostream>
#include <string>
#include "Shape.h"
#include "Arena.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"

using namespace std;

class ShapeFactory {
    public:
    static Shape* CreateShape(const string& type, istream& input, Arena& arena) {
        void* mem = nullptr;
        Shape* newShape = nullptr;
        if (type == "CIRCLE") {
            int x, y, r;
            input >> x >> y >> r;
            mem = arena.alloc(sizeof(Circle));
            if (mem) newShape =  new (mem) Circle(x, y, r);
        }
        else if (type == "RECT") {
            int x, y, w, h;
            input >> x >> y >> w >> h;
            mem = arena.alloc(sizeof(Rectangle));
            if (mem) newShape = new (mem) Rectangle(x, y, w, h);
        }
        else if (type == "TRIANGLE") {
            int x1, y1, x2, y2, x3, y3;
            input >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
            mem = arena.alloc(sizeof(Triangle));
            if (mem) newShape = new (mem) Triangle(x1, y1, x2, y2, x3, y3);
        }
        else {
            cout << "Unknown shape type" << endl;
            //clear the input buffer to prevent infinite loops on bad input
            string junk;
            getline(input, junk);
        }
        if (mem == nullptr && (type == "CIRCLE" || type == "RECT" || type == "TRIANGLE")) {
            cout << "ERROR: Arena Memory full!" << endl;
        }
        return newShape;
    }
};