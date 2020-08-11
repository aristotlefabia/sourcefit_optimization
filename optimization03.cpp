

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

typedef int DATA_TYPE;


#define OPTIMIZED


/* Project : OMT (NEC Telecom Software Philippines Inc.) */

/*
 * frequent free and new calls has a cost which has an effect on performance. For this optimization a global allocator was 
 * implemented to prevent frequent new and free call
 */

const int INITIAL_SIZE = 1000000;

#ifdef OPTIMIZED

class GlobalAllocator {

    GlobalAllocator() {

		allocated_memory = new DATA_TYPE[INITIAL_SIZE];
		allocated_size = INITIAL_SIZE;
		current_index = 0;
	}
public:

	DATA_TYPE* allocated_memory;
	long allocated_size;
	long current_index;

	DATA_TYPE* allocate(long size)
	{
		if (current_index + size >= allocated_size){
			//cout << "reallocation" << endl;
			DATA_TYPE* new_allocation = new DATA_TYPE[allocated_size + INITIAL_SIZE];
			memcpy(new_allocation, allocated_memory, allocated_size);
			allocated_size += INITIAL_SIZE;
			delete[] allocated_memory;
			allocated_memory = new_allocation;

		}
		current_index += size;
		return &allocated_memory[current_index];
	}

	void deallocate(long size)
	{
		current_index -= size;
	}

	void cleanup()
	{
	}

	static GlobalAllocator* allocator() {
		static GlobalAllocator g_allocator;
		return &g_allocator;
	}

	~GlobalAllocator()
	{
		cout << "~GlobalAllocator " << endl;
		delete[] allocated_memory;
	}

};

#endif


void optimize_test03()
{
	DWORD dwStartTime = GetTickCount();

	int range[] = { INITIAL_SIZE, INITIAL_SIZE, INITIAL_SIZE, 30000,20000,50, 100, 77, 156, 1024 };

	for (int i = 0; i < 60000; i++) {
#ifdef OPTIMIZED
		DATA_TYPE* b = GlobalAllocator::allocator()->allocate(range[i % 9]);
		GlobalAllocator::allocator()->deallocate(range[i % 9]);
#else
		DATA_TYPE* b = new DATA_TYPE[range[i%9]];
	    delete[] b;
#endif
	}
	DWORD dwFinishTime = GetTickCount();
	cout << "duration now is ms: " << dwFinishTime - dwStartTime << endl;
}