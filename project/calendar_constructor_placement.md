# CalendarTime section-placement candidate

Branch `root/calendar-constructor-placement`, base `0800d9a`. Metadata commit `109d2ab` changes only the constructor row's SectionName and an append-only decision note. The original C2 symbol remains the claim. Its unchanged interval is 0x002DA434..0x002DA4A8, 116 bytes. No rank cells or source files change.

The clean baseline build places the C1 implementation and its zero-sized C2 alias at 0x01000010. The normal checker reports the symbol absent. The explicit C1 section override follows the already accepted CalendarTime::Time C2 row. After a fresh `python make.py eu -ca`, placement is within the compact code region and the normal checker reports `M -> O (Now Matching.)`. The five existing CalendarTime/DateUtil roots report `Still matching`. These standard checker runs use the committed project-built object; rank effects are restored afterward and contribute no committed rank changes.

Final source SHA-256: `265d908447edf093f98b88d441c9098593374b4aca0825948cb84497eb4a478e`. Final header SHA-256: `21f7e62cb0a655fc5bd2fa69e6381db90e6301b09abc59c3b4c6fd004fb56b2b`. Original executable SHA-256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Source patch is empty. No ARMCC flags, tools, boundaries, pools or binary inputs change.

The integrator must verify the claim and the complete map. Local sibling checks establish family preservation only. No full-map, full-image or gameplay result is claimed here. Private build logs remain under build/root_calendar_*.log.
