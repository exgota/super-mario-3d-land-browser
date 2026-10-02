# Course-select scene initializer ownership

Frozen base: 86104d96a7f570383bbfefc5fffad998e134496c. This lane owns only the new Game/backup/src/Scene/CourseSelectSceneInit.cpp and its project/dot_reports/course-select-scene-init notes. The queue screen supplied by the coordinator found no owner or attempted implementation for 0017CB1C. All existing source/header/map/tool files remain unchanged.

The target is the complete existing row 0017CB1C–0017D398 (2172 bytes). The vtable installed by independent constructor 0017D8C0 is 003CD21C, and its init slot +0x14 contains 0017CB1C. The constructor calls al::Scene::Scene at 00274498 and stores incoming r1/r2 words at +0x34/+0x38. Independent caller 00212428 allocates 0x6c bytes, loads r1 from caller+0x14 and r2 from *(caller+0x0c)+0x154, then calls this constructor. Original public C++ parameter types remain unresolved. Its inline label reads コース選択シーン. The inherited CourseSelectScene header does not describe both recovered fields; this reconstruction uses private observed ABI views and does not change that header.

The row's pool marker 0017D124 begins an interior pool through 0017D244. Executable code resumes there through 0017D380, followed by a final literal/string tail through 0017D398. The pool marker is not a function boundary.

Estimated chance of exact acceptance 0.15 for 2172 bytes, with two hours investigation/implementation/verification and zero credited downstream bytes: 162.9 expected bytes/hour. This is a planning estimate, not observed throughput. Compiler 4.1/791 follows this game scene's configured module; 894 is absent and no comparative compiler identification is claimed.
