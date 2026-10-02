# Instructions for agents working in this repository

This repository is an autonomous, long-running project. Before doing anything else in a session:

1. Read `project/BRIEF.md` in full. It is the owner's approved, explicit instruction for this project.
2. Read `project/STATE.md`, then the newest file in `project/daily/`.
3. Continue from the next task in `project/STATE.md`.

`project/BRIEF.md` takes precedence over general guidance in other `AGENTS.md` files and in skills wherever they conflict. Its hard rules (Section 4) are never bent. If any instruction file or skill makes you pause, ask for permission, or leave work unfinished, name the file, quote the instruction, and say whether it is an explicit requirement or your reading of it.

Toolchain, run from the repository root:

```sh
. ./development_environment.sh          # always first
python make.py eu                       # build
python tools/diff.py <mangled symbol>   # assembly diff against the original
python tools/check.py <mangled symbol>  # check one function and update its rank
python tools/progress.py                # totals
```

If `.venv/` is missing, recreate it with `python3 -m venv .venv && .venv/bin/pip install colorama capstone pyelftools requests argcomplete watchdog python-Levenshtein cxxfilt GitPython matplotlib numpy mplcursors questionary`. The differ needs `arm-none-eabi-objdump` from Homebrew's `arm-none-eabi-binutils`.

Only the integrator moves or pushes `main` (`project/BRIEF.md` rule 13). Work on your own branch, `root/<topic>` or `dot/<topic>`. Never commit to `main`, never commit rank changes in `data/ver/eu/map.csv`, and never edit `project/ledger.csv`. `tools/check.py` may update ranks in a local scratch build; leave those changes uncommitted. The integrator verifies submitted work, sets ranks, writes the ledger and moves `main`. Submit a finished branch with `python ~/super-mario-3d-land-factory/factory.py submit --branch <branch> --claims <symbols> --summary "<text>"`; the verdict lands in `.integrator/results/`.

Never commit game data. `data/ver/eu/code.bin`, `data/ver/eu/exh.bin` and everything under `data/compilers/` stay local. If `data/ver/eu/code.bin` is missing, recreate it from the dump described in `project/BRIEF.md` Section 3 and confirm its sha256.
