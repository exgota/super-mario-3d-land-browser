---
name: Super Mario 3D Land Local Browser Preview
description: The implemented visual system for a finite local startup preview.
colors:
  page: "#f4f2ec"
  ink: "#252723"
  secondary: "#62665f"
  line: "#d4d5cd"
  control: "#354f40"
  focus: "#876323"
  failure: "#8b3028"
  screen-well: "#171c18"
  screen-placeholder: "#c6cdc7"
  action-text: "#fff"
  action-hover: "#263e30"
  action-disabled: "#d5d9d1"
  action-disabled-text: "#5d6459"
  file-selector-hover: "#e8eae2"
  selection: "#c7d7cb"
  scrollbar: "#969d92"
typography:
  display:
    fontFamily: '"IBM Plex Sans", sans-serif'
    fontSize: "clamp(28px, 3.2vw, 44px)"
    fontWeight: 600
    lineHeight: 1.08
    letterSpacing: "-0.025em"
  display-narrow:
    fontFamily: '"IBM Plex Sans", sans-serif'
    fontSize: "28px"
    fontWeight: 600
    lineHeight: 1.08
    letterSpacing: "-0.025em"
  body:
    fontFamily: '"IBM Plex Sans", sans-serif'
    fontSize: "16px"
    fontWeight: 400
    lineHeight: 1.55
  label:
    fontFamily: '"IBM Plex Sans", sans-serif'
    fontSize: "14px"
    fontWeight: 600
  field:
    fontFamily: '"IBM Plex Sans", sans-serif'
    fontSize: "13px"
    fontWeight: 400
  status:
    fontFamily: '"IBM Plex Sans", sans-serif'
    fontSize: "13px"
    fontWeight: 400
    lineHeight: 1.55
  help:
    fontFamily: '"IBM Plex Sans", sans-serif'
    fontSize: "12px"
    fontWeight: 400
    lineHeight: 1.55
  caption:
    fontFamily: '"IBM Plex Sans", sans-serif'
    fontSize: "11px"
    fontWeight: 400
  placeholder:
    fontFamily: '"IBM Plex Sans", sans-serif'
    fontSize: "14px"
    fontWeight: 400
    lineHeight: 1.55
rounded:
  control: "4px"
spacing:
  compact: "8px"
  label-separation: "10px"
  control-inset: "12px"
  major-separation: "24px"
components:
  button-primary:
    backgroundColor: "{colors.control}"
    textColor: "{colors.action-text}"
    typography: "{typography.label}"
    rounded: "{rounded.control}"
    width: "100%"
  button-primary-hover:
    backgroundColor: "{colors.action-hover}"
  button-primary-disabled:
    backgroundColor: "{colors.action-disabled}"
    textColor: "{colors.action-disabled-text}"
  file-selector:
    backgroundColor: "transparent"
    textColor: "{colors.ink}"
    typography: "{typography.field}"
    rounded: "{rounded.control}"
    padding: "8px 12px"
  file-selector-hover:
    backgroundColor: "{colors.file-selector-hover}"
  screen-top:
    backgroundColor: "{colors.screen-well}"
    width: "100%"
  screen-bottom:
    backgroundColor: "{colors.screen-well}"
    width: "80%"
---

# Design System: Super Mario 3D Land Local Browser Preview

## Overview

**Creative North Star: "Broadcast Monitor Inspection Bench"**

A warm pale ground surrounds charcoal screen wells and a narrow operating column. Restrained green identifies the action. Quiet typography leaves the real game pixels as the focal material. This describes the implemented local preview, not a standing visual preference for the whole project.

Source authority is `runtime/port/browser/index.html`, `browser.css` and `BrowserCapturePage.mjs`, with the direction recorded in `project/browser_preview_surface.md` and `.impeccable/surfaces/runtime-port-browser-index-html.md`. The full review at `build/browser_submission_recovery/finish_review.md` says **ship** for the captured finite startup surface at **1440 × 1080** and **390 × 1000**. It supersedes the earlier recapture disposition without deleting it. No owner visual approval is claimed. Motion and keyboard execution remain unscored. The separate raw proofs are `build/browser_submission_gpu/result.json` and `build/browser_submission_recovery/result.json`; continuous browser gameplay, input, audio, World 1-1 and complete rank-O adapters remain separate gaps.

**Key Characteristics:**

- Actual pixels with faithful screen proportions.
- One owned-file control and one run or stop action.
- Flat surfaces, quiet supporting text and reserved status space.
- Controls first at narrow widths.

## Colors

The frontmatter records every source color used by this small surface. The palette combines warm neutrals with one green action and explicit focus and failure signals.

### Primary

- **Restrained Green Action** (`control`) fills the enabled action; **Deep Green Hover** (`action-hover`) marks hover.
- **Amber Focus** (`focus`) identifies keyboard focus; **Red Failure** (`failure`) identifies an alert.

### Neutral

- **Warm Pale Ground** (`page`) and **Charcoal Text** (`ink`) establish the interface field and main text.
- **Quiet Gray Text** (`secondary`) supports captions, instructions and status; **Pale Gray Divider** (`line`) separates the file control.
- **Charcoal Screen Well** (`screen-well`) and **Pale Screen Text** (`screen-placeholder`) describe the empty displays.
- **White Action Text** (`action-text`), **Inactive Gray Ground** (`action-disabled`) and **Inactive Gray Text** (`action-disabled-text`) distinguish enabled and disabled actions.
- **Pale File Hover** (`file-selector-hover`), **Pale Green Selection** (`selection`) and **Muted Gray Scrollbar** (`scrollbar`) complete the observed interaction states.

## Typography

**Display Font:** IBM Plex Sans, with sans-serif fallback.

**Body Font:** IBM Plex Sans, with sans-serif fallback.

Only Regular (400) and SemiBold (600) are shipped. Both are unmodified interface fonts, with pinned provenance and the SIL Open Font License in `runtime/port/browser/fonts/`. There is no separate display or monospace face.

The frontmatter supplies the exact type ramp. The heading uses the responsive display role and becomes the narrow display role at the single breakpoint. Its explicit line break disappears there. Introduction text uses body; file labels and actions use label; the native file input uses field; feedback uses status; instructions and the scope note use help; screen captions use caption. Paragraphs share the recorded body line height. The scope note is limited to 42ch on wide layouts and 65ch on narrow layouts.

**The Interface Font Rule.** Preserve the Regular and SemiBold interface faces; game text remains part of the captured pixels.

## Layout

The wide layout is a centered two-column grid: `minmax(0, 720px)` for the screen pair and `minmax(260px, 330px)` for controls. Its gap is `clamp(32px, 6vw, 96px)`, padding is `clamp(24px, 5vw, 72px)`, and minimum height is `100svh`. Both columns align vertically at the center.

At `max-width: 820px`, the grid becomes one `minmax(0, 600px)` column. Controls occupy the first row. Alignment moves to the top, the grid gap becomes 40px, and padding becomes `28px 22px 42px`. The title's bottom margin changes from 24px to 16px; the introduction's changes from 36px to 22px. The scope note's top margin changes from 22px to 10px.

The screen pair stacks with a 26px gap. The top display uses 5:3 geometry and a 400 × 240 canvas; the bottom uses 4:3 geometry and a 320 × 240 canvas. The bottom well is 80% of the top well's width. Captions sit 6px below each well. Feedback reserves 64px on wide layouts and 34px on narrow layouts, with an 18px top margin.

**The Screen Geometry Rule.** Preserve each canvas aspect ratio and the bottom screen's 80% width relationship.

## Elevation & Depth

There are no shadows. Tonal contrast separates page, controls and screen wells. Screens remain flat; the frame arrival changes brightness without translating, scaling or elevating the displays.

## Shapes

Screen wells have square corners and no decorative frame. The run action and native file-selector button share the recorded control radius. The file selector has a 1px divider-color border; the full file input has a 1px bottom divider. The primary action has no border. Captured canvases fill their wells and use pixelated image rendering.

## Components

### Buttons

The full-width primary action has a minimum height of 48px. It uses the label role, green ground and white text, with a `background 160ms ease-out` transition to its hover color. The disabled state replaces both colors and uses the default cursor. No distinct active-state styling is defined.

The same action reads **Run preview** at rest and **Stop preview** during a capture. A stop aborts the session and restores usable controls. Completion restores the run action only after output has been saved and shutdown acknowledged.

### Inputs / Fields

The native single-file control accepts `.3ds` files. It uses the field role, 12px vertical padding and the bottom divider. Its selector button uses transparent ground, the recorded radius, `8px 12px` padding and a 12px right margin. Hover changes the selector ground. The input is disabled while a capture is active and re-enabled after completion or failure.

Wrong-size rejection and stop/retry recovery are verified by the recovery result. These checks cover the recorded finite startup and do not establish continuous gameplay controls.

### Feedback

Status uses polite live announcements. Failure uses an alert, the failure color, an 8px top margin and wrapping at any word boundary. Changing the selected file or starting a run hides the prior alert. The completed recovery record retains old error text internally while the alert remains hidden; the review observed no completed-state alert.

### Screens

The empty top well centers **Your game appears here.** Receiving the two validated RGBA buffers reveals both canvases and hides the placeholder. The entire pair then receives one `frame-arrival` animation: brightness 0.92 to 1 over 240ms, with `cubic-bezier(0.16, 1, 0.3, 1)` easing.

At `prefers-reduced-motion: reduce`, that animation and the action's color transition are disabled. These are source-defined behaviors. The review inspected their endpoint and source, not motion footage or reduced-motion execution.

### Focus, Selection and Scrollbars

Every focus-visible element receives a 3px solid focus-color outline offset by 5px. Selection uses the selection ground and ink text. The body requests thin scrollbars with the scrollbar thumb color and page track color. Native rendering and support remain browser-dependent; the review did not execute keyboard navigation.

## Do's and Don'ts

### Do:

- **Do** preserve actual canvas pixels and the Screen Geometry Rule.
- **Do** keep operational state readable with the existing live status, alert and visible-focus treatments.
- **Do** document new execution claims separately from visual review.

### Don't:

- **Don't** ship game frames, captures or review screenshots as public assets; keep them in ignored `build/` evidence.
- **Don't** infer motion, keyboard execution or owner visual approval from the completed still captures.
- **Don't** describe this finite startup as playable World 1-1 or continuous browser gameplay.
