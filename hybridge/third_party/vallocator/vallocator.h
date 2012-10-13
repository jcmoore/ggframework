//
//  vallocator.h
//  hybridge
//
//  Created by The Narrator on 9/4/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

// source
// vladimir@adacel.com.au
// http://www.drdobbs.com/a-quick-and-simple-memory-allocator/184403440?pgno=1

#ifndef INC_VALLOCATOR
#define INC_VALLOCATOR

// this seems to be a good instance allocator
// but I might also need a contiguous pointer array allocator (or I could just use stl...)

class MemoryAllocator
{
public:
	typedef unsigned int uint;
	
public:
    ~MemoryAllocator ();
    MemoryAllocator (uint unit_size, uint block_size =16); 
    MemoryAllocator (uint unit_size, uint block_size, bool debug); 
    void* allocate ();
    void deallocate (void*);
    bool  is_debug () const
	{ return _debug; }
//developer modified
//private:
protected:
    struct Free
	{
        Free* next;
	};
private:
    uint _requested_size;    // memory units requested for allocation
    uint _allocated_size;    // memory units to be allocated
    uint _block_size;        // The number of units in blocks 
    void** _allocated_blocks;    // Array of pointers to blocks
    uint _num_allocated_blocks;  // Number of allocated blocks 
    Free* _free;                 // Linked list of free units
    bool _debug;
	
    void more() throw(char*);
    void* debug_check(void*&) throw(char*); 
    void* debug_correct(void*&);
	
	template <typename T>
	friend class TypeAllocator;
};

inline
void*
MemoryAllocator::allocate()
{
    if (!_free)
        more();
    void* storage = _free;
    _free = _free->next;
    return _debug ? debug_correct(storage) : storage;
}

inline
void
MemoryAllocator::deallocate(void* storage) 
{
    if (_debug)
        debug_check(storage);
    Free* new_free = (Free*) storage;
    new_free->next = _free;
    _free = new_free;
}

template<class T>
class TypeAllocator : public MemoryAllocator
{
public:
	TypeAllocator(bool debug = false, uint block_size =16)
    : MemoryAllocator(sizeof(T), block_size, debug)
    {}
	
// developer modified -- added "this->" to all instance member references (required for compiling)
#if 0 // optional automated destructor invocation
	~TypeAllocator()
	{
		for (int k = 0;
			 k < this->_num_allocated_blocks;
			 ++k)
		{
			void* the_block =
			this->_allocated_blocks[k];
			for (int n = 0;
				 n < this->_block_size; ++n)
			{
				void* the_unit =
				(char*) the_block + n *
				this->_allocated_size;
				for (Free* free_unit = this->_free;
					 free_unit;
					 free_unit = free_unit->next)
				{
					if (free_unit == the_unit)
						// The unit has already
						// been freed
						break;
				}
				// developer modified -- typo?
				//if (!free_unit)
				if (!the_unit)
					// is still in use
					((T*) the_unit)->~T();
			}
		}
	}
#endif	
};

// implementation example

//class Foo
//{
//public:    // the Foo specific stuff
//	
//	void* operator new (size_t)
//    { return _allocator.allocate(); }
//	void operator delete (void* storage)
//    {_allocator.deallocate(storage); }
//	
//private:    // more of the Foo stuff
//	static TypeAllocator<Foo> _allocator;
//};
//
//TypeAllocator<Foo> Foo::_allocator;

#endif
