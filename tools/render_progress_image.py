#!/usr/bin/env python3
"""Render docs/progress.svg, the README progress image.

One box per function in address order, colored by its rank in data/ver/eu/map.csv.
Only rank O counts as matched. Run from the repository root: python tools/render_progress_image.py
"""
import csv
import datetime
import pathlib

ROOT = pathlib.Path(__file__).resolve().parent.parent
MAP = ROOT / "data/ver/eu/map.csv"
OUTPUT = ROOT / "docs/progress.svg"

COLUMNS = 160
CELL = 4
GAP = 1
MARGIN = 16
HEADER = 92
LEGEND = 34

CLASSES = {"O": "matched", "M": "nonmatching", "m": "nonmatching"}


def main():
    functions = []
    for row in list(csv.reader(open(MAP)))[1:]:
        if "f" in row[5].strip():
            functions.append((int(row[0], 16), int(row[2], 16) - int(row[0], 16), row[4].strip()))
    functions.sort()

    total_bytes = sum(size for _, size, _ in functions)
    matched_bytes = sum(size for _, size, rank in functions if rank == "O")
    matched = sum(1 for _, _, rank in functions if rank == "O")
    nonmatching = sum(1 for _, _, rank in functions if rank in ("M", "m"))
    percent = matched_bytes / total_bytes * 100

    pitch = CELL + GAP
    rows = (len(functions) + COLUMNS - 1) // COLUMNS
    grid_width = COLUMNS * pitch - GAP
    width = grid_width + 2 * MARGIN
    height = HEADER + rows * pitch - GAP + LEGEND + MARGIN
    bar_width = grid_width * matched_bytes / total_bytes

    cells = []
    for index, (_, _, rank) in enumerate(functions):
        css_class = CLASSES.get(rank)
        if css_class:
            x = MARGIN + (index % COLUMNS) * pitch
            y = HEADER + (index // COLUMNS) * pitch
            cells.append(f'<rect class="{css_class}" x="{x}" y="{y}" width="{CELL}" height="{CELL}"/>')

    full_rows, remainder = divmod(len(functions), COLUMNS)
    legend_y = HEADER + rows * pitch - GAP + 22
    date = datetime.date.today().isoformat()

    svg = f'''<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" role="img" aria-label="Decompilation progress: {percent:.2f}% of code bytes matched, {matched} of {len(functions)} functions">
<style>
  .surface {{ fill: #ffffff; }} .title {{ fill: #1f2328; }} .muted {{ fill: #59636e; }}
  .empty {{ fill: #e4e8e3; }} .matched {{ fill: #0ca30c; }} .nonmatching {{ fill: #e8a20c; }}
  text {{ font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif; }}
  .figure {{ font-family: ui-monospace, SFMono-Regular, Menlo, Consolas, monospace; font-variant-numeric: tabular-nums; }}
  @media (prefers-color-scheme: dark) {{
    .surface {{ fill: #0d1117; }} .title {{ fill: #e6edf3; }} .muted {{ fill: #9198a1; }}
    .empty {{ fill: #21262d; }} .matched {{ fill: #2ea043; }} .nonmatching {{ fill: #d29922; }}
  }}
</style>
<defs><pattern id="box" width="{pitch}" height="{pitch}" patternUnits="userSpaceOnUse" x="{MARGIN}" y="{HEADER}"><rect class="empty" width="{CELL}" height="{CELL}"/></pattern></defs>
<rect class="surface" width="{width}" height="{height}" rx="8"/>
<text class="title" x="{MARGIN}" y="34" font-size="22" font-weight="600"><tspan class="figure">{percent:.2f}%</tspan> of code bytes matched</text>
<text class="muted figure" x="{MARGIN + grid_width}" y="34" font-size="12" text-anchor="end">{matched_bytes:,} / {total_bytes:,} bytes</text>
<text class="muted" x="{MARGIN}" y="56" font-size="13"><tspan class="figure">{matched:,}</tspan> of <tspan class="figure">{len(functions):,}</tspan> functions byte-exact · EU Rev 2 · {date}</text>
<rect class="empty" x="{MARGIN}" y="68" width="{grid_width}" height="6" rx="3"/>
<rect class="matched" x="{MARGIN}" y="68" width="{max(bar_width, 3):.1f}" height="6" rx="3"/>
<rect fill="url(#box)" x="{MARGIN}" y="{HEADER}" width="{grid_width}" height="{full_rows * pitch - GAP}"/>
<rect fill="url(#box)" x="{MARGIN}" y="{HEADER + full_rows * pitch}" width="{remainder * pitch - GAP}" height="{CELL}"/>
{chr(10).join(cells)}
<rect class="matched" x="{MARGIN}" y="{legend_y - 9}" width="10" height="10" rx="2"/>
<text class="muted" x="{MARGIN + 16}" y="{legend_y}" font-size="12">Matched <tspan class="figure">{matched:,}</tspan></text>
<rect class="nonmatching" x="{MARGIN + 130}" y="{legend_y - 9}" width="10" height="10" rx="2"/>
<text class="muted" x="{MARGIN + 146}" y="{legend_y}" font-size="12">Nonmatching <tspan class="figure">{nonmatching:,}</tspan></text>
<rect class="empty" x="{MARGIN + 270}" y="{legend_y - 9}" width="10" height="10" rx="2"/>
<text class="muted" x="{MARGIN + 286}" y="{legend_y}" font-size="12">Not matched yet</text>
<text class="muted" x="{MARGIN + grid_width}" y="{legend_y}" font-size="12" text-anchor="end">One box per function, in address order</text>
</svg>
'''
    OUTPUT.parent.mkdir(exist_ok=True)
    OUTPUT.write_text(svg)
    print(f"{OUTPUT.relative_to(ROOT)}: {percent:.2f}% bytes, {matched}/{len(functions)} functions, {len(svg):,} bytes")


if __name__ == "__main__":
    main()
