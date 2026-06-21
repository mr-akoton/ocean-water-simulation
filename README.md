# Water Simulation

A procedural ocean renderer written in **C++17** and **OpenGL 4.6**. The project explores modern GPU-based water rendering using tessellation, procedural wave generation, dynamic lighting, and post-processing effects.

## Features

* Procedural ocean surface using multi-octave FBM waves
* Choppy wave displacement and analytic normal generation
* Hardware tessellation with distance-based LOD
* GPU frustum culling for efficient rendering
* Cubemap skybox with procedural sun
* Blinn-Phong lighting model
* Distance and height fog post-processing
* Real-time parameter editing with Dear ImGui
* Free-fly camera controls

## Dependencies

### Requirements

* C++17 compatible compiler
* OpenGL 4.6 compatible GPU
* GLFW3
* Make

### Linux Installation

**Debian / Ubuntu**

```bash
sudo apt install build-essential libglfw3-dev
```

**Arch Linux**

```bash
sudo pacman -S base-devel glfw
```

## Build & Run

```bash
git clone git@github.com:mr-akoton/water-simulation.git
cd water-simulation
make
./water-simulation
```

## Controls

| Key               | Action      |
| ----------------- | ----------- |
| W A S D           | Move        |
| Space / Shift     | Up / Down   |
| Left Mouse + Drag | Look around |
| T                 | Toggle UI   |
| Esc               | Quit        |

## Technical Highlights

* OpenGL 4.6 Direct State Access (DSA)
* Tessellation-based ocean mesh generation
* Distance-adaptive tessellation levels
* GPU-side frustum culling
* Runtime-configurable rendering parameters

## References

- [**Victor Gordan** — OpenGL Tutorial Series](https://www.youtube.com/watch?v=XpBGwZNyUh0&list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-)
- [**Acerola** — How Games Fake Water](https://youtu.be/PH9q0HNBjT4?si=kZ0-5nDRtUmomY7x)
- [**GPU Gems** — Effective Water Simulation from Physical Models](https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models)
- [**Wikipedia** — Blinn-Phong Reflection Model](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model)
