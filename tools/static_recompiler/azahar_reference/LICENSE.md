# Source patch license and provenance

SPDX-License-Identifier: GPL-2.0-or-later

`azahar_capture.patch`, `azahar_deterministic_io.patch`, `azahar_static_execution.patch`, `azahar_render_capture.patch`, `azahar_input_capture.patch`, `azahar_audio_capture.patch`, `azahar_replay_duration.patch` and `azahar_default_applets.patch` modify the public Azahar emulator source at commit `662d412123305a9f4be94dd3dc73ddf91a18c55e`. The upstream files identify their authors as the Citra Emulator Project and Azahar Emulator Project, with copyright notices retained in the patch context and pinned checkout. Upstream declares GPL version 2 or any later version in those source headers and provides [its license text](https://github.com/azahar-emu/azahar/blob/662d412123305a9f4be94dd3dc73ddf91a18c55e/license.txt).

The project-authored instrumentation, static platform adapter and isolated deterministic file-delay correction in these patches are supplied under the same GPL-2.0-or-later terms. Obtain the complete applicable text from [GNU General Public License version 2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html), or a later version chosen under that grant. Retain upstream notices and provide the applicable license with a distributed source derivative.

This scoped license notice does not relicense the rest of the repository. The patches contain source changes only. Azahar checkouts, downloaded dependencies, executables and owner-derived game data must remain in ignored directories. No Nintendo source or SDK material was used for this instrumentation.

## Dynarmic ARM64 control correction

`azahar_floating_point_control.patch` modifies the public Dynarmic file `src/dynarmic/backend/arm64/emit_arm64_a32.cpp` at revision `e77b1ba0b7da7cbe93021b01a663acfe7c4dd516`. That file identifies its existing source as 0BSD and retains the upstream notices. The project-authored one-line correction is also supplied under 0BSD. This scoped grant does not change Azahar's GPL obligations when distributing its combined derivative. The standalone verification driver and runner contain project-authored tooling and no Nintendo source or guest instruction words.

0BSD permission: Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

`azahar_replay_payload_limits.patch` changes only project-authored capture source added to the pinned public Azahar derivative. Its additions use the same GPL-2.0-or-later terms described above. No game bytes or Nintendo implementation are included.
