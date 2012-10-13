//
//  vallocator.cpp
//  hybridge
//
//  Created by The Narrator on 9/4/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "vallocator.h"

#include <string.h>

#define TRAILING_ 0x55555555
#define HEADING_ 0x55555555

MemoryAllocator::MemoryAllocator(uint unit_size, uint block_size)
// developer modified
//: _allocated_size (max(unit_size, sizeof(Free))), 
: _requested_size(unit_size),
_allocated_size (unit_size > sizeof(Free) ? unit_size : sizeof(Free)), 
_block_size(block_size),
_allocated_blocks(0),
_num_allocated_blocks(0),
_free(0),
_debug (false)
{}

MemoryAllocator::MemoryAllocator(uint unit_size, uint block_size,
								 bool debug)
// developer modified
//: _allocated_size (max(unit_size, sizeof(Free))), 
: _requested_size(unit_size),
_allocated_size (unit_size > sizeof(Free) ? unit_size : sizeof(Free)), 
_block_size(block_size),
_allocated_blocks(0),
_num_allocated_blocks(0),
_free(0),
_debug(debug)
{
    if (_debug)
        _allocated_size += 2 * sizeof(int);
}

MemoryAllocator::~MemoryAllocator()
{
    for (uint k = 0; k < _num_allocated_blocks; ++k)
	{
        ::operator delete(_allocated_blocks[k]); 
	}
    ::operator delete(_allocated_blocks); 
}

void
MemoryAllocator::more() throw(char*)
{
    Free* new_block = (Free*)
	::operator new(_allocated_size *_block_size);
    void** new_blocks = (void**) ::operator new(sizeof(void*)
												* (_num_allocated_blocks + 1));
    int last_element = _block_size - 1;
    if (!new_block || !new_blocks)
        throw("Memory allocation failed.");
    if (_allocated_blocks)
	{
        memcpy(new_blocks, _allocated_blocks, sizeof(void*)
			   * _num_allocated_blocks);
        ::operator delete(_allocated_blocks); 
	}
    _allocated_blocks = new_blocks;
    _allocated_blocks[_num_allocated_blocks++] = new_block; 
    _free = new_block;
	
    for (int k = 0; k<last_element; ++k, new_block = new_block->next)
	{
        new_block->next = (Free*) ((char*) new_block
								   + _allocated_size);
	}
	
    new_block->next = 0;
}

void*
MemoryAllocator::debug_correct(void*& storage)
{
    *(int*) storage = HEADING_;
    storage = (int*) storage + 1;
    *(int*) ((char*) storage + _requested_size) = TRAILING_; 
    return storage;
}

void*
MemoryAllocator::debug_check(void*& storage) throw(char*)
{
    int* tail = (int*) ((char*) storage + _requested_size); 
    int* head = (int*) (storage = (int*) storage - 1);
	
    if (*tail != TRAILING_)
        throw("Block tail has been overrun.");
    if (*head !=  HEADING_)
        throw("Block header has been overrun.");
    return storage;
}
