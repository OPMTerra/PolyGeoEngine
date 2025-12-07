filename = "test_load.txt"

print(f"Generating {filename}...")

with open(filename, "w") as f:
    for i in range(400000) :
        f.write(f"ADD RECT {i} {i} 10 10\n")

    f.write("QUIT\n")

print("Done! You can now pipe this file into your engine.")