# gGPU - GSP GPU Abstraction Layer

gGPU is an abstraction over platform and vendor specific Graphics APIs.

Available backends are as follows:

- **Vulkan** - default backend for Windows, Android and Linux/UNIX
- **Metal** - default backend for macOS, iOS and other Apple platforms
- **WebGL** - default for web (and electron-style) deployments
- **SGL (GSP Sofware Graphics Layer)** - a software-implemented graphics API intended as a fallback for compatibility reasons, or for very simple applications to be run outside of a typical desktop environment.

