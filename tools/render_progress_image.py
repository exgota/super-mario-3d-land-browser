#!/usr/bin/env python3
"""Render docs/progress.svg, the README progress image.

One box per function, with area proportional to its byte size and color from
its rank in data/ver/eu/map.csv. Only rank O counts as matched.
Run from the repository root: python3 tools/render_progress_image.py
"""
import csv
import datetime
import pathlib
from bisect import bisect_right
from itertools import accumulate

ROOT = pathlib.Path(__file__).resolve().parent.parent
MAP = ROOT / "data/ver/eu/map.csv"
OUTPUT = ROOT / "docs/progress.svg"

TREEMAP_WIDTH = 992
TREEMAP_HEIGHT = 500
MARGIN = 16
HEADER = 92
LEGEND = 34

CLASSES = {"O": "matched", "M": "nonmatching", "m": "nonmatching"}


# Binary layout ported from streemap 0.1.0, src/lib.rs, under its MIT license.
# https://github.com/Speedy37/streemap-rs
# Copyright (c) 2021 Vincent Rouillé
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
def binary_rectangles(functions, width, height):
    """Port streemap's binary partition, preserving the supplied item order.

    Like decomp.dev's layout_units, normalize the longer side to one before
    laying out, then scale to pixels. bisect_right reproduces the crate's
    first cumulative sum strictly greater than the half-total target. The
    split precedes that item unless it is first, when one item goes left.
    Equal-sided rectangles split along y, as in the Rust implementation.
    """
    if not functions or any(size <= 0 for _, size, _ in functions):
        raise ValueError("The function map must contain positive byte sizes")
    sums = list(accumulate(size for _, size, _ in functions))
    aspect = width / height
    normalized_width = 1.0 if aspect > 1.0 else aspect
    normalized_height = 1.0 / aspect if aspect > 1.0 else 1.0
    scale_x = width / normalized_width
    scale_y = height / normalized_height
    rectangles = []

    def partition(start, end, x, y, rectangle_width, rectangle_height, offset, value):
        if end - start == 1:
            rectangles.append((functions[start][2], x * scale_x, y * scale_y,
                               rectangle_width * scale_x, rectangle_height * scale_y))
            return
        middle = max(start + 1, bisect_right(sums, value / 2 + offset, start, end))
        left = sums[middle - 1] - offset
        right = value - left
        if rectangle_width > rectangle_height:
            edge = x + rectangle_width
            split = (x * right + edge * left) / value
            partition(start, middle, x, y, split - x, rectangle_height, offset, left)
            partition(middle, end, split, y, edge - split, rectangle_height,
                      sums[middle - 1], right)
        else:
            edge = y + rectangle_height
            split = (y * right + edge * left) / value
            partition(start, middle, x, y, rectangle_width, split - y, offset, left)
            partition(middle, end, x, split, rectangle_width, edge - split,
                      sums[middle - 1], right)

    partition(0, len(functions), 0.0, 0.0, normalized_width, normalized_height, 0, sums[-1])
    return rectangles


def format_coordinate(thousandths):
    return f"{thousandths / 1000:.3f}".rstrip("0").rstrip(".")


def main():
    functions = []
    with MAP.open(newline="") as source:
        rows = csv.reader(source)
        next(rows)
        for row in rows:
            if "f" in row[5].strip():
                functions.append((int(row[0], 16), int(row[2], 16) - int(row[0], 16), row[4].strip()))
    # The reference's report handler iterates items without sorting them.
    # Preserve map.csv row order, including for equally sized functions.
    rectangles = binary_rectangles(functions, TREEMAP_WIDTH, TREEMAP_HEIGHT)

    total_bytes = sum(size for _, size, _ in functions)
    matched_bytes = sum(size for _, size, rank in functions if rank == "O")
    matched = sum(1 for _, _, rank in functions if rank == "O")
    nonmatching = sum(1 for _, _, rank in functions if rank in ("M", "m"))
    percent = matched_bytes / total_bytes * 100

    width = TREEMAP_WIDTH + 2 * MARGIN
    height = HEADER + TREEMAP_HEIGHT + LEGEND + MARGIN
    bar_width = TREEMAP_WIDTH * matched_bytes / total_bytes

    groups = {"empty": [], "matched": [], "nonmatching": []}
    for rank, x, y, rectangle_width, rectangle_height in rectangles:
        # Round shared edges, then subtract, so adjacent serialized boxes tile
        # exactly instead of acquiring overlaps from independent size rounding.
        left = round((MARGIN + x) * 1000)
        top = round((HEADER + y) * 1000)
        right = round((MARGIN + x + rectangle_width) * 1000)
        bottom = round((HEADER + y + rectangle_height) * 1000)
        groups[CLASSES.get(rank, "empty")].append(
            f'<rect x="{format_coordinate(left)}" y="{format_coordinate(top)}" '
            f'width="{format_coordinate(right - left)}" height="{format_coordinate(bottom - top)}"/>'
        )
    boxes = "\n".join(
        f'<g class="{css_class}">\n' + "\n".join(group) + "\n</g>"
        for css_class, group in groups.items()
    )

    legend_y = HEADER + TREEMAP_HEIGHT + 22
    date = datetime.date.today().isoformat()

    # Reused object-bounding-box gradients follow each tile's aspect ratio.
    # This keeps three definitions instead of one pixel-space circle per tile.
    svg = f'''<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" role="img" aria-label="Decompilation progress: {percent:.2f}% of code bytes matched, {matched} of {len(functions)} functions">
<style>
  .surface {{ fill: #ffffff; }} .title {{ fill: #1f2328; }} .muted {{ fill: #59636e; }}
  .empty {{ fill: #d5dbd5; }} .matched {{ fill: #0ca30c; }} .nonmatching {{ fill: #b86e00; }}
  #treemap .empty {{ fill: url(#empty-shading); }}
  #treemap .matched {{ fill: url(#matched-shading); }}
  #treemap .nonmatching {{ fill: url(#nonmatching-shading); }}
  .empty-inner {{ stop-color: #e5e9e5; }} .empty-outer {{ stop-color: #bac4bb; }}
  .matched-inner {{ stop-color: hsl(120, 100%, 39%); }} .matched-outer {{ stop-color: hsl(120, 100%, 17%); }}
  .nonmatching-inner {{ stop-color: #d18a15; }} .nonmatching-outer {{ stop-color: #7a4500; }}
  #treemap rect {{ stroke: #ffffff; stroke-width: 0.25; }}
  text {{ font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif; }}
  .figure {{ font-family: ui-monospace, SFMono-Regular, Menlo, Consolas, monospace; font-variant-numeric: tabular-nums; }}
  @media (prefers-color-scheme: dark) {{
    .surface {{ fill: #181c25; }} .title {{ fill: #e6edf3; }} .muted {{ fill: #9198a1; }}
    .empty {{ fill: #30363d; }} .matched {{ fill: #2ea043; }} .nonmatching {{ fill: #e3b341; }}
    .empty-inner {{ stop-color: #363636; }} .empty-outer {{ stop-color: #262626; }}
    .nonmatching-inner {{ stop-color: #e3b341; }} .nonmatching-outer {{ stop-color: #875600; }}
    #treemap rect {{ stroke: #181c25; }}
  }}
</style>
<defs>
  <radialGradient id="empty-shading" cx="40%" cy="40%" r="100%"><stop class="empty-inner" offset="20%"/><stop class="empty-outer" offset="100%"/></radialGradient>
  <radialGradient id="matched-shading" cx="40%" cy="40%" r="100%"><stop class="matched-inner" offset="20%"/><stop class="matched-outer" offset="100%"/></radialGradient>
  <radialGradient id="nonmatching-shading" cx="40%" cy="40%" r="100%"><stop class="nonmatching-inner" offset="20%"/><stop class="nonmatching-outer" offset="100%"/></radialGradient>
</defs>
<rect class="surface" width="{width}" height="{height}" rx="8"/>
<text class="title" x="{MARGIN}" y="34" font-size="22" font-weight="600"><tspan class="figure">{percent:.2f}%</tspan> of code bytes matched</text>
<text class="muted figure" x="{MARGIN + TREEMAP_WIDTH}" y="34" font-size="12" text-anchor="end">{matched_bytes:,} / {total_bytes:,} bytes</text>
<text class="muted" x="{MARGIN}" y="56" font-size="13"><tspan class="figure">{matched:,}</tspan> of <tspan class="figure">{len(functions):,}</tspan> functions byte-exact · EU Rev 2 · {date}</text>
<rect class="empty" x="{MARGIN}" y="68" width="{TREEMAP_WIDTH}" height="6" rx="3"/>
<rect class="matched" x="{MARGIN}" y="68" width="{max(bar_width, 3):.1f}" height="6" rx="3"/>
<g id="treemap">
{boxes}
</g>
<rect class="matched" x="{MARGIN}" y="{legend_y - 9}" width="10" height="10" rx="2"/>
<text class="muted" x="{MARGIN + 16}" y="{legend_y}" font-size="12">Matched <tspan class="figure">{matched:,}</tspan></text>
<rect class="nonmatching" x="{MARGIN + 130}" y="{legend_y - 9}" width="10" height="10" rx="2"/>
<text class="muted" x="{MARGIN + 146}" y="{legend_y}" font-size="12">Nonmatching <tspan class="figure">{nonmatching:,}</tspan></text>
<rect class="empty" x="{MARGIN + 270}" y="{legend_y - 9}" width="10" height="10" rx="2"/>
<text class="muted" x="{MARGIN + 286}" y="{legend_y}" font-size="12">Not matched yet</text>
<text class="muted" x="{MARGIN + TREEMAP_WIDTH}" y="{legend_y}" font-size="12" text-anchor="end">One box per function, area = byte size</text>
</svg>
'''
    OUTPUT.parent.mkdir(exist_ok=True)
    OUTPUT.write_text(svg)
    print(f"{OUTPUT.relative_to(ROOT)}: {percent:.2f}% bytes, {matched}/{len(functions)} functions, {OUTPUT.stat().st_size:,} bytes")


if __name__ == "__main__":
    main()
