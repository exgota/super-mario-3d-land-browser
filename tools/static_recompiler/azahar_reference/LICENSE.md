# Source patch license and provenance

SPDX-License-Identifier: GPL-2.0-or-later

`azahar_capture.patch` and `azahar_deterministic_io.patch` modify the public Azahar emulator source at commit `662d412123305a9f4be94dd3dc73ddf91a18c55e`. The upstream files identify their authors as the Citra Emulator Project and Azahar Emulator Project, with copyright notices retained in the patch context and pinned checkout. Upstream declares GPL version 2 or any later version in those source headers and provides [its license text](https://github.com/azahar-emu/azahar/blob/662d412123305a9f4be94dd3dc73ddf91a18c55e/license.txt).

The project-authored instrumentation and isolated deterministic file-delay correction in these patches are supplied under the same GPL-2.0-or-later terms. Obtain the complete applicable text from [GNU General Public License version 2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html), or a later version chosen under that grant. Retain upstream notices and provide the applicable license with a distributed source derivative.

This scoped license notice does not relicense the rest of the repository. The patches contain source changes only. Azahar checkouts, downloaded dependencies, executables and owner-derived game data must remain in ignored directories. No Nintendo source or SDK material was used for this instrumentation.
