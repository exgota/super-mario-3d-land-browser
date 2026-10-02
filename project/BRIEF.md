# Super Mario 3D Land: Decompilation and Native Port

Project brief for an autonomous long-running model (gpt-6.1-sol in Codex).
Status: **APPROVED** by the owner on 2026-10-01. The canonical copy lives in the repository at `project/BRIEF.md`.

---

## 1. Objective

Reconstruct Super Mario 3D Land (EU) as C++ source that compiles back to the original executable byte for byte. Then use that source to build a native port that runs in a web browser, with features the original hardware could not offer.

The project exists to show what a frontier model can do with little supervision over a long horizon. Credibility is the whole point. Every claim of progress must be backed by a mechanical check that anyone can rerun. A fast result that cannot be verified is worth nothing here.

## 2. Definition of done, by milestone

Each milestone has a mechanical exit check. Never report a milestone as reached unless its check passes.

The final goal is M2: 100% of the game's code byte-exact. The project is not finished until M2 passes, even after the port milestones are done. Port work (M3 to M5) runs alongside matching and never replaces it.

| # | Milestone | Exit check |
|---|-----------|------------|
| M0 | Toolchain settled | Game code (`Game/`, `lib/al`) compiles, and the compiler build for game code is proven by at least 3 game functions that match byte-exact under it and fail under the alternative |
| M1 | Pilot: 50 functions attempted | Ledger shows match rate and wall time per function |
| M2 | Matching decompilation, complete | Every game function is byte-exact and the complete linked `code.bin` has the original EU sha256. Per-function counts alone do not finish this milestone |
| M3 | Port runtime: one level in the browser | World 1-1 loads from the owner's own dump, plays to the goal pole, and passes differential replay (Section 7) |
| M4 | Full game in the browser | Every world and special world completes under differential replay. Saves persist across reloads |
| M5 | Enhancements | Free mouse camera, widescreen, 120 fps with interpolation, gamepad support, WebXR stereoscopic mode (see D7) |

## 3. Established facts (verified 2026-10-01)

- **Dump:** `/Users/exgota/Downloads/Super Mario 3D Land EU (Rev 2).3ds`. The owner dumped it from their own 3DS. It is decrypted (NoCrypto flag on every partition). Title id 0004000000053F00, product code CTR-P-AREP, NCCH version 2.
- **Executable:** `.code` in ExeFS is LZ-compressed (backward LZ). Decompressed, it is 3,096,576 bytes, sha256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. This exactly matches RE-Pepper's `eu` version hash.
- **Do not use** `0004000000054000 Super Mario 3D Land (CTR-P-AREE) (v0.3.0) (U).legit.cia` in Downloads. It is the US build and is encrypted.
- **Repository:** `/Users/exgota/super-mario-3d-land-browser`, pushed to the public GitHub repo `exgota/super-mario-3d-land-browser`. It is RE-Pepper (https://github.com/RE-Pepper/RE-Pepper) with full history, remote `upstream`. RE-Pepper's toolchain is MIT and its decompiled code is CC0. The map (`data/ver/eu/map.csv`) has 28,043 rows, of which 18,055 are functions and 1,538 are named.
- **Clean-room baseline (done 2026-10-01).** RE-Pepper's `lib/CtrSDK`, `lib/NintendoWare` and `lib/sead` submodules were removed. Their own READMEs say they were referenced from a debug binary taken from a discarded studio hard drive, or from a library of unclear origin. Every function upstream had marked matched was in that code, so all ranks were reset to `U`. The project's matched count starts at zero, and every match from here on is clean.
- **Toolchain runs natively on macOS.** armcc is a 32-bit Windows binary. It runs through wibo's macOS build (decompals/wibo 1.2.0) under Rosetta 2. Compilers download on first use into `data/compilers/` (ignored by git). Docker is not needed. Do not use Docker: Rosetta inside Linux containers cannot run wibo.
- **Toolchain commands.** Run `. ./development_environment.sh` first. Then `python make.py eu` builds, `python tools/check.py <symbol>` checks one function and updates its rank, `python tools/diff.py <symbol>` shows the assembly diff, and `python tools/progress.py` prints the totals.
- **How a function enters the build.** The linker only pulls in functions whose rank in `map.csv` is not `U`. To work on a function, set its rank to `M`, build, then let `check.py` set the true rank. Ranks: `O` matching, `m` minor mismatch, `M` mismatch, `U` not attempted. Only `check.py` may set `O`.
- **First clean match:** `nn::os::detail::ConvertSvcToLibraryPriority` at 0x0010766C, in `lib/CtrSDK/sources/os_Priority.cpp`, compiled with ARMCC 4.0 build 902 (the module's configured compiler). This proves the build, diff and check pipeline end to end.
- **Compiler build for game code is unresolved.** RE-Pepper's config uses ARMCC 4.1 build 791 for game code. decomp.me preset 8 ("Super Mario 3D Land") uses 4.1 build 894. Nothing has tested this yet, because `Game` and `lib/al` are disabled in `data/config.json` and their headers depend on the removed sead. Settling it is M0.
- **Older project:** RedPepper (https://github.com/3dsdecomp/RedPepper). Stale since May 2025. Its `Source/` game code may be used as a reference for names and class layouts after verifying them against the binary. Its `Library/` submodules have the same unclean provenance as the removed ones and are off limits.
- **Upstream tool quirks:** `make.py --split` has a typo (`True7`) and will crash. `progress.py` counts all 28,043 map rows as "Total Functions". Fix tool bugs when they block you, in separate commits.
- **Machine:** Mac mini, Apple M4, 16 GB RAM, about 19 GB of free disk. Sleep is disabled. Clean `build/` before disk gets tight.

## 4. Hard rules

These rules hold for the whole project. If a rule blocks progress, log the blocker and move to other work. Do not bend the rule.

**Integrity of the oracle**
1. A function counts as matched only when the project's own check tool reports a byte-exact match. Never record a match on your own judgement.
2. Never edit the target binary, the version hashes, the function boundaries in `map.csv`, the differ, or the progress scripts to make something match. If a boundary in `map.csv` is genuinely wrong, fix it in a separate commit with evidence in the decision log.
3. No inline assembly, `__asm` blocks, `.s` files, or byte arrays standing in for game functions. Only the low-level SDK and runtime code that RE-Pepper already handles in assembly is exempt.
4. Do not change global compiler flags to rescue a single function. Per-file or per-module flags are allowed when the decision log records why.
5. A match from `tools/check.py --object` counts only when the object is the armcc output of committed C++ source in this repository, built by the project's own build step. Never check a hand-made, assembled, or edited object.
6. "Non-matching" is a legitimate state: functionally correct C++ that does not match byte for byte. Mark it with the project's NonMatching convention and keep it separate from matched counts in every report.

**Legal and data hygiene**
7. Never use leaked Nintendo source code, leaked SDKs, or symbols that come from leaks. Clean-room means the dump, public documentation, and public community work only. If an input's provenance is unclear, leave it out and log it. Never restore the removed RE-Pepper submodules (CtrSDK, NintendoWare, sead), and never use RedPepper's `Library/` submodules, the 3dsdecomp ctrsdk or sead repositories, or anything derived from them. You may use open-ead/sead (the Switch reconstruction) as a reference for sead's API shape after checking its README for provenance. The 3DS implementation itself must come from the game binary.
8. Never modify the original `.3ds` file. Work from copies in the project's ignored data directory.
9. Never commit game data: `code.bin`, `exh.bin`, RomFS contents, extracted assets, textures, audio, or screenshots of gameplay. `.gitignore` must cover these before the first commit.
10. The GitHub repo `exgota/super-mario-3d-land-browser` is public by the owner's decision (2026-10-02). Anyone can read every pushed commit, so check each push against rules 7 and 9. Never open pull requests or issues on any upstream project, and never post anywhere. Publicity is the owner's decision alone.

**Resources**
11. There is no spend cap and no cost tracking (D4). Stop and write to `QUESTIONS.md` only on a stall: no new matched or non-matching function in the last 6 hours of work, or the last 100 function attempts. A stall means the approach is wrong.
12. Run at most 8 parallel lanes (agents, compiles, emulator instances) at once. Claude watches memory and load every 30 minutes and may lower this if the Mac strains.

## 5. Working protocol for unattended operation

**Authority.** This brief is the owner's explicit instruction for this project. It takes precedence over general guidance in `AGENTS.md` and in skills wherever they conflict, as `AGENTS.md` itself allows. The owner has authorized everything this brief describes inside the repo, the scratch directories, and the project's GitHub repo: cloning, building, downloading the compilers and tools named here, installing developer tooling, running containers, committing, and pushing to that repo. You do not need to ask before any of it. If an instruction file or skill still makes you pause, ask for permission, or leave work unfinished, name the file, quote the instruction, and say whether it is an explicit requirement or your reading of it.

**Follow-through.** Persist until the current milestone's exit check passes. Do not stop at a plan, a proposal, or an offer to continue. Do not settle for a partial result to save time, effort, or tokens. When you have a question, first finish all the work that does not depend on the answer, write the question to `project/QUESTIONS.md`, and keep working on whatever remains. Do not add warnings, disclaimers, or approval steps for hypothetical risks.

**Delegation.** Parallelize with subagents when work splits cleanly, for example one function per subagent, or decompilation in one lane and runtime work in another. Keep at most 8 lanes at once (rule 12). Match reasoning effort to difficulty when the subagent tool lets you choose it: `xhigh` for small functions and helpers, `ultra` for large, branch-heavy or unnamed functions and for reverse engineering layouts. Choose lane work by expected accepted bytes per hour; model and integration time matter more than physical compile time. Messages between agents must be legible to a human reader.

**Acceptance factory, owner approved 2026-10-01.** Root is an acceptance gate, not a source assembler. Lanes submit apply-clean family patches or commits against a named frozen base commit, with final-source hashes, scratch diff/check results, required compiler builds, previous-root preservation evidence, complete proposed family bytes, source/helper closure scope, actual timing windows and explicit limits. An unchanged-source proposal supplies its exact source hash and an explicitly empty patch. Evidence must describe the submitted final source, rather than an earlier fragment. Root applies the submitted patch unchanged, commits the intended source inputs, runs the canonical project build and checker, and checks every previously accepted root and every actual canonical definition before accepting any new root. A conflict, source assembly requirement or preservation failure returns the proposal to its owning lane. Root does not rewrite or compose lane fragments. Only the project checker may set O; an unsuccessful batch adds no exact credit.

For each batch, assign exactly one owning lane to each shared header and translation-unit family. Record the ownership and base in STATE before work begins. A patch may not modify another lane's family. Keep ABI, data identity, table ownership and boundary questions in a separate evidence queue, with independently grounded proposals and separate evidence commits where required by rule 2. Hold only their dependent patches, so unrelated ready families can proceed. Existing acceptance, clean-room, attempt-cap and hourly clean-link requirements remain unchanged. Runtime investigation and early port work continue without a new time box.

**Small-function factory, delegated operating decision 2026-10-02.** Claude manages branch `factory` in the adjacent integration worktree under the owner's project-management authority. The factory owns new unmatched functions smaller than 0x100 bytes. Local lanes finish already in-flight work, then select functions of 0x100 bytes or more, blocked functions, packets, ABI/data/table identity and runtime work. Do not start a new U function below 0x100 bytes or edit `Game/backup/src/Factory/`. Factory workers add one self-contained source file per job and accept only committed project-built C++ under the same checker provenance rules.

At each hourly push, merge `factory` with `git merge --no-edit factory` before the normal fresh clean build and full-map preservation check. The normal gate is the only required intake step; no per-function write-ups or ledger rows are required for factory commits. The installed address-keyed CSV merge driver combines distinct rows and leaves real same-row conflicts visible. If the gate finds a regression, publish the audited checkpoint without that merge and record the regressed symbols in `project/blocked.md`. Never rewrite pushed history. All hard rules and the lane cap remain unchanged.

**Queue scoring and measurement.** Rank matching candidates roughly by `(chance of canonical acceptance * complete family bytes + downstream bytes unlocked) / (investigation + implementation + intake hours)`. Record the assumptions rather than treating estimated scores as observed throughput. Prefer whole families and shared prerequisites with independent evidence, rather than selecting solely by function count or difficulty. Before starting, check active local/dot work and Pro reservations. Keep the four-unsuccessful-form cap, packet history and other work before requeue.

Record aggregate accepted complete bytes/hour and per-lane accepted bytes/hour in STATE with the measurement interval, attributed acceptance records and elapsed wall time. Do not sum overlapping ledger minutes as elapsed time. Keep the six-column ledger unchanged. Distinguish ready inventory carried into a batch from newly investigated work, and report after-throughput only once canonical acceptance has occurred. Include integration/build/preservation time in end-to-end throughput. Zero new accepted bytes is zero throughput, not unmeasured success.

**Writing.** Daily reports and decision entries use short plain paragraphs, active voice, and the real numbers. No filler phrases and no concluding summaries.

You will lose context. Threads compact, sessions end, and the machine restarts. The repository is your memory.

**Files you keep in the repo, under `project/`:**
- `project/STATE.md`: read this first in every session. Current milestone, what is in flight, the next 3 tasks, known blockers. Keep it under 150 lines.
- `project/ledger.csv`: one row per function attempt. Columns: timestamp, function address, symbol, outcome (matched / nonmatching / abandoned), attempts, minutes.
- `project/decisions.md`: append only. Every judgement call that a reviewer might question, with the evidence behind it.
- `project/blocked.md`: functions or tasks you gave up on, with the reason and what would unblock them.
- `project/daily/<YYYY-MM-DD>.md`: a short report of what changed, the numbers, and anything the owner should know. Write one at the end of every working day.

**Session loop:**
1. Read `project/STATE.md` and the newest daily report.
2. Pick the next task from STATE. Prefer tasks that unblock other tasks.
3. Work in small verified steps. Commit after every matched function or meaningful tooling change. Push to `origin` at least once an hour. Before each push, confirm that a clean build (`python make.py eu -ca`) links: a MacBook audit clones `origin/main` every few hours, rebuilds with no shared state, and re-checks every `O` function. Use clear commit messages. Never rewrite pushed history.
4. Update the ledger, STATE, and when relevant the decision log, then continue.

**When to stop and wait for the owner (write the question to `project/QUESTIONS.md`, then continue on other work if any remains):**
- Any action outside this repo and the scratch directories.
- Any hard rule that seems to need an exception.
- The same blocker has stopped every remaining task.

**Honesty in reports.** Report what the tools measured, not what you expect. If a number went down, say so and say why. If you are unsure whether something works, say it is unverified.

## 6. Phase plan

### Phase 0: environment (exit M0)
Done on 2026-10-01: repository, native toolchain, clean-room baseline, first clean match (see Section 3).
Remaining:
1. Write clean sead headers in a new `lib/sead` module, starting with what `Game/` and `lib/al` include. Reconstruct layouts from the binary. Use open-ead/sead only for API shape.
2. Enable `lib/al` and `Game` in `data/config.json` one file at a time until both compile.
3. Pick at least 3 small game functions. Match them under 4.1 build 791, then rebuild under 4.1 build 894 (set `"compiler"` in `data/config.json`). Record which build matches in `project/decisions.md`. If both match, keep looking for a function that tells them apart.

### Phase 1: pilot (exit M1)
1. Choose 50 unmatched game functions: 20 small leaf functions, 20 medium functions, and 10 large or branch-heavy ones.
2. Per function, iterate: read the assembly, write C++, compile, diff, revise. Cap attempts at the limit in D5.
3. Write `project/pilot_report.md`: match rate per size class, minutes per function, the failure modes you saw, and a projected duration for all 18,055 functions.
4. Commit the report and continue straight into Phase 2. Do not stop for review. Claude reviews the report and the run from outside.

### Phase 2: matching at scale (exit M2)
- Score work by expected accepted complete bytes per hour, including downstream bytes unlocked and intake time. Prefer coherent families and shared prerequisites; use small, medium and large functions when their estimated acceptance yield warrants them.
- Recover class layouts, vtables and names as you go. A good name is worth more than a fast match because later functions depend on it.
- Use RedPepper's `Source/` game code and names as references (never its `Library/`), and verify every name against the binary.
- Re-run the full progress check at least once a day and record the numbers in the daily report.

### Hard-function packets for GPT-6 Pro
The owner re-enabled the relay on 2026-10-01 and approved a small measured test before scaling. The owner's ChatGPT reads this public repository through its GitHub connector. Claude sends a committed packet path; Pro fetches it and any committed headers/sources from origin. A packet is available to Pro only after an audited push. The owner reports about 200 Pro messages per week. Claude manages the small initial test, then scaling based on measured results.
- When a function reaches the attempt cap, or a lane is stuck on it, write `project/pro_requests/<address>.md` and commit it. Include the mangled symbol and address range, target disassembly with literal pool, best C++, current diff, attempted forms, compiler build and flags. Packets on origin may reference committed headers/sources instead of inlining them. Keep packets self-contained through those references and under about 600 lines.
- Claude saves answers to `project/pro_responses/<address>.md` as uncommitted proposals. Root grades them with priority: inspect the proposal, commit the intended C++/header inputs, build through this repository and run `tools/check.py` under every acceptance rule. Scratch output alone adds no exact credit.
- Append the observed outcome to the response file: matched, closer N bytes, or no help. For a closer result, record both the measured remaining differences and the previous baseline so the improvement is clear. Commit the graded response, source and actual attempt records; only the checker may set O.
- The initial three test responses are graded: one exact root through a fallback, two partial improvements. Their reservations are released subject to normal requeue and overlap rules. All eight batch-two responses001E37D8,0030E678,001D1DA0,0016A11C,0024F344,00268EB0,001EA220 and00252B1C are now graded; no current Pro reservation remains. Other work and overlap checks precede capped requeues. Exception: frog already started0030E678 on dot/bomb-hei-control before its reservation; let that work continue. Grade Pro against frog's independently verified49-differing-byte proposal when it lands, and retain the closer valid source. Pro now supplies three ranked forms. Grade every supplied form in rank order, including fallbacks, and preserve prior accepted readers/callers. Other matching and runtime work continue.

### External lane: the owner's dot
The owner may run an OpenAI dot (a cloud agent) on the hardest functions. Its brief is `project/DOT_BRIEF.md`. The dot pushes only to branches named `dot/<topic>` and never to `main`.
- Check for new `dot/*` branches at least every few hours (`git fetch origin`).
- Treat dot work as proposals. Merge only source, headers, names and notes. Never take its edits to `data/ver/eu/map.csv`, `project/ledger.csv`, ranks, or tools without reviewing them as you would your own.
- A dot function counts only after it passes `tools/check.py` here, built by this repository's build step, under the same hard rules.
- Credit it in commit messages ("from dot/<topic>") so its share is traceable.
- Local lanes may start large functions of 0x200 bytes or more and functions in `project/blocked.md` or `project/pro_requests/` without waiting for the dot. Before choosing a target, check open `dot/*` branches and `project/dot_reports/` to avoid working on the same function concurrently. Keep the four-attempt cap: after four unsuccessful attempts in a working pass, write or update its packet in `project/pro_requests/`, preserve the attempt history, and move on. Return that function to the local queue after other work; the dot may also take it. The dot is one lane among several, with no exclusive hard-function ownership. Local matching, intake/verification and runtime work continue under the current lane cap.

### Phase 3: port runtime (exit M3, M4)
Scope depends on D3. In outline:
- Build the decompiled source with a modern compiler (clang to native for debugging, Emscripten to WebAssembly for the browser) next to the ARMCC matching build. The matching build stays the source of truth.
- Replace console services with platform layers: CTR-SDK calls (file system, HID input, timing, sound, saves), and the PICA200 GPU command stream translated to WebGPU. Keep these layers out of decompiled game files.
- Load assets at runtime from the owner's own dump. The port ships no game data.

### Phase 4: enhancements (exit M5)
Implement the list in D7, in the order given there. Each enhancement sits behind an option, and the default mode stays faithful to the original.

## 7. Differential replay (the port's oracle)

Determinism is the test. The same inputs must produce the same game state on the reference and on the port.

1. Record input sequences on the reference: the Azahar emulator for automation (check its movie recording feature) and the owner's real 3DS for the final demo footage.
2. At fixed frames, dump comparable state from both: the player's position and velocity, the player's state, the camera's position, the RNG state, the level timer, and coins.
3. Replay the same inputs in the port and compare frame by frame. The first divergent frame is the bug to fix.
4. Keep a regression suite of recorded inputs per level. A level counts as done only when its whole suite passes.

## 8. Decisions

- **D1. Repository base. DECIDED:** RE-Pepper's history in the public repo `exgota/super-mario-3d-land-browser`, credited, with the unclean libraries removed. Never contribute upstream.
- **D2. Matching standard. DECIDED:** the final claim is 100% byte-exact (M2). Non-matching code is allowed as an intermediate state and never counts toward the headline number.
- **D3. When the port starts. DECIDED:** both an early demo and the pure claim. After the pilot, start the runtime in parallel, use static recompilation (ARM to C) as scaffolding for code not yet decompiled, and replace it as matches land. The scoreboard always shows "% decompiled" and "% recompiled" separately. The early demo states the real split. The pure claim waits until recompiled code reaches 0%.
- **D4. Budget. DECIDED:** no spend cap and no cost tracking. The stall rule (rule 11) replaces it.
- **D5. Attempt cap per function. DECIDED:** 8 compile-diff iterations during the pilot, then park the function in `blocked.md`. Revisit the cap in the pilot report.
- **D6. Outer loop. DECIDED:** Codex `/goal` mode. Treat every session as one that might end unexpectedly: keep `project/STATE.md` current enough that a fresh session can resume from it alone.
- **D7. Enhancement order. DECIDED:** gamepad, widescreen, free mouse camera, high frame rate, then WebXR stereoscopic mode last as the showpiece.
- **D8. Review cadence. DECIDED:** no stop for review at any point. Claude reviews the transcript, commits and reports every 30 minutes and may steer through queued messages or edits to this brief. The model stops only on the conditions in Section 5.
