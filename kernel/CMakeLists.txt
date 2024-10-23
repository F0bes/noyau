set(CMAKE_TOOLCHAIN_FILE ps2-bare-cross.txt)

add_executable(kernel)

target_include_directories(kernel PRIVATE include)

target_compile_options(kernel PRIVATE
	-O2
	-Wall
	-Wextra
	-Wno-error=unused-parameter
	-Wno-unused-parameter
	-Werror
	-fno-exceptions
	-ffreestanding
	-nostdlib
	-ffixed-k0
	-include ${CMAKE_CURRENT_SOURCE_DIR}/include/common_typedefs.hpp
)

# Using target_compile_options passes the flags to the C compiler as well
# GCC doesn't like that so ensure that -fno-rtti is only a CXX flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

target_link_options(kernel PRIVATE
	-T${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld
	-ffreestanding
	-nostdlib
	-Wl,--orphan-handling=error
)

target_link_libraries(kernel PRIVATE
	gcc
)

set(kernel_SOURCES
	src/bootstrap.s
	src/frame.s
	src/kmain.cpp
	src/memory.cpp

	src/cop0/cop0.cpp
	src/debug/debug.cpp
	src/fs/vdev.cpp
	src/fs/vfs.cpp
	src/gs/gs.cpp
	src/hw_interrupt/handlers.s
	src/hw_interrupt/v_interrupt.cpp
	src/hw_interrupt/v_common.cpp
	src/hw_interrupt/common.cpp
	src/intc/intc.cpp
	src/sif/sif.cpp
	src/sio/sio.cpp
	src/timer/timer.cpp
	src/syscall/syscall.cpp
	src/util/printf.c

	# Data. TODO: bin2c this
	src/data/vterm_font_itex.c 
)

set(kernel_HEADERS
	include/common_typedefs.hpp
	include/memory.hpp
	include/memutil.hpp

	include/cop0/cop0.hpp
	include/debug/debug.hpp
	include/fs/vdev.hpp
	include/fs/vfs.hpp
	include/gs/gs.hpp
	include/gs/regs.hpp
	include/hw_interrupt/common.hpp
	include/mmio/dma.hpp
	include/mmio/gs.hpp
	include/mmio/sif.hpp
	include/mmio/intc.hpp
	include/sif/sif.hpp
	include/sio/sio.hpp
	include/syscall/syscall.hpp
	include/timer/timer.hpp
	include/util/printf.h
)

target_sources(kernel PRIVATE ${kernel_SOURCES} ${kernel_HEADERS})
