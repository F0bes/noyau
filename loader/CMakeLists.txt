add_executable(noyau-load)

target_compile_options(noyau-load PRIVATE -Wall -Werror)
target_include_directories(noyau-load PRIVATE include)

target_sources(noyau-load PRIVATE src/loader.c kernel.c)

target_link_libraries(noyau-load kernel)

add_custom_command(OUTPUT kernel.c
	COMMAND bin2c $<TARGET_FILE:kernel> kernel.c kelf_data
	DEPENDS $<TARGET_FILE:kernel>
)

set_target_properties(noyau-load PROPERTIES SUFFIX ".elf")

add_dependencies(noyau-load kernel)

# Make up for the poor toolchain file support CMake has ->
target_link_options(noyau-load PRIVATE
	-T$ENV{PS2SDK}/ee/startup/linkfile
)

target_include_directories(noyau-load PRIVATE
	"include/"
	$ENV{PS2SDK}/ee/include
	$ENV{PS2SDK}/common/include
)

target_compile_definitions(noyau-load PRIVATE _EE)
# <-

add_custom_target(run
	COMMAND ps2client execee host:noyau-load.elf
	DEPENDS noyau-load.elf
	USES_TERMINAL
)

add_custom_target(reset
	COMMAND ps2client reset
)

add_custom_target(emu
	COMMAND $ENV{PCSX2} -elf ${CMAKE_BINARY_DIR}/loader/noyau-load.elf
	DEPENDS noyau-load.elf
	USES_TERMINAL
)
