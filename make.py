#!/usr/bin/env python3
import sys
import argparse
from tools.pypstem import main as pypstem # build system
from tools.low.glob import * # globals

def main():
    parser = argparse.ArgumentParser(description="Build RE:Pepper")
    parser.add_argument("version", nargs="?", default=None, help="Version to use")
    parser.add_argument("--warn", '-w', action='store_true', help="Omit many warnings (nintendo standard)")
    parser.add_argument("--split", '-s', action='store_true', help="Link an isolated EU source-and-placeholder full-image diagnostic")
    parser.add_argument("--vfe", '-vf', action='store_true', help="Enable VFE for building")
    parser.add_argument("--debug", '-d', action='store_true', help="Build with debug info")
    parser.add_argument("--delete", '-k', action='store_true', help="Delete temporary built files")
    parser.add_argument("--clean", '-c', action='store_true', help="Clean before building (and stop)")
    parser.add_argument("--clear_all", '-ca', action='store_true', help="Clean split and build (and continue)")
    parser.add_argument("--clear_build", '-cr', action='store_true', help="Clean build (and continue)")
    parser.add_argument("--clear_split", '-cs', action='store_true', help="Clean split (and continue)")
    parser.add_argument("--force_link", '-fl', action='store_true', help="Force linking anyways")
    parser.add_argument("--silent", '-q', action='store_true', help="Silent mode (not added yet)")
    args = parser.parse_args()
    if args.split:
        compact_options = ("warn", "vfe", "debug", "delete", "clean", "clear_all",
                           "clear_build", "clear_split", "force_link", "silent")
        if args.version != "eu" or any(getattr(args, name) for name in compact_options):
            parser.error("--split requires explicit eu and no compact-build options")
        from tools.full_image_diagnostic import main as run_full_image_diagnostic
        return run_full_image_diagnostic([])
    sys.argv = [sys.argv[0]] # clear args

    # TODO: remove this hack, upstrem should contain matches.
    cfg.macros["NON_MATCHING"] = 1

    # parse arguments
    if args.clear_all:
        args.clear_build = True
        args.clear_split = True

    cfg.flags_link.append("--tailreorder")
    if args.delete:
        cfg.keep_objects = True
    if args.debug:
        cfg.flags_compile_cxx.append("--debug")
        cfg.macros["NN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK"]=0
        cfg.macros["NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK"]=0
    else:
        cfg.flags_compile_cxx.append("--no_debug")
    if args.vfe:
        cfg.flags_link.append("--vfemode=force")
    else:
        cfg.flags_link.append("--vfemode=off")
    if args.warn:
        # a lot of spammy warnings
        cfg.flags_compile.append("--remarks")
        # default diags found in dwarf info
        cfg.flags_compile.append("--diag_suppress=186,340,401,1256,1297,1568,1764,1786,1788,2523,2819,96,1794,1801,2442,3017,optimizations")
        cfg.flags_compile.append("--diag_error=68,88,174,188,223")
        cfg.flags_compile.append("--diag_warning=177,193,228,550,826,1301")

    # starting

    # version and clear checks
    pypstem.exec_check(args.version, args.clean or args.clear_build, args.clean and not args.clear_build)

    # split assembly
    is_new_split = pypstem.exec_split(args.clear_split)

    # convert orig bin to elf
    pypstem.exec_export_elf()

    # gen comcom json
    pypstem.exec_export_comcom()

    # build binaries
    is_new_build = pypstem.exec_build()

    # export objdiff json
    pypstem.exec_export_objdiff()

    is_new = args.force_link or is_new_split or is_new_build

    # link binary
    needs_link = is_new or not getElfFile().exists()
    if needs_link:
        pypstem.exec_link()

    # Export the newly linked image, including recovery from a missing ELF.
    if needs_link or not getExportFile().exists():
        pypstem.exec_export_bin()

if __name__ == "__main__":
    sys.exit(main())
