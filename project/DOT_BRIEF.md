# Brief for the owner's dot: the hard functions

You are an external lane on a matching decompilation of Super Mario 3D Land (EU). A Codex run on the owner's Mac does the bulk of the matching. Your job is the hardest part: large, branch-heavy and unnamed functions that the main run parks.

Repository: `github.com/exgota/super-mario-3d-land-browser` (private). Read `AGENTS.md`, then `project/BRIEF.md` Sections 1 to 4, then `Guide.md`. The hard rules in `project/BRIEF.md` Section 4 bind you exactly as they bind the main run.

## Your lane

- Do all work on your own cloud computer. Do not start Codex tasks or ChatGPT Work tasks: those count against the owner's usage, and your own work does not.
- Work only on branches named `dot/<topic>`, for example `dot/fugumannen-init`. Never push to `main`.
- Commit only source (`Game/`, `lib/`), headers, and notes. Never commit changes to `data/ver/eu/map.csv`, `project/ledger.csv`, `project/STATE.md`, or anything under `tools/`. The main run owns those and re-verifies everything you send.
- You own the hard functions, so the owner's Codex usage goes to the easy ones. Your targets, in order: packets in `project/pro_requests/` that have no matching file in `project/pro_responses/` (each is a self-contained write-up of a function the main run got stuck on), the functions listed in `project/blocked.md`, then unmatched functions of 0x200 bytes or more, largest first. Fetch `main` before choosing. Skip only functions already ranked `O` (matched). Packet and blocked functions are usually ranked `M` or `m` because the main run attempted them; those are still yours.
- Push small and often. One class or one function family per branch.
- For each branch, add `project/dot_reports/<branch name>.md`: which functions you believe match, the `tools/check.py` output that shows it, and what you learned about layouts, names or callees that the main run can reuse.

## Game data

The owner uploads two files to you: `code.bin` and `exh.bin`. Place them at `data/ver/eu/code.bin` and `data/ver/eu/exh.bin`. Confirm that `sha256sum data/ver/eu/code.bin` prints `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64` before trusting any result. Both paths are gitignored. Never commit, upload or share them anywhere else.

## Toolchain

The compiler is ARMCC, a 32-bit Windows program. The build runs it through wibo, which needs x86_64 Linux or macOS.

```sh
python3 -m venv .venv && . .venv/bin/activate
pip install colorama capstone pyelftools requests argcomplete watchdog python-Levenshtein cxxfilt GitPython matplotlib numpy mplcursors questionary
sudo apt-get install -y binutils-arm-none-eabi        # the differ needs arm-none-eabi-objdump
export DEVKITARM=/usr                                   # so $DEVKITARM/bin/arm-none-eabi-objdump resolves
python3 make.py eu                                      # downloads compilers and wibo on first run
python3 tools/check.py <mangled symbol>                 # strict byte check; rank O only on an exact match
python3 tools/diff.py <mangled symbol>                  # assembly diff
```

Game code uses ARMCC 4.1 build 791. If your computer cannot run wibo (for example an ARM CPU), still do the reverse engineering: recover layouts, names, callees and draft C++. Say clearly in the report that nothing was compiled, and the main run will compile and check it.

## Honesty

A function matches only when `tools/check.py` on a build of committed source reports it. Report exactly what the tools measured. A clearly labelled partial result is useful. A claimed match that fails on the owner's Mac wastes everyone's time.
