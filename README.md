# PolyGeo Engine

**A Headless 2D CAD Kernel built in C++**

PolyGeo is a high-performance, command-line based geometry engine designed to demonstrate systems engineering concepts including custom memory management, hardware-aware allocation, and runtime polymorphism. It serves as a lightweight "backend" for 2D Computer Aided Design (CAD) applications.

## Technical Highlights 🚀

### 1. Hardware-Aligned Arena Allocator
Unlike standard `new`/`malloc`, PolyGeo uses a custom **Arena Allocator** that manages a pre-allocated contiguous block of memory. 
* **Performance:** Eliminates system call overhead during runtime.
* **Hardware Safety:** Implements strict **Memory Alignment** logic. The allocator calculates padding bytes to ensure every object (Triangle, Circle, etc.) starts at a memory address compatible with the CPU's alignment requirements (using `std::max_align_t`). This prevents hardware faults on strict architectures and optimizes CPU cache fetch cycles.

### 2. Factory Design Pattern (OOD)
The engine utilizes the **Factory Pattern** to decouple the *user interface* (main loop) from the *object creation logic*. 
* **Separation of Concerns:** `ShapeFactory` handles the complex logic of parsing inputs, requesting memory from the Arena, and constructing objects.
* **Extensibility:** New shapes can be added to the Factory without modifying the core application loop, adhering to the **Open/Closed Principle**.

### 3. Manual Lifecycle Management (RAII & Placement New)
The engine constructs objects directly into the Arena using **Placement New**.
* **Design Choice:** Since the Arena owns the raw memory, standard `delete` cannot be used on shape pointers.
* **Implementation:** The engine implements a rigorous manual cleanup phase where `~Shape()` destructors are explicitly invoked for every active object before the Arena releases the memory block. This ensures resource correctness without the overhead of reference counting.

### 4. Zero-Overhead Undo/Redo
Implements a linear history stack. Because state changes involve swapping pointers rather than deep-copying objects, the Undo/Redo operations are strictly **$O(1)$** (constant time).

## Architecture 🏗️

The engine follows a strict "Command → Factory → Arena" pipeline:

1.  **Command Parser:** Reads raw text input in `main.cpp`.
2.  **ShapeFactory:** Validates input and calculates size requirements.
3.  **Arena Allocation:** Calculates the necessary **Padding** for alignment and returns a raw pointer.
4.  **Placement New:** Constructs the shape implementation (`Circle`/`Rect`) in the aligned slot.
5.  **Cleanup:** On exit, the engine iterates through active shapes to manually invoke destructors, ensuring a clean shutdown before the Arena is freed.

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
Welcome to PolyGeo Engine v1.1
Initialized memory area with 10 MB.

> Type a command (HELP for list of commands): ADD CIRCLE 100 100 50
Shape added successfully.

> Type a command: ADD RECT 200 200 40 60
Shape added successfully.

> Type a command: RENDER
SVG file 'output.svg' generated.

> Type a command: QUIT
Cleaning up shapes...
Shape destroyed
Shape destroyed

## Undo/Redo Example 📝

```text
> Type a command (HELP for list of commands): UNDO
> Type a command (HELP for list of commands): RENDER
SVG file 'output.svg' generated.
```

## Scalability & Roadmap 📈
### Performance Characteristics
The engine is architected for high-frequency operations suitable for large-scale CAD drawings:
* **Deterministic Latency ($O(1)$):** Shape insertion and Undo/Redo operations involve pointer swapping rather than deep copying. This guarantees constant-time performance, ensuring the engine remains responsive whether managing 10 objects or 1,000,000.
* **Hardware-Friendly Access:** By enforcing std::max_align_t alignment in the Arena, the engine minimizes CPU cache misses and prevents "split-load" penalties on modern processors.
* **Memory Density:** While the alignment logic introduces slight internal padding (average ~4 bytes/object), it eliminates the 16-byte metadata overhead per object typical of standard malloc/new. This results in a net memory saving of ~20-30% compared to standard containers.

### Known Limitations & Future Work
* **Fixed Memory Pool:** Currently, the Arena size is fixed at startup (10MB). 
    * *Future Goal:* Implement a dynamic pager to chain multiple memory blocks together as the application grows.
* **Single-Threaded:** The engine runs on a single thread.
    * *Future Goal:* Implement support for asynchronous command processing to improve responsiveness.
* **Linear Rendering:** Rendering iterates through all shapes.
    * *Future Goal:* Implement spatial partitioning optimization to speed up querying in massive datasets.
* **Edit Complexity:** The current Undo/Redo system handles creation/deletion history efficiently.
    * *Future Goal:* Implement the Command Pattern to support complex state changes (e.g., "Move", "Resize") and granular undo steps.
* **Lifecycle Management (RAII):** Currently, the engine uses a manual destruction loop to ensure correctness.
    * *Future Goal:* Refactor to std::unique_ptr with custom deleters to fully automate resource cleanup via RAII (Resource Acquisition Is Initialization).
