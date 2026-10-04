# Super Mario 3D Land: Decompilation and Native Port

Project brief for an autonomous long-running model (gpt-6.1-sol in Codex).
Status: **APPROVED** by the owner on 2026-10-01. The canonical copy lives in the repository at `project/BRIEF.md`.

---

## 1. Objective

Reconstruct Super Mario 3D Land (EU) as C++ source that compiles back to the original executable byte for byte. Then use that source to build a native port that runs in a web browser, with features the original hardware could not offer.

The project exists to show what a frontier model can do with little supervision over a long horizon. Credibility is the whole point. Every claim of progress must be backed by a mechanical check that anyone can rerun. A fast result that cannot be verified is worth nothing here.

## 2. Definition of done, by milestone

Each milestone has a mechanical exit check. Never report a milestone as reached unless its check passes.

**Priorities, owner decision 2026-10-02:** the browser demo first, then 100% byte-exact. The factory and the dot keep matching while the root builds the port.

Port milestones (owner, 2026-10-02), shown on the dashboard next to percent decompiled and percent recompiled:
1. A static recompiler for `code.bin` builds natively and reaches the first frame's GPU command stream, checked against Azahar.
2. Rendering.
3. Input.
4. Audio.
5. World 1-1.
6. Browser build.

The root reports each milestone's state and evidence, and the recompiled byte count, in `.integrator/port_status.json`.

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
- **Historical provenance reset (2026-10-01; policy superseded 2026-10-03).** RE-Pepper's `lib/CtrSDK`, `lib/NintendoWare` and `lib/sead` submodules were removed and all upstream match ranks reset to `U`. This historical reset remains recorded, but the owner now permits leaked or unclear-provenance material as reference and committed source under rule 7. Existing and newly imported functions count only after the canonical checker and integrator accept them.
- **Toolchain runs natively on macOS.** armcc is a 32-bit Windows binary. It runs through wibo's macOS build (decompals/wibo 1.2.0) under Rosetta 2. Compilers download on first use into `data/compilers/` (ignored by git). Docker is not needed. Do not use Docker: Rosetta inside Linux containers cannot run wibo.
- **Toolchain commands.** Run `. ./development_environment.sh` first. Then `python make.py eu` builds, `python tools/check.py <symbol>` checks one function and updates its rank, `python tools/diff.py <symbol>` shows the assembly diff, and `python tools/progress.py` prints the totals.
- **How a function enters the build.** The linker only pulls in functions whose rank in `map.csv` is not `U`. To work on a function, set its rank to `M`, build, then let `check.py` set the true rank. Ranks: `O` matching, `m` minor mismatch, `M` mismatch, `U` not attempted. Only `check.py` may set `O`. These rank edits stay in a local scratch build. Only the integrator commits rank changes (rule 13).
- **First project-verified match:** `nn::os::detail::ConvertSvcToLibraryPriority` at 0x0010766C, in `lib/CtrSDK/sources/os_Priority.cpp`, compiled with ARMCC 4.0 build 902 (the module's configured compiler). This proves the build, diff and check pipeline end to end.
- **Compiler build for game code is unresolved.** RE-Pepper's config uses ARMCC 4.1 build 791 for game code. decomp.me preset 8 ("Super Mario 3D Land") uses 4.1 build 894. Nothing has tested this yet, because `Game` and `lib/al` are disabled in `data/config.json` and their headers depend on the removed sead. Settling it is M0.
- **Older project:** RedPepper (https://github.com/3dsdecomp/RedPepper). Its `Source/` and `Library/` material is permitted as reference and committed source under rule 7. Pin the source commit, record its stated provenance, and verify layouts and function bytes against this project's target and checker.
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
7. **Owner decision, 2026-10-03:** leaked Nintendo material and material of unclear provenance are permitted both as references and as committed source, including the removed RE-Pepper CtrSDK, NintendoWare and sead submodules and 3dsdecomp sead, nnsdk, RedPepper-Headers, LibMessageStudio and RedPepper (`Source/` and `Library/`). Pin every source to a commit, credit it, and record its own stated provenance without claiming this project is clean-room. The owner explicitly said "pls use leaked nintendo material" and "okay it's fine to push! drop 'clean' from the readme and get after it". Full owner quotations and pinned sources are recorded in `project/decisions.md`. Rule 3 still prohibits inline assembly, `__asm`, `.s` files and byte arrays standing in for game functions; its existing low-level exemption is unchanged. Rule 9 still forbids game data. Only `tools/check.py` and the integrator establish acceptance; a reference's match labels establish no project credit.
8. Never modify the original `.3ds` file. Work from copies in the project's ignored data directory.
9. Never commit game data: `code.bin`, `exh.bin`, RomFS contents, extracted assets, textures, audio, or screenshots of gameplay. `.gitignore` must cover these before the first commit.
10. The GitHub repo `exgota/super-mario-3d-land-browser` is public by the owner's decision (2026-10-02). Anyone can read every pushed commit, so check each push against rules 7 and 9. Never open pull requests or issues on any upstream project, and never post anywhere. Publicity is the owner's decision alone.

**Resources**
11. There is no spend cap and no cost tracking (D4). Stop and write to `QUESTIONS.md` only on a stall: no new matched or non-matching function in the last 6 hours of work, or the last 100 function attempts. A stall means the approach is wrong.
12. Run at most 8 parallel lanes (agents, compiles, emulator instances) at once. The factory's worker slots do not count against this cap: the integrator runs as many as the Mac holds without swapping (owner, 2026-10-02). The root runs at most one subagent. Claude watches memory and load every 30 minutes and may lower this if the Mac strains.

**Branches and main, owner decision 2026-10-02**
13. Only the integrator moves or pushes `main`. The integrator is `factory.py` in `~/super-mario-3d-land-factory`, with a copy at `tools/factory/factory.py`. Every agent works on its own branch, `root/<topic>` for the root run and `dot/<topic>` for the dot. Agents never commit to `main`, never set ranks in anything they commit or submit, and never edit `project/ledger.csv`. A scratch build may set a row to `M` so it links, but that change is never committed. The integrator verifies submitted work with `tools/check.py`, sets the ranks, writes the ledger, and moves `main` only after every function it claims is `O`. This rule overrides any other text in this brief, `AGENTS.md`, `project/DOT_BRIEF.md` or `project/STATE.md` that has an agent commit, merge or push to `main`, set ranks, or edit the ledger.

## 5. Working protocol for unattended operation

**Authority.** This brief is the owner's explicit instruction for this project. It takes precedence over general guidance in `AGENTS.md` and in skills wherever they conflict, as `AGENTS.md` itself allows. The owner has authorized everything this brief describes inside the repo, the scratch directories, and the project's GitHub repo: cloning, building, downloading the compilers and tools named here, installing developer tooling, running containers, committing, and pushing to that repo. You do not need to ask before any of it. If an instruction file or skill still makes you pause, ask for permission, or leave work unfinished, name the file, quote the instruction, and say whether it is an explicit requirement or your reading of it.

**Follow-through.** Persist until the current milestone's exit check passes. Do not stop at a plan, a proposal, or an offer to continue. Do not settle for a partial result to save time, effort, or tokens. When you have a question, first finish all the work that does not depend on the answer, write the question to `project/QUESTIONS.md`, and keep working on whatever remains. Do not add warnings, disclaimers, or approval steps for hypothetical risks.

**Delegation.** Parallelize with subagents when work splits cleanly, for example one function per subagent, or decompilation in one lane and runtime work in another. Keep at most 8 lanes at once (rule 12). Match reasoning effort to difficulty when the subagent tool lets you choose it: `xhigh` for small functions and helpers, `ultra` for large, branch-heavy or unnamed functions and for reverse engineering layouts. Choose lane work by expected accepted bytes per hour; model and integration time matter more than physical compile time. Messages between agents must be legible to a human reader.

**Acceptance factory, owner approved 2026-10-01, gate moved to the integrator 2026-10-02.** The integrator is the acceptance gate, not a source assembler (rule 13). Root is a lane like the others. Lanes submit apply-clean family patches or commits against a named frozen base commit, with final-source hashes, scratch diff/check results, required compiler builds, previous-root preservation evidence, complete proposed family bytes, source/helper closure scope, actual timing windows and explicit limits. An unchanged-source proposal supplies its exact source hash and an explicitly empty patch. Evidence must describe the submitted final source, rather than an earlier fragment. The integrator applies the submitted patch unchanged, commits the intended source inputs, runs the canonical project build and checker, and checks every previously accepted root and every actual canonical definition before accepting any new root. A conflict, source assembly requirement or preservation failure returns the proposal to its owning lane. The integrator does not rewrite or compose lane fragments. Only the project checker may set O; an unsuccessful batch adds no exact credit.

For each batch, assign exactly one owning lane to each shared header and translation-unit family. Record the ownership and base in STATE before work begins. A patch may not modify another lane's family. Keep ABI, data identity, table ownership and boundary questions in a separate evidence queue, with independently grounded proposals and separate evidence commits where required by rule 2. Hold only their dependent patches, so unrelated ready families can proceed. Existing acceptance, source-attribution, attempt-cap and hourly clean-link requirements remain unchanged. Runtime investigation and early port work continue without a new time box.

**Small-function factory and integrator, owner decision 2026-10-02.** Claude runs the integrator in `~/super-mario-3d-land-factory`. Its workers own new unmatched functions smaller than 0x200 bytes. Below 0x100 bytes, tier 1 workers run gpt-6.1-sol at high effort with a 10-minute timeout, and a job tier 1 misses moves to tier 2, gpt-6.1-sol at xhigh effort with 15 minutes. Functions of 0x100 to 0x1FF bytes (256 to 511) start at gpt-6.1-sol xhigh with a 20-minute timeout in one reserved slot, so they run alongside the small queue (owner, 2026-10-02). A miss at the last tier goes to the Pro relay. Luna medium alone takes every tier 1 job under 32 bytes, single or group (owner, 2026-10-02, replacing the earlier single-functions-only rule); Tier 1 Sol jobs take 32 to 255 bytes in ascending body-size order (owner, 2026-10-03), including the class slot when it falls back to Sol work. Jobs of 192 to 255 bytes at tier 1 wait until smaller eligible jobs have been leased or exhausted; the reserved s1 lane remains separate. Tier 2 retries retain their existing policy. Other lanes select functions of 0x200 bytes or more, blocked functions, packets, ABI/data/table identity and runtime work. Do not start a new U function below 0x200 bytes or edit `Game/backup/src/Factory/`. Factory workers add one self-contained source file per job and accept only committed project-built C++ under the same checker provenance rules.

The integrator takes every queued factory proposal that touches different files into one candidate, one build and one round of checks (owner, 2026-10-02); a proposal that misses is set aside and the rest go through a fresh cycle. It commits each candidate on a scratch branch, runs `tools/check.py` there, and moves `main` in one compare-and-swap step only after every function in the candidate is `O` and the final commit's `Game`, `lib` and `data/config.json` trees equal the checked ones. Before it pushes `main`, a clean build must link and the regression pass must show no loss: the full-image byte compare (`make.py eu --split`) checks every `O` row's bytes at original addresses, a row it reports different is demoted only when `tools/check.py` confirms, and when the compare cannot reach every `O` row the full `tools/check.py` pass decides. One full `tools/check.py` pass a day runs beside the compare as an audit (owner, 2026-10-02). A proposal or branch whose objects define a symbol that another object already defines is rejected (owner, 2026-10-02). The `factory` branch is retired; it stays at the commit where it was merged into `main` and nothing works on it. Never rewrite pushed history. All hard rules and the lane cap remain unchanged.

A periodic full check that demotes rows a single check had passed demotes them to `M`, logs them and keeps running. The integrator halts if one full check demotes more than 5 rows or any row that is not a constructor or destructor. It halts everything if `main` moves by anything but the integrator, the build breaks, or free disk drops under 5 GB, and halts the factory alone after 50 jobs in a row match nothing. A provider error, such as a model at capacity, is retried at the same tier after a back-off; it is never a miss, never goes to the Pro relay, and does not count toward the 50 (owner, 2026-10-02). Runs that end in an error or a timeout still record the tokens they reported before they stopped, and the dashboard shows those as failed spend. If a worker, the root or the relay gets a response saying a prompt was flagged for usage policy, everything halts and Claude tells the owner. It runs as many worker slots as the Mac holds without swapping: it starts at 4, drops one when the Mac swaps, and adds one after 15 quiet minutes.

**Root, owner decisions 2026-10-02.** A fresh root thread runs gpt-6.1-sol at ultra effort. After the branch it had in progress at 04:35, it works only on the port runtime (Section 2, port milestones). Class layout questions go to the Pro relay. It works on `root/<topic>` branches and submits each finished branch with `python ~/super-mario-3d-land-factory/factory.py submit --branch root/<topic> --claims <symbols> --summary "<text>"`. Claims list the functions the branch makes exact; `--nonmatching` lists functions it adds as non-matching. A branch that changes no build input (`Game`, `lib`, `data/config.json`, `map.csv`) and claims nothing lands at once. Every other branch rides the next periodic full check, every 45 minutes: one clean build and regression pass serves main and every submission waiting at that time, so submissions never hold up factory matches (owner, 2026-10-02). The integrator accepts a branch only if every claim is `O` and no exact function regresses; if a combined check fails, it checks main alone to tell main's rows from the branches', and branches that failed together ride alone from then on. Its verdict lands in `.integrator/results/<name>.json`. `project/STATE.md` belongs to the root, so a submitted branch's version of it always wins a merge. A map change (a symbol name or a boundary) goes in its own evidence commit with the evidence in `project/decisions.md` (rule 2). Changes to the oracle files of rule 2 wait for the operator's review. Root runs at most one subagent at a time.

**Browser test lifecycle, owner decision 2026-10-02.** Browser tests run without visible windows by default. Keep at most one visible project preview across project-owned browser processes and persistent automation sessions. Close test browsers after success, failure or timeout, including detached or persistent sessions that outlive their launcher. Before starting another session, identify and clean up abandoned project-owned sessions. Keep the owner's personal Chrome session separate and untouched.

Adopting this policy must preserve active tests. Verify a session's current project ownership and inactivity before closing an existing session; an old label or age alone does not prove abandonment. The runtime lane owns launcher and persistent-session enforcement and must demonstrate success, failure and timeout cleanup and the one-visible-preview limit before reporting enforcement complete. A canonical policy entry alone is not evidence that existing processes or launchers comply. This change authorizes no factory restart, model experiment or broader system cleanup.

**Source quality, owner decision 2026-10-02.** The end goal is source anyone can read, change and extend, not only exact bytes. Every factory job, match or miss, writes `docs/facts/<address>.md`: the class guess and its confidence, the struct offsets touched with size and inferred type, the callees with inferred signatures, the vtable slots, and the data references. Facts hold offsets, types, signatures, symbol names and addresses only: no disassembly listings, no raw bytes, no dumps of data tables. The integrator lands each batch's facts as a docs-only commit. Cleanup branches (`cleanup/<topic>`) rewrite exact Factory code into real headers and class files with one struct definition, named fields, member functions with real names, no raw offset casts and no duplicated externs. They may change `Game/backup/src/Factory/`, ride the full check alone, and are rejected if the set of `O` rows after the commit does not contain the set before. A cleanup job uses one slot at most. A shiftability scan counts raw image address literals in the source under `Game/`; pass is zero. The dashboard shows the class-file share of exact bytes, the class-file share of the last 24 hours' matched bytes, and the literal count.

**Class mode, owner decision 2026-10-02.** One factory slot runs class-mode jobs: the worker gets a named class's header and class file and writes each unmatched function of that class's translation unit as a member function in the class file, never in the Factory directory. Tiers follow size as elsewhere (Luna medium under 32 bytes, Sol high to 255, Sol xhigh with 20 minutes to 511). One job at a time per class file; the integrator lands a proposal only if main's copies of the class file and header are still the ones the job started from, and checks the claim and every exact function in the objects the change compiles into: the class file and, when the header changed, every object whose dependency file names the header (owner, 2026-10-02). A header change that reaches more than 240 exact functions rides the periodic full check instead. The first class is Bubble.

**Queue scoring and measurement.** Rank matching candidates roughly by `(chance of canonical acceptance * complete family bytes + downstream bytes unlocked) / (investigation + implementation + intake hours)`. Record the assumptions rather than treating estimated scores as observed throughput. Prefer whole families and shared prerequisites with independent evidence, rather than selecting solely by function count or difficulty. Before starting, check active local/dot work and Pro reservations. Keep the four-unsuccessful-form cap, packet history and other work before requeue.

Record aggregate accepted complete bytes/hour and per-lane accepted bytes/hour in STATE with the measurement interval, attributed acceptance records and elapsed wall time. Do not sum overlapping ledger minutes as elapsed time. Keep the six-column ledger unchanged. Distinguish ready inventory carried into a batch from newly investigated work, and report after-throughput only once canonical acceptance has occurred. Include integration/build/preservation time in end-to-end throughput. Zero new accepted bytes is zero throughput, not unmeasured success.

**Writing.** Daily reports and decision entries use short plain paragraphs, active voice, and the real numbers. No filler phrases and no concluding summaries.

You will lose context. Threads compact, sessions end, and the machine restarts. The repository is your memory.

**Files you keep in the repo, under `project/`:**
- `project/STATE.md`: read this first in every session. Current milestone, what is in flight, the next 3 tasks, known blockers. Keep it under 150 lines.
- `project/ledger.csv`: one row per function attempt, written only by the integrator (rule 13). Columns: timestamp, function address, symbol, outcome (matched / nonmatching / abandoned), attempts, minutes.
- `project/decisions.md`: append only. Every judgement call that a reviewer might question, with the evidence behind it.
- `project/blocked.md`: functions or tasks you gave up on, with the reason and what would unblock them.
- `project/daily/<YYYY-MM-DD>.md`: a short report of what changed, the numbers, and anything the owner should know. Write one at the end of every working day.

**Session loop:**
1. Read `project/STATE.md` and the newest daily report.
2. Pick the next task from STATE. Prefer tasks that unblock other tasks.
3. Work in small verified steps on your own branch (rule 13). Commit after every candidate function or meaningful tooling change, push your branch to `origin` at least once an hour, and submit finished work to the integrator. Only the integrator pushes `main`, and only after a clean build (`python make.py eu -ca`) links: a MacBook audit clones `origin/main` every few hours, rebuilds with no shared state, and re-checks every `O` function. Use clear commit messages. Never rewrite pushed history.
4. Update STATE and, when relevant, the decision log on your branch, then continue. The integrator writes the ledger.

**When to stop and wait for the owner (write the question to `project/QUESTIONS.md`, then continue on other work if any remains):**
- Any action outside this repo and the scratch directories.
- Any hard rule that seems to need an exception.
- The same blocker has stopped every remaining task.

**Honesty in reports.** Report what the tools measured, not what you expect. If a number went down, say so and say why. If you are unsure whether something works, say it is unverified.

## 6. Phase plan

### Phase 0: environment (exit M0)
Done on 2026-10-01: repository, native toolchain, clean-room baseline, first clean match (see Section 3).
Remaining:
1. Use and extend shared sead headers from pinned reference sources under rule 7. Reconcile target layouts and preserve accepted functions; if wholesale library restoration conflicts, migrate type by type. Never copy an existing shared type into a translation-unit-local definition.
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
- Use pinned RedPepper `Source/` and `Library/` code and names under rule 7. Verify target layouts and all proposed matches with the project build and checker.
- Re-run the full progress check at least once a day and record the numbers in the daily report.

### Hard-function packets for GPT-6 Pro
A Codex relay thread (gpt-6.1-sol, medium effort) sends questions to GPT-6 Pro in the owner's ChatGPT project "decompile", one chat per question. Class layout questions belong to this relay (owner, 2026-10-02). Order: layout questions first (`.integrator/pro_queue/layout/`), then factory tier 2 failures, largest first (`.integrator/pro_queue/failures/`, named so a reverse sort puts the largest first). There is no message cap. On any error the relay puts the item back in the queue and moves on. Answers go to `.integrator/pro_answers/layout/` for root and `.integrator/pro_answers/failures/` for the factory, which reopens the failed job once at tier 2 with the answer attached. Pro answers are proposals; only the integrator's checker run sets O.

The earlier packets in `project/pro_requests/` and graded responses in `project/pro_responses/` stay as references. Every reservation from those rounds is released.

### External lane: the owner's dot
The owner may run an OpenAI dot (a cloud agent) on the hardest functions. Its brief is `project/DOT_BRIEF.md`. The dot pushes only to branches named `dot/<topic>` and never to `main`.
- The dot "frog" owns unmatched functions of 0x200 (512) bytes or more, largest first (owner, 2026-10-02).
- Claude checks for new `dot/*` branches at every 30-minute check and submits each finished branch to the integrator with the claims from its `project/dot_reports/` entry.
- Treat dot work as proposals for the integrator. It takes only source, headers, names and notes. It never takes the dot's edits to `data/ver/eu/map.csv`, `project/ledger.csv`, ranks, or tools.
- A dot function counts only after it passes the integrator's `tools/check.py` run here, built by this repository's build step, under the same hard rules.
- Credit it in commit messages ("from dot/<topic>") so its share is traceable.
- Before root matches a function to unblock a layout, it checks open `dot/*` branches and `project/dot_reports/` to avoid working on the same function concurrently.

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

- **D1. Repository base. DECIDED:** RE-Pepper's history in the public repo `exgota/super-mario-3d-land-browser`, credited. The 2026-10-03 owner decision permits the formerly removed libraries and other pinned references under rule 7. Never contribute upstream.
- **D2. Matching standard. DECIDED:** the final claim is 100% byte-exact (M2). Non-matching code is allowed as an intermediate state and never counts toward the headline number.
- **D3. When the port starts. DECIDED:** both an early demo and the pure claim. After the pilot, start the runtime in parallel, use static recompilation (ARM to C) as scaffolding for code not yet decompiled, and replace it as matches land. The scoreboard always shows "% decompiled" and "% recompiled" separately. Decompiled functions replace recompiled ones by address: the port links a function's decompiled code wherever main has it at rank `O`, and its recompiled translation everywhere else (owner, 2026-10-02). The early demo states the real split. The pure claim waits until recompiled code reaches 0%.
- **D4. Budget. DECIDED:** no spend cap and no cost tracking. The stall rule (rule 11) replaces it.
- **D5. Attempt cap per function. DECIDED:** 8 compile-diff iterations during the pilot, then park the function in `blocked.md`. Revisit the cap in the pilot report.
- **D6. Outer loop. DECIDED:** Codex `/goal` mode. Treat every session as one that might end unexpectedly: keep `project/STATE.md` current enough that a fresh session can resume from it alone.
- **D7. Enhancement order. DECIDED:** gamepad, widescreen, free mouse camera, high frame rate, then WebXR stereoscopic mode last as the showpiece.
- **D8. Review cadence. DECIDED:** no stop for review at any point. Claude reviews the transcript, commits and reports every 30 minutes and may steer through queued messages or edits to this brief. The model stops only on the conditions in Section 5.
