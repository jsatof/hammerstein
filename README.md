# Overview
This project aims to simply experiment with the Hammerstein-style analysis ideas expressed in this [paper](https://aes2.org/publications/elibrary-page/?id=18042). Its findings represent a sine-sweep that can identify extra harmonic info from a nonlinear signal system.

# Project Goals
- Custom implementation to build understanding
- A visualization tool for nonlinear system analysis

# Installation
Requirements are SCons and a C compiler.
Running the following commands will build the project:
```bash
$ mkdir build
$ scons -j`nproc`
```
Then an executable will be built in `build/hammerstein`.
