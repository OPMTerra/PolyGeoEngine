#pragma once
#include <iostream>
using namespace std;

class Shape {
    public:

    virtual string toSVG () = 0;
    virtual ~Shape() {
        cout << "Shape destroyed" << endl;
    }
};