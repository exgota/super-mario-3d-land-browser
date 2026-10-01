#!/usr/bin/env python3
"""Index and extract the owner's decrypted NCSD RomFS without modifying it.

Format references: https://www.3dbrew.org/wiki/NCSD,
https://www.3dbrew.org/wiki/NCCH, https://www.3dbrew.org/wiki/RomFS.
Generated indexes and assets must stay in ignored directories in this repository.
"""

import argparse
import fnmatch
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import struct
import subprocess
import sys
import tempfile
import time
from dataclasses import dataclass


REPOSITORY = Path(__file__).resolve().parents[1]
UNUSED_OFFSET = 0xFFFFFFFF
COPY_BUFFER_SIZE = 1024 * 1024
MAXIMUM_ARCHIVE_SIZE = 64 * 1024 * 1024


class RomfsFormatError(ValueError):
    pass


def align(value, boundary):
    return (value + boundary - 1) // boundary * boundary


def require_range(offset, length, total, label):
    if offset < 0 or length < 0 or offset > total or length > total - offset:
        raise RomfsFormatError(f"{label} extends outside its containing region")


def ignored_path(path):
    """Resolve a path and enforce the project's local game-data boundary."""
    path = Path(path).resolve()
    try:
        relative = path.relative_to(REPOSITORY)
    except ValueError as error:
        raise ValueError("Game data must stay inside this repository") from error
    result = subprocess.run(
        ["git", "check-ignore", "--quiet", "--no-index", os.fspath(relative)],
        cwd=REPOSITORY,
        check=False,
    )
    if result.returncode != 0:
        raise ValueError(f"Game-data path is not ignored: {relative}")
    return path


def hash_file(path):
    digest = hashlib.sha256()
    with Path(path).open("rb") as source:
        for block in iter(lambda: source.read(COPY_BUFFER_SIZE), b""):
            digest.update(block)
    return digest.hexdigest()


def write_json(path, value):
    path = ignored_path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w", encoding="utf-8", dir=path.parent,
            prefix=f".{path.name}.", suffix=".partial", delete=False,
        ) as output:
            temporary = Path(output.name)
            json.dump(value, output, indent=2, ensure_ascii=False)
            output.write("\n")
        os.replace(temporary, path)
    finally:
        if temporary is not None and temporary.exists():
            temporary.unlink()


@dataclass(frozen=True)
class FileEntry:
    path: str
    offset: int
    size: int
    metadata_offset: int


def decompress_yaz0(data):
    """Decode the public Yaz0 format, including overlapping back references.

    Reference: https://wiki.cloudmodding.com/oot/Yaz_(File_Compression)
    """
    require_range(0, 16, len(data), "Yaz0 header")
    if data[:4] != b"Yaz0":
        raise RomfsFormatError("Expected Yaz0 compression")
    size = struct.unpack_from(">I", data, 4)[0]
    if size > MAXIMUM_ARCHIVE_SIZE:
        raise RomfsFormatError("Archive exceeds the 64 MiB inspection limit")
    output = bytearray()
    position = 16
    while len(output) < size:
        require_range(position, 1, len(data), "Yaz0 control byte")
        control = data[position]
        position += 1
        for bit in range(7, -1, -1):
            if len(output) == size:
                break
            if control & (1 << bit):
                require_range(position, 1, len(data), "Yaz0 literal")
                output.append(data[position])
                position += 1
            else:
                require_range(position, 2, len(data), "Yaz0 back reference")
                pair = struct.unpack_from(">H", data, position)[0]
                position += 2
                distance = (pair & 0xFFF) + 1
                length = pair >> 12
                if length:
                    length += 2
                else:
                    require_range(position, 1, len(data), "Yaz0 long copy length")
                    length = data[position] + 18
                    position += 1
                if distance > len(output) or length > size - len(output):
                    raise RomfsFormatError("Invalid Yaz0 back reference")
                for unused in range(length):
                    output.append(output[-distance])
    return bytes(output)


def narc_members(data):
    """Read the flat, named NARC variant observed in this game's tables.

    The public NARC section layout is documented at
    https://loveemu.hatenablog.com/entry/20091002/nds_formats.
    Nested or unnamed archives are rejected rather than assigned invented names.
    """
    require_range(0, 16, len(data), "NARC header")
    magic, byte_order, version, size, header_size, section_count = struct.unpack_from("<4sHHIHH", data)
    if (magic, byte_order, version, header_size, section_count) != (b"NARC", 0xFFFE, 0x100, 16, 3):
        raise RomfsFormatError("Unsupported NARC header variant")
    if size != len(data):
        raise RomfsFormatError("NARC size differs from its decoded length")
    sections = {}
    position = header_size
    for expected in (b"BTAF", b"BTNF", b"GMIF"):
        require_range(position, 8, size, "NARC section header")
        section_magic, section_size = struct.unpack_from("<4sI", data, position)
        require_range(position, section_size, size, "NARC section")
        if section_magic != expected or section_size < 8:
            raise RomfsFormatError("Unexpected NARC section")
        sections[expected] = (position + 8, section_size - 8)
        position += section_size
    if position != size:
        raise RomfsFormatError("NARC sections do not cover the archive")
    allocation, allocation_size = sections[b"BTAF"]
    require_range(0, 4, allocation_size, "NARC file count")
    count = struct.unpack_from("<I", data, allocation)[0]
    if allocation_size != 4 + count * 8:
        raise RomfsFormatError("NARC allocation table has the wrong length")
    names, names_size = sections[b"BTNF"]
    require_range(0, 8, names_size, "NARC root directory")
    name_offset, first_file, directory_count = struct.unpack_from("<IHH", data, names)
    if name_offset != 8 or first_file or directory_count != 1:
        raise RomfsFormatError("Only flat, named NARC archives are supported")
    member_names = []
    position = names + name_offset
    while True:
        require_range(position - names, 1, names_size, "NARC filename length")
        length = data[position]
        position += 1
        if not length:
            break
        if length & 0x80:
            raise RomfsFormatError("Nested NARC directories are unsupported")
        require_range(position - names, length, names_size, "NARC filename")
        name = data[position:position + length].decode("utf-8")
        if name in (".", "..") or any(char in name for char in "/\\\0"):
            raise RomfsFormatError("Unsafe NARC filename")
        member_names.append(name)
        position += length
    if len(member_names) != count or len(set(member_names)) != count:
        raise RomfsFormatError("NARC filename count or uniqueness differs from its allocation table")
    payload, payload_size = sections[b"GMIF"]
    entries = []
    previous_end = 0
    for number, name in enumerate(member_names):
        start, end = struct.unpack_from("<II", data, allocation + 4 + number * 8)
        require_range(start, end - start, payload_size, "NARC member")
        if start < previous_end:
            raise RomfsFormatError("NARC members overlap or are out of order")
        previous_end = end
        entries.append(FileEntry(name, payload + start, end - start, number))
    return entries


def decode_byaml(data, string_encoding="utf-8"):
    """Read v1/v2 scalar, array and dictionary BYAML for local inspection.

    Reference: https://nintendo-formats.com/libs/common/byaml.html
    Cyclic graphs and later-version node types are outside this reader's scope.
    """
    require_range(0, 16, len(data), "BYAML header")
    if data[:2] not in (b"YB", b"BY"):
        raise RomfsFormatError("Expected BYAML")
    endian = "<" if data[:2] == b"YB" else ">"
    byte_order = "little" if endian == "<" else "big"

    def integer(offset, length=4):
        require_range(offset, length, len(data), "BYAML integer")
        return int.from_bytes(data[offset:offset + length], byte_order)

    version = integer(2, 2)
    if version not in (1, 2):
        raise RomfsFormatError(f"Unsupported BYAML version {version}")

    def string_table(offset):
        if not offset:
            return []
        require_range(offset, 4, len(data), "BYAML string table")
        if data[offset] != 0xC2:
            raise RomfsFormatError("Invalid BYAML string table type")
        count = integer(offset + 1, 3)
        table_size = 4 + 4 * (count + 1)
        require_range(offset, table_size, len(data), "BYAML string address table")
        addresses = [integer(offset + 4 + number * 4) for number in range(count + 1)]
        if addresses[0] < table_size:
            raise RomfsFormatError("BYAML strings overlap their address table")
        require_range(offset + addresses[-1], 0, len(data), "BYAML string table end")
        result, encoded_strings = [], []
        for start, end in zip(addresses, addresses[1:]):
            if start < table_size or end <= start:
                raise RomfsFormatError("Invalid BYAML string offsets")
            require_range(offset + start, end - start, len(data), "BYAML string")
            value = data[offset + start:offset + end]
            terminator = value.find(b"\0")
            if terminator < 0 or any(value[terminator + 1:]):
                raise RomfsFormatError("Invalid BYAML string terminator or padding")
            encoded_strings.append(value[:terminator])
            result.append(value[:terminator].decode(string_encoding))
        if encoded_strings != sorted(set(encoded_strings)):
            raise RomfsFormatError("BYAML strings are not sorted and unique")
        if len(set(result)) != len(result):
            raise RomfsFormatError("BYAML text encoding aliases distinct byte strings")
        return result

    keys = string_table(integer(4))
    strings = string_table(integer(8))
    active, completed = set(), {}

    def value(node_type, raw, depth):
        if node_type == 0xA0:
            if raw >= len(strings):
                raise RomfsFormatError("BYAML string index is outside its table")
            return strings[raw]
        if node_type == 0xD0:
            if raw not in (0, 1):
                raise RomfsFormatError("Invalid BYAML boolean")
            return bool(raw)
        if node_type == 0xD1:
            return raw if raw < 0x80000000 else raw - 0x100000000
        if node_type == 0xD2:
            return struct.unpack(endian + "f", raw.to_bytes(4, byte_order))[0]
        if node_type == 0xD3 and version == 2:
            return raw
        if node_type == 0xFF:
            return None
        if node_type in (0xC0, 0xC1):
            return container(raw, node_type, depth + 1)
        raise RomfsFormatError(f"Unsupported BYAML node type 0x{node_type:02X}")

    def container(offset, expected_type=None, depth=0):
        if depth > 256:
            raise RomfsFormatError("BYAML nesting exceeds the inspection limit")
        if offset in active:
            raise RomfsFormatError("Cyclic BYAML graphs are unsupported")
        require_range(offset, 4, len(data), "BYAML container")
        node_type, count = data[offset], integer(offset + 1, 3)
        if offset % 4 or node_type not in (0xC0, 0xC1) or expected_type not in (None, node_type):
            raise RomfsFormatError("Invalid BYAML container type or alignment")
        if offset in completed:
            return completed[offset]
        active.add(offset)
        if node_type == 0xC0:
            values_offset = offset + 4 + align(count, 4)
            require_range(offset + 4, align(count, 4) + count * 4, len(data), "BYAML array")
            result = [value(data[offset + 4 + number], integer(values_offset + number * 4), depth)
                      for number in range(count)]
        else:
            require_range(offset + 4, count * 8, len(data), "BYAML dictionary")
            result = {}
            previous_key = -1
            for number in range(count):
                entry = offset + 4 + number * 8
                key = integer(entry, 3)
                if key >= len(keys) or key <= previous_key:
                    raise RomfsFormatError("BYAML dictionary keys are invalid or unsorted")
                previous_key = key
                result[keys[key]] = value(data[entry + 3], integer(entry + 4), depth)
        active.remove(offset)
        completed[offset] = result
        return result

    root = integer(12)
    return container(root) if root else None


def inspect_archive(image, entry, decode_tables, string_encoding):
    if entry.size > MAXIMUM_ARCHIVE_SIZE:
        raise RomfsFormatError("File exceeds the 64 MiB inspection limit")
    data = image.read_at(entry.offset, entry.size)
    result = {"path": entry.path, "size": entry.size, "sha256": hashlib.sha256(data).hexdigest()}
    if data[:4] == b"Yaz0":
        data = decompress_yaz0(data)
        result["compression"] = "Yaz0"
        result["decoded_size"] = len(data)
        result["decoded_sha256"] = hashlib.sha256(data).hexdigest()
    result["magic"] = data[:4].hex()
    if data[:4] == b"NARC":
        result["archive"] = "NARC"
        records = []
        try:
            members = narc_members(data)
        except ValueError as error:
            raise RomfsFormatError(f"{entry.path}: {error}") from error
        for member in members:
            content = data[member.offset:member.offset + member.size]
            record = {
                "path": member.path, "offset": member.offset, "size": member.size,
                "sha256": hashlib.sha256(content).hexdigest(), "magic": content[:4].hex(),
            }
            if decode_tables and content[:2] in (b"YB", b"BY"):
                try:
                    record["byaml"] = decode_byaml(content, string_encoding)
                except ValueError as error:
                    raise RomfsFormatError(f"{entry.path}/{member.path}: {error}") from error
            records.append(record)
        result["members"] = records
    return result


class RomfsImage:
    """Read-only NCSD/NCCH/IVFC reader for decrypted cartridge images."""

    def __init__(self, path, partition=0):
        self.path = ignored_path(path)
        self.image = self.path.open("rb")
        try:
            self.initial_stat = os.fstat(self.image.fileno())
            self.size = self.initial_stat.st_size
            self.partition = partition
            self._read_container()
            self._read_integrity_header()
            self._read_file_tree()
        except Exception:
            self.image.close()
            raise

    def __enter__(self):
        return self

    def __exit__(self, *arguments):
        self.image.close()

    def read_at(self, offset, length):
        require_range(offset, length, self.size, "Image read")
        self.image.seek(offset)
        data = self.image.read(length)
        if len(data) != length:
            raise RomfsFormatError("Image ended during a read")
        return data

    def _read_container(self):
        header = self.read_at(0, 0x200)
        if header[0x100:0x104] != b"NCSD":
            raise RomfsFormatError("Expected an NCSD cartridge image")
        if not 0 <= self.partition < 8:
            raise RomfsFormatError("NCSD partition must be between 0 and 7")
        media_size = struct.unpack_from("<I", header, 0x104)[0] * 0x200
        if media_size < self.size:
            raise RomfsFormatError("Image exceeds its declared NCSD media size")
        partitions = []
        for number in range(8):
            offset, length = struct.unpack_from("<II", header, 0x120 + number * 8)
            offset *= 0x200
            length *= 0x200
            require_range(offset, length, self.size, f"NCSD partition {number}")
            if length:
                if offset < 0x200:
                    raise RomfsFormatError("NCSD partition overlaps its header")
                partitions.append((offset, offset + length, number))
            if number == self.partition:
                self.ncch_offset, self.ncch_size = offset, length
        for previous, current in zip(sorted(partitions), sorted(partitions)[1:]):
            if previous[1] > current[0]:
                raise RomfsFormatError("NCSD partitions overlap")
        if self.ncch_size < 0x200:
            raise RomfsFormatError("Selected NCSD partition is empty or truncated")
        ncch = self.read_at(self.ncch_offset, 0x200)
        if ncch[0x100:0x104] != b"NCCH":
            raise RomfsFormatError("Selected partition is not NCCH")
        flags = ncch[0x188:0x190]
        if not flags[7] & 4:
            raise RomfsFormatError("Encrypted NCCH is unsupported; supply a decrypted copy")
        if flags[6] > 20:
            raise RomfsFormatError("Unsupported NCCH media unit exponent")
        media_unit = 0x200 << flags[6]
        content_size = struct.unpack_from("<I", ncch, 0x104)[0] * media_unit
        require_range(0, content_size, self.ncch_size, "NCCH content")
        offset, length, hash_length = struct.unpack_from("<III", ncch, 0x1B0)
        offset, length, hash_length = (
            offset * media_unit, length * media_unit, hash_length * media_unit
        )
        if not length or flags[7] & 2:
            raise RomfsFormatError("Selected partition has no mountable RomFS")
        if offset < 0x200 or not hash_length:
            raise RomfsFormatError("Invalid NCCH RomFS region")
        require_range(offset, length, content_size, "NCCH RomFS")
        require_range(0, hash_length, length, "RomFS superblock hash region")
        self.romfs_offset = self.ncch_offset + offset
        self.romfs_size = length
        self.superblock_size = hash_length
        self.superblock_hash = ncch[0x1E0:0x200]
        self.program_id = f"{struct.unpack_from('<Q', ncch, 0x118)[0]:016X}"
        self.product_code = ncch[0x150:0x160].rstrip(b"\0").decode("ascii")
        self.ncch_version = struct.unpack_from("<H", ncch, 0x112)[0]

    def _read_integrity_header(self):
        header = self.read_at(self.romfs_offset, 0x5C)
        magic, identifier, master_size = struct.unpack_from("<4sII", header)
        if magic != b"IVFC" or identifier != 0x10000:
            raise RomfsFormatError("Unsupported RomFS IVFC format")
        optional_size = struct.unpack_from("<I", header, 0x58)[0]
        if optional_size:
            raise RomfsFormatError("IVFC optional information is unsupported")
        self.master_offset = self.romfs_offset + align(0x5C, 16)
        self.master_size = master_size
        descriptors = []
        for number in range(3):
            logical, size, exponent, reserved = struct.unpack_from(
                "<QQII", header, 0x0C + number * 0x18
            )
            if not 5 <= exponent <= 20 or reserved or not size:
                raise RomfsFormatError("Invalid IVFC level descriptor")
            descriptors.append({
                "logical_offset": logical, "size": size,
                "block_size": 1 << exponent,
            })
        first, second, third = descriptors
        if first["logical_offset"] != 0:
            raise RomfsFormatError("Unsupported IVFC first logical offset")
        if second["logical_offset"] != align(first["size"], first["block_size"]):
            raise RomfsFormatError("IVFC level 2 logical offset is inconsistent")
        if third["logical_offset"] != align(
            second["logical_offset"] + second["size"], second["block_size"]
        ):
            raise RomfsFormatError("IVFC level 3 logical offset is inconsistent")
        third_physical = align(align(0x5C, 16) + master_size, third["block_size"])
        first_physical = third_physical + align(third["size"], third["block_size"])
        for number, level in enumerate(descriptors):
            physical = (
                third_physical if number == 2
                else first_physical + level["logical_offset"]
            )
            require_range(physical, level["size"], self.romfs_size, "IVFC level")
            level["offset"] = self.romfs_offset + physical
        require_range(align(0x5C, 16), master_size, third_physical, "IVFC master hashes")
        self.levels = descriptors
        self.level3_offset = third["offset"]
        self.level3_size = third["size"]
        for number, level in enumerate(descriptors):
            hash_size = master_size if number == 0 else descriptors[number - 1]["size"]
            expected_size = align(level["size"], level["block_size"]) // level["block_size"] * 32
            if hash_size != expected_size:
                raise RomfsFormatError("IVFC hash count does not cover its child level")

    @staticmethod
    def _name(data, root=False):
        if len(data) % 2:
            raise RomfsFormatError("RomFS name has an odd UTF-16 byte count")
        try:
            name = data.decode("utf-16-le")
        except UnicodeDecodeError as error:
            raise RomfsFormatError("Invalid UTF-16 RomFS name") from error
        if root and not name:
            return name
        if not name or name in (".", "..") or any(char in name for char in "/\\\0"):
            raise RomfsFormatError("Unsafe RomFS path component")
        return name

    def _metadata(self, offset, length, directory):
        table = self.read_at(self.level3_offset + offset, length)
        entries = {}
        position = 0
        fixed_size = 24 if directory else 32
        while position < length:
            require_range(position, fixed_size, length, "RomFS metadata entry")
            if directory:
                parent, sibling, child, first_file, hash_next, name_length = struct.unpack_from(
                    "<6I", table, position
                )
                entry = {
                    "parent": parent, "sibling": sibling, "child": child,
                    "first_file": first_file, "hash_next": hash_next,
                }
            else:
                parent, sibling, data_offset, data_size, hash_next, name_length = struct.unpack_from(
                    "<IIQQII", table, position
                )
                entry = {
                    "parent": parent, "sibling": sibling, "data_offset": data_offset,
                    "data_size": data_size, "hash_next": hash_next,
                }
            require_range(position + fixed_size, align(name_length, 4), length, "RomFS name")
            entry["name"] = self._name(
                table[position + fixed_size:position + fixed_size + name_length],
                root=directory and position == 0,
            )
            entries[position] = entry
            position += fixed_size + align(name_length, 4)
        return entries

    def _hash_links(self, offset, length, entries):
        if length % 4 or not length:
            raise RomfsFormatError("Invalid RomFS hash table length")
        table = self.read_at(self.level3_offset + offset, length)
        seen = set()
        for (position,) in struct.iter_unpack("<I", table):
            while position != UNUSED_OFFSET:
                if position not in entries or position in seen:
                    raise RomfsFormatError("Invalid or cyclic RomFS hash chain")
                seen.add(position)
                position = entries[position]["hash_next"]
        if seen != set(entries):
            raise RomfsFormatError("RomFS hash chains omit metadata entries")

    def _read_file_tree(self):
        fields = struct.unpack("<10I", self.read_at(self.level3_offset, 40))
        header_length, directory_hash, directory_hash_size, directory_metadata, directory_size, \
            file_hash, file_hash_size, file_metadata, file_size, file_data = fields
        if header_length != 40 or file_data % 16:
            raise RomfsFormatError("Unsupported RomFS level 3 header")
        previous_end = header_length
        for offset, length in (
            (directory_hash, directory_hash_size), (directory_metadata, directory_size),
            (file_hash, file_hash_size), (file_metadata, file_size),
        ):
            require_range(offset, length, self.level3_size, "RomFS metadata table")
            if offset % 4 or offset < previous_end:
                raise RomfsFormatError("RomFS metadata tables overlap or are unaligned")
            previous_end = offset + length
        if file_data < previous_end or file_data > self.level3_size:
            raise RomfsFormatError("RomFS file data overlaps metadata")
        directories = self._metadata(directory_metadata, directory_size, True)
        files = self._metadata(file_metadata, file_size, False)
        if 0 not in directories or directories[0]["name"] or directories[0]["parent"] != 0:
            raise RomfsFormatError("Invalid RomFS root directory")
        if directories[0]["sibling"] != UNUSED_OFFSET:
            raise RomfsFormatError("RomFS root has a sibling")
        self._hash_links(directory_hash, directory_hash_size, directories)
        self._hash_links(file_hash, file_hash_size, files)
        visited_directories, visited_files, paths = set(), set(), set()
        result = []
        pending = [(0, "")]
        while pending:
            position, path = pending.pop()
            if position not in directories or position in visited_directories:
                raise RomfsFormatError("Invalid or cyclic RomFS directory tree")
            visited_directories.add(position)
            directory = directories[position]
            child = directory["child"]
            siblings = set()
            while child != UNUSED_OFFSET:
                if child not in directories or child in siblings:
                    raise RomfsFormatError("Invalid or cyclic RomFS directory siblings")
                siblings.add(child)
                item = directories[child]
                if item["parent"] != position:
                    raise RomfsFormatError("RomFS child directory has the wrong parent")
                child_path = f"{path}/{item['name']}" if path else item["name"]
                if child_path in paths:
                    raise RomfsFormatError("Duplicate RomFS path")
                paths.add(child_path)
                pending.append((child, child_path))
                child = item["sibling"]
            child = directory["first_file"]
            while child != UNUSED_OFFSET:
                if child not in files or child in visited_files:
                    raise RomfsFormatError("Invalid or cyclic RomFS file siblings")
                visited_files.add(child)
                item = files[child]
                if item["parent"] != position:
                    raise RomfsFormatError("RomFS file has the wrong containing directory")
                file_path = f"{path}/{item['name']}" if path else item["name"]
                if file_path in paths:
                    raise RomfsFormatError("Duplicate RomFS path")
                paths.add(file_path)
                require_range(
                    file_data + item["data_offset"], item["data_size"], self.level3_size,
                    "RomFS file data",
                )
                result.append(FileEntry(
                    file_path, self.level3_offset + file_data + item["data_offset"],
                    item["data_size"], child,
                ))
                child = item["sibling"]
        if visited_directories != set(directories) or visited_files != set(files):
            raise RomfsFormatError("RomFS tree contains unreachable metadata")
        occupied = sorted((entry.offset, entry.offset + entry.size) for entry in result if entry.size)
        for previous, current in zip(occupied, occupied[1:]):
            if previous[1] > current[0]:
                raise RomfsFormatError("RomFS files overlap")
        self.directory_count = len(directories)
        self.files = sorted(result, key=lambda entry: entry.path)

    def verify(self):
        actual = hashlib.sha256(self.read_at(self.romfs_offset, self.superblock_size)).digest()
        if actual != self.superblock_hash:
            raise RomfsFormatError("NCCH RomFS superblock SHA-256 mismatch")
        counts = []
        for number, level in enumerate(self.levels):
            expected = self.read_at(
                self.master_offset if number == 0 else self.levels[number - 1]["offset"],
                self.master_size if number == 0 else self.levels[number - 1]["size"],
            )
            self.image.seek(level["offset"])
            remaining = level["size"]
            block_number = 0
            while remaining:
                length = min(remaining, level["block_size"])
                block = self.image.read(length)
                if len(block) != length:
                    raise RomfsFormatError("IVFC data ended during verification")
                block += bytes(level["block_size"] - length)
                digest = hashlib.sha256(block).digest()
                if digest != expected[block_number * 32:(block_number + 1) * 32]:
                    raise RomfsFormatError(f"IVFC level {number + 1} block {block_number} SHA-256 mismatch")
                remaining -= length
                block_number += 1
            counts.append(block_number)
        return {"superblock_sha256": actual.hex(), "ivfc_verified_blocks": counts}

    def summary(self, integrity):
        self.image.seek(0)
        digest = hashlib.sha256()
        for block in iter(lambda: self.image.read(COPY_BUFFER_SIZE), b""):
            digest.update(block)
        current = os.fstat(self.image.fileno())
        if (current.st_size, current.st_mtime_ns) != (
            self.initial_stat.st_size, self.initial_stat.st_mtime_ns
        ):
            raise RomfsFormatError("Input image changed while being read")
        return {
            "schema_version": 1,
            "image": os.fspath(self.path.relative_to(REPOSITORY)),
            "image_size": self.size, "image_sha256": digest.hexdigest(),
            "partition": self.partition, "program_id": self.program_id,
            "product_code": self.product_code, "ncch_version": self.ncch_version,
            "romfs_offset": self.romfs_offset, "romfs_size": self.romfs_size,
            "level3_offset": self.level3_offset, "level3_size": self.level3_size,
            "directories": self.directory_count, "file_count": len(self.files),
            "file_bytes": sum(entry.size for entry in self.files), **integrity,
        }

    def extract(self, entries, destination):
        destination = ignored_path(destination)
        paths, component_spellings = set(), {}
        for entry in entries:
            folded = entry.path.casefold()
            if folded in paths:
                raise RomfsFormatError("Selected paths collide on a case-insensitive filesystem")
            paths.add(folded)
            components = PurePosixPath(entry.path).parts
            for length in range(1, len(components) + 1):
                prefix = "/".join(components[:length])
                previous = component_spellings.setdefault(prefix.casefold(), prefix)
                if previous != prefix:
                    raise RomfsFormatError("Selected directory spellings collide on a case-insensitive filesystem")
        records = []
        for entry in entries:
            path = ignored_path(destination.joinpath(*PurePosixPath(entry.path).parts))
            if not path.is_relative_to(destination):
                raise ValueError("Extraction destination escapes through a symbolic link")
            if path == self.path:
                raise ValueError("Extraction cannot replace the input image")
            path.parent.mkdir(parents=True, exist_ok=True)
            temporary = None
            try:
                digest = hashlib.sha256()
                with tempfile.NamedTemporaryFile(
                    mode="wb", dir=path.parent, prefix=f".{path.name}.",
                    suffix=".partial", delete=False,
                ) as output:
                    temporary = Path(output.name)
                    self.image.seek(entry.offset)
                    remaining = entry.size
                    while remaining:
                        block = self.image.read(min(remaining, COPY_BUFFER_SIZE))
                        if not block:
                            raise RomfsFormatError("RomFS file ended during extraction")
                        output.write(block)
                        digest.update(block)
                        remaining -= len(block)
                expected = digest.hexdigest()
                if hash_file(temporary) != expected:
                    raise OSError("Extracted file SHA-256 differs from image bytes")
                if path.exists():
                    if path.stat().st_size != entry.size or hash_file(path) != expected:
                        raise FileExistsError(f"Refusing to replace different existing data: {path}")
                    outcome = "already_identical"
                else:
                    # A hard link creates the final name exclusively and atomically.
                    os.link(temporary, path)
                    outcome = "extracted"
                records.append({
                    "path": entry.path, "offset": entry.offset,
                    "size": entry.size, "sha256": expected, "outcome": outcome,
                })
            finally:
                if temporary is not None and temporary.exists():
                    temporary.unlink()
        return records


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("image", type=Path, help="Ignored local copy of a decrypted .3ds image")
    parser.add_argument("--partition", type=int, default=0, help="NCSD partition, default: 0 (application)")
    commands = parser.add_subparsers(dest="command", required=True)
    index = commands.add_parser("index", help="Validate and write a local JSON file index")
    index.add_argument("--output", type=Path, default=Path("data/runtime/romfs/index.json"))
    verify = commands.add_parser("verify", help="Validate the tree, NCCH hash and complete IVFC hash chain")
    verify.add_argument("--output", type=Path)
    listing = commands.add_parser("list", help="List validated files matching case-sensitive shell patterns")
    listing.add_argument("patterns", nargs="*", default=["*"])
    extraction = commands.add_parser("extract", help="Extract selected files and verify their written hashes")
    extraction.add_argument("patterns", nargs="+", help="Case-sensitive shell patterns; quote them")
    extraction.add_argument("--destination", type=Path, default=Path("data/runtime/extracted"))
    extraction.add_argument("--report", type=Path, default=Path("data/runtime/romfs/extraction.json"))
    inspection = commands.add_parser("inspect", help="Inspect selected Yaz0/NARC archives into a local report")
    inspection.add_argument("patterns", nargs="+", help="Case-sensitive RomFS path patterns; quote them")
    inspection.add_argument("--decode-byaml", action="store_true", help="Decode supported BYAML tables in the report")
    inspection.add_argument("--string-encoding", choices=("utf-8", "cp932"), default="utf-8",
                            help="Explicit BYAML text encoding, default: utf-8; this EU dump uses cp932")
    inspection.add_argument("--output", type=Path, default=Path("data/runtime/romfs/inspection.json"))
    arguments = parser.parse_args()
    started = time.monotonic()
    try:
        for name in ("output", "report"):
            output = getattr(arguments, name, None)
            if output is not None:
                output = ignored_path(output)
                if output == arguments.image.resolve() or (output.exists() and output.samefile(arguments.image)):
                    raise ValueError("A report cannot replace the input image")
        with RomfsImage(arguments.image, arguments.partition) as image:
            integrity = image.verify()
            summary = image.summary(integrity)
            if arguments.command == "index":
                write_json(arguments.output, {**summary, "files": [entry.__dict__ for entry in image.files]})
                summary["index"] = os.fspath(ignored_path(arguments.output).relative_to(REPOSITORY))
            elif arguments.command == "verify" and arguments.output:
                write_json(arguments.output, summary)
            elif arguments.command in ("list", "extract", "inspect"):
                entries = [entry for entry in image.files if any(
                    fnmatch.fnmatchcase(entry.path, pattern) for pattern in arguments.patterns
                )]
                if arguments.command == "list":
                    for entry in entries:
                        print(f"{entry.size:12d}  {entry.path}")
                    return 0
                if not entries:
                    raise ValueError("No RomFS files match the requested patterns")
                if arguments.command == "inspect":
                    records = [inspect_archive(image, entry, arguments.decode_byaml, arguments.string_encoding)
                               for entry in entries]
                    write_json(arguments.output, {
                        **summary, "byaml_string_encoding": arguments.string_encoding, "files": records,
                    })
                    report = arguments.output
                else:
                    records = image.extract(entries, arguments.destination)
                    write_json(arguments.report, {
                        **summary, "destination": os.fspath(ignored_path(arguments.destination).relative_to(REPOSITORY)),
                        "files": records,
                    })
                    report = arguments.report
                summary["selected_files"] = len(records)
                summary["selected_bytes"] = sum(entry.size for entry in entries)
                summary["report"] = os.fspath(ignored_path(report).relative_to(REPOSITORY))
            summary["seconds"] = round(time.monotonic() - started, 3)
            print(json.dumps(summary, indent=2))
    except (OSError, ValueError, struct.error) as error:
        parser.exit(1, f"romfs: {error}\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
