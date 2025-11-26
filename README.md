# PolyGeo Engine

**A Headless 2D CAD Kernel built in C++**

PolyGeo is a high-performance, command-line based geometry engine designed to demonstrate systems engineering concepts including custom memory management, runtime polymorphism, and serialization. It serves as a lightweight "backend" for 2D Computer Aided Design (CAD) applications.

## Technical Highlights 🚀

* **Custom Arena Allocator:** Replaces standard `new`/`delete` with a pre-allocated contiguous memory block. This eliminates system call overhead (making allocation significantly faster) and solves external fragmentation by packing objects linearly.
* **Zero-Overhead Undo/Redo:** Implements a rigorous linear history stack. Because state changes involve swapping pointers rather than deep-copying objects, the Undo/Redo operations are $O(1)$ (instantaneous) regardless of shape complexity.
* **Polymorphic Architecture:** Uses a virtual command interface to treat all geometric primitives (`Circle`, `Rect`) uniformly, allowing for easy extensibility.

## Architecture 🏗️

The engine follows a strict "Command → Allocate → Construct" pipeline to ensure memory safety and performance:

1.  **Command Parser:** Reads raw text input and validates syntax.
2.  **Arena Allocation:** Requests a block from the pre-allocated `Arena`. If successful, returns a raw pointer (`void*`).
3.  **Object Construction:** Uses **Placement New** to construct the specific shape (`Circle`/`Rect`) directly into the allocated memory slot.
4.  **Vector Storage:** Stores the pointer in the `active_shapes` vector for rendering or the `history_stack` for undo operations.

## How to Build & Run 🛠️

### Prerequisites
* A C++ Compiler (GCC/G++ recommended)
* Visual Studio Code with the [C/C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) (Recommended)

### Option 1: Visual Studio Code (Quick Start)
1.  Open the `PolyGeoEngine` folder in VS Code.
2.  Open `main.cpp`.
3.  Click the **Run / Play** button (▶️) in the top right corner (or press `F5`).
4.  Select **g++** from the compiler list if prompted.
5.  The engine will start in the internal terminal below.

### Option 2: Terminal / Command Line
1.  Open your terminal/command prompt.
2.  Navigate to the project folder.
3.  Run the following command to compile:
    ```bash
    g++ main.cpp -o polygeo
    ```
4.  Run the executable:
    * **Windows:** `.\polygeo.exe`
    * **Mac/Linux:** `./polygeo`

## Example Usage 📝

```text
Welcome to PolyGeo Engine v1.0
Memory Pool Initialized: 10MB

> Type a command (HELP for list of commands): ADD CIRCLE 100 100 50
> Type a command (HELP for list of commands): ADD RECT 200 200 40 60
> Type a command (HELP for list of commands): RENDER
SVG file 'output.svg' generated.

> Type a command (HELP for list of commands): UNDO
> Type a command (HELP for list of commands): RENDER
SVG file 'output.svg' generated.

## Undo/Redo Example 📝

```text
> Type a command (HELP for list of commands): UNDO
> Type a command (HELP for list of commands): RENDER
SVG file 'output.svg' generated.
```

## Scalability & Roadmap 📈
### Performance Characteristics
The engine is architected for high-frequency operations suitable for large-scale CAD drawings:
* **Time Complexity:** Shape insertion and Undo/Redo operations are strictly **$O(1)$** (constant time), ensuring consistent performance whether the scene contains 10 shapes or 1,000,000.
* **Memory Efficiency:** The Arena Allocator eliminates the 8-16 byte metadata overhead per object typical of standard `malloc`/`new`, resulting in ~30% denser memory storage.

### Known Limitations & Future Work
* **Fixed Memory Pool:** Currently, the Arena size is fixed at startup (10MB). 
    * *Future Goal:* Implement a dynamic pager to chain multiple memory blocks together as the application grows.
* **Single-Threaded:** The engine runs on a single thread.
    * *Future Goal:* Implement support for asynchronous command processing to improve responsiveness.
* **Linear Rendering:** Rendering iterates through all shapes.
    * *Future Goal:* Implement spatial partitioning optimization to speed up querying in massive datasets.
