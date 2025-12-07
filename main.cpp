#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include "Arena.h"
#include "ShapeFactory.h"
using namespace std;

void printHelp() {
    cout << "Available commands: \n";
    cout << "ADD CIRCLE x y radius - Adds a circle at (x, y) with the specified radius.\n";
    cout << "ADD RECT x y width height - Adds a rectangle at (x, y) with the specified width and height.\n";
    cout << "ADD TRIANGLE x1 y1 x2 y2 x3 y3 - Adds a traingle with the specified vertices.\n";
    cout << "RENDER - Generated an SVG file with the current shapes in active memory.\n";
    cout << "UNDO - Removes the last added shape from active memory.\n";
    cout << "REDO - Re-adds the last undone shape to the active memory.\n";
    cout << "HELP - Displays this help message.\n";
    cout << "QUIT - Exists the program.\n";
}

int main(){
    cout << "Welcome to the PolyGeoEngine! v1.1\n";
    Arena arena(1024 * 1024 * 10); // 10 MB block size
    cout << "Initialized memory area with 10 MB.\n";
    vector<Shape*> shapes;
    vector<Shape*> history;
    while (true) {
        cout << "> Type a command (HELP for list of commands): ";
        string command;
        cin >> command;
        if (command == "QUIT") break;
        else if (command == "ADD") {
            string type;
            cin >> type;

            Shape* s = ShapeFactory::CreateShape(type, cin, arena);
            if (s != nullptr) {
                shapes.push_back(s);

                for (Shape* orphan : history) {
                    orphan->~Shape();
                }

                history.clear();
                cout << "Shape added successfully.\n";
            }
        }
        else if (command == "RENDER") {
            ofstream svgfile("output.svg");
            svgfile << "<svg width=\"1000\" height=\"1000\" xmlns=\"http://www.w3.org/2000/svg\">\n";
            for (const auto& shape : shapes) {
                svgfile << shape->toSVG() << endl;
            }
            svgfile << "</svg>" << endl;
            svgfile.close();
            cout << "SVG file 'output.svg' generated.\n";
        }
        else if (command == "UNDO") {
            if (!shapes.empty()) {
                history.push_back(shapes.back());
                shapes.pop_back();
            }
            else cout << "No shapes to undo. \n";
        }
        else if (command == "REDO") {
            if (!history.empty()) {
                shapes.push_back(history.back());
                history.pop_back();
            }
            else cout << "No shapes to redo. \n";
        }
        else if (command == "HELP") {
            printHelp();
        }
        else {
            cout << "Invalid command. Type HELP for list of commands." << endl;
        }
    }
    cout << "Cleaning up shapes..." << endl;
    for (Shape* s : shapes) {
        s->~Shape();
    }
    return 0;
}