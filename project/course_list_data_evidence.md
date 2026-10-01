# CourseList source-data ownership

Reviewed on 2026-10-01 from origin/dot/course-list, before source intake. The unchanged EU executable has SHA-256 e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64.

The ordinary source string initializers and their padding reconstruct every byte of [0x003A2884,0x003A2958), 212 bytes. Root independently rebuilt each proposed ASCII field, implicit zero tail and final FF padding byte from the review manifest and compared the unchanged retail bytes. The complete representation hashes to 6977bc957a7bbf3f85a4155ad379869b6f43cd0f32eff93ed509568a06b233e5.

Fifteen source sections cover sixteen existing data rows. Fourteen sections fit one existing row each. The sole shared section, .sdata_dat_003A2890, contains Type at offset0, Normal at offset8 and Miniature at offset16, with sizes8,8 and12. Its complete extent is28 bytes through0x003A28AC. Existing rows [0x003A2890,0x003A28A0) and [0x003A28A0,0x003A28AC) remain separate. The first row supplies a section-base address, not a claim to own the whole section.

Independent retail World construction at0x00325BE8 loads Miniature through pool0x00325DC8, whose value is0x003A28A0. At0x00325BF0 it derives Normal by subtracting8. The Type pointer is independently retained in pool0x00325DCC. This confirms the adjacency without changing any mapped interval.

No data row, boundary, rank or function interval changes in this evidence commit. Parsed initializer equality is not compiled-data or code acceptance. Root must compare all fifteen emitted canonical sections after committed source builds. Detailed field/row hashes remain in ignored build/dot_proposals/course-list/data_sections.json.
