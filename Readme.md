# Super Mario 3D Land decompilation

A model-driven matching decompilation of Super Mario 3D Land (EU), with a browser port to follow. See `project/BRIEF.md`.

![Decompilation progress](docs/progress.svg)

## How it is built

GPT Astra orchestrates the project. It runs the integrator and the work queue, reviews submissions and decides what each lane works on. Workers run GPT-6.1 Sol and GPT-6 Luna on unmatched functions, and GPT-6 Pro takes the hardest ones. A separate root task builds the browser port.

Only the integrator moves `main`. A function counts as matched only when `tools/check.py` reproduces its original bytes exactly, so every number above can be rerun.

This repository does not contain game data. Building it requires your own copy of the game.

## Credit

Built on [RE-Pepper](https://github.com/RE-Pepper/RE-Pepper) by its contributors, which forks [RedPepper](https://github.com/3dsdecomp/RedPepper) by the 3dsdecomp contributors. The build system, tools and map come from RE-Pepper. Reference sources include RE-Pepper's [CtrSDK](https://github.com/RE-Pepper/ctrsdk), [NintendoWare](https://github.com/RE-Pepper/NintendoWare_ctr) and [sead](https://github.com/RE-Pepper/sead_ctr), and 3dsdecomp's [sead](https://github.com/3dsdecomp/sead), [nnsdk](https://github.com/3dsdecomp/nnsdk), [RedPepper-Headers](https://github.com/3dsdecomp/RedPepper-Headers), [LibMessageStudio](https://github.com/3dsdecomp/LibMessageStudio) and [RedPepper](https://github.com/3dsdecomp/RedPepper). Some derive from leaked material; pinned revisions and each source's stated provenance are recorded in `project/decisions.md`.

Assembly diffing uses [asm-differ](https://github.com/simonlindholm/asm-differ). armcc runs through [wibo](https://github.com/decompals/wibo).

Upstream's readme states that the toolchain is MIT licensed and the decompiled code is CC0. Upstream's `License` file is the GNU General Public License v3 and is kept unchanged.
