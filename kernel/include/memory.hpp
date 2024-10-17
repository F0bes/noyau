// SPDX-License-Identifier: MPL-2.0

#pragma once

#include <type_traits>

void* kmalloc(size_t size);

void kfree(void* ptr);
