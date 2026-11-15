# Project Setup and Initial Implementation

We found an OpenGL template on learnopengl.com and configured the GLFW, GLAD, and GLM libraries to establish the core framework for our water simulation.

We also set up the following files:

- **`main.cpp`**: OpenGL starter code from the template, providing window initialization and the main rendering loop. Also followed along with learnopengl to get a triangle to render, mainly to get further acquainted with the workflow. This will be removed later for the rendering of our actual objects.
- **`caustics.cpp`**: Worley noise generation for approximating underwater caustic patterns.
- **`mesh.cpp`**: Square mesh generation with adjustable dimensions and resolutions, allowing us to test the water simulation on meshes of varying sizes and densities.

## Build and Run

Requires CMake and a C++17-compatible compiler.

From the project root:

**Build (all platforms):**

```bash
cmake -S . -B build
cmake --build build
```

**Run (macOS/Linux):**

```bash
./build/WaterSimulation
```

**Run (Windows with Visual Studio):**

```powershell
.\build\Debug\WaterSimulation.exe
```
