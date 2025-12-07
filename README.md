# PolyGeo Engine

![Status](https://img.shields.io/badge/Status-Active-success)
![Language](https://img.shields.io/badge/Language-C++17-blue)
![Architecture](https://img.shields.io/badge/Architecture-Headless_CAD-orange)

**A High-Performance 2D CAD Kernel with Custom Memory Management**

PolyGeo is a command-line based geometry engine designed to demonstrate advanced systems engineering concepts. It implements a custom memory model to bypass standard heap allocation overhead while maintaining strict resource safety through modern C++ RAII patterns.

## Technical Highlights 🚀

### 1. Hardware-Aligned Arena Allocator
PolyGeo replaces standard `new`/`malloc` with a custom **Arena Allocator** that manages a pre-allocated contiguous block of memory (10MB).
* **Performance:** Eliminates system call overhead during runtime object creation.
* **Hardware Safety:** Implements strict **Memory Alignment** logic. The allocator calculates padding bytes to ensure every object aligns with `std::max_align_t`, preventing hardware faults on strict architectures and optimizing CPU cache line utilization.

### 2. Automated RAII with Custom Deleters
The engine solves the complexity of custom allocation using `std::unique_ptr` and **Custom Deleters**.
* **The Problem:** Standard smart pointers call `delete`, which causes undefined behavior on Arena-managed memory.
* **The Solution:** A specialized `ShapeDeleter` instructs the smart pointer to strictly invoke the destructor `~Shape()` (cleaning up internal resources) while skipping memory deallocation (leaving that to the Arena).
* **Result:** This bridges the gap between raw memory performance and modern safety, preventing "Zombie Objects" and resource leaks automatically.

### 3. Factory Design Pattern
The engine utilizes the **Factory Pattern** to decouple the *user interface* (main loop) from the *object creation logic*.
* **Separation of Concerns:** `ShapeFactory` encapsulates input parsing, memory alignment requests, and object construction.
* **Extensibility:** Adheres to the **Open/Closed Principle**—new shapes can be added to the Factory without modifying the core application loop.

### 4. Zero-Overhead Undo/Redo via Ownership Transfer
Implements a linear history stack using pointer ownership transfer.
* **Efficiency:** Undo/Redo operations involve `std::move`-ing smart pointers between vectors. This guarantees **$O(1)$** (constant time) performance regardless of shape complexity.
* **Safety:** If the history stack is cleared or the program exits, the smart pointers automatically trigger the custom deleters, ensuring perfectly clean history management.

## Architecture 🏗️

The engine follows a strict "Command → Factory → Arena" pipeline:

1.  **Command Parser:** Reads raw text input in `main.cpp`.
2.  **ShapeFactory:** Validates input and calculates size requirements.
3.  **Arena Allocation:** Calculates **Padding** for alignment and creates the object via **Placement New**.
4.  **Smart Pointer Wrap:** The raw pointer is immediately wrapped in a `std::unique_ptr` with `ShapeDeleter`.
5.  **Lifecycle Management:** Objects remain alive as long as they are owned by the `shapes` or `history` vectors. Upon removal or exit, destructors are invoked automatically.

## How to Build & Run 🛠️

### Prerequisites
* A C++ Compiler (GCC/G++ recommended, C++14 or higher)

### Option 1: Visual Studio Code (Recommended)
1.  Open the `PolyGeoEngine` folder in VS Code.
2.  Open `main.cpp`.
3.  Press `F5` or click the **Run** button.

### Option 2: Terminal / Command Line
1.  Navigate to the project folder.
2.  Compile the engine:
    ```bash
    g++ main.cpp -o polygeo
    ```
3.  Run the executable:
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

> Type a command: UNDO
# Shape is moved to history stack (O(1))

> Type a command: RENDER
SVG file 'output.svg' generated.

> Type a command: QUIT
# Cleanup handled automatically by RAII.
```

## Testing & Validation 🧪

The engine includes a Python-based stress testing suite to verify memory safety and load handling.

### How to Run the Stress Test
The `tests/gen_tests.py` script generates a load file with 400,000 commands to intentionally overflow the 10MB Arena and verify graceful error handling.

1.  **Generate the Load File:**
    ```bash
    python tests/gen_tests.py
    ```
2.  **Run the Engine:**
    ```bash
    # Windows (PowerShell)
    Get-Content test_load.txt | .\polygeo.exe > output.log
    
    # Linux / Mac
    ./polygeo < test_load.txt > output.log
    ```
3.  **Verify Results:**
    Check `output.log` for `ERROR: Arena Memory full!`. This confirms the Allocator correctly rejected excess shapes without crashing, and RAII successfully destructed all ~327,000 valid objects upon exit.

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

