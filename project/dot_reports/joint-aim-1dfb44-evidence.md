# Frozen verification evidence

All exact-byte checks below use the unchanged project checker on committed source. Rejected checks and diagnostic-only bindings are intentionally retained. Absolute paths name this lane’s local worktree, not transferable private data.

```json
{
  "source": {
    "checkpoint": "379883a33c831ca48c328af1098a81c60f1f0131",
    "base": "754f99a30a337756df5aa01c28b4ed6a977fecd5",
    "observed_utc": "2026-10-02T03:33:22.150950+00:00",
    "hashes": {
      "lib/al/src/Model/alJointAim1DFB44.cpp": "476e3ff8243018b1ca578df5449ad845dfeef8bc69bcda11a0c1f802151cd3ee",
      "lib/al/include/Model/alJointAim1DFB44.h": "d077579842ba8d4ae0108fa08479ba637e67d0d46c93d06eac8ca343f309a5d5",
      "data/ver/eu/map.csv": "b70e4f37358ebc14aa0c0d522c83172901f23998510a9a2a4e1a36d6cd811881",
      "data/config.json": "5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45",
      "Game/project_globals.h": "6c0b7103d61651e77e1a5db8bb1f5bdf509ad3d64c419c4ba22cadaed6a933d1",
      "make.py": "1d5ba9628e78a4aaa23d6bbbea40131101a46ee41ca43e3df124eaa97d55241a",
      "data/compilers/wibo": "aee836280b3d7c80c2031410219c907c9824c11be6b128d0744655de0f22280b",
      "data/compilers/4.1/791/bin/armcc.exe": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d",
      "data/compilers/4.1/791/bin/armlink.exe": "b9cffb71d7b58f3fd33fa8c4c8af884b75689bb5a9e9014309a08146bba676dc",
      "data/compilers/4.0/902/bin/armcc.exe": "e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe",
      "data/compilers/4.0/902/bin/armlink.exe": "f511d2d087fe0cad6f604ff774b4d75476a7d4949c100ea082eac03a368e967d",
      "tools/romfs.py": "ed09302743a1b1c4f0e25030fba057cd7cc6fcf3e5bf9af6777078df319dbcd5",
      "tools/acceptance_batch.py": "f63eacb2b4914872a8258b41d48f5010733b58b69e046c7c07cb55932eeca0f5",
      "tools/progress.py": "b3a50336b6df7bc63e984bfdc6034675dcc19b3e2adb79d86ad51920bfb5e292",
      "tools/check.py": "e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317",
      "tools/diff.py": "b365cd4efca912dabf4b71dbc28d638bc9624cff5bd0669e11d794aa367831fc",
      "tools/listsyms.py": "d71b18243c22560ebfbc1d84c461b1577861ebae6e2061f82e452544e9eb3e32",
      "tools/checkGameCompiler.py": "1f9e5cf5041a418136b87907e600f0e2a2b3a8996a351755c3b967690f960d31",
      "tools/prepare_family_handoff.py": "828eb0bfe2398c34fb44bbe7affc414258989b5f9582c9314fa4d6ccb3ba64f4",
      "tools/full_image_diagnostic.py": "ca0f3f12e9d7620880e5b30b191aa56b3f57b2345fd62467d700f2e138c42d29",
      "tools/render_progress_image.py": "9982c7a31170f58ad3f031b84e4ea8f594880c4a19bfb55579e3f33eadbe8e9d",
      "tools/splector/split.py": "b0f21b5a167e45552de4115cc0499952b738adbc80cc909637a201b5917d04e7",
      "tools/splector/check.py": "bcd90ffd6af94df57728e011b7d50923ed2e0e8381aa21a596e182a7572e49a7",
      "tools/splector/init.py": "3c3ef492b68aa636002ef777f551f8613f4eace39365f100c6a22bfffd726fc2",
      "tools/splector/_utils.py": "d25711f4f74b87aa4f6665006b6047c57e9592fef166bf8f36e060d4ea8bf23c",
      "tools/low/cfg.py": "b19248345f77132d7f259d7a1ff356757723f4c9525c6159a6bd1f5c1675b4f0",
      "tools/low/glob.py": "ce01e76d545c028d958e0b2569e3a401b6b11c2bfa4a956889cf6095656431b6",
      "tools/low/checkExactBytes.py": "aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2",
      "tools/low/readElfSym.py": "2aa0e53f24ca4e049e4e9f465d2ddbf4703b75d0eade64fc4bddc399aba6408b",
      "tools/low/utilsVersion.py": "4e74a5238c702d806074fd191e9435e436d65d7aa53141906e14b85cd99f2a79",
      "tools/low/getAssembly.py": "fc4f43fa52f8f56112d01dbec3dd40a52577167f086d90a1f3b56f743c842fe1",
      "tools/low/callAsmdiff.py": "8f3489070196faafc8f97560ebf6339c949e7e80ec778bca73ca49fde13c6776",
      "tools/low/readSymMap.py": "c8592056f34af5bc3adb71c8b396e1bfe13884b3662cc5b462761347eb682d7b",
      "tools/low/getSection.py": "80d5d7b66eb2f3137d003de3dca36c7e5939c31a1a6d635ae3c065748369c8f9",
      "tools/low/readElfMap.py": "aa1a56101e27a415988ba6ea2b81b1bcf27e4dd9efab8e6d18372a12f0ae40bc",
      "tools/low/chooser.py": "375bc0e068ca24d8e9bcf08161ba0c727fbd115f82fbd73300ef0e3855c2b84b",
      "tools/low/getSymFile.py": "343d7029ef52b471ddd0a9df049533164521f0926ef9535d66899d1828addec6",
      "tools/low/genScatter.py": "a8ff9eaba736ab43cf50c00e37b6e49c3b96a97b612ad4b639e091fa48b4f906",
      "tools/low/buildProvenance.py": "343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529",
      "tools/low/genComcom.py": "bef90318dd6d126a66b12881fc665afff2818c21b05020e51ba9cd15ba530df8",
      "tools/low/readHeader.py": "727937f03f11abe44f9d670bf8f242d8911e9e9e49b9fd74d01a98bba2360371",
      "tools/low/convBinToElf.py": "2a1df9092776eddd3b2b9a3ce063c9825323ab40bb274ab15de16169c4e6d493",
      "tools/low/utilsPrint.py": "4adfc55f8cc3974349ffa6a59e87b4923826df62fe347303d09cd26fa6079967",
      "tools/low/updateMap.py": "80bb0ccac77b1f939851fbb491802cc182f58100f8114e6926726039591f4b1d",
      "tools/low/genContext.py": "bae8a506e078f3fd58568fa138d6ac270c0a85a70fbc20df2e941879b03c054b",
      "tools/low/genObjdiff.py": "b2627bad72f96ead04f14bd9111ab51a1676e2ae19e01b2c7d85dd335a1335d7",
      "tools/pypstem/stepExport.py": "c2429b0ea8fa3f260f29b5e42e6d9163fc6baebc594860924cf9a54bacbd393d",
      "tools/pypstem/defaultFlags.py": "6167825c47ca7ad8f6ba4c27568aab4e85703b1ef0d8fa4c3f18417d53694548",
      "tools/pypstem/manSetup.py": "1f16c5b5fb214a1bdaef2273e71a0138ce55eae9807bda2b9bb942a0b7fe5aed",
      "tools/pypstem/stepBuild.py": "b7be5977a0a1afc339eb4c9087b447cdc25b1c42daa3fe23af4b765008b0aac0",
      "tools/pypstem/_utils.py": "a27b180c94f49d4f431b5070a6c0ba0006e2902086e699e0037035738c4fb64d",
      "tools/pypstem/callProcess.py": "14c756f0649eab1ae65ea184febc914c3f942436557fb185fdf6b8e51de66623",
      "tools/pypstem/stepCheck.py": "53c83bd0d9a3292108a4946c58cdba6ca543ae99147ee5690783abc3712c92a4",
      "tools/pypstem/stepLink.py": "2d5abfefb6d8150d1ea4309353442375b80f7acbd6a66a848e6a9c5e5285c877",
      "tools/pypstem/stepSplit.py": "64396e0207f2c2971cf6fabbf109ec852dbe0ae9b7dc5624b4a533a08af30093",
      "tools/pypstem/main.py": "cb7a3c49b937eb350b57ae39ec52d18c91bbe931f6cc696c131a7c79094552cd",
      "tools/pypstem/user/initMap.py": "9ed8043394b7f384204c584087a585d793bae249aa05db0dd8e30df306f598e6"
    },
    "candidate_build_provenance": {
      "schema": 1,
      "build_step": "tools.pypstem.stepBuild",
      "language": "C++",
      "source": "lib/al/src/Model/alJointAim1DFB44.cpp",
      "object": "build/eu/obj/lib/al/src/Model/alJointAim1DFB44.o",
      "version": "eu",
      "compiler": "data/compilers/4.1/791/bin/armcc.exe",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d",
      "command": [
        "/workspace/scratch/73cdb2c524af/mario-root-1dfb44/data/compilers/wibo",
        "/workspace/scratch/73cdb2c524af/mario-root-1dfb44/data/compilers/4.1/791/bin/armcc.exe",
        "-I/workspace/scratch/73cdb2c524af/mario-root-1dfb44/Game/backup/include",
        "-I/workspace/scratch/73cdb2c524af/mario-root-1dfb44/lib/al/include",
        "-I/workspace/scratch/73cdb2c524af/mario-root-1dfb44/lib/CtrSDK/include",
        "-I/workspace/scratch/73cdb2c524af/mario-root-1dfb44/lib/sead/include",
        "-DVERSION=EU",
        "-DNN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK=1",
        "-DNN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK=1",
        "-DNON_MATCHING=1",
        "--cpu=MPCore",
        "--fpmode=fast",
        "--apcs=/interwork",
        "--depend_format=unix_quoted",
        "--diag_suppress=1608",
        "--arm_only",
        "--no_exceptions",
        "--diag_style=gnu",
        "--arm_only",
        "--no_exceptions",
        "--diag_style=gnu",
        "--signed_chars",
        "--dollar",
        "--force_new_nothrow",
        "--no_rtti",
        "--no_debug_macros",
        "--no_depend_system_headers",
        "-O3",
        "-Otime",
        "--gnu",
        "--split_sections",
        "--force_new_nothrow",
        "--multibyte_chars",
        "--enum_is_int",
        "--signed_chars",
        "--no_rtti_data",
        "--forceinline",
        "--remove_unneeded_entities",
        "--no_debug",
        "--preinclude=/workspace/scratch/73cdb2c524af/mario-root-1dfb44/Game/project_globals.h",
        "--sys_include",
        "-c",
        "-D__BASE_FILE_NAME__=\"alJointAim1DFB44.cpp\"",
        "-o",
        "/workspace/scratch/73cdb2c524af/mario-root-1dfb44/build/eu/obj/lib/al/src/Model/alJointAim1DFB44.o",
        "--depend",
        "/workspace/scratch/73cdb2c524af/mario-root-1dfb44/build/eu/obj/lib/al/src/Model/alJointAim1DFB44.d",
        "/workspace/scratch/73cdb2c524af/mario-root-1dfb44/lib/al/src/Model/alJointAim1DFB44.cpp"
      ],
      "inputs": {
        "lib/CtrSDK/include/nn/types.h": "40aa4e5ee7d8980e077963ee87bc6bdeb8908249603c073b5980aed937d5e0bc",
        "lib/sead/include/math/seadMatrix.h": "c4ecd6d6b98b4851e81d9fd144de80b51ca13f4370fb2baf5982a42f6e69d914",
        "Game/project_globals.h": "6c0b7103d61651e77e1a5db8bb1f5bdf509ad3d64c419c4ba22cadaed6a933d1",
        "data/config.json": "5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45",
        "lib/al/src/Model/alJointAim1DFB44.cpp": "476e3ff8243018b1ca578df5449ad845dfeef8bc69bcda11a0c1f802151cd3ee",
        "lib/sead/include/math/seadVector.h": "1416056d4201ee62fdc2d2c06b5bd418f3195f8154a74b122869ff8c5f517169",
        "lib/sead/include/math/seadQuat.h": "84b3de2e49e5f0505d26ae1e9f5acf782b446507254c6b0e94657350cd1355be",
        "lib/al/include/Model/alJointAim1DFB44.h": "d077579842ba8d4ae0108fa08479ba637e67d0d46c93d06eac8ca343f309a5d5"
      },
      "inputs_stable": true,
      "object_sha256": "bfd51a2f9db710445d141edd65c15e6e80401e8dc961437922cf9f82f35ea0df"
    },
    "compiled_sources": {
      "lib/al": 133,
      "lib/CtrSDK": 1,
      "Game": 45
    },
    "compiler_894_present": false
  },
  "check_first": {
    "source_commit": "379883a33c831ca48c328af1098a81c60f1f0131",
    "seconds": 1.362815328000579,
    "returncode": 1,
    "output": "\u001b[38;5;221mSource closure rejected: An unresolved source helper is referenced by a non-branch relocation.\u001b[0m\u001b[K\n"
  },
  "check_final": {
    "source_commit": "379883a33c831ca48c328af1098a81c60f1f0131",
    "seconds": 1.2226607769989641,
    "returncode": 1,
    "output": "\u001b[38;5;221mSource closure rejected: An unresolved source helper is referenced by a non-branch relocation.\u001b[0m\u001b[K\n"
  },
  "clean_build": {
    "returncode": 0,
    "seconds": 35.59181663500203,
    "source_commit": "379883a33c831ca48c328af1098a81c60f1f0131"
  },
  "definitions": [
    {
      "symbol": "fn_001DFB44",
      "size": 2388,
      "kind": "STB_GLOBAL"
    }
  ],
  "imports": [
    {
      "symbol": "_ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f",
      "address": 2608228,
      "kind": "A",
      "end": 2608252,
      "canonical_enrolled": true
    },
    {
      "symbol": "_ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_",
      "address": 2607944,
      "kind": "A",
      "end": 2607972,
      "canonical_enrolled": true
    },
    {
      "symbol": "_ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_",
      "address": 2607972,
      "kind": "A",
      "end": 2608000,
      "canonical_enrolled": true
    },
    {
      "symbol": "_ZN4sead14Vector3CalcCtrIfE6rotateERN2nn4math4VEC3ERKNS3_5MTX34ERKS4_",
      "address": 2608140,
      "kind": "A",
      "end": 2608200,
      "canonical_enrolled": true
    },
    {
      "symbol": "_ZN4sead15Matrix34CalcCtrIfE12makeIdentityERN2nn4math5MTX34E",
      "address": 2607324,
      "kind": "A",
      "end": 2607360,
      "canonical_enrolled": true
    },
    {
      "symbol": "_ZN4sead15Matrix34CalcCtrIfE5makeQERN2nn4math5MTX34ERKNS3_4QUATE",
      "address": 2557816,
      "kind": "A",
      "end": 2558020,
      "canonical_enrolled": true
    },
    {
      "symbol": "_ZN4sead15Matrix34CalcCtrIfE8multiplyERN2nn4math5MTX34ERKS4_S7_",
      "address": 2607004,
      "kind": "A",
      "end": 2607232,
      "canonical_enrolled": true
    },
    {
      "symbol": "_ZN4sead4QuatIfE4unitE",
      "address": 4392192,
      "kind": "D",
      "end": 4392208,
      "canonical_enrolled": true
    },
    {
      "symbol": "_ZN4sead7Vector3IfE2exE",
      "address": 4392392,
      "kind": "D",
      "end": 4392404,
      "canonical_enrolled": false
    },
    {
      "symbol": "_ZN4sead7Vector3IfE2eyE",
      "address": 4392404,
      "kind": "D",
      "end": 4392416,
      "canonical_enrolled": false
    },
    {
      "symbol": "fn_0024AE40",
      "address": 2403904,
      "kind": "A",
      "end": 2404272,
      "canonical_enrolled": true
    },
    {
      "symbol": "fn_0026A958",
      "address": 2533720,
      "kind": "A",
      "end": 2533816,
      "canonical_enrolled": true
    },
    {
      "symbol": "fn_0026E82C",
      "address": 2549804,
      "kind": "A",
      "end": 2550116,
      "canonical_enrolled": true
    },
    {
      "symbol": "fn_0026F7F8",
      "address": 2553848,
      "kind": "A",
      "end": 2554148,
      "canonical_enrolled": true
    },
    {
      "symbol": "fn_00270844",
      "address": 2558020,
      "kind": "A",
      "end": 2558108,
      "canonical_enrolled": true
    },
    {
      "symbol": "fn_0027306C",
      "address": 2568300,
      "kind": "A",
      "end": 2568340,
      "canonical_enrolled": true
    },
    {
      "symbol": "fn_0027A488",
      "address": 2598024,
      "kind": "A",
      "end": 2598104,
      "canonical_enrolled": true
    },
    {
      "symbol": "fn_0027D4D8",
      "address": 2610392,
      "kind": "A",
      "end": 2610464,
      "canonical_enrolled": true
    },
    {
      "symbol": "fn_0027D5C4",
      "address": 2610628,
      "kind": "A",
      "end": 2610716,
      "canonical_enrolled": true
    },
    {
      "symbol": "fn_00287908",
      "address": 2652424,
      "kind": "A",
      "end": 2652880,
      "canonical_enrolled": true
    }
  ],
  "diagnostic": {
    "diagnostic_only": true,
    "project_checker_exact": false,
    "source_generated_bytes": 2388,
    "original_complete_bytes": 2456,
    "overlap_differing_bytes": 2106,
    "unpaired_bytes": 68,
    "source_interval_sha256": "6b8ee4da93aca1fd15b6c3a050fad1e3d23dfe7cd620004a02a2b5939c00a0a6",
    "original_interval_sha256": "ddcc5b159c6055b8d1119fb27ea4bdc7115d34e1b3eca3290f42f90a7ae67679",
    "imports_without_canonical_rows": [
      "sead::Vector3<float>::ex",
      "sead::Vector3<float>::ey"
    ],
    "command": [
      "data/compilers/wibo",
      "data/compilers/4.1/791/bin/armlink.exe",
      "--cpu=MPCore",
      "--fpu=VFPv2",
      "--arm_only",
      "--no_exceptions",
      "--inline",
      "--datacompressor=off",
      "--no_debug",
      "--no_scanlib",
      "--mangled",
      "--symbols",
      "--map",
      "--entry=fn_001DFB44",
      "--keep=fn_001DFB44",
      "--ro_base=0x001DFB44",
      "--output=build/root-1dfb44/diagnostic-original-address.axf",
      "--list=build/root-1dfb44/diagnostic-original-address.map",
      "build/eu/obj/lib/al/src/Model/alJointAim1DFB44.o",
      "build/root-1dfb44/imports.sym"
    ]
  },
  "replay": {
    "cases": 575,
    "outcomes": {
      "return": 569,
      "fault": 6
    },
    "failures": [],
    "total_steps": 1418266,
    "seconds": 17.997137517995725,
    "original_root_instructions_visited": 604,
    "models": [],
    "faults": [
      {
        "name": "null-self",
        "status": "fault",
        "original_steps": 6,
        "candidate_steps": 6,
        "original_pc": 1964888,
        "candidate_pc": 5242900,
        "failures": []
      },
      {
        "name": "null-matrix",
        "status": "fault",
        "original_steps": 26,
        "candidate_steps": 24,
        "original_pc": 1964968,
        "candidate_pc": 5242972,
        "failures": []
      },
      {
        "name": "null-buffer",
        "status": "fault",
        "original_steps": 23,
        "candidate_steps": 21,
        "original_pc": 1964956,
        "candidate_pc": 5242960,
        "failures": []
      },
      {
        "name": "negative-first",
        "status": "fault",
        "original_steps": 23,
        "candidate_steps": 21,
        "original_pc": 1964956,
        "candidate_pc": 5242960,
        "failures": []
      },
      {
        "name": "oversize-first",
        "status": "fault",
        "original_steps": 23,
        "candidate_steps": 21,
        "original_pc": 1964956,
        "candidate_pc": 5242960,
        "failures": []
      },
      {
        "name": "oversize-count-budget",
        "status": "fault",
        "original_steps": 8687,
        "candidate_steps": 7773,
        "original_pc": 1964956,
        "candidate_pc": 5242960,
        "failures": []
      }
    ]
  },
  "preservation": {
    "baseline_checkpoint": "754f99a30a337756df5aa01c28b4ed6a977fecd5",
    "baseline_report_sha256": "c208f39fadf49e66c5c98c97b445881ba2e51ade07b5724610cf0309175ec028",
    "roots": 733,
    "definitions": 753,
    "prior_objects": 178,
    "raw_equal_objects": 0,
    "allocated_equal_objects": 178,
    "unchanged_inputs": 370,
    "extra_sources": [
      "lib/al/src/Model/alJointAim1DFB44.cpp"
    ],
    "seconds": 22.900381090003066,
    "map_sha256": "b70e4f37358ebc14aa0c0d522c83172901f23998510a9a2a4e1a36d6cd811881",
    "scope": "Exhaustive unchanged-input and compiler-object equivalence to the separately verified 733-root/753-definition baseline. This is not a new canonical 753-check run."
  },
  "evidence_file_sha256": {
    "link.json": "70ec2b738752007fc7795c0316b91b2de8e98a06c93f1b10601c07a62636dca9",
    "check-form1.json": "7070bb6933e48c2fe42a7c060e37f3f09a481ccd9ff7aaec736cd8cf83e2f757",
    "diagnostic-distance.json": "42656b766bb63fe18f72286c7011226f1ab24ee05a60f89b910e5a1741e9ae68",
    "replay-form1.json": "1629378cce1be40d36dbb97de1dda0c564f757225af838f9d942e8434129c274",
    "definitions.json": "4be68fc8ea514a71c795e43f9884092dcbda9297fe10a52df6aeafdf3f491172",
    "initializer.json": "416cff4128e4b00bf711d659f1867fb6039008fac043a4e2eec577a0502ec644",
    "replay-expanded.json": "0320eae60b4310232cf41ea74889ab2986979786832f25599c2d5e489d8b3d34",
    "imports.json": "f7b459f7b6ef2054962befb24c1ca3cd5b6f1e0634c6e19e86b54f8d5c476bf2",
    "preservation-equivalence.json": "f445a133658ce3abc8552958039f7a1bffcdde94473458dc4629f1befa20c08d",
    "clean-build.json": "98420ef45556320293658494daf458e043f71a1b8af28c50f296628f955fde79",
    "evidence.json": "4f313a363045f76ba3e6e083b98107e142adc95c85fb7540394e5f7c9a855926",
    "replay-final.json": "d6d10ff8c730749eb9abb10fc1dcb31a34ad1ebfdd34eaff3d4ab74240c8273c",
    "check-final.json": "cfc604527d9e3d8044d709a0b0881587928f0ed3713af0c00b8ab628816eb647"
  }
}
```
