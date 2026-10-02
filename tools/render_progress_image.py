#!/usr/bin/env python3
"""Render docs/progress.svg, the README progress image.

One box per function, with area proportional to its byte size and color from
its rank in data/ver/eu/map.csv. Only rank O counts as matched.
Run from the repository root: python3 tools/render_progress_image.py
"""
import csv
import datetime
import pathlib

ROOT = pathlib.Path(__file__).resolve().parent.parent
MAP = ROOT / "data/ver/eu/map.csv"
OUTPUT = ROOT / "docs/progress.svg"

TREEMAP_WIDTH = 992
TREEMAP_HEIGHT = 500
MARGIN = 16
HEADER = 92
LEGEND = 34

CLASSES = {"O": "matched", "M": "nonmatching", "m": "nonmatching"}


def squarified_rectangles(functions, width, height):
    """Lay out descending byte sizes using Bruls, Huizing and van Wijk's algorithm.

    Grow each row while its worst aspect ratio improves, then place it along
    the remaining rectangle's shorter side. Keep the geometric areas intact;
    SVG strokes provide the visual gaps without shrinking smaller functions.
    """
    total_bytes = sum(size for _, size, _ in functions)
    if not functions or any(size <= 0 for _, size, _ in functions):
        raise ValueError("The function map must contain positive byte sizes")
    scale = width * height / total_bytes
    areas = [size * scale for _, size, _ in functions]
    rectangles = []
    x = y = 0.0
    index = 0

    while index < len(functions):
        side = min(width, height)
        end = index + 1
        row_area = areas[index]
        maximum_area = areas[index]
        worst = max(side * side / row_area, row_area / (side * side))
        while end < len(functions):
            candidate_area = row_area + areas[end]
            candidate_worst = max(
                side * side * maximum_area / candidate_area ** 2,
                candidate_area ** 2 / (side * side * areas[end]),
            )
            if candidate_worst > worst:
                break
            row_area = candidate_area
            worst = candidate_worst
            end += 1

        vertical = width >= height
        thickness = row_area / side
        if end == len(functions):
            thickness = width if vertical else height
        offset = 0.0
        for position in range(index, end):
            length = areas[position] / thickness
            if position == end - 1:
                length = side - offset
            rank = functions[position][2]
            if vertical:
                rectangles.append((rank, x, y + offset, thickness, length))
            else:
                rectangles.append((rank, x + offset, y, length, thickness))
            offset += length

        if vertical:
            x += thickness
            width -= thickness
        else:
            y += thickness
            height -= thickness
        index = end

    return rectangles


def coordinate(value):
    return f"{value:.3f}".rstrip("0").rstrip(".")


def main():
    functions = []
    with MAP.open(newline="") as source:
        rows = csv.reader(source)
        next(rows)
        for row in rows:
            if "f" in row[5].strip():
                functions.append((int(row[0], 16), int(row[2], 16) - int(row[0], 16), row[4].strip()))
    functions.sort(key=lambda function: (-function[1], function[0]))
    rectangles = squarified_rectangles(functions, TREEMAP_WIDTH, TREEMAP_HEIGHT)

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
        groups[CLASSES.get(rank, "empty")].append(
            f'<rect x="{coordinate(MARGIN + x)}" y="{coordinate(HEADER + y)}" '
            f'width="{coordinate(rectangle_width)}" height="{coordinate(rectangle_height)}"/>'
        )
    boxes = "\n".join(
        f'<g class="{css_class}">\n' + "\n".join(group) + "\n</g>"
        for css_class, group in groups.items()
    )

    legend_y = HEADER + TREEMAP_HEIGHT + 22
    date = datetime.date.today().isoformat()

    svg = f'''<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" role="img" aria-label="Decompilation progress: {percent:.2f}% of code bytes matched, {matched} of {len(functions)} functions">
<style>
  .surface {{ fill: #ffffff; }} .title {{ fill: #1f2328; }} .muted {{ fill: #59636e; }}
  .empty, .nonmatching {{ fill: #d5dbd5; }} .matched {{ fill: #0ca30c; }}
  #treemap rect {{ stroke: #ffffff; stroke-width: 0.25; }}
  text {{ font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif; }}
  .figure {{ font-family: ui-monospace, SFMono-Regular, Menlo, Consolas, monospace; font-variant-numeric: tabular-nums; }}
  @media (prefers-color-scheme: dark) {{
    .surface {{ fill: #0d1117; }} .title {{ fill: #e6edf3; }} .muted {{ fill: #9198a1; }}
    .empty, .nonmatching {{ fill: #30363d; }} .matched {{ fill: #2ea043; }}
    #treemap rect {{ stroke: #0d1117; }}
  }}
</style>
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
