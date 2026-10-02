# Original startup-provider observation correction

The original JSON observation descriptions swapped two startup-channel offsets. Function00265128 loads controller+0x2C at26513C/265160. Function0024FCB8 loads controller+0x30 at24FCCC/24FCF0. Their complete original decoded instructions were already correct. Their neutral names, actor/name boolean declarations and ready call order remain unchanged.

This correction supersedes only those two descriptions. The original32-path review seal remains immutable. All six separate presence-provider slot observations and the CPP-local prefix remain correct. No source, header, ABI, map row or metadata patch change.
