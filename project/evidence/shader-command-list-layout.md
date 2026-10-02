# Shader command-list initialization: observed ABI and flow

The original EU root is 002CA5E4..002CADBC, 2008 complete bytes. Its instruction
ranges are 002CA5E4..002CA9BC and 002CA9F0..002CAD94, totaling 479 instructions.
An embedded literal pool separates them; the original map's pool marker is not
changed. The final pool occupies 002CAD94..002CADBC.

Entry r0 is a state object of observed extent 0x684; r1 is vertex storage, r2
command storage, and r3 a binary header. The sixth parameter, entry stack+4,
controls optional upload. The fifth parameter is unused in the observed body.
The binary's +4 word selects a program record at binary+8+4*value. Its +8 word
selects a shader record by byte offset. These fields are now named in the local
view, avoiding a fictitious one-element flexible array for the fixed header.

The state has independent 16-byte buffer records at +4 and +14. It saves command
storage at +678, vertex storage at +67C, and aligned physical address at +680.
The constructor fills state command fields at +28/+2C, +88/+8C, +238/+23C and
+448/+44C and zeros four floats at +74..+80. All remaining object bytes are left
alone. No shared public class layout was changed.

Optional upload first checks the byte at 003F03E0. If clear, it generates 26
sets of six 16-byte vertices at 00429AF0, using six integer indices from
003AEF20. The attributes are group index, corner index, an odd/even X selector,
and Y chosen by an unsigned comparison of index+1 with 3. Then it sets the byte,
copies 0x9C0 bytes and flushes that storage. Addresses are observed identities,
not newly invented global boundaries. Static source does not embed game tables.

For each of two program streams, the allocation contribution is 132 words per
128 entries plus ((remainder+4)&~1) words for a nonzero remainder. The main buffer
allocation adds 0x320 bytes; the secondary buffer follows it and has 0xD8 bytes.
The first stream consumes contiguous four-byte instructions. The second uses
only the first word of each eight-byte descriptor. Both split into at most
128-entry groups and align each completed group to eight bytes. Loop bounds are
reloaded after modeled imported calls, as in the original.

Twenty-byte constants provide an eight-bit index and four words whose low
24-bit payloads are packed into three words. Register entries have four unsigned
16-bit fields. At most seven are processed. Semantic 9 is skipped, with no slot
or mask access afterward. Other semantic and slot values are not range-checked
by this root. The source deliberately does not invent validation; bounded tests
use semantic 0..7 or 9 and slot 0..6. Malformed indices beyond those tested remain
unverified and may corrupt memory in the original.

The seven output slots start as 0x1F1F1F1F. Component masks select sequential byte
identifiers from the original mapping data at 003AED28. Used-slot count and
semantic masks construct the remaining command flags. The shared 8-byte mapping
header uses original data 003AED20+0x14, not 003AF078+0x14; a paired replay exposed
and corrected that initial literal-reference transcription error.

Imported call identities come from the unchanged map: __rt_memcpy at 0028BA44,
nngxGetPhysicalAddr at 0010B2BC, and address-named functions 00296048, 00298460,
002284B0, 0029848C. The diagnostic replay supplies explicit fixtures for these
calls. It does not execute their original bodies or validate GPU consumption.

The lib/al compilation module is only an existing ARMCC 4.1/791 source carrier.
It does not prove the original library or compiler identity of this unnamed root.
