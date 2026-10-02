# Bunbun initialization matrix needs a row

Branch: dot/root-169234. Base: 713975727c0447ea8cdea708a9ab13cc539a92d0.
Target: 00169234, 552 bytes, U. Exact claims: none.
Missing address: 00430A88. No exact or containing row exists in the frozen map.
At 00169270 and 0016927C, this target supplies the address to mapped Matrix34CalcCtr<float>::copy at 0027C18C.
That helper reads and copies twelve floats, establishing a 0x30-byte minimum readable span. Two destination matrices at actor +0x64/+0x94 do not imply two source objects.
The matrix semantic name, independent owner, and complete allocation extent remain unproved.
No source, build, canonical check, or replay was attempted. Integrator should establish the justified data row before retrying.
