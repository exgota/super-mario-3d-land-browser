# Saved-profile cold-boot replay

The stock recorder can boot from an ordinary copy of a previous capture's final on-disk user profile. This transfers files, not guest RAM. The recording creates its own initial snapshot and movie clock. Native playback must use that new recording's movie and initial snapshot together.

The completed checks at 1,200 and 1,800 presentations match all delivered input, GPU events and ticks, PICA payloads, final screen metadata, RGBA pixels, raw framebuffers and audio samples. Both native runs use zero interpreter/JIT fallbacks. The 1,800 endpoint still shows the opening cinematic. A save file's presence does not establish that saved progress was loaded or that World 1-1 was entered.

Keep profiles, movies, captures, pixels and generated code in ignored directories. Use the owner's local dump copy. Build the pinned Azahar provider as described in [BUILD.md](azahar_reference/BUILD.md), with the accepted capture-range dependency from [GAMEPLAY_CAPTURE_BOUNDS.md](GAMEPLAY_CAPTURE_BOUNDS.md). The family evidence records the actual isolated build inputs and limits. It does not claim a hermetic operating-system linker closure.

For a new recording, select an absent ignored output, an explicit controller script and an ordinary final profile from a completed stock capture. The recording-only options are:

```text
ROOT_PORT_RECORD_INITIAL_USER_STATE=<ordinary previous capture/user directory>
ROOT_PORT_INPUT_SCRIPT=<explicit seven-column controller script>
ROOT_PORT_CAPTURE_INPUTS=1
ROOT_PORT_CAPTURE_AUDIO=1
ROOT_PORT_CAPTURE_PRESENTATION=1200 or 1800
ROOT_PORT_CAPTURE_WALL_SECONDS=360 or 540
ROOT_PORT_CAPTURE_PICA_BYTES=536870912
```

Leave the recording base-clock override unset. Retain the recorder's actual clock. Remove inherited ROOT_PORT options before choosing this configuration. The provider copies the supplied profile into its own output user directory and records a fresh initial snapshot before CPU execution. Preserve the source profile and every original recording byte before and after playback.

The root-authored diagnostic script presses A at renderer frames 360, 500, 620, 740, 860 and 980. Each press lasts four frames; frame zero and all other intervals are neutral. Circle and touch remain zero. All thirteen states reach actual HID polls. This is a menu diagnostic, not a player-control witness.

Replay the completed original movie with its own snapshot through the native provider. The unchanged comparison is:

```sh
. ./development_environment.sh
python tools/static_recompiler/compare_movie_replay.py "$REFERENCE_CAPTURE" "$NATIVE_CAPTURE" \
  --movie "$REFERENCE_CAPTURE/input_movie.ctm" \
  --report "$NEW_REPORT" --preview-directory "$NEW_PREVIEW_DIRECTORY"
```

Each child has a finite deadline, aggregate PICA limit, per-file limit and monitored output/file/free-space limits. The measured checks used 1 GiB total output, 720,000 files, 128 MiB per file, a 5 GiB free-space floor and two-second polling. Outer child deadlines were 420 and 600 seconds. These monitored thresholds are not filesystem quotas. Incomplete outcomes or crossed bounds receive no replay credit. Runtime and source identities, complete whole-original capture seals and the observed endpoint images are recorded in [the family evidence](../../project/world_one_level_entry_evidence.md).

These two records are independent cold boots. Their base clocks differ. Their matching native playbacks do not prove a common cross-recording state prefix, save persistence after level progress, actor identity, completed game updates or the Section 7 state suite.
