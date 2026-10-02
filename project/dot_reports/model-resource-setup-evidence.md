# Model/resource setup sealed evidence

These records contain hashes, fixture parameters and measured results, not game
bytes. Runtime-only objects remain ignored local artifacts. No canonical acceptance
or combined integration is claimed.

## check-result.json

```json
{
  "returncode": 1,
  "stdout": "\u001b[38;5;221mU -> M: The complete compiled section, including its literal pool, has a different size from the original interval.\u001b[0m\u001b[K\n",
  "stderr": "",
  "map_restored": true,
  "map_sha256": "94ac5673e907db521bf6b4b606d02be64ee7bae8a8fd2f08328b0c30d85b3db9"
}
```

## clean-build-timing.json

```json
{
  "start_utc": "2026-10-02T06:03:16.475924+00:00",
  "end_utc": "2026-10-02T06:03:56.721320+00:00",
  "seconds": 40.24540196299495,
  "returncode": 0
}
```

## guard-byte-preservation.json

```json
[
  {
    "object": "build/model-resource-setup/history/form1/ModelResourceSetup167CAC.o",
    "size": 2064,
    "section_sha256": "c01019db6e1876bf964f4feb4853615cd89f3d2672eed4d041f96049dd909591"
  },
  {
    "object": "build/eu/obj/lib/al/src/ModelResourceSetup167CAC.o",
    "size": 2064,
    "section_sha256": "c01019db6e1876bf964f4feb4853615cd89f3d2672eed4d041f96049dd909591"
  }
]
```

## physical-history.json

```json
{
  "meaningful_forms": 1,
  "physical_clean_compiles": 2,
  "canonical_checks": 2,
  "source_commits": [
    "607feb1",
    "663c7bc"
  ],
  "publication": "held",
  "pre_guard_artifacts": {
    "history/form1/ModelResourceSetup167CAC.o": "91ca161164753e24adf83245212662d0af0afb2f6da488858ba2cc0271301339",
    "history/form1/check-result.json": "b98626a9f322a8ddd2d6629f77465a4a58f7c5cd33797cee7e9fea36e38d857f",
    "history/form1/ModelResourceSetup167CAC.provenance.json": "a7cf5a05d8ae18ecdfd18c667fda0768bd86b103cd49c38bc2ffc22edfc7e501",
    "history/form1/build.log": "52235c89f1c80f748737fe47365cd69d3631103386aa74d9dd65f80ff007f50e",
    "history/form1/preservation.json": "9d8b474c3d23842b7e49de90eacfc7645c13a9d7f15a687033e1c034d1f2b1f5",
    "history/form1/replay-results.json": "ca8017aa91cf8ac39ca391ae380d8125eb67df0e1e9bfd1ebd2c645cdd2982cc"
  },
  "evidence_script_failures": [
    {
      "step": "initial wrapper",
      "result": "AssertionError on copied wrong row before map mutation or canonical checker call"
    },
    {
      "step": "initial audit",
      "result": "FileNotFoundError data/ver/eu/config.json; no such optional config exists",
      "traceback_sha256": "017b3c86b7df76add61b8452b44cb6e42823ec6947564b774a5ad707e39422e0"
    },
    {
      "step": "readelf inspection",
      "result": "arm-none-eabi-readelf command unavailable; pyelftools used"
    }
  ]
}
```

## audit.json

```json
{
  "timestamp_utc": "2026-10-02T06:04:59.897169+00:00",
  "base": "86104d96a7f570383bbfefc5fffad998e134496c",
  "source_sha256": "05f3ef02cccf219d934e6ca6cc42d9b1dbfe15f1ce64d7b7ab04bf96558a0d6a",
  "object_sha256": "91ca161164753e24adf83245212662d0af0afb2f6da488858ba2cc0271301339",
  "provenance_sha256": "9e70f1c2a8fa6a110b2d5f5331ce1104d4f4124b4ab08f043faa4f537afccf0e",
  "candidate_complete_section_bytes": 2064,
  "original_complete_bytes": 2116,
  "original_interval_sha256": "94c2b514a0cb15bfe390672d19369b00f17ee11bf2e71b2389fb51abae96f3fc",
  "original_pool_sha256": "cc9367de3f9ff68601807c8c798685d8f1cad3f2bf1d6be9cc4a55df3434f759",
  "original_input_sha256": "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64",
  "definitions": [
    {
      "name": "fn_00167CAC",
      "size": 2064,
      "section": "i.fn_00167CAC"
    }
  ],
  "allocated_sections": [
    {
      "name": "i.fn_00167CAC",
      "bytes": 2064,
      "sha256": "c01019db6e1876bf964f4feb4853615cd89f3d2672eed4d041f96049dd909591"
    },
    {
      "name": ".ARM.exidx",
      "bytes": 8,
      "sha256": "01acecb507abfe1a354aa8064f4af5d3f1acd019e37db3c11c97523b71c76e9d"
    }
  ],
  "imports": [
    "dat_003A2D40",
    "dat_003E26CC",
    "fn_0025C334",
    "fn_0028ECAC",
    "fn_0028EED4",
    "fn_002A9DC4",
    "fn_002AA23C"
  ],
  "root_relocations": [
    {
      "offset": 1036,
      "type": 28,
      "symbol": "fn_0028EED4"
    },
    {
      "offset": 1128,
      "type": 28,
      "symbol": "fn_0025C334"
    },
    {
      "offset": 1392,
      "type": 28,
      "symbol": "fn_002AA23C"
    },
    {
      "offset": 1408,
      "type": 28,
      "symbol": "fn_002AA23C"
    },
    {
      "offset": 1988,
      "type": 28,
      "symbol": "fn_002A9DC4"
    },
    {
      "offset": 2024,
      "type": 28,
      "symbol": "fn_0028ECAC"
    },
    {
      "offset": 2044,
      "type": 2,
      "symbol": "dat_003E26CC"
    },
    {
      "offset": 2060,
      "type": 2,
      "symbol": "dat_003A2D40"
    }
  ],
  "tool_input_hashes": {
    "tools/check.py": "e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317",
    "tools/low/checkExactBytes.py": "aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2",
    "tools/low/buildProvenance.py": "343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529",
    "data/config.json": "5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45",
    "data/ver/eu/map.csv": "94ac5673e907db521bf6b4b606d02be64ee7bae8a8fd2f08328b0c30d85b3db9",
    "make.py": "1d5ba9628e78a4aaa23d6bbbea40131101a46ee41ca43e3df124eaa97d55241a",
    "data/compilers/4.1/791/bin/armcc.exe": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d",
    "data/compilers/4.1/791/bin/armlink.exe": "b9cffb71d7b58f3fd33fa8c4c8af884b75689bb5a9e9014309a08146bba676dc",
    "data/compilers/wibo": "aee836280b3d7c80c2031410219c907c9824c11be6b128d0744655de0f22280b",
    "build/model-resource-setup/check.py": "598f8051d7635390f2584800407721d4d25e2c1eaa966602fd54e9641a12553e",
    "build/model-resource-setup/link.py": "2a38be4e3ba36817e97cac457f9aa741af03744dc8a19016e5c526ccfd264423",
    "build/model-resource-setup/replay.py": "e447a93827b3859ed89be7dd8ccd3f77e6ab8d00decdf1c3234e3d57f9e3e577",
    "build/model-resource-setup/preserve.py": "c3fb8fad7766f571b0c8239d88ba93deb9b4de4b5b384106d1b2bb540d2be8fb",
    "build/model-resource-setup/audit.py": "b9dca42b7c6b8d166ffba0355afc2e250fff006eb2d3f77623d52b62c3f5b4ea"
  },
  "declaration_audit": {
    "working_trees": 118,
    "heads": {
      "/workspace/scratch/73cdb2c524af/mario-dot": "15b86c1261540e76259cb58f2ad053d8b2ed349a",
      "/workspace/scratch/73cdb2c524af/mario-20fc34": "9afb977aace02ba36176b2b04ec50a12cc39eedc",
      "/workspace/scratch/73cdb2c524af/mario-actor-scale": "6446a5bfb53b2ea2183f04745e5977ead9a97ee8",
      "/workspace/scratch/73cdb2c524af/mario-audio-disable": "6518b13c5f88ed6d6b6f458c3be35407eecd879a",
      "/workspace/scratch/73cdb2c524af/mario-beat-holder": "3795d021f883d46fb69cead8a82243ace2d03002",
      "/workspace/scratch/73cdb2c524af/mario-block-dragon": "d47fe01cf42d89aae7acbf1306f47ed0526d4426",
      "/workspace/scratch/73cdb2c524af/mario-bomb-hei-control": "4f2a1c0403218badcad1aed9975fe6cc2bc1c19e",
      "/workspace/scratch/73cdb2c524af/mario-bomb-product-layout": "81e753ac52ffe72caa07c54ab797b19082fc1d45",
      "/workspace/scratch/73cdb2c524af/mario-bubble-init-literals": "cbf8ff77d73a7439a8604f480b8fa3968e822ce7",
      "/workspace/scratch/73cdb2c524af/mario-builder-source": "958b879f4e2a6980505695fcacc2224935249811",
      "/workspace/scratch/73cdb2c524af/mario-byaml-component": "0a693e93508e8bda2ac8de8a34c613fc59fdd3d5",
      "/workspace/scratch/73cdb2c524af/mario-byaml-key": "f77d544888650d27eafcb5c73690184246cab624",
      "/workspace/scratch/73cdb2c524af/mario-byaml-string-getter": "ef6f03542b3d39e343f9652b39198e07cda03529",
      "/workspace/scratch/73cdb2c524af/mario-cfl-texture-root": "325a26b44eced9173b6a9f08fabe3d200a0473eb",
      "/workspace/scratch/73cdb2c524af/mario-collider-b16-recheck": "5e5c56f137e31313109ffc4c38ce34b1225e7561",
      "/workspace/scratch/73cdb2c524af/mario-collider-invalidation": "22b76f7a1df175170a24fe835e5a0f33172390c7",
      "/workspace/scratch/73cdb2c524af/mario-course-residual": "7ccde0ac3c5d0118800a65372d1382dabf6149c9",
      "/workspace/scratch/73cdb2c524af/mario-course-select-scene-init": "8e084bc0835a4a8796216cce5bf981537af54546",
      "/workspace/scratch/73cdb2c524af/mario-cube-intersection": "531f1b944c41fed92651305bfdeeb637eb53ab07",
      "/workspace/scratch/73cdb2c524af/mario-current": "684e2381159b3e9a3444893f581840582a46d6ee",
      "/workspace/scratch/73cdb2c524af/mario-current-preservation": "36a86e9b72b5b7cd90d0f853871ba308cada32ed",
      "/workspace/scratch/73cdb2c524af/mario-display-init": "1fdb12fcbc747e87a8b5db4b95d31ccfae6145b5",
      "/workspace/scratch/73cdb2c524af/mario-effect-binding": "3e26956d51589a1df4d3bb0121794c198148ea2b",
      "/workspace/scratch/73cdb2c524af/mario-effect-math-import-8991": "48dc3ccbd958e33cfecef040c43f283f62a09c3c",
      "/workspace/scratch/73cdb2c524af/mario-effect-resource-init": "9a6d9788ab300d3b6a1c3ea63f430d4520e28880",
      "/workspace/scratch/73cdb2c524af/mario-emitter-transform": "00928bd3eb7a782caffde77db68845d32360f7b5",
      "/workspace/scratch/73cdb2c524af/mario-executor-kit": "8c0dd474200d2d7bbf65b9b0b361a5f803a40698",
      "/workspace/scratch/73cdb2c524af/mario-executor-residual": "08f91ec88c956712bd97c62b9182d5b3579afdae",
      "/workspace/scratch/73cdb2c524af/mario-factory-identifiers": "a716a73b7f35c8a6f01ff9820bcd4bb08380bbb8",
      "/workspace/scratch/73cdb2c524af/mario-file-handle-read-odr": "7d8afdb6230960e2504ac686ef7fd2f71d5daca9",
      "/workspace/scratch/73cdb2c524af/mario-fixed-point": "a749e484199d897552c271e571446f3cd32eb2aa",
      "/workspace/scratch/73cdb2c524af/mario-float-state-root": "1c6f030b5c64ce09cd0a00695ee4218888cfb88b",
      "/workspace/scratch/73cdb2c524af/mario-float-validator": "4b0a9128f688b933df7eea2aa202f6253dcb416a",
      "/workspace/scratch/73cdb2c524af/mario-graphics-integration": "2fe0650d8e667b59a450d11ae256799722a9562d",
      "/workspace/scratch/73cdb2c524af/mario-intake": "c2de8c23738a0ca8f4858869565e9b45626dd052",
      "/workspace/scratch/73cdb2c524af/mario-integer-state": "1ec65fc9013a6782fd5f3dcccbb705a4eec83834",
      "/workspace/scratch/73cdb2c524af/mario-item-holder-family": "bdf89e53cf9a21e88380b335813d6d6e61c950f1",
      "/workspace/scratch/73cdb2c524af/mario-item-holder-ownership": "8df182f6163cd451369555eb712746f9553a2a08",
      "/workspace/scratch/73cdb2c524af/mario-item-spawn-dispatcher": "a76a8e762ebbb751dac23e484ebf1e82a25bf69f",
      "/workspace/scratch/73cdb2c524af/mario-jpeg-root": "32cc8f9cd90fdd466db551a3a395fff46ea4b506",
      "/workspace/scratch/73cdb2c524af/mario-keeper-residual-8991": "91070b64879b061fe8540213a4b36640f35c36f4",
      "/workspace/scratch/73cdb2c524af/mario-largest-current": "f15d147ee0d9b25dc8933f16dcea1eb6b9693715",
      "/workspace/scratch/73cdb2c524af/mario-largest-root": "29310b0d4c1d68953778330ab57ed5dff7241352",
      "/workspace/scratch/73cdb2c524af/mario-latest": "6b74a275309173bde0164b87b86ddba33f8263d3",
      "/workspace/scratch/73cdb2c524af/mario-list-clear-audit": "d3a5f3db1339b360aa6674bc4e0c289adcc033ea",
      "/workspace/scratch/73cdb2c524af/mario-list-sentinel": "493cb648893c2c5be4ad21457b808ab211bfc743",
      "/workspace/scratch/73cdb2c524af/mario-main21b": "f41cb6229a64e3238c83b469a951444e3dd9f129",
      "/workspace/scratch/73cdb2c524af/mario-main3de": "3de056e0dcb33619c32f8b46ef64f74c90d28934",
      "/workspace/scratch/73cdb2c524af/mario-main45a": "45a4305466a7c4cbd87589169384102e88f8d1b5",
      "/workspace/scratch/73cdb2c524af/mario-main48": "8cf3391f553e6daacde91b42b371e63a6aab36fc",
      "/workspace/scratch/73cdb2c524af/mario-main4f70": "bbc659c1bcae2970990c8bdde4ad82e0c2576153",
      "/workspace/scratch/73cdb2c524af/mario-main5025": "5025a6cd5ec8531fb1bc40ff4b570a0c01ef201c",
      "/workspace/scratch/73cdb2c524af/mario-main54d": "54d39734c4edffeeac012157ab71fb11c40d815b",
      "/workspace/scratch/73cdb2c524af/mario-main57f": "57f902421f6874f132d5ea971d1aa5b224c5a528",
      "/workspace/scratch/73cdb2c524af/mario-main754": "754f99a30a337756df5aa01c28b4ed6a977fecd5",
      "/workspace/scratch/73cdb2c524af/mario-main861": "86104d96a7f570383bbfefc5fffad998e134496c",
      "/workspace/scratch/73cdb2c524af/mario-main8991": "8991bec8cb27ff1a2dac0003f059d270a0b17493",
      "/workspace/scratch/73cdb2c524af/mario-main8ca": "b294c8e8c3424f9893407c339347b1ce31863437",
      "/workspace/scratch/73cdb2c524af/mario-maina4f": "a4f1579f9f0f13dacfe91d92501cf74154c131ad",
      "/workspace/scratch/73cdb2c524af/mario-maina74": "a74ef6247dcf52073d9b05fd0e091aa7e3579563",
      "/workspace/scratch/73cdb2c524af/mario-mainb16": "b16e2a0cc32e0cdacac5ba94feabe67432433391",
      "/workspace/scratch/73cdb2c524af/mario-mainbc5": "bc5b236802b0300bc6b00f0ac55cf5ae5749c55c",
      "/workspace/scratch/73cdb2c524af/mario-mainc2e": "c2e343e1d07a8b2d16b2c86bbf7b5bb9f65edda2",
      "/workspace/scratch/73cdb2c524af/mario-maine2": "e2cbf8db72566da78fc16b77a5b90026bfd9ea0a",
      "/workspace/scratch/73cdb2c524af/mario-maine87": "e87d556cd4d993606a03e205b4604d863d718bd4",
      "/workspace/scratch/73cdb2c524af/mario-mainf1": "58499c26f836bacfbe5d271642e57c4178d04561",
      "/workspace/scratch/73cdb2c524af/mario-matrix-inverse": "5aa3339c953d03f10548494b7bb217964ec90948",
      "/workspace/scratch/73cdb2c524af/mario-matrix-native": "87b07eeadb252bd8b66f4781efe53a1d8b9a94f2",
      "/workspace/scratch/73cdb2c524af/mario-method-tree": "71ad7c479dfbad99a9ff71fa02bcbb607d30276b",
      "/workspace/scratch/73cdb2c524af/mario-model-resource-setup": "663c7bc80cdb2251ba744cadcfd83089456a9767",
      "/workspace/scratch/73cdb2c524af/mario-note-generator": "e70e77a66f4768bd5b7d31469ef60571fef161d6",
      "/workspace/scratch/73cdb2c524af/mario-packed-state": "51d9a0c1459963917a2a2f73051fa8af11ed6728",
      "/workspace/scratch/73cdb2c524af/mario-packets-a74": "4ff711bdcb4304008813f6cfbdf5e609a15bab2e",
      "/workspace/scratch/73cdb2c524af/mario-particle-editor-parameters": "646a14e2d07ff73717ac524403ca470e271067f3",
      "/workspace/scratch/73cdb2c524af/mario-particle-shader-setup": "86104d96a7f570383bbfefc5fffad998e134496c",
      "/workspace/scratch/73cdb2c524af/mario-particle-shape-2a451c": "d65414a32464f5d68cf920d6fdfb6c66a884e645",
      "/workspace/scratch/73cdb2c524af/mario-pending-exact-integration": "f8e6da9a4b701cb143daf63c89d069218db34c7f",
      "/workspace/scratch/73cdb2c524af/mario-player-actor-family": "1837a2f4d9a7b95a5406d8e8d14683b646b909cc",
      "/workspace/scratch/73cdb2c524af/mario-program-state": "a67da622a95949b7213e6475c19cc4a5e4f3897d",
      "/workspace/scratch/73cdb2c524af/mario-proposal-preservation-report": "5c9c99281729d9595422e79859551bb4c9dc8801",
      "/workspace/scratch/73cdb2c524af/mario-quat-matrix": "10ed1be60097f7d1bd138c20ccbae176fde26914",
      "/workspace/scratch/73cdb2c524af/mario-rail-rider": "27047bf018fc827918a6da4878f83c0a8423f8a0",
      "/workspace/scratch/73cdb2c524af/mario-resource-298c58": "0a3ea889700c86b90551aabbd90ab29d4e55d829",
      "/workspace/scratch/73cdb2c524af/mario-resource-particle-family-8991": "6f854591fe3370ef10b5976f5ee11ca0024eb7a1",
      "/workspace/scratch/73cdb2c524af/mario-root-130438": "ee962a4a096d6001d65211dacd735cbf7ed252a3",
      "/workspace/scratch/73cdb2c524af/mario-root-14414c": "7914b8ef3d18216a8ac6e3ec3b0a5cfc62c68673",
      "/workspace/scratch/73cdb2c524af/mario-root-1c6410": "1b0c0507e6845b5cd9d10c6d545aa8c85bd9af4c",
      "/workspace/scratch/73cdb2c524af/mario-root-1dfb44": "283ae00173fe4224e810e04f9d2ff4c0cd4c1fe3",
      "/workspace/scratch/73cdb2c524af/mario-root-1eeba8": "93e3e9817771d1512986ecdbc07d40cc9569e927",
      "/workspace/scratch/73cdb2c524af/mario-root-22b6e8": "25b50ee2b2433bef3777f2033537ed7f755045ae",
      "/workspace/scratch/73cdb2c524af/mario-root-22f804": "240e6ce111ebe3e50a8e74caea801919b62cf98a",
      "/workspace/scratch/73cdb2c524af/mario-root-230794": "077c6a475a125216a4fac977c61229ebe44e138f",
      "/workspace/scratch/73cdb2c524af/mario-root-269a60": "868df7197ed2f307c6fa20e93c3622974875a981",
      "/workspace/scratch/73cdb2c524af/mario-root-275828": "cd620f97827f8a0ea3c48aeb92c76660f639f613",
      "/workspace/scratch/73cdb2c524af/mario-root-280798": "8ad9366461a5f8a4f8e30afc06ed9cb9acb658a8",
      "/workspace/scratch/73cdb2c524af/mario-root-2eeb70": "a6bde63d849484c8ca451641b24032b8d4fc14d6",
      "/workspace/scratch/73cdb2c524af/mario-root-33ba3c": "16f73469a7f5cd32943aa9647c7a8663a175ae0e",
      "/workspace/scratch/73cdb2c524af/mario-root-33ce74": "021f5e1cfd5d2145ef08d4e84f1ab5627cb2036b",
      "/workspace/scratch/73cdb2c524af/mario-root-3910c0": "8304fe09e7d148f2b0e3bbbb2bff7c70aaf74af2",
      "/workspace/scratch/73cdb2c524af/mario-root-395114": "f7f27140bbd42d9ed69a0a6942f258463ff68bab",
      "/workspace/scratch/73cdb2c524af/mario-root-39f3fc": "16ffdbe34c8e533a4b3b3a30c75df1be2385695c",
      "/workspace/scratch/73cdb2c524af/mario-roulette-state": "802b255f733b467201aa0ecc6069314a48180958",
      "/workspace/scratch/73cdb2c524af/mario-shader-full": "d1870ea1921885e4c9bf56e5b69de57591dc5a77",
      "/workspace/scratch/73cdb2c524af/mario-shader-initializer": "d8c4bfe3e8a65024eb03bfb126c45ec9e6061d34",
      "/workspace/scratch/73cdb2c524af/mario-shader-partial": "df55d6b6ac7f5d6075a6ba59ffa030131175739e",
      "/workspace/scratch/73cdb2c524af/mario-shadow-init": "f7b7697b41aefb88fb3287962d8efeb7fa1c16ba",
      "/workspace/scratch/73cdb2c524af/mario-skeletal-animation-construction": "84483e6252e02d125d88bacb154d2160eca95ab7",
      "/workspace/scratch/73cdb2c524af/mario-sound-dispatcher": "bbb67bca76048605d2e757085301ddc0df4d6dad",
      "/workspace/scratch/73cdb2c524af/mario-sound-track": "2294399f464e422dff6bd0c8ff7ad36d79ea3b81",
      "/workspace/scratch/73cdb2c524af/mario-string-current": "d41966fb139f84e8827c7623f8ce0fddf7f20c94",
      "/workspace/scratch/73cdb2c524af/mario-string-layer": "86844777f5ac500db3c17c3abd9a87e230785a10",
      "/workspace/scratch/73cdb2c524af/mario-stringcheck": "97a92e5b6c5dd911377c677e5e22840de701a508",
      "/workspace/scratch/73cdb2c524af/mario-swing-roller": "64d7955c22ebbd6e6e8b9aaeb50aefd4beab7bb3",
      "/workspace/scratch/73cdb2c524af/mario-texture-binding-state": "2e66ccfa2e3dbfba6da5d6736464e86d43e17897",
      "/workspace/scratch/73cdb2c524af/mario-transform-state-factory": "32af50ad3d5949e83bac26db5d75b077745a9973",
      "/workspace/scratch/73cdb2c524af/mario-tree-b16-family": "1454120a760becbea77c2629f160f807355669c7",
      "/workspace/scratch/73cdb2c524af/mario-tree-current": "f2f08a2b0185b5e4200baf5fa34a76a478b17183",
      "/workspace/scratch/73cdb2c524af/mario-tree-front": "37844b080b6f7469fb06558c4d272883bc3b6ee6"
    },
    "physical_source_headers": 47803,
    "distinct_source_headers": 727,
    "hits": [
      {
        "path": "/workspace/scratch/73cdb2c524af/mario-model-resource-setup/lib/al/src/ModelResourceSetup167CAC.cpp",
        "sha256": "05f3ef02cccf219d934e6ca6cc42d9b1dbfe15f1ce64d7b7ab04bf96558a0d6a",
        "matches": [
          {
            "line": 116,
            "text": "void* fn_0025C334(void*, const char*, unsigned int);"
          },
          {
            "line": 117,
            "text": "void fn_0028ECAC(void*);"
          },
          {
            "line": 118,
            "text": "void fn_0028EED4(void*);"
          },
          {
            "line": 119,
            "text": "void fn_002A9DC4(void*, unsigned int);"
          },
          {
            "line": 120,
            "text": "unsigned int fn_002AA23C(void*, void*, void*);"
          },
          {
            "line": 121,
            "text": "extern void* dat_003E26CC;"
          },
          {
            "line": 122,
            "text": "extern const unsigned int dat_003A2D40[]; // whole 32-byte row, never partitioned"
          },
          {
            "line": 125,
            "text": "extern \"C\" unsigned int fn_00167CAC(void*, void* resourceView,"
          },
          {
            "line": 191,
            "text": "fn_0028EED4(dat_003E26CC);"
          },
          {
            "line": 196,
            "text": "Entry<void>* shaderEntry = static_cast<Entry<void>*>(fn_0025C334("
          },
          {
            "line": 209,
            "text": "fn_002AA23C(primary, contextView, secondary->resource);"
          },
          {
            "line": 211,
            "text": "Word result = fn_002AA23C(primary, contextView, primary->resource);"
          },
          {
            "line": 222,
            "text": "Word mapping[4] = { dat_003A2D40[0], dat_003A2D40[1],"
          },
          {
            "line": 223,
            "text": "dat_003A2D40[2], dat_003A2D40[3] };"
          },
          {
            "line": 235,
            "text": "if (!(set->flags & 1)) fn_002A9DC4(&set, 0);"
          },
          {
            "line": 239,
            "text": "fn_0028ECAC(dat_003E26CC);"
          }
        ]
      }
    ]
  },
  "baseline_report_sha256": "a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374"
}
```

## preservation.json

```json
{
  "base": "86104d96a7f570383bbfefc5fffad998e134496c",
  "object_count": 179,
  "input_count": 372,
  "all_baseline_tracked_files": 625,
  "total_objects_with_addition": 181,
  "prior_canonical_cpp_objects": 179,
  "all_equal": true,
  "generated_stub_delta_verified": true,
  "compact_image_equal": true,
  "excluded_difference": "Only repository prefix in STT_FILE absolute source path and in non-allocated .comment, and resulting raw symbol/string table indices; all other symbols, section data, attributes and resolved relocations compare equal",
  "objects": [
    {
      "object": "Game/backup/src/Enemy/BlockDragonGenerator.o",
      "current_object_sha256": "c5dd4ed319b740f1d7e5d820af11464c0584030084572061451e9ebf8f01ffeb",
      "base_object_sha256": "53cdae589ffd5d2ab00deed71b5ef613775f7c42123fda5a871b2ea63ad7d768",
      "normalized_elf_sha256": "9aa1387f1c085297a24495f68d8330bc865f579a2087cafc78603ff10ee8fc3e",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Enemy/Bubble.o",
      "current_object_sha256": "e6f9288e2cd55922400125fc7334d0475e273970ab8efed0c030f39320e31eff",
      "base_object_sha256": "8702a8e1d91e3cc7fde375bbb837c179194db7e93f899d9417b3dad473949b4b",
      "normalized_elf_sha256": "709846601744c5696008f382ddbb13bd754bec97a610ac5c4d63eddf93409317",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Enemy/FireBall.o",
      "current_object_sha256": "3bda53539eeb84f1973336f8b6295beba7fb4e08cd3100e8465d0f8e636b4c0b",
      "base_object_sha256": "c926e64e52c3283a2da1082cb774cbc457e2cbc4e172dd1ecb1ecb7e9c8b614d",
      "normalized_elf_sha256": "026d910c4e708f8b1e2fad7612bcb83532abb8ef3d90c851373db6dbeeb116ea",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Enemy/Fugumannen.o",
      "current_object_sha256": "ed268c1c8433bf8a49572cef703772fe1a65cda3de33ec7347a4933b24de2b18",
      "base_object_sha256": "355c43aa7eba0c2c257d7136bbf7501796dfc28a95da8154708561c4d6be422a",
      "normalized_elf_sha256": "e8ab2077b5540b40993b799bafbfc5b058236e9ef0a038d7508b8e2f9823d3b4",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Enemy/Togezo.o",
      "current_object_sha256": "c4942c3644bb0c83b4ebeab79187012b7171e7c5868328c2681476efdfb03ab7",
      "base_object_sha256": "57572f94ef6759e0db343770ed9cd49d8d4fb3a3e0f8a4ce572d1c6bda993218",
      "normalized_elf_sha256": "c3ecdc241295e0d59e5a6cd623db3d936f67faa99a1e9ee5d27b39fac2f44ad2",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Enemy/WalkerStateChase.o",
      "current_object_sha256": "d7ef85145e35d1176a0cc7eb69a2126705cd1a3062579b3864babafaf70ce825",
      "base_object_sha256": "ce9f6399c3f75496f63cb2dada97d936b8692e2e1492c8add0f2d161f3fc4297",
      "normalized_elf_sha256": "9673cdb540b62131e9440b3bd336af9ece23107b6c898447d5564f4665487cfe",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Layout/CourseSelectMap.o",
      "current_object_sha256": "6980408d65d979d8524ef15c708e2154a70e7bf4aa1243e7563e54fff8959fa5",
      "base_object_sha256": "6f7df08283781f5fe09899836074272eec1483cad894b09949ea117cb9a875ca",
      "normalized_elf_sha256": "6724db3c31657cd69ab9c1b52d9e6a3bb838c306f86cf637695532171645b628",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/MapObj/AppearStep.o",
      "current_object_sha256": "dadfbe8ae6e0119e82a498dd5641ea232bfbd9e6b308eef1700a50f5b8015550",
      "base_object_sha256": "4fca62eaf13abf8d5f5caf58616402643fe52956af017bc8556e7e262328b1b2",
      "normalized_elf_sha256": "85bff1a142da4aa543509dadf05073e64ed904ae21d59bca37438dd7cd3d8a08",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/MapObj/CoinRotater.o",
      "current_object_sha256": "1d1a687b8545785382c51b211b83e3ce20aa42c7809a5eb8f5f32f259e5ceeeb",
      "base_object_sha256": "ccebe10c3e68974027b80a918c8a50f37e2fc0edd4dec6ecad9b1f71679b7949",
      "normalized_elf_sha256": "3933dfdc935c1f6b24973624c3fc8aa5bc51ad0a626955953da9be383c429758",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/MapObj/Garigari.o",
      "current_object_sha256": "7bd629049bce16ad4e6f3f4efe3629a56f90f93c24e7cfbac9f94a75462c906d",
      "base_object_sha256": "659fd0f03355bb129f1c1c6ec925619f4f2ae5e9051cc09733cc0d9c8a53499f",
      "normalized_elf_sha256": "e39864f779b02dee7025fd57e5b73ec5061edcc12110dbd37ba5936b477157fd",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/MapObj/NoteObj.o",
      "current_object_sha256": "25a1cd8874604d36e60a3f3141160a22f7a2806b88010ca5134d18f988343572",
      "base_object_sha256": "253d043cbecab9bb3a71687c4321ec44fa18cb52bd746f6654c0f1e426697643",
      "normalized_elf_sha256": "6ffbf94a7549e63111bf967e2149ad2a1631e2dd5dd0c8d95b974b16e455763a",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/MapObj/NoteObjGenerator.o",
      "current_object_sha256": "b1322c375a52f0a0f97c4b74c0802346a21ea9771eec41b6d2f0c782fe9e9e76",
      "base_object_sha256": "e24302e7eb3af8c2c8ec8fe2797d88db248b3b87475e9f355e1aeb8c4aa88c7b",
      "normalized_elf_sha256": "4010425fd37b26642e4e6f2d92506c2b4c86048552d76549b9501c388fdca272",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/MapObj/TransparentWall.o",
      "current_object_sha256": "417f208fbb6397330f61e728c757190c20e8d59214c6a2f647338be6b5e15175",
      "base_object_sha256": "0ededd99ad348299836d482b945b15ea2f9e4679bad1ac9510c5d4b210b55757",
      "normalized_elf_sha256": "79cacbbba2702668d5a8a6db1e845b7ada22cb8d9c495f92dc929e8b23e46d84",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/MapObj/TrickHintPanel.o",
      "current_object_sha256": "db9c0bc64d8641cfece814c5b6dea63263fb38b57443352d843ba5d8759e6048",
      "base_object_sha256": "414add56fdcad9f419b3d0596a4e7b7003f0066bac2f8e3aa942e0461c459315",
      "normalized_elf_sha256": "99bf139832dde3bd94e7a11de6bb5b86b214ea536c34f468b5f3fbf43fb4ce37",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Npc/AquariumSwimDebris.o",
      "current_object_sha256": "5b6323ae81f8a8d27cb4be46e0d944137c29ce6c87d1a857cf562ad339297d7f",
      "base_object_sha256": "bf5ddf2168720488458cb4e595c248f1f7f5d4df350e3469cdfef25bca638e67",
      "normalized_elf_sha256": "d9a48cfdb1c0d8a8249bf8f03edbc6fd9569bbf406286fe2aabbbc70c8edd1d7",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Npc/FlowerPot.o",
      "current_object_sha256": "af56677071d87cc2334f5cb927dc6187c4365b6a91068d8fb4bb97767311db1c",
      "base_object_sha256": "ad41b48747c64bc6e06ba97549fea0f497f0ff74f8d8611cfc1498a55dfa4825",
      "normalized_elf_sha256": "4bffe7b991ebc5bf39bba22937bdb04fd53c5ad0b7149131d7c775011e78cd28",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Npc/PeachRope.o",
      "current_object_sha256": "c28540119b4d11c0d869c4ff1d301b6919fed018a586f0db08481fc3562a8961",
      "base_object_sha256": "b5363828bec6eeecb8c339374377ee64140f6c61639057a9b75be5ee1d892d4d",
      "normalized_elf_sha256": "cb670d37765c67f27d2d7a770ce24577d5203c2716b06a549dc455872e943f62",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Npc/RailDot.o",
      "current_object_sha256": "10c7e194235aaebc1e8c9fb740c970cb139c8d30af14c95462bab557c660f3ab",
      "base_object_sha256": "46efbf0b87c5129da1582da83ce8e5a1ae780f757a2b89fc31dbffdac23ef626",
      "normalized_elf_sha256": "ab6686e2c89b92fc8f3c4407f97bce85e400e15c167b76ad02a812a719a7e4f5",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Npc/RailDotEnd.o",
      "current_object_sha256": "0e650dc13407f25706e01bff028d64ebb44e3ec2cef63a8988b71f3592d33ea1",
      "base_object_sha256": "133c203cd729fa32bdafc6f2c5fb801c6e732c2855f4bbd5461a8e32cc72dd6a",
      "normalized_elf_sha256": "d3981a1b064127e5b14f15643385a23761111f2f8cc7c9fb406cfdcf0b469230",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/GhostPlayer.o",
      "current_object_sha256": "1bb35cce4d84f45fa253a0e42516b68e92a2e945d4f7f7c2276b066ac3088752",
      "base_object_sha256": "7601e0cefa4b239cc8411131cb85c24a4e4d5f132f51c5131e108d7c6d7691d9",
      "normalized_elf_sha256": "3829ce7b092bf67612d0f0ec3b6dc64e03fabed889987ea7eea018b4ce4632e6",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/GhostPlayerRecorder.o",
      "current_object_sha256": "4e945d34f4ae3a044eeb85ec693f04118755f47051efe2739a31877070c1744c",
      "base_object_sha256": "801dc4658695fec93f4689aea001f30bf0bd426600b0457ac9a63df8fbee8e19",
      "normalized_elf_sha256": "d4baedb477120c277cc22d8a560ffd3d95ae530b8fe1aec46743a77db431f3a4",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerActionCondition.o",
      "current_object_sha256": "88d6f35141f5dab982407d534d4d261f3fbf98b1db4adb37518283ccd4820dba",
      "base_object_sha256": "6a1b8aae4e66aabb0278e3cb2a53b2ec222a46285c4a17a29b71fb3e4f6d1d1e",
      "normalized_elf_sha256": "6db3db1d32dc74758c4cbeb3f58bb8919459d0d58505a1d776d03516788b8f1a",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerActionConditionAnimEnd.o",
      "current_object_sha256": "d050d603f26f8f967ca7ad1ef809cd6186fda6e61f95a73322ece110b718ba0f",
      "base_object_sha256": "786d3c17eac51c79111fe0ff9a4cdcc6474402646d78a52357aec230fc585617",
      "normalized_elf_sha256": "b3600e203b6f7e8bfce43372811db184ae91447187bd4232d94148d1651da61c",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerActionGraph.o",
      "current_object_sha256": "06eae5193f3f1cbe3b5004c0f605c7b2c9910c5b2627974e40259babfb89f107",
      "base_object_sha256": "d17fae5f459e936405fdcedffea02862aed52d28234f85937032eba49e45ee2c",
      "normalized_elf_sha256": "2aceca5015dcbe4c270e6dee2772b4f34be2ce5012e3a5d19993654072e28601",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerActionGraphBuildOutputs.o",
      "current_object_sha256": "bb798141b7a12766509e99b755d448eb7f0b21600998e81045eeccbaa866ef5d",
      "base_object_sha256": "f1b3e11d749430eedf7b52195f1f246ab8c51105c2a7173c3060375f7c0546f2",
      "normalized_elf_sha256": "1ae61ac2b723ebc3fcc27c4a2f3b6a50022f6879db510a12dcfba37258fb7799",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerActionMultiCondition.o",
      "current_object_sha256": "d5d51fa98ee6abd311c57e97a6f02b4c9c2c637205dd2f3576dec4c7c7eed0d8",
      "base_object_sha256": "47c3b0fab1ca4d149b73954120b32fcd88cea4a73b06e822b957e74c51acb1cf",
      "normalized_elf_sha256": "599e75e2e5e6df04e60e7be29de806f9c37a118a6b0d6105602c768edd7bbcf9",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerActor.o",
      "current_object_sha256": "b6c4a6db955f6b5d472f33e0369bb50c2850116a088f8a01783d65a921b5f2de",
      "base_object_sha256": "3a36262f8baf17899a2dac6b494072494ccbceb638e99eed2fb14ab8f760fad2",
      "normalized_elf_sha256": "4e0c8fb1793d0a23304cf6692203d700deaa03c750bf7936fc4c010df915aadf",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerActorInitInfo.o",
      "current_object_sha256": "b96ed92217fd030d345485433803746d59ef2adbb4ce69fb3d25f0d1fb100a5f",
      "base_object_sha256": "4888d30c9cda19b391511799fa299cd137bbe2c834673f4d8da1d95653dcca3a",
      "normalized_elf_sha256": "7eb6c0307bf6f302eca588f541fc1d3a5153c754fae27b16e4388379ede9b88f",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerAnimFrameCtrl.o",
      "current_object_sha256": "3523b0024577b167c81794a6988aa26dcfb7b85d794eeab853691803910b19c3",
      "base_object_sha256": "0ef9f0dfc45d16c930d7ecb9996ac4eb91fc340b5ad56debe3c068cc638735f7",
      "normalized_elf_sha256": "2d20c4020858ee0d6bdb891cf43d7f22ee454c20402154787629594890abbb8a",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerAnimator.o",
      "current_object_sha256": "3ea07aeb1787f51c878aaa168efd1512b82437946efc99aacfb02627d4c94192",
      "base_object_sha256": "3c5f2df9efb618a62d8b23403b6214e7fbbc8e7ce7f345a5bba1ebef96bd9e57",
      "normalized_elf_sha256": "f44e9c6c5a3d605b6c7f93b77682e3674f90a1d551bc538fa42277af2d5a1e7e",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerFlagFunction.o",
      "current_object_sha256": "265158ff2b87ca92bf61293d50f37850967c20888833d29af04b7a6dea5536da",
      "base_object_sha256": "5b17baad65a1bcda2325381296f2b6dc369d963c3a637277dfb55ce880fe83ac",
      "normalized_elf_sha256": "5641633c66cf503595897373d7c3bb9f36c4d790a16be190457cf6efaab41d03",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerFunction.o",
      "current_object_sha256": "aba3c07f48391491df4fdac685074589df0c1c52e6fa52c755a74b9d5cd0fca6",
      "base_object_sha256": "017ae44782de6a91624c2e2798281494d8d63ea12a06f439efc155f1fcd0f96f",
      "normalized_elf_sha256": "cfac410fbc9ebd58534b1d97b4c0a07fe4a22f528b653060e74e537eddb70ae2",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerProperty.o",
      "current_object_sha256": "abd7dd6d68803ca060c562f7db5b08c5ace9e9c44482436ebf8e2715ab1d7236",
      "base_object_sha256": "f5a8664d92d419f0d5f8c899bb658373643360bfb7ed7b61c33dfb69181b64e6",
      "normalized_elf_sha256": "5df5450a81efce42780c0e7689dd63a2951f3b3cf6d3d1a5e7a7db835b29ba2c",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Player/PlayerTrigger.o",
      "current_object_sha256": "e4abda53e796571e18d4c757b8ef01743da28af71fb05ce1b58b2cc5103ba783",
      "base_object_sha256": "e3f5470dcbbc9cc4b6807c0a5054529a7b29db844956808a57f36e7d1f27b8bb",
      "normalized_elf_sha256": "32872f0fbea3a39cbf347c9a997e60aefff9d6113adc83e278f79f48cd8ea159",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Scene/SceneObjFactory.o",
      "current_object_sha256": "44176875ad6d048ec14079aa12f2a89ee9326a889a49882d7df7997dffd80069",
      "base_object_sha256": "d5e49763b6aaeab29a2dfbe41e65c9e8065398fe7e89b97ff1ceb1ab967ceba6",
      "normalized_elf_sha256": "d430586a868744457a27f14c4b88d5eaaedaafa639dbd999f57b665fab84aa17",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Sequence/ProductSequence.o",
      "current_object_sha256": "a11ec7c2bf09dc05c681c1b453ed8ec791d2db84c02fa0b7e70d5a4219fff3b7",
      "base_object_sha256": "42c300eecdf78f9863b71d981f51cc9e343e29c86c3a78ea07bb6be3a3af37b3",
      "normalized_elf_sha256": "d611bf8a24683f21e1d447d9f7fbc7559e4ae4389713bb0b891f172595984235",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Sequence/ProductStateStage.o",
      "current_object_sha256": "aba40f1b0e38c04047fabfa08d31ea9a4005056c02f9c9ffe0df9df17a51ded7",
      "base_object_sha256": "ee3d2a6919f74fd50e2644b20dfb4ed34e9d86dfb3cdd464a59955bd59076c1b",
      "normalized_elf_sha256": "d46533ebfbe5e22ec0c248edb164879b89ad59f84df8c17d78bad19f08a5fae3",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Sequence/ProductStateTitle.o",
      "current_object_sha256": "be32ecb9e719a15d8c8e99fd585036b57db2b6f213084fcf7ec53906c63a6261",
      "base_object_sha256": "b5eb41903e2b1e176b1419dccaa774f7234b70a7bca85dad63ae3d92e2640c72",
      "normalized_elf_sha256": "9724ebe033c8fd8bf580ac9880e443b98abcdb50ca1379897fefbe304e5f4607",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Stage/StageProgress.o",
      "current_object_sha256": "40a57d4b7ff2db8b886a4cbc78ce5da3ce4c6302f78cfaca2937b201eecca2bf",
      "base_object_sha256": "ab5c67983d62b7ce8c2d51b58d0bc3c110394247c851f4f747145d6f62be4845",
      "normalized_elf_sha256": "8c073afd97f8a742e26bb4ac31fad626d4d866af6be191a63fbb5cb342e6a5f9",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Stage/StageProgressAccessors.o",
      "current_object_sha256": "7803d5274acbf8c3a2bb5a4beb8fb92901cb4f975cdb9a38cccb74cd751279ab",
      "base_object_sha256": "578f68311f5fb41fc1929f5d140e59156e8de8aaf221e25ba784885814785e0a",
      "normalized_elf_sha256": "c8e7a27ac61b89e2a13733dd23f3e03b954bcd86cde6ec7861bd50b395d01a72",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/System/Application.o",
      "current_object_sha256": "f6e6f546a80896e92d0308fe16f1c04a3bc84a1083a9ed310122865279cfaa1f",
      "base_object_sha256": "b44d52ed11f3799c9f66f81f0f740b4deaf631506ddbb889d03ced75a9de1577",
      "normalized_elf_sha256": "8c7220eca867ff8f83f5df4b9acfe462e8e2c6736acd8c7e6df87fdfc25e1145",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/System/CourseList.o",
      "current_object_sha256": "39071178ceb8350dd01bb5b2811d5363ca2f2a968bbe40a6aa54ea41a86b1534",
      "base_object_sha256": "5b771d9be3350ca6afd2f5f8185ae39be689a1a6ebfcfb89f3c08211ecb8da7a",
      "normalized_elf_sha256": "d0517bbc6d9d294e01a96bbfc71a88fe6c756a8c880f4af7970613a7357505d1",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/System/CourseListCourse.o",
      "current_object_sha256": "a795d3baad007ef7874c5e596685f22abde8c0e052ff720c1f151e430d61c705",
      "base_object_sha256": "413888c7c1587dcddbb9dae29ae0ec296950e61cb34524d19054ed4233b2bccb",
      "normalized_elf_sha256": "5a15d899bb12e517ff8412965d12555d3b19d56526d9bfa500860cb169257c75",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/System/GameSystem.o",
      "current_object_sha256": "0bd78118e6941c6300a1a29c65b82080a739227a5eeae1ae777625ebd304c08e",
      "base_object_sha256": "c76f4b7e59af465b7ba19b479a725decf2552e146acd2bd2121ce93c878cf561",
      "normalized_elf_sha256": "8552ca129c487e8b3f5212cbf9ac1806fbdc0b07974b211e4562d4a6371986f0",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Util/PhotoScenarioLookup.o",
      "current_object_sha256": "f5bf23a260774779e1ce295b65e5f3b215eae96c4a596b8983349bcff6959bd7",
      "base_object_sha256": "b1610b829273318e5d47c7b4a8a17e8c6532b23b2e2b8c94f588d0b93c1c671a",
      "normalized_elf_sha256": "6f3f60fcf36ec517bc1f0ea1d0e549ee9adbfaa8e4a8cffcc67cd07cf615bb4b",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "Game/backup/src/Util/PlacementCategoryLookup.o",
      "current_object_sha256": "68f83e6c438c957083be52d0e6248d4c4a834dcd7f0fbc41e2503d24a4cb20eb",
      "base_object_sha256": "714e68408deab500e4776cf5f5564a6da8d1b0bb2081218e3cf7e7bcd6d0beb0",
      "normalized_elf_sha256": "70806b5ff10e90f162f410fb4d8368003a442e86e48cae3eecc9a3d14a3017e4",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/CtrSDK/sources/os_Priority.o",
      "current_object_sha256": "36b78c4e19165699137d823db949e89659ce915e030deb1757366d3edaf9c605",
      "base_object_sha256": "ac9190e96097565a681a06e8587b62f71ef191e6ed09c665cd2400c9ad893db6",
      "normalized_elf_sha256": "07c0b17414ddc7e4c51fad81c62de724ba1b89b8930e5ced5e081ae1060ae177",
      "compiler_sha256": "e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe"
    },
    {
      "object": "lib/al/src/Area/alAreaFlagFunction.o",
      "current_object_sha256": "7c03963d5fc229d1d7218a53f48bb0fb06b9aef7a65ddb060fe6e3ff8a2b8542",
      "base_object_sha256": "e6bee1627da9fb7c82bda949ac3c0e84dcae561857d41ce0bf6a0ffde7a9aa0a",
      "normalized_elf_sha256": "0d67eba2463fda7c9ad500574de634ebc20627e85764cf023cc73c6e8a3572cb",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/AreaObj/alAreaObj.o",
      "current_object_sha256": "e700b4dcbf28f28a78dd5309de0e7efacc6a7f4f52800e42df102a851f89c6da",
      "base_object_sha256": "e7a6a3405544d944da47ad71669d6a8f1fb17baf6d94ac8556f7b177338af996",
      "normalized_elf_sha256": "502014fd3a3a15b8df8d9f9bdd75f8da1a4bde127ab32631e1d02cff8f8686e0",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/AreaObj/alAreaShape.o",
      "current_object_sha256": "0dfd3351bbd4d2eb94c088bd351893ae6ab9eb2d1ed043b8f04c711d7ce82cf7",
      "base_object_sha256": "6b5e759f612aec0325266a29c6d2425fe8df717f099d60943460a2533cefb5e5",
      "normalized_elf_sha256": "a5b0ac65ae8b535139c83390beeba11d1f8a291a6b3d67792140758cda8f4fbf",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/AreaObj/alAreaShapeCube.o",
      "current_object_sha256": "4fa6637ab7ce33f5d691bf8cdc4c67eaa235aeeda2a523b252d95af28dd4095e",
      "base_object_sha256": "60098e34b17baf37cb4bb7770a31154254e6cc33a02b7db9b8ce7de165100280",
      "normalized_elf_sha256": "c537026e69ac0dd2cf9021c3398d68b4893035ec9a51bb453c703be1bb9c5720",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/AreaObj/alSwitchAreaDirector.o",
      "current_object_sha256": "9ba3929aea9207dd853699faa171576e97a09f5a311d52878b75d2f555dd8b50",
      "base_object_sha256": "c1c44ba5aac9fd128940d4ba16795b08dd5a6630a36c5e3fe0a6aeeedaf6a6fd",
      "normalized_elf_sha256": "1791eacd0363b7d7833a8ca17eb15c7a45a3f001382a471edb26a741fbd1c9dc",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioActionStateFunction.o",
      "current_object_sha256": "2720e5a324b50387cac747d505b6c1781c2ee7e03fd94a05a0c87a986ded2cdd",
      "base_object_sha256": "b3bebf9b27d936b7fbcfe276ecca3200decf1651cb9d5f881b3fb019ed45f969",
      "normalized_elf_sha256": "ab342fe92f7d88f83546dd178d47f022d405ae1d25873bdfb0ebb5543c9321ca",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioControlEntryFunction.o",
      "current_object_sha256": "a551e98547bc6853e156674c2cce67fa9b6ac30cc829ad3df15ece670f5dff2f",
      "base_object_sha256": "4687b62f3bb1360d45c919c8e05a188cbfd1cff6162ca86ef08ff66e0e21786a",
      "normalized_elf_sha256": "492de0097b890b9610b8a6476311ccf2c7b7353cc85855389a23af71433c0c9e",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioControlFunction.o",
      "current_object_sha256": "57cb6a0c10407d692403b0ced50ae1dcf34bc696cf2236cfd9c78144d62d5986",
      "base_object_sha256": "68c20c7c025d7317875758d2d8322bc498dc6787946302e587012684038ae2ff",
      "normalized_elf_sha256": "ac94a9550168f1ca361fef37e6b0c8e7cf4a5da85d2cd04ec4d3617fbb04631b",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioControlInterfaceFunction.o",
      "current_object_sha256": "9a0251ebed5b0e07a5d79381b311b67b4facdcd9b8601786b2e7d3ca68479d9f",
      "base_object_sha256": "6bf252778f402b90f60f22f2b86c1475a64d34ac349ee5ff32fbbcd17193ddeb",
      "normalized_elf_sha256": "dcd1c7978729a721be2db9d777af9365e430eca2faee90321d4adee2c39061d6",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioHandleStateFunction.o",
      "current_object_sha256": "b9a28a2f56d304870f1cbdf8fc5caae71d891d142691218db963f3b76a45b646",
      "base_object_sha256": "2882a871d57d4316e1320f556aa85326e40c66f7ca6411548925ad4f791df7d7",
      "normalized_elf_sha256": "e48098a0bd0db5a4a8bfbdef84793ac8c26874b64b21c923db8d8b914841052d",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioInterfaceHandleFunction.o",
      "current_object_sha256": "273acc7f8d47cca8ebcb21f62a1a68baccfd5175b1ca46b1b12b9dad0ca96b98",
      "base_object_sha256": "24a6a27eaef12ab0998eaf15bc0627281ea1e00efa7832e22ff5b1544c32f12a",
      "normalized_elf_sha256": "fc722dc1d4272db7840860b05cfd45d9e1febde8372680d7cd59a849a77e8a8a",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioKeeperControlFunction.o",
      "current_object_sha256": "af7d60a8531d543c7e9e2e376d69c09bc30cc1ca48ae224e961f35678125034d",
      "base_object_sha256": "60f790889116fde83da8c62677626364ef0095cbca54174baf9c94aba46c7303",
      "normalized_elf_sha256": "62364cac39c51e850b7908196a113592da4b88c4dcc332f067eb4d01cdee7711",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioKeeperFunction.o",
      "current_object_sha256": "3773c3181b4f0c5aaa9ad4835f093ddfd87839beef4c7db9921976dd7b8c2759",
      "base_object_sha256": "0eebb197a3b44ad4047742605a6835b52e0f6bb44c56e503f590cde1995ec1a4",
      "normalized_elf_sha256": "860cba7087da23a8318048c10a3ebfda0b59ee39d86768f206f8279f2f30e776",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioKeeperInterfaceFunction.o",
      "current_object_sha256": "51be8a7e08d42222be0bcb27f802bc6b165617f3fa7b4db004a2af3a86898e90",
      "base_object_sha256": "c01ab0d3bcf949f71b663486f6c471aa6cb5e11f85c77bd8715bb3078c740131",
      "normalized_elf_sha256": "f20280900413d04173ddb515237635596dbdd7381dd83be836c15a6e89c5b135",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioParameterFunction.o",
      "current_object_sha256": "0abc5350277a084d812e924dedda8bc6214376efdeea64987a186734fe221dd3",
      "base_object_sha256": "6f15828a096c95ad91c000469f341bd575ff97bae0c799fb461c1483544fb1c5",
      "normalized_elf_sha256": "2cbfc28a7384fb0c74c9eaae75970546f66ecce51f82c2d8a5cf254732d57c87",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioResourceEntryFunction.o",
      "current_object_sha256": "70ccf3e1b563cd739027ae2a99ebaab2438854224a0af762931aaa0f18a4fc3a",
      "base_object_sha256": "7dbc11e86f4bc89d9f92cd5a3b770fc81eed076513a81fc29fc7ddcc5381ceee",
      "normalized_elf_sha256": "2e979d3d0187718645473f76687a6cc4248ae03a936feac97efe999f4c7a6943",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioResourceFunction.o",
      "current_object_sha256": "915f5482f16429a2bbca0ee78221f08527c926f88b8d60eebea8005677a7091f",
      "base_object_sha256": "ee5d4bb15ced44ce51f6aeb664f88cd141da71fb3746793b55f419481f4b1c6b",
      "normalized_elf_sha256": "bc1cd8d7f28629b0e872587c78f9aae93de9c4f9a63baff351d9a1f547b95d97",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioSourceFieldFunction.o",
      "current_object_sha256": "5ddbd550b7b980c3c3b7d63673aa35dce09d12635686f2b240a853a13d4cf6f7",
      "base_object_sha256": "1ef708a86ef539abeb5d036de0c806db8fdf8c63fc62842702dc7627f40047bb",
      "normalized_elf_sha256": "64b9acafd021b86818c042f86b0ee1b8ff429214b100c3c0eb936dd03e505c76",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioSourceFlagFunction.o",
      "current_object_sha256": "58a602263174ba2a1e3e43dad3062c9e5ea8ea2ad6573cf91bb4abc0f06ce2a8",
      "base_object_sha256": "8fc07f54b4eebb2eabb0a2e2d85df7d6a4dc8376733ff53e37b0f1724e6f4f60",
      "normalized_elf_sha256": "a2aaf4faefd2b0222811d36871691b85336012126509d0602520e5c41944fcef",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioSourceInterfaceQueryFunction.o",
      "current_object_sha256": "f6524ad8cc2c579916c821b750dd1b8e50473101c0acb3dc71e56c99979bb947",
      "base_object_sha256": "041781c684a9a3c75cd75922fd5497f0a60ff9d8d9289130e37c0a2261fa31b4",
      "normalized_elf_sha256": "05efd1b9e8662c7516573303480728fa337309c386b2661a4b6b88b47bdba205",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioSourceListFunction.o",
      "current_object_sha256": "9c3b38dca12c7cd10515de84fd75b48796b1981f68ce2ee4ca86cceb2228004f",
      "base_object_sha256": "f13fda5536b92ec6f3b974a8b7e045fc1056291c2a65ff4207ad7db6e7bd7ee8",
      "normalized_elf_sha256": "f146057fa31e0dbdf697fe7b07a52942900c71ef3c540a3497e8b480b37bccd2",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioSourcePredicateFunction.o",
      "current_object_sha256": "c1816c70ca1301fcdc2d496014a1e84490cb069fda6abe5f1d7967d333da8511",
      "base_object_sha256": "db0e89ba29c72d9d691af687641ceddd0f36d0b35160a7b76a9c30d159deb24f",
      "normalized_elf_sha256": "14f7cf291cc9db52a4a828e54d033a634cc35ff90e770340c08ef5b4b43f433e",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioSourceStateFunction.o",
      "current_object_sha256": "2768c6d46f4b23c3a3d6a87e8740c85dc962fc4636cddca9949165f599549601",
      "base_object_sha256": "582135af68f90927e67755fb173a800c1c98b58c056400e4d18eb39633daddc0",
      "normalized_elf_sha256": "6768d431e752782c0a89e8e0c79a8a91b8ed586c4446638d01a756297c41be74",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioSourceStatusFunction.o",
      "current_object_sha256": "be5db89ca8ccd807b87a4b44b7f66f2a48bd54da30bb8c05327583f882580d9d",
      "base_object_sha256": "439593039e947eefcafa6b8b9036ebc4379e84ae3a1a095b4b17a22cb869d9c6",
      "normalized_elf_sha256": "e337fed04896925606fce749f5bbd5e74661a335f94dca1140d4952c2b763ef4",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Audio/alAudioStateFunction.o",
      "current_object_sha256": "a9fe832efddd9c9259fd17d21eed1dae54254fa777f238e7a636d3cfaf26edec",
      "base_object_sha256": "9a90a9724f695b646b7a393c83cb4cdce16f59666203bfb439d71d66f2cb11bd",
      "normalized_elf_sha256": "6b6b86e834c66b0a4422b7ce6c364979cf0e1b809bffe8d1cc017ebe911120ef",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Camera/alCamera.o",
      "current_object_sha256": "9ddd0518d14da8e875bc5fc50067d2d57abbaba46c04b058ea734c9a25e967c4",
      "base_object_sha256": "6bb4ce652fc6f8c53df0127adfc0b6b26c219ba179872d114ebbe352a2f58a98",
      "normalized_elf_sha256": "ae4ff00df5708a44cf5934e93ce34100ac2b955bbbb2e2b5686af64f0dd6bf16",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Camera/alCameraDashAngleTunerParam.o",
      "current_object_sha256": "5caadbf5c5db72396acccd96b4fb043788c74520b16db644d9aea931662fada0",
      "base_object_sha256": "9bba6e2a2ec2fd5a4bf0e7706432464f99c3afa122fd43d0567f9fbd70a421ac",
      "normalized_elf_sha256": "dc7f16e96cb262f66a5169817f0ff855868323ac56ab84ab687a944b2f27b8d3",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Camera/alCameraParamVision.o",
      "current_object_sha256": "317bcdb537538e4ee6ae06f11cae8154ad1b64b3154813db882c71e06bd3ba1e",
      "base_object_sha256": "f713f86d78dc4a07623b8ac858dd35100e37ed4c1ead796a385674f56a4fe5bb",
      "normalized_elf_sha256": "188a4e474cc6388ef519ea5d7ba29aaa6dded28dfe708fd142815fdecdc083ff",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Camera/alCameraRotatorParam.o",
      "current_object_sha256": "4671b860e0d0c80592307240969d603029f4c9751577112e215182f3b73a6480",
      "base_object_sha256": "58e761c79ab86a584e099c9a2d023dfb6af46c64c99dd469ffd05b56c6683ee0",
      "normalized_elf_sha256": "bb55c887c15a22e026d63d9ffa06a9473b70ff808b944e201b7e7b5e6d10ef70",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Collision/alCollisionUtil.o",
      "current_object_sha256": "ef5df2af9c0600cb28227fbc685af131d3344666b1a8d770da5dda4fe68f8b6e",
      "base_object_sha256": "a29e78c725f83167e2226bc581ae800aca548f9c69ff7ba0d2686e0b77933951",
      "normalized_elf_sha256": "019177d129b8dbac928726733478317865624a50411a3f925da5ee6c9968c5f6",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Collision/alKCollisionServer.o",
      "current_object_sha256": "f3e5fa68aebcdbfab8c8c6793bd7fca86d398a19dd61ad466860b6c4d343f2d9",
      "base_object_sha256": "c62cd68bbbe43fce3d4eb1da0e4dd1df32e8cf582e229bdb545ccb930044c04a",
      "normalized_elf_sha256": "1e150f40d2babe12325c8c82a090467dbbcff73789986a926ea8ed7dc997599a",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Container/alPointerArrayFunction.o",
      "current_object_sha256": "061f720d01c5602a9707ec54e82f2a1f81717dea277c8b4b16e24161975415ec",
      "base_object_sha256": "9e76e1e4edb0a1af238f9f95ffc10089abf0755c6e850b6e5d30f1e3f3a72119",
      "normalized_elf_sha256": "ce4689714947b2952a57a31ce886813866edecd260100b8ae980e585d6628765",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Controller/alControllerUtil.o",
      "current_object_sha256": "c33d9d9e3ed53deb4c75fa1766e2ef533b1a7f20b545adaf883abf43a2eca24c",
      "base_object_sha256": "87419bae5c9bca541212e63f9e240103cbdc6e77540e9dc5b0049a409b464e5d",
      "normalized_elf_sha256": "6dfabe29a88dc16179fd86abc54b7cd13aa34b492e4fb0d9864b516ea30a59c9",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Effect/alEffectDeletionFunction.o",
      "current_object_sha256": "6a8fc14181e5fb51ddcd9aee09c319790e43c3d143ed5933a342901caee185d9",
      "base_object_sha256": "cf3889bf665582c7e82cdf09b517a0dba1d78c490ea11ff4e3547bf82328e024",
      "normalized_elf_sha256": "cbbe38758bd33e51579aa9edd9100b59cf47ddb4e6a9e791ff37e0ad623f8014",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Effect/alEffectFlagFunction.o",
      "current_object_sha256": "85c720c17a9fe1177055e271891c482d25aae8ae567b0d9b481119ac21b6c69b",
      "base_object_sha256": "f4468063df8c3c87dc022e904f5ab36d58b14a9e6e96353e8da620163989279e",
      "normalized_elf_sha256": "84f2a6b6ca23bd761a7bdcf270e7ec1ca5f45e0cac3a06339286eecbd1b63c37",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Effect/alEffectHandleFunction.o",
      "current_object_sha256": "2a921e0cca191ca3c5877d7c558c4a9e6499f5c4a1f893827ae07c19fb3a757f",
      "base_object_sha256": "d3982c76f170f128a0e4478386539e4044778b53171096b39fcd4c268827d1a1",
      "normalized_elf_sha256": "2388f1826dc701394284ab4b97d62751978531e2d5c383e2d9410b64b088e7d1",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Effect/alEffectKeeper.o",
      "current_object_sha256": "cd499977f9f4d63dc9ad9c20c2d146581a9562881cdd1c21243e2269bbf1c7a3",
      "base_object_sha256": "af6680c69d1451c2e67f317e49aa5723139ddb4ad449c2acfb9a87641c35eeb9",
      "normalized_elf_sha256": "85d0d71f225dce6dc2a7ea4258ed9f78a028283fe32d87bccf49d04bd5460163",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Effect/alEffectKeeperEmissionFunction.o",
      "current_object_sha256": "ace546fddf4f673f98306b583e72e4e082044eb81023c0f1935c47bd509ed1be",
      "base_object_sha256": "3fc857b180f88d22b5a7361b84a69bc45c8aedb4a7c94196813179a8e306dbf4",
      "normalized_elf_sha256": "321a567cf9e568390f4285f981ce1cec45996d59a7e6b4ca058bdfa9c0ec0a47",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Effect/alEffectKeeperFunction.o",
      "current_object_sha256": "b131506a162862fe9c4883ac429b6b10996a3e4c6d51cfa9780c5d31ddc067aa",
      "base_object_sha256": "ba523474030e2ed458c703d605b594c825b6df537f0de1a9fe3eb4fe18a07718",
      "normalized_elf_sha256": "5ce8527c88746895850a3f588d6ee128dd910e4835a53e3af60db1b64f565a4a",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Effect/alEffectKeeperInterfaceFunction.o",
      "current_object_sha256": "a26074ac2b2c71e57016435308d77154654833bc301881f0671a6b7056b894d8",
      "base_object_sha256": "2f9129659015a912701dd126f44c6236aa87e7e1e3848ce0bf13f7d0976dea4e",
      "normalized_elf_sha256": "d40acbbe28305eb81ce5a1000006c58f5e984015813e4de2524fa199fee2f40f",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Effect/alEffectObjectDeletionFunction.o",
      "current_object_sha256": "4831ef9f5018db86e723c54c722f801c216277b29a854a2d537722b09425d55a",
      "base_object_sha256": "ec5bc78d26796899589a871f0a46d3766a2c96453ace160ff870f8c772d5f57a",
      "normalized_elf_sha256": "022187f9238e570f401d73feff8a42ef07846788a9d8e49f447ed74ff3d8ef7f",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Effect/alEffectSetFunction.o",
      "current_object_sha256": "69153351c37acf84659397749089f0eb700ea4539059763f31625d02a1fcd6b2",
      "base_object_sha256": "ab4e61a4ad666f8c0ed22913dc81e20b657200b42db14ce9e843c4676909f19f",
      "normalized_elf_sha256": "7cc850670d42321b7f85b9b84a4eaefb0dae488243c5dd09bd07314895b0da92",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Execute/alExecuteDirector.o",
      "current_object_sha256": "305eb15324da2d36143377d092405598a6204a132e22517b9ef7482d34e4def4",
      "base_object_sha256": "8a0cca330a173d908ccaa3e151989ef83457e81670bcd5b061b62c8a30c40e9d",
      "normalized_elf_sha256": "e0b3486fc467f10d01c3ca698d58404dcc633a7959c8cfa37960239f8482e22d",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Execute/alExecuteOrderFunction.o",
      "current_object_sha256": "2780f02052ab593aa3107dc4c85de3dd64b59f91fbf11c906a17e47d279cab2a",
      "base_object_sha256": "21c13918a209af3b642eebe78890252ed64b0b72cdc931b26c526efe95485a3b",
      "normalized_elf_sha256": "c8163e5a22041977d74050960d23f05dddc1e27290a672b2577114244b841a5f",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Execute/alExecuteRequestKeeperFunction.o",
      "current_object_sha256": "aec02bfbc4269c15679aa654de8cbb15606ccc451dfb7a2cf766c0ccd617dc91",
      "base_object_sha256": "8b1b83dc5985b1bdefdf3944b5bf38f295eeb41d546e6aee293645e83e340213",
      "normalized_elf_sha256": "5fd8d549aaba271825414d115588e44ebfaa86c72d0274ca26d7d40c83742fba",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Execute/alExecuteTableHolder.o",
      "current_object_sha256": "033838fdb5cd685c5de6d3a185148bfdf7bd7f99dc8fa8ab3f3b2e649a02ae72",
      "base_object_sha256": "0d8fe6fe2ffeef24a5a7cf7c6ba3d8ce36fe845be12663151f156ad860f49466",
      "normalized_elf_sha256": "eb1e1cade7880e9a5a30e22e7990355b50bf73a0f6042656722fd9d60e8d68db",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Execute/alExecuteTableHolderDraw.o",
      "current_object_sha256": "9e8c4b0923311a621bf5b25e15df624e27025f74dfd741222709f2b6fa8629f2",
      "base_object_sha256": "24c7fb647c08a3d903d3ad418350bc1d1a3a8b00b876310c5c64dc585c931810",
      "normalized_elf_sha256": "e4086bd310432444a08c76edd5b1dcba6f37a8e8be2af3cb538b4e55b6522c8e",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Execute/alExecuteTableHolderUpdate.o",
      "current_object_sha256": "93d62796582fd172f2fd2ab6b45f9dd86cf164193bc6a9471af1e48efe773785",
      "base_object_sha256": "2a8635d2e8fb34e0259f6889432fc12c6ac5c1b8670adc29e629c6fc11349918",
      "normalized_elf_sha256": "62379dad634fdba57bf41bcde163c660592047dafe1ed2240112c7231ba66c57",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Execute/alExecuteTableHolderUpdateInit.o",
      "current_object_sha256": "2e97cb5d28761a4da769e3e255a7cd103de35b6c60ee54f8417bd431cee98abe",
      "base_object_sha256": "669b8ab899a210bcd7e81ca82851342944f64757f3c568fee990ff8ce89bc073",
      "normalized_elf_sha256": "8d4fa9afa75e5c7a8c12caf55709c0909170a97f0ba997a92e7cd90e48a8035a",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Execute/alExecutorActorList.o",
      "current_object_sha256": "8827b7fc149facb994f8ebc015ca943e7da5476b886862f7b6c250d2e8fc87ca",
      "base_object_sha256": "cc0fe10c1057fc891310c47d23864bab70ead374d553fa0165c191ef66107640",
      "normalized_elf_sha256": "ed48453027908eb2b3743cdd5174984012405cc5c1fa7db1fb339aba59cad24a",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Execute/alExecutorTableListFunction.o",
      "current_object_sha256": "01d8931496652936867a52249b87cb4227857d50ea99fe2042e4a3044f39cb27",
      "base_object_sha256": "47d13f13873c7c33f488cabce7a88e6f579dd4b2cfeff07e033d4a1942b2e35e",
      "normalized_elf_sha256": "31c3c255cd6c269e3ebd2154d7e525f16c0468e30a0bf1eba9a9abe4f6582d81",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Factory/alActorFactory.o",
      "current_object_sha256": "b172eb43c03ba0fb76b8fd238107f45f64a9d45483124b8eedc7028df527bc91",
      "base_object_sha256": "66ce09ae751b535298ce99756adbcccf8218454072ce3567eb137cb67c88471f",
      "normalized_elf_sha256": "58137e9cf0d94e96afe3306f15390f493194c3512f016d01ecbb8d62d6bf5438",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Factory/alAreaObjFactory.o",
      "current_object_sha256": "364cc6c5abbbeef7dbd8feec4aee96afb11128e66d03eda01e032bf370d7198c",
      "base_object_sha256": "d02d2cc47dfa828e4dc7b2f58ee338431a9bb8e38aa6ad53ecf898803083a180",
      "normalized_elf_sha256": "ca670c065d85301ec6f4113668443dee3914f5c236f777315532c40cd3fcdfac",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/File/alFileFunction.o",
      "current_object_sha256": "048696ff0e3e5abfc327cd19c042619a6dea6a94e71f3202f150b9fd6b5a5468",
      "base_object_sha256": "f0c8ff33f2b0e73335feca00f2774f2dbd083ab3bde7fad4ef4ae9d2c3b769d6",
      "normalized_elf_sha256": "548470ebe845baef1582574787060572106650f6e0eaf95f3af22043cc7ae9ef",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/File/seadFileDevice.o",
      "current_object_sha256": "d669c041b989db6bede427e651dc37bc2a21351321e936c28c45c8982016a4d6",
      "base_object_sha256": "9f5592dd75763ca73334f69518dc1bb1fb449eb158980f1b7f178b47484178f7",
      "normalized_elf_sha256": "dd2084aee9f35cecd855c2146e7c4595aa2db08d5001258b78946a319c4a7fa9",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/HitSensor/alHitSensor.o",
      "current_object_sha256": "313e67addfa17fad79e88353c6e56bfd974b7c0d99536e9120286131f2f2f22c",
      "base_object_sha256": "3cb27d2be0d0d08afa23c6cacb0c236949659a58eedd31874a7e56546ab60c78",
      "normalized_elf_sha256": "5b8e54356e29bd13f6e896fe250e589907e3ee1c5f1551f8c2de54215003ad5b",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/HitSensor/alSensorHitGroup.o",
      "current_object_sha256": "a2fb7ee8a957564fb01eeb10b67d4ca311571c4276ba57412ee101be92c720e1",
      "base_object_sha256": "b3dfb508e40d321bf6d74044910680faf5a5d190298118b2e2170c9e6bcaf137",
      "normalized_elf_sha256": "290f99c883094638112871d97f17b7789b0b70b586f0edaab1b6b5c5409daf8c",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/ItemHolder275828.o",
      "current_object_sha256": "1d02e87978fa152819cec7799f4984343f8fe6ce7847a676c6b8a498d9adc8a0",
      "base_object_sha256": "f98a3baa07d676b08e7493adbfcf83c22cc2c923dab102b5cac7468b6ea15820",
      "normalized_elf_sha256": "3257cdbd557cedd36af097b314bf99f62337b4d13f05d4f983e40952ba1ca39f",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/KeyPose/alKeyPose.o",
      "current_object_sha256": "b2d06580cc8a3f27f0e43c600b8280a16afbaf3c33eb2708a395ed5bfec20b40",
      "base_object_sha256": "1d7adec98626c1af92d7e5e14748a14a4f6b2dd3acbdebd2a479053944098016",
      "normalized_elf_sha256": "fe35bcc4bb5130e60ea200e41b8d277391f615f7c241b1ebe59b5478b727b593",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/KeyPose/alKeyPoseFunction.o",
      "current_object_sha256": "8fc2a217b58671e0cf76a4c0bf8ccf6f3c176161ca5a7d96647c5ddb3d7f533f",
      "base_object_sha256": "e91c329833969e1dbc7c4ffe981c060712b4e35cb040f978766df58b0db3f3a4",
      "normalized_elf_sha256": "93d58a76562a902c57828e54092c0031f74323bc35830ecdda9612bd54e89657",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/KeyPose/alKeyPoseKeeper.o",
      "current_object_sha256": "a6b4d2e70c12fac501e9b1d19c344ad86087653fa5baefcd1026b1f65029cdd9",
      "base_object_sha256": "47fa36d5c913540ae7748ecefc56b9f97d3e131089be85d9977413db2b945dea",
      "normalized_elf_sha256": "9a7eaadba877601a5dd6e7b291d263d1c336dc015b6a1a292586386b34afc945",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Layout/alLayoutActor.o",
      "current_object_sha256": "e73390e00a9b8239a6574e6ed5cfda9c7ce014d9877319cf9fe39936c6bf27d8",
      "base_object_sha256": "c559d438b6aea9e33054f6a1f74092b17c825b4474865f572e1d2c30c904d47a",
      "normalized_elf_sha256": "f0ddb9a2ad79d8539a7868f57311033c9beb5cd5bde39846d27b67500ae70145",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Layout/alLayoutKit.o",
      "current_object_sha256": "d56449ef22465380780d2e3e48c71ac4b1650c15258861f6f31b5e814e4d0e1d",
      "base_object_sha256": "84c851efe6e696251791e8eeefa12f8f36fa2abb03cb70a4d51448893e8800ff",
      "normalized_elf_sha256": "e45a3cc52bfd71249c3e9aced20f0843602cab7944d1d3065877b48ebb47cd7f",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Layout/alWipeSimple.o",
      "current_object_sha256": "316e7ea66aa1cd17f0e0360f27df2f3656db45a34ed9b957ebebe5318b165b54",
      "base_object_sha256": "425ecdeb7ba51618bf52b31675b50958771cde5210f88c17927b98dfb2e10403",
      "normalized_elf_sha256": "07ad8b6138aa78decdfcbba797b5d3b12354e6930613b10d5575ae4bdeafec5c",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alActorActionKeeper.o",
      "current_object_sha256": "578394fa16b1da42ce374f201c2790635be87d5d52622c1a453e046f8856cd5f",
      "base_object_sha256": "4611f5d686afefc6c87aa188c17246d179797247878a769ec214b7e7d97e5d25",
      "normalized_elf_sha256": "954973cae9535a2313f150495b51452c92977c8150ae676f673c9e5c2c4b42a8",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alActorExecuteInfo.o",
      "current_object_sha256": "ff3644fbe13bb92ce9ba573d4b86622ee016fcd1af7f00d02f9ef53b4071c1b8",
      "base_object_sha256": "d3d084bf9b6b5a92d6e921af63f334404ccc8db25ef8dc3b2431fd0f3e6f0266",
      "normalized_elf_sha256": "3541d2d7ef5e8178b3a1b36afebd510769bec521f8934f3da01fc65712b5035c",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alActorExecutionHooks.o",
      "current_object_sha256": "2c8ff66c94e463d8cca8a5563f454fb443ec0af5c2555e9c41ca25997f349475",
      "base_object_sha256": "cecf81f1414d5da33417eca6ac5d11f238276b2cbf6813aa71c84938d7ee9193",
      "normalized_elf_sha256": "246300ede3bb07128aa1e5e2b3a8ab1a25222c9a4963fce892618f930e1d463e",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alActorInitInfo.o",
      "current_object_sha256": "49b89beb349a0831c618b3363081e1f0c556734e12581b901b7165fe92baa268",
      "base_object_sha256": "0bbb26a650569c0db11aa7688f4c447148ac8035702bc679a34f2bb70cc5d1ae",
      "normalized_elf_sha256": "0a3f861a61deeb6759a69dd04a97056be9a450e22b9ea41953722f107fde8b53",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alActorInitUtil.o",
      "current_object_sha256": "4077895a00fdbc455b92d82a4e587c55db629a568148ab502b4aaf3b239a1ef7",
      "base_object_sha256": "e8438aaa44c7183c42230e11a993f81e2fbfc777392b99130e56aa13c2b595ae",
      "normalized_elf_sha256": "375bde962394653aceb976023f97e473e6f2ab1eb52e245695dc09b88f1b5963",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alActorPoseKeeper.o",
      "current_object_sha256": "f29b1e8bc3e6bc7121f41405d4408af02c8c7d52fba82ed7921f36deb8e10501",
      "base_object_sha256": "5863883a878199b13a4683ccf527e90fc18008d1d54f260214f74298a83f4a10",
      "normalized_elf_sha256": "e5e6ff93deaf5cdea1f17bf9978bb90213291c2713c781b6cc064eba317b0975",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alHitSensorDirector.o",
      "current_object_sha256": "8002d3b3480e8f4d77fbb88740f2add57adc133daacddd149880cf120e185393",
      "base_object_sha256": "2440474217a72c4be4fa28382a60f41714d893436df79da92f47d74f31efd527",
      "normalized_elf_sha256": "ba9946fafa8950d67f33f1cfd6c42c61c43002937ecee5027be32166b9f97f3b",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alHitSensorFunction.o",
      "current_object_sha256": "e59acf7f853d5986b056ee373089487884cc611a45976aeeee6d9faa76210e59",
      "base_object_sha256": "4dbc39449bc53ac4dc310ae3175505ac3df16b5b634a00f1446994c8438ad5bb",
      "normalized_elf_sha256": "f9df54bc6c6cff2074faf8882395affcda7909184321aebe1c90c447d7c873c1",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alHitSensorKeeper.o",
      "current_object_sha256": "d01e6bcf720aff28cfd9567551f3cbe853a6d66d2f37ab118abfcc6527c1f98d",
      "base_object_sha256": "c1f1de8a07e99e8675ea56fd3b8bf6bde8895b2bc832ff097bf6b994b627c705",
      "normalized_elf_sha256": "18d057bf06967d596fe10e30c379e2909cd2f9c21ea4a9fe3e62b9de7064ef74",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alLiveActor.o",
      "current_object_sha256": "50a7108d78a67c927a78c74e13dd9e445fc945dc620da3fe109180bbe894dbf9",
      "base_object_sha256": "134e51b51cc687de9695774711d5480518dfc5a60ec767050679d6a2824e5eb4",
      "normalized_elf_sha256": "a9f59d60014f8026858939036a039c422723851e4c7b22cc9f5f0b3fc3bdb090",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alLiveActorFlag.o",
      "current_object_sha256": "962a1eb6ebdbf940f70e284591e1c9e6af76daa8918149df0db4f2fb919d1524",
      "base_object_sha256": "413ede9937024b2bdc996ee6733c86f23dc30879ab07293407cd94c96704abe1",
      "normalized_elf_sha256": "8082b9cb0fcc9494fe0f32bcba46249a988b5b0c89fd49f716d959c08a7ae9e1",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alLiveActorFunction.o",
      "current_object_sha256": "db2840f9162f17537d02d145231d0d2b7e67a53f6c5afcb884321e2a924183ef",
      "base_object_sha256": "b44f46dfe765a7da95286875b210e28ff37c16f4c98b4379fc8cebe5b38c28c1",
      "normalized_elf_sha256": "0f3b5eb4d9b2c90188af5a6dfcb09a3716d8d9ac321cb34f0b870b27c17ac9b4",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alLiveActorGroup.o",
      "current_object_sha256": "659379650a4d4edbab705750ba03cb0b414fd681c429b5545801569226b61ec9",
      "base_object_sha256": "8fa535d52375705b5a9154026cc07da4ba65cc67799c52598e1720c583fcf88b",
      "normalized_elf_sha256": "778ff7f047101871a568a5feafdd7af8f6e41b726c1180c29d5e8c561e71beb7",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alLiveActorKit.o",
      "current_object_sha256": "86f31c80e024dfd1ae8992210718e8087f805cd806844147585356feb444ef8e",
      "base_object_sha256": "9840ed76135926a556f8cad22ee4f03c6d058e15267c34f2a0c745065585b088",
      "normalized_elf_sha256": "c5353a0dbdf020db5919f3b6404806545f73c3893ea3e9d3958389aca0157a71",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alSensorFunction.o",
      "current_object_sha256": "447e642008cc44ab3f0d5283b9fdb751f1994dc66e2f193b303d05c70caba83e",
      "base_object_sha256": "10cc6bb6e027b83f81fbed15d214c5e9e8d6c2162b7e9ae534605ca4f4d4e371",
      "normalized_elf_sha256": "553b948cd5386cf21678ca32674c63fea8b49bd968e480132b72b8a7f806be08",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alSensorMsg.o",
      "current_object_sha256": "673e640252fa75c5cd99e0f10ca4d0b0e4f91ce04dd8d14d877c120106cf3b1c",
      "base_object_sha256": "b055d5ffd71738ff87263b8a32de7d1e5d8a670cd14b3bc69e1b4a5a4070a7c9",
      "normalized_elf_sha256": "c3fa7a1f10d79423f2b6bbd4b132bf537d91a15c9dccf5bbd27ae37f0d0cc790",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/LiveActor/alSubActorFunction.o",
      "current_object_sha256": "1e8eaab307220e072399ac459d04493904e0564b5fba7efa4b8abb81327c94df",
      "base_object_sha256": "a0d461f8ec04c8192575b6f912915663442ede82ee4d5e80dfbc2265d6872e91",
      "normalized_elf_sha256": "897887099a1d2859b73c89654b748bf338a3db90bfdda1d95640ae31d7e2d967",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/MapObj/alFallMapParts.o",
      "current_object_sha256": "fc2121c908dafb1b1e572a542dff964bf57a2d47874377f3bf7d47bedd13504f",
      "base_object_sha256": "135670d137a1c8bdcee37e54903727431f638f3973961a6d924de823b3c45fb0",
      "normalized_elf_sha256": "7392fa2db18f33e179778788379a0943ba442cc5a0e83c1919ed3578ac6d70fd",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/MapObj/alMapObjActor.o",
      "current_object_sha256": "c3b3713b8432f7e32acf4bb910d730adf3b93b7fd77252dd078ef27609a105c6",
      "base_object_sha256": "7e26aa1e6df51c3db4d21a7f7e116c1e3f2abf34867ee6ec12e68714bbf7265a",
      "normalized_elf_sha256": "a53411fe1402b995ede453c66279116c721f4a9b5be9a87fb45d2f6139fc8e94",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Math/alHashUtil.o",
      "current_object_sha256": "9f77c38c9d44e68e2d0d7a6534418971c3b1c1ad0d83dd12b95241f4ca5d109c",
      "base_object_sha256": "bcdd8543219046b5776af505e056da686778580061bb431d2b928e7aae7948b1",
      "normalized_elf_sha256": "33fbeba015157ce5e1d5934487a10c819a15e981a299fd00241c63e1aeb3d789",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Math/alMathUtil.o",
      "current_object_sha256": "1a069a3ab2957606125d4f80a51508498acc78b9e2cd2e4cb1732c7afc83f9f8",
      "base_object_sha256": "6f24b89bcdb237f28f69844fa4b105b9126f45c6eb129c592508d6bdd9172781",
      "normalized_elf_sha256": "8ba59b41c1bc1d69e5569db3546e992c7cf98650b896bdc6f3a4fa6316a18d27",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Math/alVectorUtil.o",
      "current_object_sha256": "a3216a911b46d37d6d1b34a13c52909ee3bcd60f63d6ace8295f04a51828edbf",
      "base_object_sha256": "2c019344faa0b92b93dc52acf205b405cf930c87858ff5aca9af851792fe5b1c",
      "normalized_elf_sha256": "c583a59577c260773399b6a061f41b7fec0e8d4e1f0443566ee6e3d122981c33",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Math/seadMathAngle.o",
      "current_object_sha256": "41bb043ab31d536a1f8412f02392e37add69498b2e8281bce7db4ed5f4c62584",
      "base_object_sha256": "602fa8224030fc55cc2d0cf7a460ae22bf4572a9d569b74898558fcbc0bd2f3b",
      "normalized_elf_sha256": "ea6693552c2c29f82c68bb893494c399446d68dbbc46f6d9f868a93e23b62909",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Math/seadMatrixConversion.o",
      "current_object_sha256": "e91710af2bdcff94246671057fe5fa42190845aac9bf44526b6467efa3711866",
      "base_object_sha256": "3cdb01e0331cd7c5f7168e19949a2dc6dde052ed8e98325b15098412a2df7346",
      "normalized_elf_sha256": "ad7381d4b4f521704e7b6e5ea7fa5d00d609f3f7fe462d11acfd7927c4d06833",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Math/seadMatrixCopy.o",
      "current_object_sha256": "2f39d11b30ae86f6dbe31275cd1fd2046f08d6d985b37edbcc6f71407d548ca8",
      "base_object_sha256": "11f553afd72c8851cbc145dadb0f12822523342193b621a5c0aa154b4c1264a0",
      "normalized_elf_sha256": "d641a89f1383e75d37e226f0ee4f57b3dd5cef459e23568df09b65da6345828e",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Math/seadStereoUtil.o",
      "current_object_sha256": "e4952a33c8258576d379cd9d6fac628c7f9fa168a494bb539915adf4c4f272e7",
      "base_object_sha256": "0ba1bf89bf5e31ef48736083cf90ac24eb1055a07c89bf294bf689a846b3c56e",
      "normalized_elf_sha256": "2be9db263544dd65a502ccc1a9354c5a3a40afe34956fbd168839fbcaf093a3c",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Memory/alMemorySystem.o",
      "current_object_sha256": "28bd99477567b8be8b7e674cd8611c524122666e8a5013b21dccd21866663167",
      "base_object_sha256": "6d31c2c763cfa062af896204ab0f2f7ff6ed9e3ae1bb998087d5f726d94ded1e",
      "normalized_elf_sha256": "11f614696e8834e8ecf52e4367d20efa5c5f5f27c63e483a26eb334bcc72e298",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Nerve/alNerveActionCtrl.o",
      "current_object_sha256": "fd63f30a7ef1abd193f7dd10d2afa816bda4f7176f331b2d575e305b738fad40",
      "base_object_sha256": "b71a9551a68ba87b32bb154a819ac455333a9f4d017f988628c396f4c084b9e2",
      "normalized_elf_sha256": "226becf8af9e430d140da8fe642add303bc9f462372cc96e28450b46129039ff",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Nerve/alNerveExecutor.o",
      "current_object_sha256": "1a707fe526c3d814ed07c7257a45f7874bc6e2b3c2cd0b8203b6f9ff59b2ce30",
      "base_object_sha256": "e507105345bea858291b8ee12102db0063d886b0ee9a0318107a5c2782c4e982",
      "normalized_elf_sha256": "05485aedd0b461410917930457a77faf7384ded1101b4c9deb6e01806bc760d1",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Nerve/alNerveFunction.o",
      "current_object_sha256": "22ff80cb1a575fb10e63fbf7859e67182c9375eb313349d29103d09613387ebd",
      "base_object_sha256": "1f70f483589ffb54276cace32514b54eb0eed3d24ba553a7ee6ea39e1b8424d4",
      "normalized_elf_sha256": "1e5243296183e4a11a1f0416b29c8c1c79db7721ac38aaaff35f46974980b17d",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Nerve/alNerveKeeper.o",
      "current_object_sha256": "20ddc24b1058fac942ac7be176b61c3f9694dc3c935b52c4b65b06a36c1432d4",
      "base_object_sha256": "8183a1701a427480ab8c17b522fb2dab4a840d9d7bfbf9eebc442780227ab683",
      "normalized_elf_sha256": "50e9fdb06c9b400c74c01b2b0114fd4f33dbe70cd3bb467f2a671f44108d03a4",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Nerve/alNerveStateBase.o",
      "current_object_sha256": "be811a628e0b36c6bd7187b3ab11b343cd6f563cd0b04aacdde0edb9f4a0a0d7",
      "base_object_sha256": "f4daf5eadb4900b8049129fcf47cc7e05b06aa93106cd02d48483422283c05f7",
      "normalized_elf_sha256": "8c9e4fa90372a4ebab4b2d0bc95cd2b01bd90b65ec251dcf74277ea157004ff4",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Nerve/alNerveStateCtrl.o",
      "current_object_sha256": "db723e384ad39f3e71f34fa0cd49b33b1ef5be0c6e9bb87f5a25c92fda5a40a6",
      "base_object_sha256": "27ce590f5bfbba633fefb3cff4ef928a65503716036c4e0c988c460211cf588d",
      "normalized_elf_sha256": "a400bb79d9a634af890ec311ae335fd1d10462f92d6eb321060b7be4e25aa7b5",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Nerve/alNerveStepFunction.o",
      "current_object_sha256": "2d54c3629d9a84a774ef4559bffd3c2ac277c6f43276618c40e5c60528768cc8",
      "base_object_sha256": "a5a32873a2b35187e7f5ce2ae4f813d55f88645e251ee6d2dcf793e2c6590b04",
      "normalized_elf_sha256": "592c1df675eb94d1a8a583159d1d4d73a7fdd6cc9376cf7a8ac7e42668219f56",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Npc/alBreakModel.o",
      "current_object_sha256": "168a40040b53a0d5f0fa10511f5b28257dc68bab77ded3af2d8064d71e130c41",
      "base_object_sha256": "9b44a67a699b7e2f26e4945dc6a58c8e0235d72fbc80ee90db946ea4c0ca6f9f",
      "normalized_elf_sha256": "b56397d8b83100775b5b20dfb2ea8d7a6a9085404c6a12e6afe096b5b6ebfd50",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Npc/alEffectObj.o",
      "current_object_sha256": "96a559f4ff75c5d6ee45134de5e5f69ab39c451a72ab8bfb9fa6ef71e6b572f1",
      "base_object_sha256": "c7d337bc0491bec899a61effb05db0eb6955f857f310f133e95cd00e6f00c6f4",
      "normalized_elf_sha256": "3f68b4e05165f05f6ae5963b622ad75f3d15db5de61da1eb724f62bd5c197e3d",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Npc/alSky.o",
      "current_object_sha256": "f602b79105ab76634cb6f51d2b1ca5b0a47977a48b423b107ecd8101c4f3e5e9",
      "base_object_sha256": "96ffed697bae284a882609fc9f12892aa9f6512633f9e1bbf419cb5d63a258fa",
      "normalized_elf_sha256": "c6d663edb7a5077c886fd9c97d7901613d6e477b4968d5d614ccfcaddb633052",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Placement/alPlacementArgumentFunction.o",
      "current_object_sha256": "158173fe2d7f8580914c3f94f6df933a2e27180cda79986cd5b10bcf3348967e",
      "base_object_sha256": "3e7b7e79b42911fee83f39e005a7d86fae634e8d82930dadeafe4f334a61fd76",
      "normalized_elf_sha256": "796186b353a42222a0eedfcbfefe8b4f8d1dbb2389129653b244c5d909ae297d",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Placement/alPlacementFunction.o",
      "current_object_sha256": "1cf552b64e4b39b4901a8cd1af5842577619b6052adff8718213a92ea54e6bef",
      "base_object_sha256": "89d6137800a8a7deaa9673a690f1bded28f7eeea54eeb0551641f30b5f4f97d2",
      "normalized_elf_sha256": "8332d44c45ac01f98467bcab3fe3b1169db8270f6b656315bcfbab6d8156cc54",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Rail/alRail.o",
      "current_object_sha256": "594f875475b001b3399a4eefffd36eb74a4f3602ed03adc80581b34663bb34fb",
      "base_object_sha256": "d864d82e65597f5735bc54de9703aefb8a04438bf80ab0dfbaff968313a0c4ed",
      "normalized_elf_sha256": "55e13513ed34e3d582bb9f9e599871f296ecc6ca24a37f1f50e3985757baaed0",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Rail/alRailFunction.o",
      "current_object_sha256": "0582ad7a04b5353a12cb4ec80d7615afb23bf22b16b5c0bac4a5e41344960d7c",
      "base_object_sha256": "926020d99a6f50b8da4d55dbf74fc577486fc928251792543d7d1e78195484f9",
      "normalized_elf_sha256": "97630dbc90ad5d4a9af7fcaf1d841e33c52424e1953edf2e57a5279057fc3d50",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Rail/alRailKeeper.o",
      "current_object_sha256": "a7bb0494fb579f0d1fa47989a86cd9a934e03acff9fa95f8d0701c7905543b3f",
      "base_object_sha256": "b2c05443266674fc3f69bae43bfbfda275b5b3145b4cebbd18f41db7026b580f",
      "normalized_elf_sha256": "ae420b0f898613b592534e1b9d2e274deaf28e10bf9b5aad31e40ef7d8e51f94",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Rail/alRailMoveMovement.o",
      "current_object_sha256": "428deadece45e9a5ef2e526d7afa7c97f37522ab5955d89c05530b3059e09290",
      "base_object_sha256": "a29e4f3f005b1549df1d7a1eed5b14f3eab495e69abf3168f2a85a3ae2bccf16",
      "normalized_elf_sha256": "95efe7be60f7deef5c1fd5df4ed63a60f52efd7b1ee2015474305f3f8139f4b9",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Rail/alRailRider.o",
      "current_object_sha256": "afba40983b472915c1f0dbb8e8cb08beddf2ef6c9d08cfe1ae290c3cb4ada424",
      "base_object_sha256": "0aaf73aae8cf759a18e94a94d6cd5f907af677b183ad7ebc1a176a3822e0425a",
      "normalized_elf_sha256": "7afa2586151d257fb2dde2d80ab5057d4a8d44ecb7dcd67bd2010e8a61a36b3a",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Scene/alScene.o",
      "current_object_sha256": "afba964e47743728d4c510fd86c901a57ca04b9f4fd94ef8960d95412e47e45f",
      "base_object_sha256": "82efbcdfb6c03874a7ae46949b279888ff0c83b1458fd80951a5e15ee10b329f",
      "normalized_elf_sha256": "aa0991717afcdddc362ea6432e6fd3a3ee2e46f03c7c3819132b284ebf743758",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Scene/alSceneFunction.o",
      "current_object_sha256": "cd3a248091496f1aa7b538d33ddd55de59910f05c0c3d0a6f4440ab6ee977abc",
      "base_object_sha256": "6e5ccf18ff1b8bf6e4b2ad280741db98d73f11dedbaaefb928c937df25b74d37",
      "normalized_elf_sha256": "cac8b6cd9f04b439b93286f580321b2ef80690327bec135b49e68fbab982b382",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Scene/alSceneObjHolder.o",
      "current_object_sha256": "fa520a37b8cbbec02ca401c777210d23d092886b782abb04a0eecfecbdbc0472",
      "base_object_sha256": "eca0f06c4ad9e74e68c3575a806d7e3acc925e1cf93abc4e4576728080f42958",
      "normalized_elf_sha256": "5a46ba568c93de46c417ef858b86a30ee5b6818d37106db40f5b2b4a6b728ba3",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Sequence/alSequence.o",
      "current_object_sha256": "322805690f14a01447043d760ca5b8949ae9d520d609a0d72840c50a515d2f97",
      "base_object_sha256": "53aef3dddf7202e59c32b38b8b9ea9efc5a2711b67eab1b025b2e5ceec087ca9",
      "normalized_elf_sha256": "cfb0e11ac4ca141a15a0c79ac81f25ee6d8d4ec33535ecf40d3a568d5f35a41a",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Stage/alStageResourceKeeper.o",
      "current_object_sha256": "7d6f0d2c924b32ee4c13e87f50672d3e5d041533326cf8e6a937b6d5834f0d11",
      "base_object_sha256": "a012aef53b4468197fe021d30598a11b65b7cabd7d7c601f42f36bc2d14d81dd",
      "normalized_elf_sha256": "bd59bc86b62d24e4db084b3f344ad1df9bf9f2946324aac7c002c4c31fe10812",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Stage/alStageSwitchAccesser.o",
      "current_object_sha256": "d4e20c983e175ed833b41b6b0f72b83c3fe8c3df511832e5115f5baf187576f9",
      "base_object_sha256": "fd5a6c4466a01f530783fcfdfbff7397dbaa70268c6f928825570389643934b7",
      "normalized_elf_sha256": "561b1d6ac22fd88b68d679a3d466a945431484621e2f04ac953985c1a32ffa59",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Stage/alStageSwitchKeeper.o",
      "current_object_sha256": "35d4a55f998449623544665d596f1a2ec5921e43a99ee1cb04a11ac987c3d84f",
      "base_object_sha256": "95666a213fe62f694516fc523597545b3797fdc95ac5672829903e8aed29dc9a",
      "normalized_elf_sha256": "a4140f84a08461ab41178dfe6b5394bb14c7152cec07d3b09d944ded615566e8",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Stage/alStageSwitchType.o",
      "current_object_sha256": "782bee6fcde1f1a189af41bd00d682a0cf6ff352ff5c296adf18e4d5455b06b3",
      "base_object_sha256": "df7fa6672aaf421c3d470528dd54fbc00ac940959456cd137e6f2a988d5a2dcf",
      "normalized_elf_sha256": "64ea9db7707abfb9ea0a5c84310b8f9572571722cc96cac5322d154016fbb8db",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Stage/alStageSwitchTypeCount.o",
      "current_object_sha256": "cc789070184ec7b4249f21452c082e817e65707b60dee83819fbeef34a18d706",
      "base_object_sha256": "dfb95b44114c98ca19ec9536a71606e8fd99764413a81b9de2705805f60772eb",
      "normalized_elf_sha256": "6836d2a632229c14f24a29ab666d28f5943745aea1da175c7a465e6ca012a0f3",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/System/alSystemKit.o",
      "current_object_sha256": "b902da1345cb857c741dfa1e7c8ce681ad97010673a9d08f1ea69173f0340456",
      "base_object_sha256": "8d3c248ac5d0063726e4d438b6ba68d513f4faa42d7cc5f74730397015026412",
      "normalized_elf_sha256": "87168e4fddcac755633fdeb2753716e592dd95df30b6790df17f47959f27d5f5",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Util/alItemTypeLookup.o",
      "current_object_sha256": "ea076ccc271e0572257cc545e65ec1991929e19841f2dc14a7c3711fb0e19f1d",
      "base_object_sha256": "b1e67951e276e03eb7efd59509cafe0710dcfea304c76e289d33d05d6f49bec7",
      "normalized_elf_sha256": "7ff951bce9f1805540777a70798e28ddff0e9e7d345281bb48937d3372e7999f",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Util/alJMapInfo.o",
      "current_object_sha256": "02ff20cac296e3e214c03df4b45cb977ee7920a0c442fa33906ba1b48c4f1302",
      "base_object_sha256": "acee45efa6e4337c79d6868d61d942a678b74aabc2fd091fc2576cf506c41860",
      "normalized_elf_sha256": "b4ff065af42834aea893c95a65c609321f35798c9cdb61e8ed6c5f9c77419656",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Util/alStringUtil.o",
      "current_object_sha256": "d390c162ec022c752e01fe7215ecbc92d4ee18f9dcf661bcc72bac455b97f947",
      "base_object_sha256": "ef2200fbf8ccaecbcf84a12622f4a982e226734f916aa65973f0a6d2ff31749c",
      "normalized_elf_sha256": "36cfcdd408f762656bbfbb77efc13bfc5accd8e963b015d2b4d86d86be18477c",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Util/seadCalendarTime.o",
      "current_object_sha256": "2421623255d90087dc9804fa7594009ea6bb13f9d2c19026b27519f94786d063",
      "base_object_sha256": "fd72fc6618878cebb14a3f83327b2bd5a98a342b0a1408402835705545f27ad1",
      "normalized_elf_sha256": "af7ea804b5f6c43f5695b1e88608ace6794442bd30eb9032525e91089ac998f5",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Util/seadDateUtil.o",
      "current_object_sha256": "d9398fd2c34cef81a93d3f6e844a52614f1a20d4436aaac7a6868034cf01e056",
      "base_object_sha256": "d1c1a1d1aebf630fcf1c3153015a4a893f244130a73ba855deb65df41dc91f47",
      "normalized_elf_sha256": "49ae62815491f5ec7ab0e017df68d5720d4af4cd2d1ddf82b437f7786be519ea",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Util/seadList.o",
      "current_object_sha256": "f9eeac6f4a23ab5d989252d7548277915f128b342dffcfc4986cacd4514c0a68",
      "base_object_sha256": "fc325112100caa189182c3298ab09aeeb90a75e71ad4a78a4b13aa6cfc61ee51",
      "normalized_elf_sha256": "d06f6c5719eaab5583753694146312a5c21dcd2c92c4fcac55ed941a3f9098d9",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Util/seadPtrArrayImpl.o",
      "current_object_sha256": "106e52ecf32d64d6a0f4921fb04ede25dbf7930d02c794412ee22bfb46339231",
      "base_object_sha256": "c60d809d674a1eb5e4bd0964812d2bb95cf9be2f1ed1672c3908a63154e4ef22",
      "normalized_elf_sha256": "742228c40858b7ebef690309eaf81d36025b924b96700730835484f3abc66f52",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Util/seadRandom.o",
      "current_object_sha256": "a593865fd1047937150c8738e4e1608d9bc0b9d44479559ae1bddece4abd30db",
      "base_object_sha256": "05c5dd310d29874273c4e79e5230095bf6c1527f6ec2f9a0dd680b21ae7267a3",
      "normalized_elf_sha256": "453e752b4c16f73dabb4eec1f3aac79e387c0573a45df25da7ea8726c828ebc4",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Util/seadStreamScalar.o",
      "current_object_sha256": "17e1e1422687efc150fe5e6529770984e07ef4cbeca3b89188201c80ac2325c4",
      "base_object_sha256": "d8209eebbbfe8f262e9d43542b7d0b80e94e13d5fb86896d144aff6fba9bd757",
      "normalized_elf_sha256": "c8f5c29ef2f732208b0eeb8990843a6d86d672ab3e341631aecde876d0ffed14",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Util/seadTreeNode.o",
      "current_object_sha256": "ad6b651183c5016a21fae197dccac40863012479d8697db10f91bace01200b7d",
      "base_object_sha256": "44355e09f9b0be120ad7a353fe6fd3af9767d0241ca19d4b2a7f734eabf052fe",
      "normalized_elf_sha256": "7cad21bdb52ebde3df8d6c5facd30337365dab3dfd127916aebd6eaa45a2ba1c",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Yaml/alByamlHashIter.o",
      "current_object_sha256": "6edc55b2ac71b5ee0d6a17d52f0f136498bee98fdec7a22756a10b7fd379d269",
      "base_object_sha256": "908662469116df8aad55f4a45c0b31e02df12452f97a6ab05af6412f4efbe114",
      "normalized_elf_sha256": "0b1aed48214ccbabc6edb4c56b10c4de1e15db4c7d8a917dd2f0f38dc1d910ef",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Yaml/alByamlIter.o",
      "current_object_sha256": "f6cc8b870923f582a9fb3f4354c3e57623f7a5b8c70db4d660ae1c79c194ed1b",
      "base_object_sha256": "047f8e6ba97120c81bc98c2692bf3f535e466ef9f5e9aa839e4de05bbab88ea6",
      "normalized_elf_sha256": "6b0041e845031b391c31b8532559489b87a00005aee427aa54e6df953dfcb73d",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Yaml/alByamlIteratorConstructors.o",
      "current_object_sha256": "b0ec48f1567fe4085839c3b507fb6f0dcc1ce0f4131e481bd2a73f679d6e02f4",
      "base_object_sha256": "a94caf1f6758da77b92845a46ff0c6be655827eb602cbd31f924871f70e0894e",
      "normalized_elf_sha256": "57c85d8676f8f3d90ccf7f182ecf1544557849ca836536fbe61fbc672aa2b9b7",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    },
    {
      "object": "lib/al/src/Yaml/alByamlStringTableIter.o",
      "current_object_sha256": "95d5a30cf606f750d0761901bcf498ddf3e28c9b694e7a080521954686055c92",
      "base_object_sha256": "5976bef18b7453d0fd9d738ce60445dad1e72b4d25830cc9443eb3699027212b",
      "normalized_elf_sha256": "7c103daa8ac8f23c70845932f62e0ff2da83f5cccb81ad2d2805fe2cd000aff8",
      "compiler_sha256": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d"
    }
  ],
  "inputs": {
    "lib/CtrSDK/include/nn/types.h": "40aa4e5ee7d8980e077963ee87bc6bdeb8908249603c073b5980aed937d5e0bc",
    "Game/project_globals.h": "6c0b7103d61651e77e1a5db8bb1f5bdf509ad3d64c419c4ba22cadaed6a933d1",
    "lib/al/include/Nerve/alNerve.h": "131fc15b422681d47eccb569de9e24f3990b7ad6721d9fafa30be6c791a27ec0",
    "lib/al/include/MapObj/alMapObjActor.h": "2034a69366356207c8975bfcb8b2f2ef385d1d614fabfceb24e7b4810800b2e3",
    "lib/al/include/Nerve/alNerveFunction.h": "cc7b91a20cf27d2f1f9fb86e0472cb90ebee5609ea7d15fb623b43c2ee1cd267",
    "lib/al/include/Nerve/alNerveKeeper.h": "28065b7f6feb35d8a0ab88de3bb9596758dd816d3084e037dfd2f129eb16fb46",
    "lib/sead/include/math/seadMatrix.h": "c4ecd6d6b98b4851e81d9fd144de80b51ca13f4370fb2baf5982a42f6e69d914",
    "lib/al/include/Effect/alEffectKeeper.h": "6d29cbb16f52fb8f3cc14374d3784e261d19309b3475456170a7bbc66a073451",
    "lib/sead/include/prim/seadSafeString.h": "e1a13277a1e5c3278d9811cd92e7d737caca4d7ae1bc9b604b99a45e3bbd1da7",
    "Game/backup/include/Enemy/BlockDragonGenerator.h": "45612d99acc8d51a41568aa6a993b831a810f20ce7eff27e42f7498923fc1f5b",
    "lib/al/include/Stage/alStageSwitchKeeper.h": "58e83a38a685cdca5d47900c75f5618049aec86b9f16bbf2876807ab7d625287",
    "lib/al/include/Audio/alAudioKeeper.h": "69a1a49e3aa18ed22c79592ff78c73a974a3022acf05048365e347bedb7c4a4d",
    "lib/al/include/Nerve/alNerveExecutor.h": "7b0b79c632956a3697160a53f5102c6634238473b2313212919d2937a0100d5f",
    "lib/al/include/LiveActor/alLiveActorFlag.h": "bc855adbda47baffa6061c368ea907c0ee22d9b4009a2e638cbb9af0b8a85c8b",
    "lib/sead/include/math/seadVector.h": "1416056d4201ee62fdc2d2c06b5bd418f3195f8154a74b122869ff8c5f517169",
    "data/config.json": "5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45",
    "Game/backup/src/Enemy/BlockDragonGenerator.cpp": "e9df9c903a8e8c0e0cb2876b59a79b1614f8ee0b640b16a45ee4c0f74c737721",
    "lib/al/include/Nerve/alNerveStateBase.h": "115f8687977cdb0d075d335f02b8c958ec85682418e84e15df5d2ecd392ae953",
    "lib/sead/include/math/seadQuat.h": "84b3de2e49e5f0505d26ae1e9f5acf782b446507254c6b0e94657350cd1355be",
    "lib/al/include/LiveActor/alLiveActor.h": "472992d0d1a423b4de484c00ffe79fb556dfdd111b84b859518142d5d5b48f34",
    "Game/backup/include/Enemy/Bubble.h": "c930766da9ebd67ee82a229d8027fbb601e58cfa3b01bdde15d6f499c1542ea2",
    "lib/al/include/LiveActor/alSensorMsg.h": "f2a557f0bac77850ba1d23da1941fa1a458983fa20abacdf1d4f6d863d671892",
    "lib/al/include/LiveActor/alHitSensorFunction.h": "e50328b39d81b7daf03e1c22742f9d06ee73a6cbe11b6b49578064972d541723",
    "Game/backup/src/Enemy/Bubble.cpp": "20159a74c4fec2e9258bc19d7864396409f8bdde859d3fbf46bf16826a17758b",
    "lib/al/include/LiveActor/alActorActionKeeper.h": "4deeada74186225f74d1ccbcaa29ee983be20a713ebcebcaa051000864a1508e",
    "lib/al/include/LiveActor/alLiveActorFunction.h": "d572d752bebc4c5f37571bd1f7a96a8f3e2f5dc7d27721abee641fbd78518233",
    "lib/al/include/LiveActor/alActorInitUtil.h": "689ae3d58eb6c6801fcdf0c37866b0c05a28cc42abb36a08b498187821701c2f",
    "Game/backup/src/Enemy/FireBall.cpp": "a9041ef217e9ad23b06a8a87b95c224ec55bdfc0ef67e9aa9c6b355850ff447b",
    "lib/al/include/Placement/alPlacementInfo.h": "654fc0b4e0df90e6320615cd2de35a785e455270a2449b9b649bca92103c73af",
    "lib/al/include/Collision/alCollisionUtil.h": "bfe26de48b1ad713db43428fd66e56b77fb6482294e550a683123b68b1b60896",
    "lib/al/include/Yaml/alByamlIter.h": "3ceeca89571abeb56431268928af2c13cf412debf7bc751a0b46ffeeabf6a4c4",
    "Game/backup/include/Enemy/FireBall.h": "76e7495d5d3a1d19b0b5bb92cf5ff27602cad4a5e1d9f84a51d57e5a14dace12",
    "Game/backup/src/Enemy/Fugumannen.cpp": "7811a9b887e53b8e114961f87fa2529c8fcdf9681b107658a9d4e64f00dff15d",
    "Game/backup/include/Enemy/EnemyStateBlowDown.h": "1d5f96a4459f2f12be538a1e69f42c9bc7d3e5cde90e32f65c19cdad42c56e5e",
    "lib/al/include/Placement/alPlacementFunction.h": "a75dd1efbebb8554cb9cc105758857ef4aa5237ffab131e60deb4d72bcf0e586",
    "lib/al/include/Nerve/alActorStateBase.h": "4c1e9741c95a4a40fdae00d33a972638ae00c7209a3dd7e6b317647de6995ee7",
    "lib/al/include/Rail/alRailFunction.h": "0e0764d003ec197ed319fecbdeef2dc59635064e0e0928e5a8179702394a6535",
    "Game/backup/include/Enemy/Fugumannen.h": "de6203a8062d061b30959bf6fcb198774ffa9413b0de3c227fe26cab94116d1f",
    "Game/backup/include/Enemy/WalkerStateWanderParam.h": "80f1a7de9d7356e3c0e031887f9272cbdc652012fd60b6f28894781047de7e9a",
    "Game/backup/include/Enemy/WalkerStateParam.h": "d71ae6df12fd9cb76b3a39ea578dd40eeb0d75f3ca2717e3f190a58aab898b2a",
    "Game/backup/include/Enemy/WalkerStateChaseParam.h": "45e846360f83482e1c40aa4eecaebf52ed6ca97eba576e0a8a244f1ab1f87cfd",
    "Game/backup/include/Player/PlayerFunction.h": "44c8a1fa811de71dc70dcc1b1231185c1a6bd8b331833b66cc3be33e26182616",
    "Game/backup/include/Enemy/WalkerStateWander.h": "5de6f5061c0b9bf79757c85de5f0cce8ab0275d8453c0400ed9936b423538606",
    "Game/backup/include/Enemy/WalkerStateChase.h": "77dffd7df67b3c707154cdbf84a0e427d9bd1664624717e876b764f7ceb5bb5f",
    "Game/backup/src/Enemy/Togezo.cpp": "f567b2921c4bcfc69e7790d250a7de21dd772e57d011f461454a7ac537f15dcf",
    "Game/backup/include/Enemy/Togezo.h": "50877e3f270163c5d99d5b6236de33030f63c3a17dfb29cae6f15bef3c1fb8ab",
    "lib/al/include/LiveActor/alActorPoseKeeper.h": "b7ae48418c39def43ff701ca81857fd7279759d11f289fbde42ea9fc53c982af",
    "Game/backup/src/Enemy/WalkerStateChase.cpp": "b5b800ad0a86e94fe8d757a66853750fdb76a4c8755b817434a9859da8b3725e",
    "Game/backup/src/Layout/CourseSelectMap.cpp": "5d8cf9270d4eba9fdd8611fbbed0b4db9fbf8a2470d50b7bd3b58aa068c3939b",
    "Game/backup/include/Layout/CourseSelectMap.h": "d429ef304dc97d6728e66d35d94a8890240bebd264c6f450bbf1128070064a1b",
    "lib/al/include/Layout/alLayoutActor.h": "81cc5b06fd3c48cb8c32f96b107fc241ff7ce123c13c49e544a72078bcfc5be1",
    "lib/al/include/Util/alStringUtil.h": "771f227839db432fa33bed9f8490dca04388ffae402987ece80b6d0c109240ad",
    "Game/backup/src/MapObj/AppearStep.cpp": "1a11067152893db55542fd91462cc8df09facadec6a37135a4a786bb3121581f",
    "lib/al/include/Functor/alFunctorBase.h": "148e911b94f9e70f2dca871827f4215f761bdeb2a8b8bb00852d6554fe848fed",
    "Game/backup/include/MapObj/AppearStep.h": "a8d8a62f76fc2521eb772ab3dde614e202192f675d026c5e7557806eeafb067a",
    "lib/al/include/Functor/alFunctorV0M.h": "0755b49dc38c2da496ccfbcdec77c08fbf5da40fb96534478fced67fff67e639",
    "lib/al/include/Scene/alSceneObjHolder.h": "561a2d5412707a9ca2895aa0c0868b36a029b805a2535a9ba51891f8aedd3a01",
    "Game/backup/src/MapObj/CoinRotater.cpp": "d45772f71b645b69da2a9ec71a7e80ac340ec83d3bf8e43c96388af19ec95153",
    "Game/backup/include/MapObj/CoinRotater.h": "fd069daacfa27f6bbc82d2e6cada22e85d5ea6429be615d0f0a8619703f1139a",
    "lib/al/include/Scene/alISceneObj.h": "31b797683621be67385e24f74d4708867edc94db2e749a791c2c77baf3bb7a41",
    "Game/backup/include/Scene/SceneObjFactory.h": "5603a43f3f0b4106b71aa506632f8d9be0045d78de8f812f1d01bb8dc2166dad",
    "Game/backup/src/MapObj/Garigari.cpp": "0e29883915cf60c19875c694e228113ced6eb3e85aea8e819610c288132ead14",
    "Game/backup/include/MapObj/Garigari.h": "4bda7a7fcc0edb73bba8141163b1548b6bd5f9af8fc49cd6bd7fb3191ea2afc4",
    "Game/backup/include/MapObj/NoteObj.h": "5503d53e9d25a3d234ac4b425da3078e98bac6a31d8fad28b62d9eac51762b6d",
    "Game/backup/src/MapObj/NoteObj.cpp": "6db80468a498b063d0e695f2ea11fe44be017b5320c258845dea438547be2799",
    "Game/backup/include/MapObj/NoteObjGenerator.h": "5a17023db653e9f8a007706d681bc350970df5fde94df64b5ef1d3c9e5da2dd1",
    "lib/al/include/Nerve/alNerveActionCtrl.h": "04742c3b612755e2757870fcfbd7cabfdaab439234a44fbfb68618db9485f875",
    "Game/backup/src/MapObj/NoteObjGenerator.cpp": "4444edf33f7e1119aedde293ed061c767fd2e8cd2d010c02eb8b8d1e8857ba1f",
    "Game/backup/src/MapObj/TransparentWall.cpp": "1b5ef45491883ba345834bc11d6642a8f72f230549e40ccb4b2c834e3933ad01",
    "Game/backup/include/MapObj/TransparentWall.h": "e976de66e940aec9ab799273d4ed5fb04291abb779cfe7ccff8d784a4dea3d38",
    "Game/backup/include/MapObj/TrickHintPanel.h": "d577cd76e18661cd7067ab832609cff48f5d95d87e97e0e5eb9dffde988a5fc0",
    "Game/backup/src/MapObj/TrickHintPanel.cpp": "d6edb690c17030cc57c153d4cc2acc509e0bb5647a2bb38f88aae873afedfc8c",
    "Game/backup/src/Npc/AquariumSwimDebris.cpp": "1dc37a6c417ff58af734c449915fb41689af1ec04db8ce9558fc37a99b27adc2",
    "Game/backup/include/MapObj/AquariumSwimDebris.h": "513dcd3aabebb83d2a184c6b65a2f4dc64d00000be0a119d4e36b61ac8df5fcf",
    "Game/backup/include/MapObj/FlowerPot.h": "f137717bd259b144e4a332cc646450943c6094cd29e94fb071489e1cfd15af1b",
    "Game/backup/src/Npc/FlowerPot.cpp": "d81665e80e361718c5ce4a7cceba2f35804fbef15c9b5a4d6d17517a475297f6",
    "Game/backup/src/Npc/PeachRope.cpp": "8b32931ab917888a9b3d48279c8a6c82075a1a73b441fcb64f1b9e55f4667ec9",
    "Game/backup/include/MapObj/PeachRope.h": "59bcae018eb44cf0a9a2338a7f0a1ae422730f828ddbf12c0a1cbace64d2c6d5",
    "Game/backup/src/Npc/RailDot.cpp": "e26a5ff2c50d4e3191548d048b679ecb767e861a6560a0d6fff47685bf3047cb",
    "Game/backup/include/MapObj/RailDot.h": "c46b6980c69a5766ae825429fe916fcc6ac20453b559d11926d5004ea3f156ce",
    "Game/backup/include/MapObj/RailDotEnd.h": "584a6ff9b8ceb3981d640227579bbcf741f322ec771e5968cd57acbdbff78cf9",
    "Game/backup/src/Npc/RailDotEnd.cpp": "3bb3eaab9dea29cf11654724c4c2ac7e4001ddb869c578bf9c2e11a22261dc41",
    "Game/backup/include/Player/GhostPlayerRecorder.h": "73eadad586f57f5edd0007eebcf7335555a53334815104b69fd2d6a30f9eae6d",
    "Game/backup/include/Util/SensorMsg.h": "db2cbec45c0af4e8e5e863a37a8059c2d9aa1c9da6553f3f729fbdd226adb73c",
    "Game/backup/include/Player/GhostPlayer.h": "ede44f23adca000aeec1f05861e6e56f38bb61db0fae71a78a0d9bb47bad3195",
    "Game/backup/src/Player/GhostPlayer.cpp": "cfbe6459351dc787d2aaa4fc11d8f83a99cf5ab38951e8cf1586826985007df7",
    "Game/backup/src/Player/GhostPlayerRecorder.cpp": "06ed22e6a70893a089d12d488660b7f38e0b73cc91df3e66755b3d7360500760",
    "Game/backup/src/Player/PlayerActionCondition.cpp": "29d0474d4907164dd4071d4fc3c2e9365595c34d65dba14d62cb68b8b52754b0",
    "Game/backup/include/Player/PlayerActionCondition.h": "9a5e092e31de04580965a7474db27cf5e650d677adfc65986b2c4c91ca878a50",
    "Game/backup/include/Player/PlayerAnimator.h": "be87af4c2e2fd6c6a6534c9c0634ba2f25c7a97412018fad5e224ba8b39d57f5",
    "Game/backup/src/Player/PlayerActionConditionAnimEnd.cpp": "0b62d212ddf1696b5b9bad7f5c44ed52e6c7df43ff8f909b7e867a0c86d51c0c",
    "Game/backup/include/Player/PlayerActionMultiCondition.h": "9b0d1b076550c3392401c660c68678f0f23da73404aafd0f3d9d6f751c14a893",
    "lib/sead/include/container/seadOffsetList.h": "e19263f7a30ae4f48e9626ca82ac651eb21d4b1302e1f867bc4e84d719bbcb78",
    "lib/sead/include/container/seadListImpl.h": "84883f9628d178fae551a77c9efb36cd94c20663246f6935bf7eaa4de7fa23a1",
    "Game/backup/include/Player/PlayerActionConditionAnimEnd.h": "fe75f7345c9ef8e0a7e5069dd1b2ec2e7aea5017267e1c0d824d20676018b9e6",
    "Game/backup/src/Player/PlayerActionGraph.cpp": "62e5e6546df94022ddbb3c5478f053f9fb4ac4933fee2ce06392574ec434243b",
    "Game/backup/include/Player/PlayerActionNode.h": "49bd3b77a84b301becb699f326d30f9a44bbad0550f45537cf8d4d2e85fa743b",
    "lib/sead/include/prim/seadRuntimeTypeInfo.h": "57674477a7b07d5f982969a6824f9b739873956b493cd5b1d9d2fb7313bde5d1",
    "Game/backup/include/Player/PlayerActionGraph.h": "fbe5b9dccf2335b6c4eceb473af2a15831fde21c7578e2a1bb2fad2162fc74b4",
    "Game/backup/include/Player/PlayerAction.h": "b1d399524fc9dc83dfa364ada75d58207d6afa6e0d85b836072826a54b221204",
    "Game/backup/include/Player/PlayerActionGraphBuildOutputs.h": "5bb9bb3cfff38033c796897fcb78baeaae1e0fa7f9323883890012e09c894582",
    "Game/backup/src/Player/PlayerActionGraphBuildOutputs.cpp": "92287c5dd401406058900f92bc2e3d4decc71a2dd5280bba89b4359aed2c2b7d",
    "Game/backup/src/Player/PlayerActionMultiCondition.cpp": "2090e792c1335e924d263b3d1c58376dc1f4ad564e88ef91c5b467c1ee387184",
    "Game/backup/include/Player/PlayerProperty.h": "ca37c48fd1852837de5a242f0b3948d21c2b7b62f951ffdfdca8fae16f09c8a2",
    "Game/backup/src/Player/PlayerActor.cpp": "3f7b62d3cea0670f0199e1f52a31936e6ad4bed130704a73a00a94cab57996e7",
    "Game/backup/include/Player/PlayerActor.h": "b7a19a01f09cb6e9692cd2a2773d00c795054ee0a1636d849434df30289342e8",
    "Game/backup/include/Player/Player.h": "174402dbc2bc9ce05d4ab4ac4078acfc6bc57e67354fa9b81d2e6bb454e88257",
    "Game/backup/include/Player/PlayerActorInitInfo.h": "7ec348b5f89f6d1ea5c2464d81a5d0018efce2d70a375c4f192df8363b81d02e",
    "Game/backup/src/Player/PlayerActorInitInfo.cpp": "226a05d6693a15be5be99b550f4785c3b78a5be94bfdd540ef40a0445fce46cf",
    "Game/backup/include/Player/PlayerInitFunc.h": "6813e21742ee6d54b36d564f8db006b16af97d159d2d21bc6baede01ba5cf835",
    "Game/backup/include/Player/PlayerAnimFrameCtrl.h": "fd3e099b7ebdadd2743a1e530b41cf4ad3d98645406e83ced7ebe54badb7d980",
    "Game/backup/src/Player/PlayerAnimFrameCtrl.cpp": "44b84f10445b164091c1eda5b693223926d51f1ce78c6aa7863f1153f846af86",
    "Game/backup/src/Player/PlayerAnimator.cpp": "39091c04d1d68c7de6ee4ac7f7349540b684990a111cbc96319241b02c2c9f93",
    "Game/backup/src/Player/PlayerFlagFunction.cpp": "dfda6f23ac35597c3bffffbb5dd23069f94d6b55bd225402e03ab663ec10a32a",
    "Game/backup/src/Player/PlayerFunction.cpp": "7d375d0729b56fa9748f3d02d4e795169dfe1a567fa288aceef162ae8d74438b",
    "lib/al/include/System/Application.h": "f70d0bbf05702665afd91bd0b768098a1509f04b4bd937ef3d6ca491145faeb6",
    "lib/sead/include/heap/seadHeap.h": "8aa7bfc83bd48ee72016f904bc404a26c8b745efb97056a659049a8a1d8a2204",
    "lib/sead/include/heap/seadDisposer.h": "98a2209619a0ea238bf9d253f0532053bbbc32cb0f17c1e6cc7376e697d384d0",
    "Game/backup/src/Player/PlayerProperty.cpp": "2840fda1976fb989add3a9c083f1b63d786a8d198682ff7450de802dcc2bcf0d",
    "Game/backup/src/Player/PlayerTrigger.cpp": "9dd1427bc1c677de8d2c89af9056b0c0942585d70aec844bc1db97bc1a0fe344",
    "Game/backup/include/Player/PlayerTrigger.h": "40c2829e6444626d5f2fa2b20c00d18109e1f4afc1891456eb3632c7c874b3a8",
    "Game/backup/src/Scene/SceneObjFactory.cpp": "79ee6a6e19e10db6420c0d882121480fb6ddfa48b40eb461ebf36b621011ef4b",
    "Game/backup/include/Sequence/ProductStateCourseSelect.h": "a02130937023f3ddfc5b416830de63495cf51ec2ba5298919b55dd4c4f637f32",
    "lib/al/include/Sequence/alSequence.h": "3fd0f8365be1cf52ee9036ecf2168d0f53c78b3e16128cad4ef731f0823c5862",
    "lib/al/include/Nerve/alHostStateBase.h": "56688fe1eaf506a930d00338bf7cbbc514f6ff7326cb51202ec26cc78a6a2eb9",
    "Game/backup/include/Sequence/ProductSequence.h": "90daf65f284c42536a46bfb37ecb5ae41b17439116487821a281b3ff6a84dd56",
    "Game/backup/include/Sequence/ProductStateStage.h": "4b17871c2a00bab22708cdb24e06849871c2c51cba1d685ab3034a796c154347",
    "Game/backup/src/Sequence/ProductSequence.cpp": "07d82c65ed86010b0001a9597307782cbb331a6ddb086d997381954cfb1f8358",
    "Game/backup/include/Sequence/ProductStageStartParam.h": "d84bc9df686243654b54563b712f72e34b369aa78f6faa9dd78986901945791f",
    "Game/backup/src/Sequence/ProductStateStage.cpp": "d899bebc6f561457ab813443fc384208a75568f7041c33703fe0467d5ffd6fae",
    "Game/backup/src/Sequence/ProductStateTitle.cpp": "e80e0af06426e436af5c1b412d146efc9786d7d64c46cb4541aecf3f491b362b",
    "Game/backup/include/Layout/WindowConfirmSingle.h": "16ffc71abd6073d610ef9b1d9ecd73898c3576f781823887314e487c66476e0c",
    "Game/backup/include/Layout/WindowConfirmButton.h": "61e38b452fb82102b2c238cdeb7b8fd5a53f083113bceb88b38aa8e197cdaf81",
    "Game/backup/include/Sequence/ProductStateTitle.h": "b08561af464f64e8105865af3574bcb1c4a620cefb32a89e653769e898b1e18c",
    "Game/backup/src/Stage/StageProgress.cpp": "814aa04d68923d5eee26502c85e58b19b99c4402ddc4a3490fa2f6270163fcb6",
    "Game/backup/include/Stage/StageProgressAccessors.h": "5b77389a0786b920e4282ab7d874df851f55778db55cda8ed7b44d2d4e27fbd0",
    "Game/backup/src/Stage/StageProgressAccessors.cpp": "d399921817ddbd26535fb2e3bdfd6c7e2322067e7538c89dfd9c1713d69895fd",
    "Game/backup/src/System/Application.cpp": "cf20f7ba469382b22fd5958f826437bfdb44a3addf833aa7578edb99afb67ae7",
    "lib/al/include/Resource/alResource.h": "9f2cdc2bf50ed82994fe19cc50e22f960d94a4154ce985ac83ade910d9f372db",
    "Game/backup/include/System/CourseList.h": "57cc60910de32098b88364e84c7ca54824d043d4aea4984bf42f266e45d67642",
    "Game/backup/src/System/CourseList.cpp": "7dce8815207a9e8b7b1214c2c6995ae5cdaec00ae2749259eb58ea5b5152465a",
    "Game/backup/include/System/GameSystem.h": "7f1042f75764f035764fb0f145c2109e592f7f6f9ca2b4371785473243c13c12",
    "Game/backup/include/System/RootTask.h": "783ed16a48ccc69bd0dca6c92fa0f7c64645d194a7189bb223f1b6bbac4e0ebe",
    "Game/backup/src/System/CourseListCourse.cpp": "fdbd40aed6e474f098167648f61d34e16c8e3c834ff9ff21b80eb9ea03005b89",
    "Game/backup/src/System/GameSystem.cpp": "24924ec5af1b4e404e8aaa2139c4e30dfd112c2e4fabc6e26ba932e93a9a79f0",
    "lib/CtrSDK/include/nn/ndm/ndm_Api.h": "91d25b884a2849b2560574fc6b62b3ce913acd0525593f287c47b2a12184b500",
    "lib/CtrSDK/include/nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h": "a889489dc1cbb40fdd4201c90f0d7233beaee0a0d7f5d8012a62863dd5230859",
    "lib/al/include/System/ApplicationFunction.h": "9fbee0b513658a3a8e5b8d4889138e6661ef672919fe424751584f0144bc30cc",
    "lib/sead/include/heap/seadHeapMgr.h": "d3b42b46dfb0cd80ce12f1fb390a2f32ba72936479ec16d4306b4ed402daec0c",
    "lib/CtrSDK/include/nn/cfg/CTR/cfg_Api.h": "8edd36b460a30c7898cbe4e3e2253180632422849a0b99982eab7b82d8c066bf",
    "Game/backup/src/Util/PhotoScenarioLookup.cpp": "b107dc45fc711ef37dacd4307fda355246af0c8b0115ee223d88048423bda4fe",
    "Game/backup/src/Util/PlacementCategoryLookup.cpp": "6e828c0b55c36793e7a0c379f61732959070188529a0e32d3bf28e74d2209b16",
    "lib/CtrSDK/sources/os_Priority.cpp": "a67047fd2d666c9155874f54023837cee5d2a5ba026f0402761d898d9365e569",
    "lib/al/src/Area/alAreaFlagFunction.cpp": "e1a60c5a97d8d3f806a6a38af759089dc9fe1e3e6f4fdad7c830c3c17e8b1a0c",
    "lib/al/include/AreaObj/alAreaShape.h": "1b93de282db39232cb2105f65a1ee6060ab8ba10ded3a57d52dd68e57ed31bc0",
    "lib/al/include/AreaObj/alAreaObj.h": "a599a6d47901ea7f9d3ef1cf8cb72d8d51328d8f0cc8f27fb18ed1f7fad31841",
    "lib/al/src/AreaObj/alAreaObj.cpp": "59bb0150295774c388becc30411a4f22c39a0db60b33dd25e2368f2358b667ba",
    "lib/al/src/AreaObj/alAreaShape.cpp": "f0077a1a6446523fdd7a20085cd6e78a54dcd2740f66eaed4444e946942534df",
    "lib/al/include/AreaObj/alAreaShapeCube.h": "e14479ec9ab97b5704f2edb8f5ee6238ca587aa1195c44a9a9a9f0237565d92d",
    "lib/al/src/AreaObj/alAreaShapeCube.cpp": "42b47209d18acbb437814405ba45e9cca42ccbf32a3f5506ace0693f9730a2c1",
    "lib/al/include/AreaObj/alSwitchKeepOnAreaGroup.h": "08bdaf7a8e9f9f7111df1b0b4170a3c314109127d95600b22439e976c9dbd7b1",
    "lib/al/src/AreaObj/alSwitchAreaDirector.cpp": "e4a0ea396135d8c48e064e14c460f9e3d8e8901ad82e13569b8e15d049c1c9eb",
    "lib/al/include/AreaObj/alSwitchOnAreaGroup.h": "13130483da1af65a098856fae02ba2e6b251d04c3de7d9cd496d34acacee6a0f",
    "lib/al/include/AreaObj/alSwitchAreaDirector.h": "41261d3b84c73a4513803e541daed599a886f71f00117f8252a055e064611a1b",
    "lib/al/src/Audio/alAudioActionStateFunction.cpp": "0dcb686cba3442f1ef365bf0b6945819325cf8e2e20675350101c0ab6eb0e7c0",
    "lib/al/src/Audio/alAudioControlEntryFunction.cpp": "2c5b4639f5687c7e1ac5f43c1b3c67b414ae9aaf6a04471ae6eee7bcbbce491b",
    "lib/al/src/Audio/alAudioControlFunction.cpp": "5f5a441428a20a6d8c9bdb431909bab6933acd53961f4c317d210b3190ff964c",
    "lib/al/src/Audio/alAudioControlInterfaceFunction.cpp": "4685379cedf53f7a1435eb2d5b130c9a915ac23f769c01a9b68d8dd41720feed",
    "lib/al/src/Audio/alAudioHandleStateFunction.cpp": "90246f33566734103a9c34301b1248f8049e2f5759f9ea15b7ea22ea9c34891a",
    "lib/al/src/Audio/alAudioInterfaceHandleFunction.cpp": "f12f471a91d3085a929405d8ea93b48c0746d9b5ccc88d37830f1186fc97d646",
    "lib/al/src/Audio/alAudioKeeperControlFunction.cpp": "f58656e19caef2ae5fb029df5cd254bf7e428014f1eb49043538f470c4112766",
    "lib/al/src/Audio/alAudioKeeperFunction.cpp": "b2f084133e862a224b9867b15e5a7642a8bc6673005ae5994dae43d2ef2d3d2c",
    "lib/al/src/Audio/alAudioKeeperInterfaceFunction.cpp": "66fe0bcfd8022e470fea9edda1130dfd7a04e30d6470302c0c4ad6a4a4188716",
    "lib/al/src/Audio/alAudioParameterFunction.cpp": "fa9479b35450097dec1ff05acff82d16f31166b5383a1e24bdd7ecbee1806abd",
    "lib/al/src/Audio/alAudioResourceEntryFunction.cpp": "4c5d2cc4b50d6c1bf4a05fb7ae22c8203cff4a05a8361cd2fe096128072851e3",
    "lib/al/src/Audio/alAudioResourceFunction.cpp": "dcbdd83d232c0864208c269642d3f926bd50efcd9156ff948dfd8a7ffa6dc377",
    "lib/al/src/Audio/alAudioSourceFieldFunction.cpp": "4c7f616438a8ff3305c89a68c0dd5f774a723280deb6ff0c3ff17dc2636f18fa",
    "lib/al/src/Audio/alAudioSourceFlagFunction.cpp": "221d4c0410f7418bfdabb98baf46c5e6e6507bee363a3163a4b6a689aefb52c6",
    "lib/al/src/Audio/alAudioSourceInterfaceQueryFunction.cpp": "de6395416e59ed37449d656eee7b401716325dcb3fd4491d2424f45eee4b03fe",
    "lib/al/src/Audio/alAudioSourceListFunction.cpp": "2364dcf6c503df6364b0b46255e4646914c95678c5b8b445159b3e0596b14cce",
    "lib/al/src/Audio/alAudioSourcePredicateFunction.cpp": "fb1ebfc853fc060bebeba1a017ffd5568a54685525462868bc2bf18fee0563dc",
    "lib/al/src/Audio/alAudioSourceStateFunction.cpp": "cbfad10d1fa0a806edca32af86c8bd505eae5ebed3217f0842e85643a207faff",
    "lib/al/src/Audio/alAudioSourceStatusFunction.cpp": "60718b624a3e76c7fbd6b6d8d6bc2f1fbaf71564c89e289074947a8a56585be6",
    "lib/al/src/Audio/alAudioStateFunction.cpp": "2ed6da9e6c0bcd6e8bb32c1fce929df412f1d9f723439cac137c03a4cfaa7fbe",
    "lib/al/include/Camera/alCamera.h": "5b0b4edcced39f5eee15c4971d1869e555e03b248508f4b321b5100bc6ef8ba9",
    "lib/al/include/Camera/alCameraDashAngleTunerParam.h": "3e4e96121ce7a1c0be6d5964760eb466dbac6b5ea8c129cb00b23c0358903632",
    "lib/al/include/Camera/alCameraParamVision.h": "2b6e803d610b4c2aa222e26f338cc3df728bf729addec82eda83c4ab0681c4bd",
    "lib/al/include/Camera/alCameraRotatorParam.h": "52473a857c6932f21d95c044c72aff654cb736838563d5aefcc5f958333a32e7",
    "lib/al/src/Camera/alCamera.cpp": "b44f141df52183dbce3431be9446674aa06a19867dc77fd0219d1978069d39fb",
    "lib/al/src/Camera/alCameraDashAngleTunerParam.cpp": "40e61486f1ae34bd23888874d03f80e20d7c562d505f8abc04641acef90b9903",
    "lib/al/src/Camera/alCameraParamVision.cpp": "9a09421aa398c1fef192d753a7b013c4e41656a82fd10dd28bf052e1c0f47199",
    "lib/al/src/Camera/alCameraRotatorParam.cpp": "8ae7347bdfe9929aa55ad49785743aa40fd35c287092363ecc0b39d32639a68b",
    "lib/al/src/Collision/alCollisionUtil.cpp": "67452aa63eddc5f693dae9b2b5ddbf328d8db1117a81c276fb851769ace0c28a",
    "lib/al/include/Collision/alCollider.h": "e27c39d1a354b904fde1257f0a70816a1b8e878802f8e51c1f898dc7a44e9f9f",
    "lib/al/include/Collision/alKCollisionServer.h": "be8e513b4bd39c6616453de5f8e55079004a80d972e83e0d1fe0328a98d23f5d",
    "lib/al/src/Collision/alKCollisionServer.cpp": "0c7b2569e7cf10f49c40fa02b4bca20854bc77ee02e5fcbda7c750dff69b0834",
    "lib/al/include/Util/alJMapInfo.h": "e803b69f74397331006ad5d7eeb2cacf99dd9f83f61b52558706fe4949674789",
    "lib/al/src/Container/alPointerArrayFunction.cpp": "4c29d21d1cfc4fa816899ae2bdea79c83beeb480e310e31afcdce2c2b0513365",
    "lib/al/include/Controller/alControllerUtil.h": "cabb1fa9aa5d9595b48adf8c240d5d715e60195a257d1673f73c1ed61fcc66bf",
    "lib/al/src/Controller/alControllerUtil.cpp": "83700823bcc17641172cdca8c58c543d632ccb62a7807ec3f6e5970596c80285",
    "lib/sead/include/controller/seadControllerMgr.h": "7de871024e0aff16f33d516ea9da9fc82d036f00df4cdd1824dc4cec0d512d0f",
    "lib/al/src/Effect/alEffectDeletionFunction.cpp": "8dd2736b781b4f53205659416c748b39851d5d79a56c6e449a3408229ea2d125",
    "lib/al/src/Effect/alEffectFlagFunction.cpp": "63a65cb772923f6729b58105a0568f0d65e5ada9d2c7ae0bb6822b1207d296d9",
    "lib/al/src/Effect/alEffectHandleFunction.cpp": "c8635e91630827ca08173d90c50bef4def010bb5dd4fc096fd985fb2429e6645",
    "lib/al/src/Effect/alEffectKeeper.cpp": "8354a86cdd67c266360447c15738345d863b3f92b6b074f6a21fe98cfa311dbc",
    "lib/al/src/Effect/alEffectKeeperEmissionFunction.cpp": "ddf782772dee8355be95ac1afae5f14a76ed2c7cf641807c1f5e4d7236bfd189",
    "lib/al/src/Effect/alEffectKeeperFunction.cpp": "63c85bf3dcfb3ae30e343777eb5de2c8618260ca883b1138ae65ab8347b65495",
    "lib/al/src/Effect/alEffectKeeperInterfaceFunction.cpp": "21b9ba6176cc2e33d6cdbdd6541237adb775b60bdaf083cbf8f941ea55d7930e",
    "lib/al/src/Effect/alEffectObjectDeletionFunction.cpp": "919cc5f4aa09704a36f7e1db5271ddda6df33bf2fa3c73be97e63b0f924172b0",
    "lib/al/src/Effect/alEffectSetFunction.cpp": "07f0e5e2a0aba4102ac71e6818735669498d056d10d6382b2ca3a9412dbfeb9b",
    "lib/al/src/Execute/alExecuteDirector.cpp": "c4e590f66fd12cb33747e9e738559cc5dfd496263466f3c3955c797aa75090ba",
    "lib/al/include/Execute/alExecuteDirector.h": "9ad974aec48501d135b4766b9c519d98b0782367bb4a39c0102d0ebfa4f84848",
    "lib/al/include/Execute/alExecuteTableHolderDraw.h": "38528da78b90033d1834f5c62dbea76d7966baaf02fc661240e1a1b65c846835",
    "lib/al/include/Execute/alExecuteRequestKeeper.h": "52bdab31fc2c55ee2639b8bc0f69ccf60cd004e25cf8b6bf1b184998a2c13d62",
    "lib/al/include/Execute/alExecuteTableHolderUpdate.h": "f4899ca0e5e976c961e1bc94644e7acb26ce52db8992ad6c5a00ba3dbfc8abde",
    "lib/al/include/Execute/alExecuteOrder.h": "b2f4a7562cbb3870bb40832f141d98705a3649eb8411ceb4072a343201020d75",
    "lib/al/src/Execute/alExecuteOrderFunction.cpp": "c88e4b76a8ffe4cf4941e8c1d367fb76c101fcc36bdae47b47d42fc11de18f24",
    "lib/al/src/Execute/alExecuteRequestKeeperFunction.cpp": "520ec605019cc86c7bc47c1ffbfee61d8db44144e09ef1416c5508510cbd08e2",
    "lib/al/include/Execute/alExecuteTableHolder.h": "c372ee57ac4eec0b75e212eaf8b2897cf890c3aedd5fc1627bd0c07db435d7d1",
    "lib/al/include/LiveActor/alActorExecuteInfo.h": "2d0f34fd737f83553f844f0dcc9c5e267b76744052d9ba1de358eef8ea70354c",
    "lib/al/src/Execute/alExecuteTableHolder.cpp": "46288b5f303de1a7ab90239fdfe6876dcf1bdc42b669db5979f306e800dace0f",
    "lib/al/include/LiveActor/alLiveActorKit.h": "4ab8281c6b5b83640150ca850fd9d6f922b741ab9092c524bdc9dda2e6ccb74c",
    "lib/al/src/Execute/alExecuteTableHolderDraw.cpp": "e0bcc9e72b2bae9c4c149d5eb411e8285e79b5a9b6dd56bcfe9d6ab313f6f500",
    "lib/al/include/Execute/alExecutorRegistrationList.h": "daf8417981b0676c9905304ddf5baba066d968ca42c4551b2fead59962426c81",
    "lib/al/src/Execute/alExecuteTableHolderUpdate.cpp": "3f361ee90c6831b6cfa1564e3f17f6d9f3a87e52f51eb47812b67b82d2b57c25",
    "lib/al/src/Execute/alExecuteTableHolderUpdateInit.cpp": "26247eeed950d1436f031abfa395ba75d7b1418424ce9caf0e02539368ebd1cd",
    "lib/al/src/Execute/alExecutorActorList.cpp": "4e87a1608499de7399aac9d4b33183ed9b3da8caf42adeed958baee08bae0ff6",
    "lib/al/src/Execute/alExecutorTableListFunction.cpp": "08613fd0135d7cacfd04647529d9212bcceac3289d56b4bc1f031c885366d7d0",
    "lib/al/include/Factory/alActorFactory.h": "ccde3437189b8926138d6d91349a5466a75bf5df82f2e7dc01e1e418fd9828b0",
    "lib/al/include/Npc/alEffectObj.h": "4fb6ef8c9c5e4123b7ec0d03abde4f76270ca3086ea22ac5b2ea75965b3ca1cb",
    "lib/al/include/Factory/alFactory.h": "bfa6821597a7dcb002bd711960dab91ae1041c7139a1ddaa732d40bd7866beab",
    "lib/al/src/Factory/alActorFactory.cpp": "b8168216f9a1e1888f2efa8cdce76e774f595e05a96e6358c5d2c36afdf3a0a1",
    "lib/al/include/Npc/alSky.h": "ce316ab99b71ee9c49e9fe4ae0037568e420fdbfdc2f1d5249794a341db465c7",
    "lib/al/include/MapObj/alFallMapParts.h": "de98dd38d31c7b4a77c7fc84e9ee7b9b30cfdb2cd8b3698db7fa5bf7b9f70214",
    "lib/al/include/Factory/alAreaObjFactory.h": "8ff4dcb149f0b0284e99a2214ece0bbc09cffc7ac44bf2f4ee1f9e5b92a08342",
    "lib/al/src/Factory/alAreaObjFactory.cpp": "df254ae3cc0a45dbfd6b3c8e62b222fe08247e100a8cc940eb0b7e27d43a915f",
    "lib/al/include/Message/alMessageFunction.h": "3796a8edadd47afb1f96e0992160124e0345fb33822ab0d1c35d1ab509f20524",
    "lib/al/src/File/alFileFunction.cpp": "d2003fd18e861c9a6db6b445cbb928d15e10a9861141d0b9d5b8458f3efb93c7",
    "lib/al/include/File/alFileLoader.h": "e69e61c28e9bba2b198ca556909ce91172b1e2a2bf57b118204bf81e00de4fd9",
    "lib/al/include/File/alFileFunction.h": "dbcf99b495a2e662d892239322432ccb3c6d6ae02cde41b736fb9ee651f9e040",
    "lib/al/include/System/alSystemKit.h": "19f9029b3df9d5bbe6ef014ba0ed469003c4010afef145e998da1255878f5a4a",
    "lib/al/src/File/seadFileDevice.cpp": "33fb0ce7cf4d33e7a6f6aaae8a4d98366c1303bf7323cdbba834c513cd023b2f",
    "lib/al/include/HitSensor/alSensorType.h": "6d314743059ec2edff82cbeddfc8a5095b5816a5f7c75355fb95974339cbea30",
    "lib/al/include/HitSensor/alSensorHitGroup.h": "9e1f4252e911d8aed6e0508c6edeb0ca0bcaad3cd00419ecde291a9243eef70f",
    "lib/al/src/HitSensor/alHitSensor.cpp": "7b3384ff5b17e73bfe9e963ef8506a91c529ed33ace60550832c85669cdad937",
    "lib/al/include/HitSensor/alHitSensor.h": "6e6f511cd27862fbff798abf3240d70bb71f6aaaf19fdc8de0041a2426060f8f",
    "lib/al/src/HitSensor/alSensorHitGroup.cpp": "93e07fa4f6f0b4ae57af8db8065081987c0bd29af5bcaba6e6d26c5b363ef41f",
    "lib/al/src/ItemHolder275828.cpp": "5daf27f0b8c00287a8ad413213bbcdbfe912c6cc5986b3f40f44c41be67a4d2c",
    "lib/al/include/Item/ItemHolder275828.h": "359ecde541a4056493fe1bfcebbabbc10f577313fe8a5b61db90125ba6790a58",
    "Game/backup/include/MapObj/ItemHolder.h": "14eee7da5665fc99f71f2bf9b05c72206d0bb15b5891380b36ead7b4d0fe657b",
    "lib/al/src/KeyPose/alKeyPose.cpp": "ee741b06c0b74ad66726e8d0523c63bd2944b9da6cabf4e8153b3ed726739e71",
    "lib/al/include/KeyPose/alKeyPose.h": "a216990d7e70f7e3d51b26b682f61ecd895ffef0292789d5693985945d475ae7",
    "lib/al/include/KeyPose/alKeyPoseKeeper.h": "b4b50c69d44f8d83ee73c5973d476da66d1d0054ffbfcea935b96964170dc408",
    "lib/al/src/KeyPose/alKeyPoseFunction.cpp": "6b62c905bfc996da894bd8522617d7cc90289f02a1f5ea42e0538a98254a46e2",
    "lib/al/include/LiveActor/alActorInitInfo.h": "85f4e76719327e5ae1e33b69f06c0fee443613f397bd04bb232467450be9dac9",
    "lib/al/src/KeyPose/alKeyPoseKeeper.cpp": "73d2cc06ca3674cfdb560b64d89edfcee294f11bb55325d3739070a8bee49f59",
    "lib/al/src/Layout/alLayoutActor.cpp": "8c097153adf48c4dc6fd0727d3e0adccfdb1603f19faf33db63811dbdd5ecd25",
    "lib/al/src/Layout/alLayoutKit.cpp": "6d839ab784e2c42f7984c029d02679adecad2fa266de59847db5264ed0d7d505",
    "lib/al/include/Layout/alLayoutKit.h": "6e9ae4483e9621af05f1a83beb58b8c13caddebcd04724de1d04ce4e6e03b717",
    "lib/al/include/Layout/alWipeSimple.h": "e26bfb75d3a5d5591791c293975d6da7739b0d26396a36ef8c689049902b2a24",
    "lib/al/src/Layout/alWipeSimple.cpp": "fffc0fa9799b30b78769dddb70d6117a697b552e9a6af06703761bc7ba8dc167",
    "lib/al/src/LiveActor/alActorActionKeeper.cpp": "d9bad7fb1c4a49f3aa4887abd80f6e01b7d94b5a333b0c152fa244c74ca72be0",
    "lib/al/src/LiveActor/alActorExecuteInfo.cpp": "ca652cbc8d1e71a804987e9837f12a8e55c6f42cdd262fc3f185b294b2e73228",
    "lib/al/src/LiveActor/alActorExecutionHooks.cpp": "d72fd6fe1b73222b50c9ca2e2d22a94d77ee9deb62e6b1431664d2c21e8d62d1",
    "lib/al/src/LiveActor/alActorInitInfo.cpp": "42dc1ca452d1276b95881c4bbec4ab8ec2b7a4b76d7906c7f30faccc43ae5b75",
    "lib/al/src/LiveActor/alActorInitUtil.cpp": "fff6af2ac898a7a2167544fec72de55480c7f1008edd1ab2e1d775783af8b72c",
    "lib/al/src/LiveActor/alActorPoseKeeper.cpp": "c3ae15fb5beb71fc8752ebbd594246d1ceaa13ed58365a6df035a5d45775b59c",
    "lib/al/src/LiveActor/alHitSensorDirector.cpp": "324038a4994ab5d858ea95b5af30dfda2cbb4da2996f90d365359ba7100bf288",
    "lib/al/include/LiveActor/alHitSensorDirector.h": "ab521a71b3e8bb351d4db9a20a7aac221111e317ba0490bd4ead88cc693b1c54",
    "lib/al/src/LiveActor/alHitSensorFunction.cpp": "c5529052c094be081cab54bea2befe579e84294a7ff5995ed89fbfc72228b0b5",
    "lib/sead/include/container/seadPtrArray.h": "2c3f93c561da6f13edefd2a44ec318d035885cbeb62bf3c1e8f4bc204f7ab632",
    "lib/al/src/LiveActor/alHitSensorKeeper.cpp": "f68e04b8642387152d10418a1593b9db1d43b126aa55eb369708bd444f48582b",
    "lib/al/include/LiveActor/alHitSensorKeeper.h": "8cbea99a91ae271a286093ab3a0bdde0c72d5a280b1ff40213b614578cf48e58",
    "lib/al/include/LiveActor/alSubActorFunction.h": "49446ccaa10de75db185c3b6aff0af89cee5805d04dd919cb22e31903d1c7a33",
    "lib/al/include/Model/alModelKeeper.h": "ae11c07ff61d6c372da372e055650504a2a9c8ba88849cf599197bbc1cf96dbe",
    "lib/al/include/Rail/alRailKeeper.h": "b4d841aaf278d6d8b44b8e2a65cede6635d5c4bc0c2e78197edace976258135e",
    "lib/al/src/LiveActor/alLiveActor.cpp": "2cc35623b9312a462ae13a9b744f6488f09dbab9e5521fa5cda0524a8a4aaa5a",
    "lib/al/include/LiveActor/alLiveActorGroup.h": "84d0a4c05650bb8209efac90c3164dc59be7de344cb1869a99751fffaaa223d1",
    "lib/al/src/LiveActor/alLiveActorFlag.cpp": "1a43a99430ab982c6b910df804c566ad4b542c0583668f91221cf2ba4f199203",
    "lib/al/src/LiveActor/alLiveActorFunction.cpp": "b88f280d69669a8f026f8f9a7230b4136363cf167c72db5594326d364f8eefff",
    "lib/al/include/LiveActor/alActorPoseFunction.h": "54306d9e34290505c3e54b792740cf5c10ae982e576c7b1d778c1042c0ef4c64",
    "lib/al/include/Clipping/alClippingDirector.h": "1931de3ecde3427f9d87c8a08397c6ff367c6017cce095dc8e4621fbeda62b84",
    "lib/al/include/Clipping/alClippingActorHolder.h": "4c1d53cd2eba2918545dd98a6f883062c4eec5f778d12ff2141485eb30f0c8bf",
    "lib/al/include/LiveActor/alSensorFunction.h": "d34a2c03ddb04a87ae743498f0d3ea0e851c8a6089a3ffe8a442e1c7f565a337",
    "lib/al/include/Model/alModelCtr.h": "f56f567125bf6d454f2e38eed0758321f8670fa4d1810ef8ab5ce43dbbd5cfab",
    "lib/al/include/Model/alAnimPlayerSimple.h": "bfdb89b64b3ca8bbb268918e387997adf1233b0ec3e20bf82dde38dac523c07a",
    "lib/al/include/Math/alMtxUtil.h": "b516b0a06e3fb840c9d073a1ffaad246d31260af708b37d4640bdb5bf4f3d1fc",
    "lib/al/src/LiveActor/alLiveActorGroup.cpp": "befbfeb4e21ecd8c06a819415a16279c6928212d95b709da301fdd6e68528df8",
    "lib/al/src/LiveActor/alLiveActorKit.cpp": "be6ae8aac5e49cb94cebace615b9af8643962e2c0fd31412a5151a499bf87bc3",
    "lib/al/include/Collision/alCollisionDirector.h": "539c9eba6f23c2a83f0dfe818339666d435144df575d919105c0b71057a35bef",
    "lib/al/include/Effect/alEffectSystem.h": "c522c866af2866c40f75369eca5e48c262285f941fee98b119af9f056a5ee9a8",
    "lib/al/include/Functor/alFunctorV0F.h": "58289d52b90c7ff7c6872f60a4143bdcce8ac4453b94f1da95d6dad618054682",
    "lib/al/include/Fog/alFogDirector.h": "dd3e9b5c94752d9b2313a77480c484e75c38a5a12b8f26d57bb7609d39cdd2fe",
    "lib/al/src/LiveActor/alSensorFunction.cpp": "4d04539c0bb5104426a00285f91eb005907ac9964eec5de4afc08d4fe42a5568",
    "lib/al/src/LiveActor/alSensorMsg.cpp": "5ed131408b78116ab55ab7ed7c0320abe19b5ef68de442de393539bdf3093d5a",
    "lib/al/src/LiveActor/alSubActorFunction.cpp": "80d7c7802182313f7598d142d47f3dad6b074a495c7daf1a1f827ae1df9a4f48",
    "lib/al/include/LiveActor/alSubActorKeeper.h": "b52139ea8a832c575a2ca7cd5e2592263c7bf336b34dde911fdd8e7a0e8ad1fe",
    "lib/al/src/MapObj/alFallMapParts.cpp": "7e382874db190a3149cae3fe369d14e8ff008153f0ba3d20662a1ae19b9d4577",
    "lib/al/src/MapObj/alMapObjActor.cpp": "87424e4226fe5241a85b247a3c92d48012f6fa5c9825147f8514eb84759ed0d7",
    "lib/al/include/Math/alHashUtil.h": "3f61bc6c2ef71bb4a4f962d98787ab10c374d3848addf67a03ed8d90e588ff08",
    "lib/al/src/Math/alHashUtil.cpp": "7a9867cca0686a45bd65f08312240efa3f60f8a9967c18b48d286a725300cc0e",
    "lib/al/src/Math/alMathUtil.cpp": "05794f5a5d2b97367726dfa5621c80efccc479b6caf0c3baeba074f880d141fb",
    "lib/al/include/Math/alMathUtil.h": "75fee13aa91883d2526665e4fe0985b695150ae95108a2744c574ca049418961",
    "lib/al/include/Math/alVectorUtil.h": "85c329a883954124a4cc37ff504ab5efcf416b09155982ed01ac95fd23e9a9fb",
    "lib/al/src/Math/alVectorUtil.cpp": "8857173e40f4b00d53ed9325c8c864e2980d1d983fac53916baddedd1f7aa1e8",
    "lib/al/src/Math/seadMathAngle.cpp": "28fa62d1c67d0a535785ca64b9546514bff7b6026d620055abb002be3272d67c",
    "lib/al/src/Math/seadMatrixConversion.cpp": "9026b2c94752b7102726b00262022491649a0ed8359c0e71b0b8f336b86ff6a6",
    "lib/al/src/Math/seadMatrixCopy.cpp": "774ce29a89d6dfdc0970f1622d49990ec6075eed70da7b083dcc54a8c71d8e8b",
    "lib/al/src/Math/seadStereoUtil.cpp": "b43c9354048188214d4446109e6c09c4b8e0c2ab17f43a894600be278d4da0dd",
    "lib/sead/include/heap/seadExpHeap.h": "f6cc3a1e5548cc18bfe3b3065a6939f0c9b06f135ce3b932b9d46cd5ac21d2e8",
    "lib/sead/include/heap/seadFrameHeap.h": "fe54cd11242a2160a3a41f73533410ddc975b4a0cbd7b4f10d08f946d0ca131e",
    "lib/al/src/Memory/alMemorySystem.cpp": "f0a2403ee32b7564255eea215ef7b97884d6d2c5fe76a80cbd3fadec7bec387e",
    "lib/al/include/Memory/alMemorySystem.h": "264302282bf54ea39451a8c0c0a7a5896d9b7f6475302cae7f9df345e96f0d40",
    "lib/al/src/Nerve/alNerveActionCtrl.cpp": "bf7447a5175fbeeef046cab16b3d8a2336fc5138e32c55d73b546c939fbb079f",
    "lib/al/src/Nerve/alNerveExecutor.cpp": "292ac054b540ec82bace6d00f525f2f4db25b00d009f1ae198d85029c8d58710",
    "lib/al/src/Nerve/alNerveFunction.cpp": "c6720f0195461413907f17d2fb403bb5acc9160648e93894320cfdda1a110542",
    "lib/al/include/Nerve/alNerveStateCtrl.h": "99d940a4facac17778534a270a7f16900ec1fcd0b6eda6637d002dba31e2429a",
    "lib/al/src/Nerve/alNerveKeeper.cpp": "3059fa1dc52e3b6a382ebf65d611d03d199c10bae4112db595db7c7cd0d8e13d",
    "lib/al/src/Nerve/alNerveStateBase.cpp": "80182d3127744729108619015146b416f28c3decc7f0444441f4a6cd4c6c3c30",
    "lib/al/src/Nerve/alNerveStateCtrl.cpp": "028420ae18fb5c50eb1587ae6f2d20c13dba59f38f2056617305aff32629f505",
    "lib/al/src/Nerve/alNerveStepFunction.cpp": "435943584b2a559791a75ea69ed7d47f331c7834b1d70c26d12281d0ab59b026",
    "lib/al/include/Npc/alBreakModel.h": "1b0a7ddfebe9165b18ac4f18f99e3379c20f1559cadcb87e9279bfa86438cbdf",
    "lib/al/src/Npc/alBreakModel.cpp": "609a3f087c1865b4275fdccb289bc7499663a9eb63e9b2b4879b6d911d601e93",
    "lib/al/src/Npc/alEffectObj.cpp": "e82fbb614b01cc2a8392ba7ef6a2300047748b9441aab722f1ae7a4bc8beb575",
    "lib/al/include/Se/alSeFunction.h": "fd5f1ccceeac488f2537b4ee265b8af7a7411695943ae8a57f3dd1c367b7ba64",
    "lib/al/src/Npc/alSky.cpp": "b52de11b35368c1cb55de043669e8b34952500dfdd5b155741474aefe7ed1f68",
    "lib/al/src/Placement/alPlacementArgumentFunction.cpp": "665882b723d3c5dbbcb2e7fcf03e0189e7b073d3c936b0ea4f99d15764fe586f",
    "lib/al/src/Placement/alPlacementFunction.cpp": "0ff7003bbadf1d7695103e96a739999945f0066a52efef310dbe31e62081f675",
    "lib/al/src/Rail/alRail.cpp": "666768e5a7f7dd1c54970a4fe612d336c72109a9c55e4ed12b925e584b86778b",
    "lib/al/include/Rail/alRail.h": "e6ed6859d4c67b4d814325f321ab9d6c167ad7bb74c009db7e526ca7337f63a5",
    "lib/al/src/Rail/alRailFunction.cpp": "9aa2a9c332c22e02adb46bcd557ef224f14b2d91de7af270dda1eefc2a143800",
    "lib/al/include/Rail/alRailRider.h": "d98db7f5f9a661be05b57f3a774c77f8dc50da007e0c0656a6530e7b94efa102",
    "lib/al/src/Rail/alRailKeeper.cpp": "8572376c26ac66a475161b2df7fa2b7a750ecf538ad2a5312bc8f08dcb95a8c0",
    "lib/al/src/Rail/alRailMoveMovement.cpp": "a51634251229c54012b8eb4d79b02b8a689a9ba7afbd138db54516e016816b81",
    "lib/al/include/Rail/alRailMoveMovement.h": "01a2ebbfc3b80889b91d4312535accabc3abb95e17ac4c8d9cd6cf42d9c722d3",
    "lib/al/src/Rail/alRailRider.cpp": "df31ea77c6aa75b321e25d70cec45f92038c27cf657e1a21f91bf8a932af6150",
    "lib/al/src/Scene/alScene.cpp": "b65ef161dcf8f515f41f08c55df1861ed4d649429b02eeff9194d654bac6a959",
    "lib/al/include/Stage/alStageResourceKeeper.h": "8e7b936ad38c30c69e8631f141e800059e13423345b9966de610ff3d7333a0b8",
    "lib/al/include/Scene/alScene.h": "e06da0e04e1349985c439e80a4331fc5ca3687e4bad1f5991e24c5a9e0f78e35",
    "lib/al/include/Scene/alSceneFunction.h": "7c7f091ce9533855eb4084f9e995581a02bda93698bf59936a001476a9771bf8",
    "lib/al/src/Scene/alSceneFunction.cpp": "fd5e23ceb5b24deac34b8249a499e901c3e18ffb8977d2bc0a7fd4a39bbddee7",
    "lib/al/src/Scene/alSceneObjHolder.cpp": "999ad0df20e923b02197d81dfe76b99fff79efdcc894dd1c4bd02c26ed075be8",
    "lib/al/src/Sequence/alSequence.cpp": "5bb16d7ef3b8f588d74b4000f7b7436e3ce1d910864f7e17b64787b38aec5860",
    "lib/al/src/Stage/alStageResourceKeeper.cpp": "9f9894860c9ccb4306671f1b584a50df28c79e04c29e01e86056f58b9989a363",
    "lib/al/include/Stage/alStageSwitchType.h": "4495a6a174ed8dbd82fb86f1d04fcee6e5b12ea399d994f85ea571549df7d33a",
    "lib/al/include/Stage/alStageSwitchAccesser.h": "2892e10118e2493a2c5ec26341bd769338c93665c8c058b9c47338e032b7011e",
    "lib/al/src/Stage/alStageSwitchAccesser.cpp": "88158e4fa7115c6be40343e289bef201d41404d59b7213e1da24e9f7c2ab37e8",
    "lib/al/src/Stage/alStageSwitchKeeper.cpp": "3e7afa79b23da313e2a94b313280bde4b7e3cd92bd4967ee5afdde77e20a8d0d",
    "lib/al/src/Stage/alStageSwitchType.cpp": "52402fe1c915d52e5de541790576345f280cbece2f4dab318334b6bdd9255133",
    "lib/al/src/Stage/alStageSwitchTypeCount.cpp": "d019e1d81ee8898ee939d20db273343aca90884ca9738b2f0e8e42613b72d912",
    "lib/al/src/System/alSystemKit.cpp": "e1ef1035b76a9c45db17b8c2761c34ac158559903c59207eabb1de9a61f83ffa",
    "lib/al/include/Save/alSaveDataDirector.h": "be655a26fef8d1f23d18022afcfda6257ce440036c8adec008060745c622f61e",
    "lib/al/src/Util/alItemTypeLookup.cpp": "0430d41ceefa8631cb5ad97d31bf939e1b33a2b8c1f23c4817e27644b2a4bf37",
    "lib/al/src/Util/alJMapInfo.cpp": "77abe5de279f05b939f33d8d5b3187da7c61ecca422dce25ccf5f5a9fe95d681",
    "lib/al/src/Util/alStringUtil.cpp": "6dd1a1eb32196c179e024dc572f975392e43fd6325ff911acd0a93f62a530ca4",
    "lib/al/include/Util/seadDateUtil.h": "21f7e62cb0a655fc5bd2fa69e6381db90e6301b09abc59c3b4c6fd004fb56b2b",
    "lib/al/src/Util/seadCalendarTime.cpp": "265d908447edf093f98b88d441c9098593374b4aca0825948cb84497eb4a478e",
    "lib/al/src/Util/seadDateUtil.cpp": "c992c770be8d2539f774dc9b2a7c8528063d52446ea1dc47df5c05942e28aabb",
    "lib/al/src/Util/seadList.cpp": "b76d5bd49adf8c195b5f4012b47a909810aab3beed6ed5ff362d467dbb8f777d",
    "lib/al/src/Util/seadPtrArrayImpl.cpp": "ac3681735b577b4e71043f7ec4360d27f9f5ce86ceb83ec9f8fac643d0e4ccdf",
    "lib/al/src/Util/seadRandom.cpp": "5322dfe6b218ad5e09e5961abcaa1e931459ce33e12133494e93d9de070c0657",
    "lib/al/src/Util/seadStreamScalar.cpp": "09d226b7567ff54e0b565d48aa60e076aa9e1304376b3b14eda46d4f3420e056",
    "lib/al/src/Util/seadTreeNode.cpp": "2c3f4d48ad6f72933dde4e6b472bb733095aa8ce345af04f24588991bf7acdef",
    "lib/al/include/Yaml/alByamlData.h": "3ef431c7a54350bbfc9e01c22ea779b4a394214fbc3bede338b3cab10d3c4ca1",
    "lib/al/include/Yaml/alByamlHashIter.h": "895eb2598237911c13b6a962de895545ae38845ebf0f7a33c023cd41de9e04c3",
    "lib/al/src/Yaml/alByamlHashIter.cpp": "fad3fe0fc3e6fc23cd2d879efa1e9f5938449e180915e7a00e96847c7acae23d",
    "lib/al/include/Yaml/alByamlStringTableIter.h": "801b05e2dc41f7e4f8cc48059d8caf19a1591e57c3ae92d2308b643588a4527a",
    "lib/al/include/Yaml/alByamlHeader.h": "7d0af621639b4d70355c5355b52dcd3b14681701062bd5cd2aaefbcef172b95b",
    "lib/al/src/Yaml/alByamlIter.cpp": "86c96d8d0d1dada7b9a7227695e738e98202f3644b3c97ad915cb06f483fa7de",
    "lib/al/include/Yaml/alByamlContainerHeader.h": "befbc814ba6564811d7003f6f1affe7ba222588611e68d48b295ea461b53204b",
    "lib/al/src/Yaml/alByamlIteratorConstructors.cpp": "8809735b32c7deffa93041eaa081ff983318f72d1366bc567cfe5c968df642c3",
    "lib/al/src/Yaml/alByamlStringTableIter.cpp": "ba5cee2db169501269a2a9a0a54984b76d7bcd710ffe216b82c4ac8952543a91"
  }
}
```

## generated-stub-delta.json

```json
{
  "all_prior_sections_symbols_unchanged": true,
  "old_debug_frame_count": 1934,
  "new_debug_frame_count": 1940,
  "debug_normalization": "Compare every CFI byte and resolved relocation per function; compiler-generated frame-CIE ordinal names and section-table indices normalize only to that per-function association",
  "added_sections": [
    ".sdata_dat_003A2D40",
    ".sdata_dat_003E26CC",
    "i.fn_00167CAC",
    "i.fn_0025C334",
    "i.fn_0028ECAC",
    "i.fn_0028EED4",
    "i.fn_002A9DC4",
    "i.fn_002AA23C"
  ],
  "added_symbols": [
    [
      "$a",
      "STT_NOTYPE",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_00167CAC",
      0,
      0
    ],
    [
      "$a",
      "STT_NOTYPE",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_0025C334",
      0,
      0
    ],
    [
      "$a",
      "STT_NOTYPE",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_0028ECAC",
      0,
      0
    ],
    [
      "$a",
      "STT_NOTYPE",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_0028EED4",
      0,
      0
    ],
    [
      "$a",
      "STT_NOTYPE",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_002A9DC4",
      0,
      0
    ],
    [
      "$a",
      "STT_NOTYPE",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_002AA23C",
      0,
      0
    ],
    [
      ".sdata_dat_003A2D40",
      "STT_SECTION",
      "STB_LOCAL",
      "STV_DEFAULT",
      ".sdata_dat_003A2D40",
      0,
      32
    ],
    [
      ".sdata_dat_003E26CC",
      "STT_SECTION",
      "STB_LOCAL",
      "STV_DEFAULT",
      ".sdata_dat_003E26CC",
      0,
      4
    ],
    [
      "i.fn_00167CAC",
      "STT_SECTION",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_00167CAC",
      0,
      0
    ],
    [
      "i.fn_0025C334",
      "STT_SECTION",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_0025C334",
      0,
      0
    ],
    [
      "i.fn_0028ECAC",
      "STT_SECTION",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_0028ECAC",
      0,
      0
    ],
    [
      "i.fn_0028EED4",
      "STT_SECTION",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_0028EED4",
      0,
      0
    ],
    [
      "i.fn_002A9DC4",
      "STT_SECTION",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_002A9DC4",
      0,
      0
    ],
    [
      "i.fn_002AA23C",
      "STT_SECTION",
      "STB_LOCAL",
      "STV_DEFAULT",
      "i.fn_002AA23C",
      0,
      0
    ],
    [
      "__ARM_grp_.debug_frame$5807",
      "STT_OBJECT",
      "STB_LOCAL",
      "STV_DEFAULT",
      ".debug_frame",
      0,
      0
    ],
    [
      "__ARM_grp_.debug_frame$5810",
      "STT_OBJECT",
      "STB_LOCAL",
      "STV_DEFAULT",
      ".debug_frame",
      0,
      0
    ],
    [
      "__ARM_grp_.debug_frame$5813",
      "STT_OBJECT",
      "STB_LOCAL",
      "STV_DEFAULT",
      ".debug_frame",
      0,
      0
    ],
    [
      "__ARM_grp_.debug_frame$5816",
      "STT_OBJECT",
      "STB_LOCAL",
      "STV_DEFAULT",
      ".debug_frame",
      0,
      0
    ],
    [
      "__ARM_grp_.debug_frame$5819",
      "STT_OBJECT",
      "STB_LOCAL",
      "STV_DEFAULT",
      ".debug_frame",
      0,
      0
    ],
    [
      "__ARM_grp_.debug_frame$5822",
      "STT_OBJECT",
      "STB_LOCAL",
      "STV_DEFAULT",
      ".debug_frame",
      0,
      0
    ],
    [
      "dat_003A2D40",
      "STT_OBJECT",
      "STB_WEAK",
      "STV_HIDDEN",
      ".sdata_dat_003A2D40",
      0,
      32
    ],
    [
      "dat_003E26CC",
      "STT_OBJECT",
      "STB_WEAK",
      "STV_HIDDEN",
      ".sdata_dat_003E26CC",
      0,
      4
    ],
    [
      "fn_00167CAC",
      "STT_FUNC",
      "STB_WEAK",
      "STV_HIDDEN",
      "i.fn_00167CAC",
      0,
      16
    ],
    [
      "fn_0025C334",
      "STT_FUNC",
      "STB_WEAK",
      "STV_HIDDEN",
      "i.fn_0025C334",
      0,
      16
    ],
    [
      "fn_0028ECAC",
      "STT_FUNC",
      "STB_WEAK",
      "STV_HIDDEN",
      "i.fn_0028ECAC",
      0,
      16
    ],
    [
      "fn_0028EED4",
      "STT_FUNC",
      "STB_WEAK",
      "STV_HIDDEN",
      "i.fn_0028EED4",
      0,
      16
    ],
    [
      "fn_002A9DC4",
      "STT_FUNC",
      "STB_WEAK",
      "STV_HIDDEN",
      "i.fn_002A9DC4",
      0,
      16
    ],
    [
      "fn_002AA23C",
      "STT_FUNC",
      "STB_WEAK",
      "STV_HIDDEN",
      "i.fn_002AA23C",
      0,
      16
    ]
  ],
  "old_generated_source_sha256": "bc3686eaae9090ccae6ed3aaf97c0c9f4085be78f687ea8629757fb61dd159ec",
  "new_generated_source_sha256": "2f262ae82cf2d7732068097c2256e5e1f54abe10d4517e1ef1e689ac5fd1a245"
}
```

## replay-results.json

```json
{
  "returning_fixture_pairs": 160,
  "returning_invocation_pairs": 320,
  "compared_bytes": 1494528,
  "compared_events": 4365,
  "fault_diagnostics": [
    {
      "case": {
        "label": "fault-primary_wrapper",
        "fault": "primary_wrapper"
      },
      "same": true,
      "original_fault": [
        19,
        0,
        4
      ],
      "candidate_fault": [
        19,
        0,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 6,
      "candidate_instructions": 8
    },
    {
      "case": {
        "label": "fault-primary_resource",
        "fault": "primary_resource"
      },
      "same": true,
      "original_fault": [
        19,
        40,
        4
      ],
      "candidate_fault": [
        19,
        40,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 7,
      "candidate_instructions": 10
    },
    {
      "case": {
        "label": "fault-model_dict",
        "fault": "model_dict"
      },
      "same": true,
      "original_fault": [
        19,
        12,
        4
      ],
      "candidate_fault": [
        19,
        12,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 51,
      "candidate_instructions": 48
    },
    {
      "case": {
        "label": "fault-secondary_resource",
        "fault": "secondary_resource"
      },
      "same": true,
      "original_fault": [
        19,
        64,
        4
      ],
      "candidate_fault": [
        19,
        64,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 315,
      "candidate_instructions": 304
    },
    {
      "case": {
        "label": "fault-null_lock",
        "fault": "null_lock"
      },
      "same": true,
      "original_fault": [
        19,
        16,
        4
      ],
      "candidate_fault": [
        19,
        16,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 287,
      "candidate_instructions": 277
    },
    {
      "case": {
        "label": "fault-null_shape",
        "fault": "null_shape"
      },
      "same": true,
      "original_fault": [
        19,
        48,
        4
      ],
      "candidate_fault": [
        19,
        48,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 79,
      "candidate_instructions": 76
    },
    {
      "case": {
        "label": "fault-null_material",
        "fault": "null_material"
      },
      "same": true,
      "original_fault": [
        19,
        644,
        4
      ],
      "candidate_fault": [
        19,
        644,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 482,
      "candidate_instructions": 435
    }
  ],
  "max_instructions": 6368,
  "original_direct_helper_entries": {
    "0x28eed4": 323,
    "0x2aa23c": 538,
    "0x28ecac": 320,
    "0x2a9dc4": 115,
    "0x292158": 217,
    "0x25c334": 217,
    "0x28aa60": 217
  },
  "original_coverage_offsets": [
    0,
    4,
    8,
    12,
    16,
    20,
    24,
    28,
    32,
    36,
    40,
    44,
    48,
    52,
    56,
    60,
    64,
    68,
    72,
    76,
    80,
    84,
    88,
    92,
    96,
    100,
    104,
    108,
    112,
    116,
    120,
    124,
    128,
    132,
    136,
    140,
    144,
    148,
    152,
    156,
    160,
    164,
    168,
    172,
    176,
    180,
    184,
    188,
    192,
    196,
    200,
    204,
    208,
    212,
    216,
    220,
    224,
    228,
    232,
    236,
    240,
    244,
    248,
    252,
    256,
    260,
    264,
    268,
    272,
    276,
    280,
    284,
    288,
    292,
    296,
    300,
    304,
    308,
    312,
    316,
    320,
    324,
    328,
    332,
    336,
    340,
    344,
    348,
    352,
    356,
    360,
    364,
    368,
    372,
    376,
    380,
    384,
    388,
    392,
    396,
    400,
    404,
    408,
    412,
    416,
    420,
    424,
    428,
    432,
    436,
    440,
    444,
    448,
    452,
    456,
    460,
    464,
    468,
    472,
    476,
    480,
    484,
    488,
    492,
    496,
    500,
    504,
    508,
    512,
    516,
    520,
    524,
    528,
    532,
    536,
    540,
    544,
    548,
    552,
    556,
    560,
    564,
    568,
    572,
    576,
    580,
    584,
    588,
    592,
    596,
    600,
    604,
    608,
    612,
    616,
    620,
    624,
    628,
    632,
    636,
    640,
    644,
    648,
    652,
    656,
    660,
    664,
    668,
    672,
    676,
    680,
    684,
    688,
    692,
    696,
    700,
    704,
    708,
    712,
    716,
    720,
    724,
    728,
    732,
    736,
    740,
    744,
    748,
    752,
    756,
    760,
    764,
    768,
    772,
    776,
    780,
    784,
    788,
    792,
    796,
    800,
    804,
    808,
    812,
    816,
    820,
    824,
    828,
    832,
    836,
    840,
    844,
    848,
    852,
    856,
    860,
    864,
    868,
    872,
    876,
    880,
    884,
    888,
    892,
    896,
    900,
    904,
    908,
    912,
    916,
    920,
    924,
    928,
    932,
    936,
    940,
    944,
    948,
    952,
    956,
    960,
    964,
    968,
    972,
    976,
    980,
    984,
    988,
    992,
    996,
    1000,
    1004,
    1008,
    1012,
    1016,
    1020,
    1024,
    1028,
    1032,
    1036,
    1040,
    1044,
    1048,
    1052,
    1056,
    1060,
    1064,
    1068,
    1072,
    1076,
    1080,
    1084,
    1088,
    1092,
    1096,
    1100,
    1104,
    1108,
    1112,
    1116,
    1120,
    1124,
    1128,
    1132,
    1136,
    1140,
    1144,
    1148,
    1152,
    1156,
    1160,
    1164,
    1168,
    1172,
    1176,
    1180,
    1184,
    1188,
    1192,
    1196,
    1200,
    1204,
    1208,
    1212,
    1216,
    1220,
    1224,
    1228,
    1232,
    1236,
    1240,
    1244,
    1248,
    1252,
    1256,
    1260,
    1264,
    1268,
    1272,
    1276,
    1280,
    1284,
    1288,
    1292,
    1296,
    1300,
    1304,
    1308,
    1312,
    1316,
    1320,
    1324,
    1328,
    1332,
    1336,
    1340,
    1344,
    1348,
    1352,
    1356,
    1360,
    1364,
    1368,
    1372,
    1376,
    1380,
    1384,
    1388,
    1392,
    1396,
    1400,
    1404,
    1408,
    1412,
    1416,
    1420,
    1424,
    1428,
    1432,
    1436,
    1440,
    1444,
    1448,
    1452,
    1456,
    1460,
    1464,
    1468,
    1472,
    1476,
    1480,
    1484,
    1488,
    1492,
    1496,
    1500,
    1504,
    1508,
    1512,
    1516,
    1520,
    1524,
    1528,
    1532,
    1536,
    1540,
    1544,
    1548,
    1552,
    1556,
    1560,
    1564,
    1568,
    1572,
    1576,
    1580,
    1584,
    1588,
    1592,
    1596,
    1600,
    1604,
    1608,
    1612,
    1616,
    1620,
    1624,
    1628,
    1632,
    1636,
    1640,
    1644,
    1648,
    1652,
    1656,
    1660,
    1664,
    1668,
    1672,
    1676,
    1680,
    1684,
    1688,
    1692,
    1696,
    1700,
    1704,
    1708,
    1712,
    1716,
    1720,
    1724,
    1728,
    1732,
    1736,
    1740,
    1744,
    1748,
    1752,
    1756,
    1760,
    1764,
    1768,
    1772,
    1776,
    1780,
    1784,
    1788,
    1792,
    1796,
    1800,
    1804,
    1808,
    1812,
    1816,
    1820,
    1824,
    1828,
    1832,
    1836,
    1840,
    1844,
    1848,
    1852,
    1856,
    1860,
    1864,
    1868,
    1872,
    1876,
    1880,
    1884,
    1888,
    1892,
    1896,
    1900,
    1904,
    1908,
    1912,
    1916,
    1920,
    1924,
    1928,
    1932,
    1936,
    1940,
    1944,
    1948,
    1952,
    1956,
    1960,
    1964,
    1968,
    1972,
    1976,
    1980,
    1984,
    1988,
    1992,
    1996,
    2000,
    2004,
    2008,
    2032,
    2036,
    2040,
    2044,
    2048,
    2052,
    2056,
    2060,
    2064,
    2068,
    2072,
    2076,
    2080,
    2084,
    2088,
    2092,
    2096,
    2100,
    2104,
    2108,
    2112
  ],
  "seconds": 4.421253680993686,
  "cases": [
    {
      "case": {
        "label": "base-0-0-0",
        "models": 0,
        "secondary": false,
        "flags": 0,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 686,
      "candidate_instructions": 670
    },
    {
      "case": {
        "label": "base-0-0-ffffffff",
        "models": 0,
        "secondary": false,
        "flags": 4294967295,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 686,
      "candidate_instructions": 670
    },
    {
      "case": {
        "label": "base-0-0-2000000",
        "models": 0,
        "secondary": false,
        "flags": 33554432,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 686,
      "candidate_instructions": 670
    },
    {
      "case": {
        "label": "base-0-0-12345678",
        "models": 0,
        "secondary": false,
        "flags": 305419896,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 686,
      "candidate_instructions": 670
    },
    {
      "case": {
        "label": "base-0-1-0",
        "models": 0,
        "secondary": true,
        "flags": 0,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 694,
      "candidate_instructions": 680
    },
    {
      "case": {
        "label": "base-0-1-ffffffff",
        "models": 0,
        "secondary": true,
        "flags": 4294967295,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 694,
      "candidate_instructions": 680
    },
    {
      "case": {
        "label": "base-0-1-2000000",
        "models": 0,
        "secondary": true,
        "flags": 33554432,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 694,
      "candidate_instructions": 680
    },
    {
      "case": {
        "label": "base-0-1-12345678",
        "models": 0,
        "secondary": true,
        "flags": 305419896,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 694,
      "candidate_instructions": 680
    },
    {
      "case": {
        "label": "base-1-0-0",
        "models": 1,
        "secondary": false,
        "flags": 0,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1595,
      "candidate_instructions": 1600
    },
    {
      "case": {
        "label": "base-1-0-ffffffff",
        "models": 1,
        "secondary": false,
        "flags": 4294967295,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1595,
      "candidate_instructions": 1600
    },
    {
      "case": {
        "label": "base-1-0-2000000",
        "models": 1,
        "secondary": false,
        "flags": 33554432,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1595,
      "candidate_instructions": 1600
    },
    {
      "case": {
        "label": "base-1-0-12345678",
        "models": 1,
        "secondary": false,
        "flags": 305419896,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1595,
      "candidate_instructions": 1600
    },
    {
      "case": {
        "label": "base-1-1-0",
        "models": 1,
        "secondary": true,
        "flags": 0,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2579,
      "candidate_instructions": 2466
    },
    {
      "case": {
        "label": "base-1-1-ffffffff",
        "models": 1,
        "secondary": true,
        "flags": 4294967295,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2579,
      "candidate_instructions": 2466
    },
    {
      "case": {
        "label": "base-1-1-2000000",
        "models": 1,
        "secondary": true,
        "flags": 33554432,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2579,
      "candidate_instructions": 2466
    },
    {
      "case": {
        "label": "base-1-1-12345678",
        "models": 1,
        "secondary": true,
        "flags": 305419896,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2579,
      "candidate_instructions": 2466
    },
    {
      "case": {
        "label": "base-2-0-0",
        "models": 2,
        "secondary": false,
        "flags": 0,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2273,
      "candidate_instructions": 2312
    },
    {
      "case": {
        "label": "base-2-0-ffffffff",
        "models": 2,
        "secondary": false,
        "flags": 4294967295,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2273,
      "candidate_instructions": 2312
    },
    {
      "case": {
        "label": "base-2-0-2000000",
        "models": 2,
        "secondary": false,
        "flags": 33554432,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2273,
      "candidate_instructions": 2312
    },
    {
      "case": {
        "label": "base-2-0-12345678",
        "models": 2,
        "secondary": false,
        "flags": 305419896,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2273,
      "candidate_instructions": 2312
    },
    {
      "case": {
        "label": "base-2-1-0",
        "models": 2,
        "secondary": true,
        "flags": 0,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3301,
      "candidate_instructions": 3222
    },
    {
      "case": {
        "label": "base-2-1-ffffffff",
        "models": 2,
        "secondary": true,
        "flags": 4294967295,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3301,
      "candidate_instructions": 3222
    },
    {
      "case": {
        "label": "base-2-1-2000000",
        "models": 2,
        "secondary": true,
        "flags": 33554432,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3301,
      "candidate_instructions": 3222
    },
    {
      "case": {
        "label": "base-2-1-12345678",
        "models": 2,
        "secondary": true,
        "flags": 305419896,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3301,
      "candidate_instructions": 3222
    },
    {
      "case": {
        "label": "base-3-0-0",
        "models": 3,
        "secondary": false,
        "flags": 0,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2951,
      "candidate_instructions": 3024
    },
    {
      "case": {
        "label": "base-3-0-ffffffff",
        "models": 3,
        "secondary": false,
        "flags": 4294967295,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2951,
      "candidate_instructions": 3024
    },
    {
      "case": {
        "label": "base-3-0-2000000",
        "models": 3,
        "secondary": false,
        "flags": 33554432,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2951,
      "candidate_instructions": 3024
    },
    {
      "case": {
        "label": "base-3-0-12345678",
        "models": 3,
        "secondary": false,
        "flags": 305419896,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2951,
      "candidate_instructions": 3024
    },
    {
      "case": {
        "label": "base-3-1-0",
        "models": 3,
        "secondary": true,
        "flags": 0,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 4023,
      "candidate_instructions": 3978
    },
    {
      "case": {
        "label": "base-3-1-ffffffff",
        "models": 3,
        "secondary": true,
        "flags": 4294967295,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 4023,
      "candidate_instructions": 3978
    },
    {
      "case": {
        "label": "base-3-1-2000000",
        "models": 3,
        "secondary": true,
        "flags": 33554432,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 4023,
      "candidate_instructions": 3978
    },
    {
      "case": {
        "label": "base-3-1-12345678",
        "models": 3,
        "secondary": true,
        "flags": 305419896,
        "texture_types": [
          536870921,
          536870929,
          536870913,
          0
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 4023,
      "candidate_instructions": 3978
    },
    {
      "case": {
        "label": "shapes0",
        "shapes": 0
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1670,
      "candidate_instructions": 1568
    },
    {
      "case": {
        "label": "shapes1",
        "shapes": 1
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "shapes2",
        "shapes": 2
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2421,
      "candidate_instructions": 2296
    },
    {
      "case": {
        "label": "shapes3",
        "shapes": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2719,
      "candidate_instructions": 2582
    },
    {
      "case": {
        "label": "subs0",
        "subs": 0
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1920,
      "candidate_instructions": 1806
    },
    {
      "case": {
        "label": "subs1",
        "subs": 1
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "subs2",
        "subs": 2
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2326,
      "candidate_instructions": 2214
    },
    {
      "case": {
        "label": "subs3",
        "subs": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2529,
      "candidate_instructions": 2418
    },
    {
      "case": {
        "label": "sets0",
        "sets": 0
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1904,
      "candidate_instructions": 1782
    },
    {
      "case": {
        "label": "sets1",
        "sets": 1
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "sets2",
        "sets": 2
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2187,
      "candidate_instructions": 2082
    },
    {
      "case": {
        "label": "sets3",
        "sets": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2251,
      "candidate_instructions": 2154
    },
    {
      "case": {
        "label": "primitives0",
        "primitives": 0
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2086,
      "candidate_instructions": 1973
    },
    {
      "case": {
        "label": "primitives1",
        "primitives": 1
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "primitives2",
        "primitives": 2
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2160,
      "candidate_instructions": 2047
    },
    {
      "case": {
        "label": "primitives3",
        "primitives": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2197,
      "candidate_instructions": 2084
    },
    {
      "case": {
        "label": "materials0",
        "materials": 0
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1651,
      "candidate_instructions": 1558
    },
    {
      "case": {
        "label": "materials1",
        "materials": 1
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1769,
      "candidate_instructions": 1671
    },
    {
      "case": {
        "label": "materials2",
        "materials": 2
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1887,
      "candidate_instructions": 1784
    },
    {
      "case": {
        "label": "materials3",
        "materials": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2005,
      "candidate_instructions": 1897
    },
    {
      "case": {
        "label": "meshes0",
        "meshes": 0
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1968,
      "candidate_instructions": 1854
    },
    {
      "case": {
        "label": "meshes1",
        "meshes": 1
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "meshes2",
        "meshes": 2
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2229,
      "candidate_instructions": 2118
    },
    {
      "case": {
        "label": "meshes3",
        "meshes": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2335,
      "candidate_instructions": 2226
    },
    {
      "case": {
        "label": "empty_dictsFalse",
        "empty_dicts": false
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "empty_dictsTrue",
        "empty_dicts": true
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "empty_arraysFalse",
        "empty_arrays": false
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "empty_arraysTrue",
        "empty_arrays": true
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "shader_dictFalse",
        "shader_dict": false
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1895,
      "candidate_instructions": 1842
    },
    {
      "case": {
        "label": "shader_dictTrue",
        "shader_dict": true
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "missing_shaderFalse",
        "missing_shader": false
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "missing_shaderTrue",
        "missing_shader": true
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2095,
      "candidate_instructions": 1978
    },
    {
      "case": {
        "label": "null_shaderFalse",
        "null_shader": false
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "null_shaderTrue",
        "null_shader": true
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2149,
      "candidate_instructions": 2030
    },
    {
      "case": {
        "label": "secondaryFalse",
        "secondary": false
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1315,
      "candidate_instructions": 1320
    },
    {
      "case": {
        "label": "secondaryTrue",
        "secondary": true
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "shape-type0",
        "shape_type": 0,
        "attribute_types": [
          null,
          0,
          1,
          1073741824,
          4294967295
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2013,
      "candidate_instructions": 1912
    },
    {
      "case": {
        "label": "shape-type1",
        "shape_type": 1,
        "attribute_types": [
          null,
          0,
          1,
          1073741824,
          4294967295
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2013,
      "candidate_instructions": 1912
    },
    {
      "case": {
        "label": "shape-type268435456",
        "shape_type": 268435456,
        "attribute_types": [
          null,
          0,
          1,
          1073741824,
          4294967295
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2013,
      "candidate_instructions": 1912
    },
    {
      "case": {
        "label": "shape-type268435457",
        "shape_type": 268435457,
        "attribute_types": [
          null,
          0,
          1,
          1073741824,
          4294967295
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2173,
      "candidate_instructions": 2048
    },
    {
      "case": {
        "label": "shape-type4294967295",
        "shape_type": 4294967295,
        "attribute_types": [
          null,
          0,
          1,
          1073741824,
          4294967295
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2173,
      "candidate_instructions": 2048
    },
    {
      "case": {
        "label": "existing-shader0",
        "existing_shader": 0
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2117,
      "candidate_instructions": 2005
    },
    {
      "case": {
        "label": "absent-link0",
        "no_shader_link": 0
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2083,
      "candidate_instructions": 1969
    },
    {
      "case": {
        "label": "existing-shader1",
        "existing_shader": 1
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2117,
      "candidate_instructions": 2005
    },
    {
      "case": {
        "label": "absent-link1",
        "no_shader_link": 1
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2083,
      "candidate_instructions": 1969
    },
    {
      "case": {
        "label": "existing-shader2",
        "existing_shader": 2
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2117,
      "candidate_instructions": 2005
    },
    {
      "case": {
        "label": "absent-link2",
        "no_shader_link": 2
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2083,
      "candidate_instructions": 1969
    },
    {
      "case": {
        "label": "existing-shader3",
        "existing_shader": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2117,
      "candidate_instructions": 2005
    },
    {
      "case": {
        "label": "absent-link3",
        "no_shader_link": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2083,
      "candidate_instructions": 1969
    },
    {
      "case": {
        "label": "primitive-flags0",
        "set_flags": 0,
        "meshes": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2335,
      "candidate_instructions": 2226
    },
    {
      "case": {
        "label": "primitive-flags1",
        "set_flags": 1,
        "meshes": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2286,
      "candidate_instructions": 2178
    },
    {
      "case": {
        "label": "primitive-flags2",
        "set_flags": 2,
        "meshes": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2335,
      "candidate_instructions": 2226
    },
    {
      "case": {
        "label": "primitive-flags3",
        "set_flags": 3,
        "meshes": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2286,
      "candidate_instructions": 2178
    },
    {
      "case": {
        "label": "primitive-flags2147483648",
        "set_flags": 2147483648,
        "meshes": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2335,
      "candidate_instructions": 2226
    },
    {
      "case": {
        "label": "primitive-flags4294967295",
        "set_flags": 4294967295,
        "meshes": 3
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2286,
      "candidate_instructions": 2178
    },
    {
      "case": {
        "label": "fpscr0",
        "fpscr": 0
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "fpscr4194304",
        "fpscr": 4194304
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "fpscr8388608",
        "fpscr": 8388608
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "fpscr12582912",
        "fpscr": 12582912
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "fpscr16777216",
        "fpscr": 16777216
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "fpscr33554432",
        "fpscr": 33554432
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "fpscr50331648",
        "fpscr": 50331648
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "unused-r00",
        "unused_r0": 0
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "unused-r01",
        "unused_r0": 1
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "unused-r04294967295",
        "unused_r0": 4294967295
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2123,
      "candidate_instructions": 2010
    },
    {
      "case": {
        "label": "recursive-lock1",
        "recursive": 1
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2091,
      "candidate_instructions": 1978
    },
    {
      "case": {
        "label": "recursive-lock2",
        "recursive": 2
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2091,
      "candidate_instructions": 1978
    },
    {
      "case": {
        "label": "recursive-lock17",
        "recursive": 17
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2091,
      "candidate_instructions": 1978
    },
    {
      "case": {
        "label": "random0",
        "models": 1,
        "shapes": 0,
        "subs": 1,
        "sets": 1,
        "primitives": 2,
        "materials": 0,
        "meshes": 0,
        "secondary": true,
        "flags": 2632486956,
        "existing_shader": 2,
        "no_shader_link": 0,
        "set_flags": 245626231,
        "shape_type": 4294967295,
        "texture_types": []
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1198,
      "candidate_instructions": 1116
    },
    {
      "case": {
        "label": "random1",
        "models": 0,
        "shapes": 1,
        "subs": 1,
        "sets": 1,
        "primitives": 0,
        "materials": 4,
        "meshes": 2,
        "secondary": false,
        "flags": 333027116,
        "existing_shader": -1,
        "no_shader_link": -1,
        "set_flags": 298955583,
        "shape_type": 4294967295,
        "texture_types": [
          536870921,
          3735928559,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 616,
      "candidate_instructions": 600
    },
    {
      "case": {
        "label": "random2",
        "models": 2,
        "shapes": 3,
        "subs": 2,
        "sets": 1,
        "primitives": 1,
        "materials": 2,
        "meshes": 2,
        "secondary": false,
        "flags": 1912693902,
        "existing_shader": -1,
        "no_shader_link": 0,
        "set_flags": 2992723265,
        "shape_type": 268435457,
        "texture_types": [
          536870929,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3946,
      "candidate_instructions": 3916
    },
    {
      "case": {
        "label": "random3",
        "models": 3,
        "shapes": 0,
        "subs": 0,
        "sets": 2,
        "primitives": 0,
        "materials": 0,
        "meshes": 3,
        "secondary": false,
        "flags": 1744660122,
        "existing_shader": 1,
        "no_shader_link": 2,
        "set_flags": 1060249685,
        "shape_type": 268435457,
        "texture_types": [
          536870929,
          536870921,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1328,
      "candidate_instructions": 1340
    },
    {
      "case": {
        "label": "random4",
        "models": 0,
        "shapes": 1,
        "subs": 2,
        "sets": 2,
        "primitives": 2,
        "materials": 4,
        "meshes": 0,
        "secondary": false,
        "flags": 2960645985,
        "existing_shader": 3,
        "no_shader_link": 0,
        "set_flags": 3146753658,
        "shape_type": 268435457,
        "texture_types": [
          536870921,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 546,
      "candidate_instructions": 530
    },
    {
      "case": {
        "label": "random5",
        "models": 3,
        "shapes": 1,
        "subs": 0,
        "sets": 1,
        "primitives": 1,
        "materials": 4,
        "meshes": 3,
        "secondary": true,
        "flags": 2984647543,
        "existing_shader": 0,
        "no_shader_link": 3,
        "set_flags": 3546350437,
        "shape_type": 268435457,
        "texture_types": [
          3735928559,
          536870929,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3540,
      "candidate_instructions": 3474
    },
    {
      "case": {
        "label": "random6",
        "models": 3,
        "shapes": 1,
        "subs": 0,
        "sets": 0,
        "primitives": 3,
        "materials": 1,
        "meshes": 1,
        "secondary": false,
        "flags": 484238956,
        "existing_shader": 2,
        "no_shader_link": 1,
        "set_flags": 3685717457,
        "shape_type": 268435457,
        "texture_types": [
          536870929,
          536870929,
          536870929,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2082,
      "candidate_instructions": 2064
    },
    {
      "case": {
        "label": "random7",
        "models": 1,
        "shapes": 0,
        "subs": 0,
        "sets": 0,
        "primitives": 1,
        "materials": 4,
        "meshes": 0,
        "secondary": false,
        "flags": 2123822648,
        "existing_shader": 1,
        "no_shader_link": 2,
        "set_flags": 4043262208,
        "shape_type": 0,
        "texture_types": [
          536870921,
          536870921,
          536870921,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1130,
      "candidate_instructions": 1136
    },
    {
      "case": {
        "label": "random8",
        "models": 1,
        "shapes": 3,
        "subs": 2,
        "sets": 2,
        "primitives": 0,
        "materials": 1,
        "meshes": 2,
        "secondary": true,
        "flags": 1316433462,
        "existing_shader": 1,
        "no_shader_link": 0,
        "set_flags": 3705512848,
        "shape_type": 4294967295,
        "texture_types": [
          536870921,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3334,
      "candidate_instructions": 3268
    },
    {
      "case": {
        "label": "random9",
        "models": 3,
        "shapes": 0,
        "subs": 1,
        "sets": 0,
        "primitives": 2,
        "materials": 3,
        "meshes": 2,
        "secondary": true,
        "flags": 647351175,
        "existing_shader": 2,
        "no_shader_link": 2,
        "set_flags": 177048270,
        "shape_type": 268435457,
        "texture_types": [
          536870929,
          536870929,
          3735928559,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2708,
      "candidate_instructions": 2666
    },
    {
      "case": {
        "label": "random10",
        "models": 1,
        "shapes": 1,
        "subs": 2,
        "sets": 1,
        "primitives": 0,
        "materials": 2,
        "meshes": 3,
        "secondary": true,
        "flags": 4213304652,
        "existing_shader": 3,
        "no_shader_link": 1,
        "set_flags": 2543799428,
        "shape_type": 0,
        "texture_types": [
          536870921,
          536870929,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2504,
      "candidate_instructions": 2409
    },
    {
      "case": {
        "label": "random11",
        "models": 0,
        "shapes": 2,
        "subs": 2,
        "sets": 2,
        "primitives": 2,
        "materials": 3,
        "meshes": 2,
        "secondary": true,
        "flags": 1938717850,
        "existing_shader": 2,
        "no_shader_link": 1,
        "set_flags": 4193761663,
        "shape_type": 268435457,
        "texture_types": [
          536870929,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 554,
      "candidate_instructions": 540
    },
    {
      "case": {
        "label": "random12",
        "models": 1,
        "shapes": 1,
        "subs": 0,
        "sets": 2,
        "primitives": 0,
        "materials": 2,
        "meshes": 1,
        "secondary": true,
        "flags": 916930915,
        "existing_shader": -1,
        "no_shader_link": -1,
        "set_flags": 2533498370,
        "shape_type": 4294967295,
        "texture_types": []
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1684,
      "candidate_instructions": 1580
    },
    {
      "case": {
        "label": "random13",
        "models": 0,
        "shapes": 2,
        "subs": 2,
        "sets": 0,
        "primitives": 3,
        "materials": 2,
        "meshes": 0,
        "secondary": false,
        "flags": 1436794065,
        "existing_shader": 1,
        "no_shader_link": 3,
        "set_flags": 4174004343,
        "shape_type": 4294967295,
        "texture_types": [
          3735928559,
          536870921,
          3735928559,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 686,
      "candidate_instructions": 670
    },
    {
      "case": {
        "label": "random14",
        "models": 1,
        "shapes": 2,
        "subs": 0,
        "sets": 0,
        "primitives": 1,
        "materials": 4,
        "meshes": 1,
        "secondary": true,
        "flags": 635256831,
        "existing_shader": -1,
        "no_shader_link": 3,
        "set_flags": 1203357235,
        "shape_type": 4294967295,
        "texture_types": []
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2002,
      "candidate_instructions": 1863
    },
    {
      "case": {
        "label": "random15",
        "models": 0,
        "shapes": 3,
        "subs": 2,
        "sets": 1,
        "primitives": 1,
        "materials": 1,
        "meshes": 0,
        "secondary": false,
        "flags": 2110419974,
        "existing_shader": 0,
        "no_shader_link": 2,
        "set_flags": 1026842790,
        "shape_type": 268435457,
        "texture_types": [
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 476,
      "candidate_instructions": 460
    },
    {
      "case": {
        "label": "random16",
        "models": 2,
        "shapes": 2,
        "subs": 1,
        "sets": 2,
        "primitives": 0,
        "materials": 2,
        "meshes": 3,
        "secondary": true,
        "flags": 932282361,
        "existing_shader": 0,
        "no_shader_link": 0,
        "set_flags": 1257964250,
        "shape_type": 0,
        "texture_types": [
          536870921,
          3735928559,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3318,
      "candidate_instructions": 3281
    },
    {
      "case": {
        "label": "random17",
        "models": 3,
        "shapes": 3,
        "subs": 1,
        "sets": 2,
        "primitives": 3,
        "materials": 0,
        "meshes": 1,
        "secondary": false,
        "flags": 3743051006,
        "existing_shader": 0,
        "no_shader_link": 1,
        "set_flags": 3770860426,
        "shape_type": 4294967295,
        "texture_types": [
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 5219,
      "candidate_instructions": 5196
    },
    {
      "case": {
        "label": "random18",
        "models": 2,
        "shapes": 2,
        "subs": 1,
        "sets": 2,
        "primitives": 0,
        "materials": 1,
        "meshes": 3,
        "secondary": true,
        "flags": 2661894674,
        "existing_shader": -1,
        "no_shader_link": 3,
        "set_flags": 2549345809,
        "shape_type": 0,
        "texture_types": []
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2812,
      "candidate_instructions": 2785
    },
    {
      "case": {
        "label": "random19",
        "models": 2,
        "shapes": 3,
        "subs": 2,
        "sets": 2,
        "primitives": 2,
        "materials": 0,
        "meshes": 3,
        "secondary": true,
        "flags": 4181933856,
        "existing_shader": 3,
        "no_shader_link": 0,
        "set_flags": 1482374253,
        "shape_type": 268435457,
        "texture_types": [
          536870921,
          536870921,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 5852,
      "candidate_instructions": 5838
    },
    {
      "case": {
        "label": "random20",
        "models": 0,
        "shapes": 2,
        "subs": 1,
        "sets": 0,
        "primitives": 1,
        "materials": 3,
        "meshes": 3,
        "secondary": true,
        "flags": 4145778271,
        "existing_shader": 1,
        "no_shader_link": 0,
        "set_flags": 485508350,
        "shape_type": 0,
        "texture_types": [
          536870929,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 554,
      "candidate_instructions": 540
    },
    {
      "case": {
        "label": "random21",
        "models": 1,
        "shapes": 3,
        "subs": 1,
        "sets": 0,
        "primitives": 1,
        "materials": 3,
        "meshes": 3,
        "secondary": true,
        "flags": 4187752076,
        "existing_shader": -1,
        "no_shader_link": 1,
        "set_flags": 3873970349,
        "shape_type": 0,
        "texture_types": []
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1884,
      "candidate_instructions": 1762
    },
    {
      "case": {
        "label": "random22",
        "models": 3,
        "shapes": 2,
        "subs": 2,
        "sets": 2,
        "primitives": 2,
        "materials": 1,
        "meshes": 0,
        "secondary": true,
        "flags": 1913014077,
        "existing_shader": -1,
        "no_shader_link": 2,
        "set_flags": 1277318240,
        "shape_type": 0,
        "texture_types": [
          3735928559,
          3735928559,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 5206,
      "candidate_instructions": 5295
    },
    {
      "case": {
        "label": "random23",
        "models": 2,
        "shapes": 2,
        "subs": 2,
        "sets": 1,
        "primitives": 0,
        "materials": 2,
        "meshes": 2,
        "secondary": false,
        "flags": 1757982747,
        "existing_shader": -1,
        "no_shader_link": 3,
        "set_flags": 78600830,
        "shape_type": 4294967295,
        "texture_types": [
          536870921,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3134,
      "candidate_instructions": 3132
    },
    {
      "case": {
        "label": "random24",
        "models": 3,
        "shapes": 2,
        "subs": 2,
        "sets": 2,
        "primitives": 2,
        "materials": 3,
        "meshes": 2,
        "secondary": true,
        "flags": 1006673602,
        "existing_shader": 1,
        "no_shader_link": -1,
        "set_flags": 1792776531,
        "shape_type": 268435457,
        "texture_types": [
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 6332,
      "candidate_instructions": 6368
    },
    {
      "case": {
        "label": "random25",
        "models": 2,
        "shapes": 2,
        "subs": 1,
        "sets": 2,
        "primitives": 1,
        "materials": 0,
        "meshes": 1,
        "secondary": true,
        "flags": 388780545,
        "existing_shader": 3,
        "no_shader_link": 1,
        "set_flags": 2130418476,
        "shape_type": 268435457,
        "texture_types": [
          3735928559,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3285,
      "candidate_instructions": 3202
    },
    {
      "case": {
        "label": "random26",
        "models": 2,
        "shapes": 1,
        "subs": 1,
        "sets": 1,
        "primitives": 3,
        "materials": 4,
        "meshes": 0,
        "secondary": true,
        "flags": 1834249988,
        "existing_shader": 1,
        "no_shader_link": 2,
        "set_flags": 1890727964,
        "shape_type": 0,
        "texture_types": [
          536870921,
          536870921,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2818,
      "candidate_instructions": 2752
    },
    {
      "case": {
        "label": "random27",
        "models": 2,
        "shapes": 3,
        "subs": 2,
        "sets": 1,
        "primitives": 2,
        "materials": 4,
        "meshes": 3,
        "secondary": false,
        "flags": 3272055529,
        "existing_shader": 2,
        "no_shader_link": -1,
        "set_flags": 3751220735,
        "shape_type": 0,
        "texture_types": [
          536870929,
          536870921,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3888,
      "candidate_instructions": 3980
    },
    {
      "case": {
        "label": "random28",
        "models": 2,
        "shapes": 1,
        "subs": 0,
        "sets": 2,
        "primitives": 1,
        "materials": 2,
        "meshes": 0,
        "secondary": true,
        "flags": 3123050804,
        "existing_shader": 2,
        "no_shader_link": 0,
        "set_flags": 629260846,
        "shape_type": 268435457,
        "texture_types": [
          536870921,
          536870921,
          536870921,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2550,
      "candidate_instructions": 2441
    },
    {
      "case": {
        "label": "random29",
        "models": 0,
        "shapes": 3,
        "subs": 1,
        "sets": 0,
        "primitives": 1,
        "materials": 2,
        "meshes": 2,
        "secondary": true,
        "flags": 3779089487,
        "existing_shader": 3,
        "no_shader_link": 3,
        "set_flags": 4013136768,
        "shape_type": 0,
        "texture_types": []
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 414,
      "candidate_instructions": 400
    },
    {
      "case": {
        "label": "random30",
        "models": 2,
        "shapes": 2,
        "subs": 2,
        "sets": 2,
        "primitives": 0,
        "materials": 0,
        "meshes": 0,
        "secondary": true,
        "flags": 1266828634,
        "existing_shader": 0,
        "no_shader_link": -1,
        "set_flags": 2981050877,
        "shape_type": 4294967295,
        "texture_types": [
          3735928559,
          536870921,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3692,
      "candidate_instructions": 3664
    },
    {
      "case": {
        "label": "random31",
        "models": 3,
        "shapes": 2,
        "subs": 1,
        "sets": 1,
        "primitives": 1,
        "materials": 1,
        "meshes": 0,
        "secondary": false,
        "flags": 3472742545,
        "existing_shader": 0,
        "no_shader_link": 3,
        "set_flags": 3549746331,
        "shape_type": 268435457,
        "texture_types": [
          536870929,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3172,
      "candidate_instructions": 3136
    },
    {
      "case": {
        "label": "random32",
        "models": 2,
        "shapes": 1,
        "subs": 2,
        "sets": 0,
        "primitives": 1,
        "materials": 4,
        "meshes": 2,
        "secondary": true,
        "flags": 1482161655,
        "existing_shader": 1,
        "no_shader_link": 2,
        "set_flags": 1155309393,
        "shape_type": 268435457,
        "texture_types": []
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2600,
      "candidate_instructions": 2490
    },
    {
      "case": {
        "label": "random33",
        "models": 1,
        "shapes": 2,
        "subs": 0,
        "sets": 0,
        "primitives": 0,
        "materials": 2,
        "meshes": 0,
        "secondary": true,
        "flags": 240884897,
        "existing_shader": 1,
        "no_shader_link": 2,
        "set_flags": 2300012043,
        "shape_type": 0,
        "texture_types": [
          536870929,
          3735928559,
          536870921,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2036,
      "candidate_instructions": 1933
    },
    {
      "case": {
        "label": "random34",
        "models": 1,
        "shapes": 1,
        "subs": 1,
        "sets": 2,
        "primitives": 0,
        "materials": 4,
        "meshes": 0,
        "secondary": true,
        "flags": 2428057845,
        "existing_shader": 2,
        "no_shader_link": -1,
        "set_flags": 3417593886,
        "shape_type": 4294967295,
        "texture_types": [
          536870921,
          536870921,
          536870921,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2450,
      "candidate_instructions": 2345
    },
    {
      "case": {
        "label": "random35",
        "models": 2,
        "shapes": 1,
        "subs": 0,
        "sets": 1,
        "primitives": 3,
        "materials": 4,
        "meshes": 3,
        "secondary": true,
        "flags": 519243940,
        "existing_shader": 1,
        "no_shader_link": 2,
        "set_flags": 3054560800,
        "shape_type": 0,
        "texture_types": [
          536870929,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2608,
      "candidate_instructions": 2548
    },
    {
      "case": {
        "label": "random36",
        "models": 2,
        "shapes": 3,
        "subs": 1,
        "sets": 1,
        "primitives": 1,
        "materials": 4,
        "meshes": 3,
        "secondary": false,
        "flags": 1628955857,
        "existing_shader": -1,
        "no_shader_link": 2,
        "set_flags": 1960776454,
        "shape_type": 0,
        "texture_types": []
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2811,
      "candidate_instructions": 2856
    },
    {
      "case": {
        "label": "random37",
        "models": 3,
        "shapes": 0,
        "subs": 1,
        "sets": 1,
        "primitives": 1,
        "materials": 4,
        "meshes": 2,
        "secondary": true,
        "flags": 1350247318,
        "existing_shader": 3,
        "no_shader_link": 1,
        "set_flags": 2196801756,
        "shape_type": 4294967295,
        "texture_types": [
          3735928559,
          3735928559,
          3735928559,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2904,
      "candidate_instructions": 2874
    },
    {
      "case": {
        "label": "random38",
        "models": 3,
        "shapes": 2,
        "subs": 1,
        "sets": 1,
        "primitives": 1,
        "materials": 0,
        "meshes": 0,
        "secondary": true,
        "flags": 2686281543,
        "existing_shader": 0,
        "no_shader_link": 2,
        "set_flags": 3766651490,
        "shape_type": 0,
        "texture_types": []
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2838,
      "candidate_instructions": 2784
    },
    {
      "case": {
        "label": "random39",
        "models": 1,
        "shapes": 2,
        "subs": 2,
        "sets": 1,
        "primitives": 2,
        "materials": 0,
        "meshes": 1,
        "secondary": true,
        "flags": 1765888554,
        "existing_shader": 2,
        "no_shader_link": -1,
        "set_flags": 3962134166,
        "shape_type": 4294967295,
        "texture_types": [
          536870929,
          3735928559,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2712,
      "candidate_instructions": 2614
    },
    {
      "case": {
        "label": "random40",
        "models": 0,
        "shapes": 3,
        "subs": 0,
        "sets": 0,
        "primitives": 1,
        "materials": 3,
        "meshes": 1,
        "secondary": true,
        "flags": 3886345999,
        "existing_shader": 0,
        "no_shader_link": -1,
        "set_flags": 3157566101,
        "shape_type": 0,
        "texture_types": [
          3735928559,
          536870929,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 624,
      "candidate_instructions": 610
    },
    {
      "case": {
        "label": "random41",
        "models": 0,
        "shapes": 3,
        "subs": 2,
        "sets": 1,
        "primitives": 3,
        "materials": 2,
        "meshes": 2,
        "secondary": false,
        "flags": 3608265225,
        "existing_shader": 3,
        "no_shader_link": 2,
        "set_flags": 4053466995,
        "shape_type": 268435457,
        "texture_types": [
          3735928559,
          3735928559,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 616,
      "candidate_instructions": 600
    },
    {
      "case": {
        "label": "random42",
        "models": 2,
        "shapes": 0,
        "subs": 1,
        "sets": 1,
        "primitives": 3,
        "materials": 1,
        "meshes": 2,
        "secondary": false,
        "flags": 1192269238,
        "existing_shader": 0,
        "no_shader_link": 2,
        "set_flags": 2730211049,
        "shape_type": 0,
        "texture_types": []
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 990,
      "candidate_instructions": 1004
    },
    {
      "case": {
        "label": "random43",
        "models": 2,
        "shapes": 3,
        "subs": 1,
        "sets": 0,
        "primitives": 2,
        "materials": 2,
        "meshes": 3,
        "secondary": true,
        "flags": 980854263,
        "existing_shader": 0,
        "no_shader_link": 2,
        "set_flags": 2892195779,
        "shape_type": 268435457,
        "texture_types": [
          536870921,
          536870921,
          536870929,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3628,
      "candidate_instructions": 3447
    },
    {
      "case": {
        "label": "random44",
        "models": 2,
        "shapes": 2,
        "subs": 1,
        "sets": 1,
        "primitives": 2,
        "materials": 0,
        "meshes": 1,
        "secondary": false,
        "flags": 532461967,
        "existing_shader": 0,
        "no_shader_link": 0,
        "set_flags": 1628965498,
        "shape_type": 268435457,
        "texture_types": [
          536870929,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2478,
      "candidate_instructions": 2429
    },
    {
      "case": {
        "label": "random45",
        "models": 0,
        "shapes": 3,
        "subs": 0,
        "sets": 2,
        "primitives": 1,
        "materials": 2,
        "meshes": 0,
        "secondary": true,
        "flags": 1476556482,
        "existing_shader": 3,
        "no_shader_link": 1,
        "set_flags": 1871247204,
        "shape_type": 4294967295,
        "texture_types": [
          536870929,
          3735928559,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 624,
      "candidate_instructions": 610
    },
    {
      "case": {
        "label": "random46",
        "models": 3,
        "shapes": 1,
        "subs": 1,
        "sets": 1,
        "primitives": 1,
        "materials": 0,
        "meshes": 1,
        "secondary": true,
        "flags": 1529674783,
        "existing_shader": 2,
        "no_shader_link": -1,
        "set_flags": 3873632405,
        "shape_type": 0,
        "texture_types": [
          536870921,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2608,
      "candidate_instructions": 2556
    },
    {
      "case": {
        "label": "random47",
        "models": 1,
        "shapes": 0,
        "subs": 1,
        "sets": 0,
        "primitives": 1,
        "materials": 2,
        "meshes": 1,
        "secondary": true,
        "flags": 3365921964,
        "existing_shader": 1,
        "no_shader_link": 3,
        "set_flags": 498592506,
        "shape_type": 268435457,
        "texture_types": [
          536870929,
          536870921,
          536870929,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1884,
      "candidate_instructions": 1793
    },
    {
      "case": {
        "label": "random48",
        "models": 1,
        "shapes": 2,
        "subs": 1,
        "sets": 2,
        "primitives": 0,
        "materials": 2,
        "meshes": 2,
        "secondary": false,
        "flags": 3954981301,
        "existing_shader": 0,
        "no_shader_link": -1,
        "set_flags": 2172693015,
        "shape_type": 4294967295,
        "texture_types": [
          536870921,
          536870929,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1860,
      "candidate_instructions": 1856
    },
    {
      "case": {
        "label": "random49",
        "models": 3,
        "shapes": 1,
        "subs": 2,
        "sets": 2,
        "primitives": 0,
        "materials": 2,
        "meshes": 2,
        "secondary": false,
        "flags": 3883794304,
        "existing_shader": 3,
        "no_shader_link": 3,
        "set_flags": 690801648,
        "shape_type": 0,
        "texture_types": [
          3735928559,
          536870921,
          536870929,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 3150,
      "candidate_instructions": 3268
    },
    {
      "case": {
        "label": "random50",
        "models": 0,
        "shapes": 1,
        "subs": 2,
        "sets": 2,
        "primitives": 0,
        "materials": 1,
        "meshes": 3,
        "secondary": false,
        "flags": 586513050,
        "existing_shader": 1,
        "no_shader_link": 2,
        "set_flags": 2687934367,
        "shape_type": 0,
        "texture_types": [
          536870921,
          3735928559,
          3735928559,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 686,
      "candidate_instructions": 670
    },
    {
      "case": {
        "label": "random51",
        "models": 2,
        "shapes": 3,
        "subs": 1,
        "sets": 2,
        "primitives": 3,
        "materials": 1,
        "meshes": 3,
        "secondary": true,
        "flags": 2719755441,
        "existing_shader": 3,
        "no_shader_link": 3,
        "set_flags": 3850558144,
        "shape_type": 268435457,
        "texture_types": [
          536870929,
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 4989,
      "candidate_instructions": 4903
    },
    {
      "case": {
        "label": "random52",
        "models": 3,
        "shapes": 1,
        "subs": 2,
        "sets": 0,
        "primitives": 1,
        "materials": 4,
        "meshes": 2,
        "secondary": false,
        "flags": 3667872879,
        "existing_shader": 0,
        "no_shader_link": 2,
        "set_flags": 1997517466,
        "shape_type": 0,
        "texture_types": [
          3735928559,
          3735928559,
          536870921,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 2382,
      "candidate_instructions": 2430
    },
    {
      "case": {
        "label": "random53",
        "models": 0,
        "shapes": 2,
        "subs": 1,
        "sets": 2,
        "primitives": 1,
        "materials": 2,
        "meshes": 1,
        "secondary": false,
        "flags": 1047665074,
        "existing_shader": 2,
        "no_shader_link": 0,
        "set_flags": 1775298407,
        "shape_type": 268435457,
        "texture_types": [
          3735928559,
          3735928559
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 546,
      "candidate_instructions": 530
    },
    {
      "case": {
        "label": "random54",
        "models": 3,
        "shapes": 1,
        "subs": 0,
        "sets": 1,
        "primitives": 3,
        "materials": 0,
        "meshes": 1,
        "secondary": false,
        "flags": 3617792083,
        "existing_shader": -1,
        "no_shader_link": 2,
        "set_flags": 2540321601,
        "shape_type": 268435457,
        "texture_types": [
          536870929,
          536870921,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1950,
      "candidate_instructions": 1914
    },
    {
      "case": {
        "label": "random55",
        "models": 2,
        "shapes": 3,
        "subs": 2,
        "sets": 2,
        "primitives": 2,
        "materials": 3,
        "meshes": 1,
        "secondary": false,
        "flags": 882104964,
        "existing_shader": 2,
        "no_shader_link": 2,
        "set_flags": 303040345,
        "shape_type": 4294967295,
        "texture_types": [
          536870929,
          3735928559,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 5104,
      "candidate_instructions": 5188
    },
    {
      "case": {
        "label": "random56",
        "models": 3,
        "shapes": 0,
        "subs": 0,
        "sets": 0,
        "primitives": 2,
        "materials": 4,
        "meshes": 2,
        "secondary": false,
        "flags": 3103068749,
        "existing_shader": 2,
        "no_shader_link": -1,
        "set_flags": 41082040,
        "shape_type": 0,
        "texture_types": [
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1692,
      "candidate_instructions": 1800
    },
    {
      "case": {
        "label": "random57",
        "models": 1,
        "shapes": 0,
        "subs": 0,
        "sets": 1,
        "primitives": 3,
        "materials": 1,
        "meshes": 0,
        "secondary": true,
        "flags": 2017310939,
        "existing_shader": 2,
        "no_shader_link": 3,
        "set_flags": 1039217805,
        "shape_type": 4294967295,
        "texture_types": [
          536870921
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 1430,
      "candidate_instructions": 1343
    },
    {
      "case": {
        "label": "random58",
        "models": 0,
        "shapes": 1,
        "subs": 0,
        "sets": 1,
        "primitives": 3,
        "materials": 2,
        "meshes": 1,
        "secondary": true,
        "flags": 3975415712,
        "existing_shader": 1,
        "no_shader_link": 3,
        "set_flags": 2216956626,
        "shape_type": 4294967295,
        "texture_types": [
          536870929,
          536870921,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 624,
      "candidate_instructions": 610
    },
    {
      "case": {
        "label": "random59",
        "models": 3,
        "shapes": 3,
        "subs": 1,
        "sets": 2,
        "primitives": 2,
        "materials": 3,
        "meshes": 0,
        "secondary": true,
        "flags": 1470924225,
        "existing_shader": 0,
        "no_shader_link": 3,
        "set_flags": 215363011,
        "shape_type": 4294967295,
        "texture_types": [
          536870929,
          536870929,
          536870929
        ]
      },
      "same": true,
      "original_fault": null,
      "candidate_fault": null,
      "original_returned": true,
      "candidate_returned": true,
      "original_instructions": 6198,
      "candidate_instructions": 6142
    },
    {
      "case": {
        "label": "fault-primary_wrapper",
        "fault": "primary_wrapper"
      },
      "same": true,
      "original_fault": [
        19,
        0,
        4
      ],
      "candidate_fault": [
        19,
        0,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 6,
      "candidate_instructions": 8
    },
    {
      "case": {
        "label": "fault-primary_resource",
        "fault": "primary_resource"
      },
      "same": true,
      "original_fault": [
        19,
        40,
        4
      ],
      "candidate_fault": [
        19,
        40,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 7,
      "candidate_instructions": 10
    },
    {
      "case": {
        "label": "fault-model_dict",
        "fault": "model_dict"
      },
      "same": true,
      "original_fault": [
        19,
        12,
        4
      ],
      "candidate_fault": [
        19,
        12,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 51,
      "candidate_instructions": 48
    },
    {
      "case": {
        "label": "fault-secondary_resource",
        "fault": "secondary_resource"
      },
      "same": true,
      "original_fault": [
        19,
        64,
        4
      ],
      "candidate_fault": [
        19,
        64,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 315,
      "candidate_instructions": 304
    },
    {
      "case": {
        "label": "fault-null_lock",
        "fault": "null_lock"
      },
      "same": true,
      "original_fault": [
        19,
        16,
        4
      ],
      "candidate_fault": [
        19,
        16,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 287,
      "candidate_instructions": 277
    },
    {
      "case": {
        "label": "fault-null_shape",
        "fault": "null_shape"
      },
      "same": true,
      "original_fault": [
        19,
        48,
        4
      ],
      "candidate_fault": [
        19,
        48,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 79,
      "candidate_instructions": 76
    },
    {
      "case": {
        "label": "fault-null_material",
        "fault": "null_material"
      },
      "same": true,
      "original_fault": [
        19,
        644,
        4
      ],
      "candidate_fault": [
        19,
        644,
        4
      ],
      "original_returned": false,
      "candidate_returned": false,
      "original_instructions": 482,
      "candidate_instructions": 435
    }
  ],
  "source_sha256": "05f3ef02cccf219d934e6ca6cc42d9b1dbfe15f1ce64d7b7ab04bf96558a0d6a"
}
```
