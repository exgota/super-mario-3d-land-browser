//! Generate C from the owner's raw executable. Generated output is private game data.
use std::collections::BTreeSet;
use std::fs;
use std::path::Path;

use recomp3ds::codegen::{self, Unit};
use recomp3ds::discover::{self, Source};
use recomp3ds::image::{Image, Segment};

fn word(bytes: &[u8], offset: usize) -> Result<u32, String> {
    Ok(u32::from_le_bytes(bytes.get(offset..offset + 4)
        .ok_or("truncated executable header")?.try_into().unwrap()))
}

fn address(value: &str) -> Result<u32, String> {
    u32::from_str_radix(value.trim().trim_start_matches("0x"), 16)
        .map_err(|e| e.to_string())
}

fn run() -> Result<(), String> {
    let args: Vec<String> = std::env::args().collect();
    if args.len() != 6 {
        return Err("usage: static-recompiler <code.bin> <exh.bin> <map.csv> <replacement.cpp> <ignored-output-directory>".into());
    }
    let code = fs::read(&args[1]).map_err(|e| e.to_string())?;
    let header = fs::read(&args[2]).map_err(|e| e.to_string())?;
    let mut offset: usize = 0;
    let mut segment = |at: usize| -> Result<Segment, String> {
        let base = word(&header, at)?;
        let pages = word(&header, at + 4)? as usize;
        let size = word(&header, at + 8)? as usize;
        let capacity = pages.checked_mul(4096).ok_or("segment page overflow")?;
        if size > capacity { return Err("segment exceeds page extent".into()); }
        let bytes = code.get(offset..offset.checked_add(size).ok_or("segment overflow")?)
            .ok_or("segment exceeds executable")?.to_vec();
        offset = offset.checked_add(capacity).ok_or("image extent overflow")?;
        base.checked_add(size as u32).ok_or("segment address overflow")?;
        Ok(Segment { base, bytes })
    };
    let text = segment(0x10)?;
    let rodata = segment(0x20)?;
    let data = segment(0x30)?;
    if offset != code.len() { return Err("executable length disagrees with segment pages".into()); }
    let image = Image { entry: text.base, text, rodata, data };
    let mut program = image.into_program(&[], &[]);
    let map = fs::read_to_string(&args[3]).map_err(|e| e.to_string())?;
    let mut slots = BTreeSet::new();
    let mut function_count = 0;
    for line in map.lines().skip(1) {
        let fields: Vec<&str> = line.split(',').map(str::trim).collect();
        if fields.len() != 8 { return Err("unexpected map row shape".into()); }
        let start = address(fields[0])?;
        let end = address(fields[2])?;
        if start < program.text.base || start >= program.text.end() { continue; }
        if fields[5].contains('f') {
            program.seeds.push((start, Source::Hint));
            function_count += 1;
            if !fields[1].is_empty() {
                for at in (address(fields[1])?..end).step_by(4) { slots.insert(at); }
            }
        } else {
            for at in (start..end).step_by(4) { slots.insert(at); }
        }
    }
    program.slots = Some(slots);
    let replacements = recomp3ds::overrides::load(Path::new(&args[4]))?;
    let overrides: Vec<_> = replacements.into_iter().flat_map(|file| file.overrides).collect();
    // The orchestrator independently gates every replacement on frozen main's rank O.
    for item in &overrides { program.seeds.push((item.address, Source::Override)); }
    let analysis = discover::analyze(&program);
    let generated = codegen::generate(&[Unit { module: None, program: &program, analysis: &analysis }], &overrides);
    let output = Path::new(&args[5]);
    fs::create_dir_all(output).map_err(|e| e.to_string())?;
    for (name, contents) in &generated {
        fs::write(output.join(name), contents).map_err(|e| e.to_string())?;
    }
    // This is potential coverage only. Credit requires a successful native link and table audit.
    let mut coverage = String::from("address,mode\n");
    let mut words = BTreeSet::new();
    for (&entry, function) in &analysis.functions {
        if !codegen::recompiles(entry, function) { continue; }
        for &at in &function.instructions {
            let width = if function.mode == discover::Mode::Arm { 4 } else { 2 };
            words.insert((at, width));
        }
    }
    for (at, width) in &words { coverage.push_str(&format!("{at},{width}\n")); }
    fs::write(output.join("instruction_coverage.csv"), coverage).map_err(|e| e.to_string())?;
    println!("map functions {function_count}, discovered {}, generated files {}, candidate instructions {}",
        analysis.functions.len(), generated.len(), words.len());
    Ok(())
}

fn main() {
    if let Err(error) = run() { eprintln!("{error}"); std::process::exit(1); }
}
