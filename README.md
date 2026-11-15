# Project Setup and Initial Implementation

We found an OpenGL template on learnopengl.com and configured the GLFW, GLAD, and GLM libraries to establish the core framework for our water simulation.

We also set up the following files:

- **`main.cpp`**: OpenGL starter code from the template, providing window initialization and the main rendering loop.
- **`caustics.cpp`**: Worley noise generation for approximating underwater caustic patterns.
- **`mesh.cpp`**: Square mesh generation with adjustable dimensions and resolutions, allowing us to test the water simulation on meshes of varying sizes and densities.
