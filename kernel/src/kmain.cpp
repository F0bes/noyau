// SPDX-License-Identifier: MPL-2.0

#include "memutil.hpp"
#include "debug/debug.hpp"
#include "timer/timer.hpp"
#include "syscall/syscall.hpp"
#include "fs/vfs.hpp"
#include "sif/sif.hpp"
#include "util/util.hpp"
#include "memutil.hpp"
#include "memory.hpp"
#include "gs/gs.hpp"

struct fio_open
{
	s32 mode;
	char name[256];
} STRUCT_ALIGNED;

struct _fio_read_data
{
	u32 size1;
	u32 size2;
	void* dest1;
	void* dest2;
	u8 buf1[16];
	u8 buf2[16];
} STRUCT_ALIGNED;

struct fio_read
{
	int fd;
	void* ptr;
	int size;
	struct _fio_read_data* read_data;
} STRUCT_ALIGNED;

extern "C" {
void kmain_bootstrap(void)
{
	vfs::vfs_init();
	timer::timer_init();
	gs::gs_init();
	gs::vterm_init();
	gs::vterm_hook();
	sif::sif_init();

	asm volatile("ei");

	sif::sif_rpc_client_t* client = new sif::sif_rpc_client_t;
	sif_bind(client, sif::SERVERS::SIF_SERVER_SYS_FILEIO);

	fio_open* open = reinterpret_cast<fio_open*>(_kseg1(new fio_open));
	open->mode = 'C';
	strcpy(open->name, "host:test.txt");

	printk("Calling %p, %p\n", client, &open);
	u32 handle STRUCT_ALIGNED = 10;
	sif_call(client, 0, open, sizeof(*open), &handle, 4);
	printk("opened id: %d\n", handle);

	kfree(open);
	_fio_read_data* read_data = reinterpret_cast<_fio_read_data*>(_kseg1(new _fio_read_data));
	fio_read* read = reinterpret_cast<fio_read*>(_kseg1(new fio_read));
	read->fd = handle;
	read->ptr = (void*)_kseg1(new char[256]);
	read->size = 256;
	read->read_data = (_fio_read_data*)client->client_buf;
	s32* buf = (s32*)_kseg1(kmalloc(256 * 4));
	sif_call(client, 2, read, sizeof(*read), buf, 254 * 4);

	printk("read_data: %s\n", read_data->buf2);
	while (1)
		;
	asm volatile("break");
	while (1)
		;
}
}
