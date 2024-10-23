// SPDX-License-Identifier: MPL-2.0

#include "intc/intc.hpp"
#include "memutil.hpp"
#include "mmio/intc.hpp"
#include "mmio/dma.hpp"

struct handler_t
{
	intc::handler_fun_t handler;
	handler_t* next;
};

static handler_t* handlers[intc::CAUSE_CNT] = {};

static handler_t* dmac_handlers[intc::DMAC_CAUSE_CNT] = {};

s32 intc::register_handler(CAUSE cause, handler_fun_t handler, bool first)
{
	handler_t* new_handler = new handler_t{handler, handlers[cause]};

	handler_t* current = handlers[cause];
	if (current == nullptr)
	{
		handlers[cause] = new_handler;
		return 0;
	}

	if (first)
	{
		handler_t* current_head = handlers[cause];
		new_handler->next = current_head;

		handlers[cause] = new_handler;
		return 0;
	}
	else
	{
		for (u32 i = 0; i < MAX_HANDLERS_PER_INTERRUPT; i++)
		{
			if (current->next == nullptr)
			{
				current->next = new_handler;
				return i + 1;
			}
			current = current->next;
		}
	}
	printe("Too many handlers for interrupt %d\n", cause);
	return -1;
}

s32 intc::register_dmac_handler(DMAC_CAUSE cause, handler_fun_t handler, bool first)
{
	handler_t* new_handler = new handler_t{handler, handlers[cause]};

	handler_t* current = dmac_handlers[cause];
	if (current == nullptr)
	{
		dmac_handlers[cause] = new_handler;
		return 0;
	}

	if (first)
	{
		handler_t* current_head = dmac_handlers[cause];
		new_handler->next = current_head;

		dmac_handlers[cause] = new_handler;
		return 0;
	}
	else
	{
		for (u32 i = 0; i < MAX_HANDLERS_PER_INTERRUPT; i++)
		{
			if (current->next == nullptr)
			{
				current->next = new_handler;
				return i + 1;
			}
			current = current->next;
		}
	}
	printe("Too many dmac handlers for interrupt %d\n", cause);
	return -1;
}

s32 intc::unregister_handler(CAUSE cause, s32 handler_id)
{
	if (handler_id == 0)
	{
		handler_t* head = handlers[cause];
		handlers[cause] = handlers[cause]->next;
		delete head;
		return 0;
	}

	handler_t* current = handlers[cause];
	for (s32 i = 0; i < handler_id - 1; i++)
	{
		if (current->next == nullptr)
		{
			printe("Handler %d not found for interrupt %d\n", handler_id, cause);
			return -1;
		}
		current = current->next;
	}

	handler_t* to_delete = current->next;
	current->next = current->next->next;
	delete to_delete;

	return 0;
}

s32 intc::unregister_dmac_handler(DMAC_CAUSE cause, s32 handler_id)
{
	if (handler_id == 0)
	{
		handler_t* head = dmac_handlers[cause];
		dmac_handlers[cause] = dmac_handlers[cause]->next;
		delete head;
		return 0;
	}

	handler_t* current = dmac_handlers[cause];
	for (s32 i = 0; i < handler_id - 1; i++)
	{
		if (current->next == nullptr)
		{
			printe("DMAC handler %d not found for interrupt %d\n", handler_id, cause);
			return -1;
		}
		current = current->next;
	}

	handler_t* to_delete = current->next;
	current->next = current->next->next;
	delete to_delete;

	return 0;
}

void intc::handle_interrupt(CAUSE cause)
{
	handler_t* current = handlers[cause];
	while (current != nullptr)
	{
		current->handler(cause);
		current = current->next;
	}
}

void intc::handle_dmac_interrupt(DMAC_CAUSE cause)
{
	handler_t* current = dmac_handlers[cause];
	while (current != nullptr)
	{
		current->handler(cause);
		current = current->next;
	}
}

void intc::enable_interrupt(CAUSE cause)
{
	// bits 0->15 are the interrupt masks in I_MASK
	_s32(I_MASK, (_l32(I_MASK) ^ (1 << cause)) & (1 << cause));
	_s32(I_STAT, 1 << cause);
}

void intc::enable_dmac_interrupt(DMAC_CAUSE cause)
{
	// bits 16->25 are the interrupt masks in D_STAT
	_s32(D_STAT, ((_l32(D_STAT) ^ (1 << (cause + 16))) & (1 << (cause + 16))) | (1 << cause));
}

void intc::disable_interrupt(CAUSE cause)
{
	_s32(I_MASK, _l32(I_MASK) & (1 << cause));
}

void intc::disable_dmac_interrupt(DMAC_CAUSE cause)
{
	_s32(D_STAT, _l32(D_STAT) & (1 << (cause + 16)));
}
