// SPDX-License-Identifier: MPL-2.0

#include "memory.hpp"
#include "memutil.hpp"
#include "debug/debug.hpp"

extern u8 __heap_start;
extern u8 __heap_end;

struct block_t
{
	size_t size;
	block_t* next;
	u64 : 52;
	bool free;
} __attribute((packed));

// heh this is garbage
static_assert(sizeof(block_t) == 16, "block_t is not 16 bytes");

block_t* last_block = nullptr;
static u8* heap_ptr = &__heap_start;

// Attempt number 2

void* kmalloc(size_t size)
{
	// See if we can find a free existing block
	while (last_block != nullptr)
	{
		if (last_block->free && last_block->size >= size)
		{
			last_block->free = false;
			return (void*)(last_block + 1);
		}
		last_block = last_block->next;
	}

	// We don't have a free block, make a new one from the heap

	// Check if we are OOM
	if ((ee_ptr_t)(heap_ptr + size + sizeof(block_t)) >= (ee_ptr_t)_kseg1(&__heap_end))
	{
		printe("OUT OF MEMORY\n");
		return nullptr;
	}

	block_t* heap_allocated_block = (block_t*)_kseg1(heap_ptr);
	heap_allocated_block->size = size;
	heap_allocated_block->next = nullptr;
	heap_allocated_block->free = false;
	last_block = heap_allocated_block;

	heap_ptr += size + sizeof(block_t);

	return (void*)(heap_allocated_block + 1);
}

void kfree(void* ptr)
{
	block_t* block = (block_t*)_kseg1(((block_t*)ptr) - 1);
	block->free = true;
	if (block->next == nullptr)
	{
		heap_ptr -= block->size + sizeof(block_t);
		last_block = nullptr;
	}
	last_block = block;
}

void* operator new(size_t size)
{
	return kmalloc(size);
}

void* operator new(size_t size, std::align_val_t align)
{
	//printk("new with alignment of %u\n", (size_t)align);
	return kmalloc(size);
}


void* operator new[](size_t size, std::align_val_t align)
{
	//printk("new[] with alignment of %u\n", (size_t)align);
	return kmalloc(size);
}

void* operator new[](size_t size)
{
	return kmalloc(size);
}

void operator delete(void* ptr)
{
	kfree(ptr);
}

void operator delete(void* ptr, size_t size)
{
	kfree(ptr);
}

void operator delete(void* ptr, std::align_val_t align)
{
	kfree(ptr);
}

void operator delete[](void* ptr)
{
	kfree(ptr);
}

void operator delete[](void* ptr, size_t size)
{
	//printi("operator delete[] with ptr %p and size %x\n", ptr, size);
	kfree(ptr);
}

void operator delete[](void* ptr, std::align_val_t align)
{
	//printi("operator delete[] with ptr %p and align %x\n", ptr, (size_t)align);
	kfree(ptr);
}
