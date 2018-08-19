# Zany80

![Build Status](https://travis-ci.org/Zany80/Zany80.svg?branch=master "Build status from Travis CI")

Zany80 is a plugin-based Fantasy Console. It is a piece of software that emulates a game system that never existed. It also includes tools to make creation of games for the system easier.

Important licensing note: Zany80 requires kcc and scas from the amazing
[KnightOS](https://github.com/KnightOS) project. kcc is licensed under GPLv2.
However, Zany80 neither links against nor includes any of it's source, so GPL is
*not* required for Zany80, and will *not* be used here. kcc is invoked via
`system()` calls in order to compile C source. It is a vanilla build of kcc, no
changes. The source can be found [here](https://github.com/knightos/kcc). scas
is invoked similarly in order to assemble z80 assembly files. scas's license can
be found [here](https://github.com/KnightOS/scas/blob/master/LICENSE) and the
source is in [the same repo](https://github.com/KnightOS/scas). **Both are
vanilla builds, and neither are linked against; they are bundled with the
installer/package. No code is used from either.**

The sdcpp.exe files used on Windows are licensed under the GPLv3 license. They
are not linked against, and none of their source is used. They are invoked
indirectly by KCC.

Note: in compliance with the relevant licenses, a copy of each can be found next
to the executables in misc/win32 and misc/win64.
