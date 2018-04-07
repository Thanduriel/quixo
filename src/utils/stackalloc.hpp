#pragma once

#include <vector>

namespace Utils{
	template < size_t _BlockSize>
	class StackAllocator
	{
	public:
		StackAllocator() 
			: bp(0),
			memBlocks(1, malloc(_BlockSize))
		{
		}
		~StackAllocator()
		{
			for (auto block : memBlocks)
				free(block);
		}

		template< typename _T, typename... _Args>
		_T* construct(_Args&&... _args)
		{
			//check whether enough space is available
			if (sp - (char*)memBlocks[bp] + sizeof(_T) > _BlockSize) newBlock();

			//call constructor
			auto spBeg = sp;
			// increase before in case that the constructor allocates a buffer with this allocator 
			sp += sizeof(_T);
			auto ptr = new(spBeg)(_T)(std::forward< _Args >(_args)...);
			return ptr;
		}

		//allocate a block with the given amounts of bytes
		void* alloc(size_t _bytes)
		{
			if (sp - (char*)memBlocks[bp] + _bytes > _BlockSize) newBlock();
			
			auto ptr = sp;
			sp += _bytes;

			return ptr;
		}

		//resets state to overwrite current data
		void reset()
		{
			bp = 0;
		}
	private:
		void newBlock()
		{
			++bp;
			if(bp >= memBlocks.size())
				memBlocks.push_back(malloc(_BlockSize));
			sp = (char*)memBlocks[bp];
		}

		char* sp; // stack pointer
		size_t bp; //block pointer
		std::vector< void* > memBlocks;
	};
}