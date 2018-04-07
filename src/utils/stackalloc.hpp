#pragma once

#include <vector>

namespace Utils{
	template < size_t _BlockSize>
	class StackAllocator
	{
	public:
		StackAllocator() 
		{
			newBlock();
			bp = 0;
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

		//throws away extra blocks and resets to the default state
		void reset()
		{
			for (size_t i = 1; i < memBlocks.size(); ++i)
				free(memBlocks[i]);

			memBlocks.resize(1);
			bp = 0;
		}
	private:
		void newBlock()
		{
			memBlocks.push_back(malloc(_BlockSize));
			bp++;
			sp = (char*)memBlocks.back();
		}

		char* sp; // stack pointer
		size_t bp; //block pointer
		std::vector< void* > memBlocks;
	};
}