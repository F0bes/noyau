# noyau

Hello, if you've somehow stumbled upon here, this is my PS2 kernel. If you have the PS2 toolchain and SDK installed it should be quite straightforward for you to build.

Because CMake has lackluster toolchain support, building both the loader and kernel requires some extra CMake arguments.

`cmake .. -DCMAKE_C_COMPILER:FILEPATH=$PS2DEV/ee/bin/mips64r5900el-ps2-elf-gcc -DCMAKE_CXX_COMPILER:FILEPATH=$PS2DEV/ee/bin/mips64r5900el-ps2-elf-g++`

With the CMake and CMake Tools VSCode extensions you can simply select the detected `mips64r5900el-ps2-elf` kit when prompted and it will take care of the rest.

Ideally it works on both an emulator and a real PS2. I provide some targets to aid, but you can run the loader elf manually if you choose.

To run on the emulator run `ninja emu` or `make emu` depending on your generator. The only requirement is that you have a `$PCSX2` environment variable pointed to a pcsx2-qt.

To run on your console run `ninja run` or `make run` depending on your generator. The requirement is that you have a proper ps2client and ps2link setup running.


There is no project roadmap. In the current state most of the L1 interrupts are handled enough so we don't crash. GS initialization is done but is hardcoded and not ideal. There are some vdevs implemented too.

