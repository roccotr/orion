#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <unistd.h>

using namespace boost::interprocess;

int main()
{
	srand (time(NULL));

	shared_memory_object shdmem(open_or_create, "Boost", read_write);
	shdmem.truncate(1024);
	mapped_region region(shdmem, read_write);
	int *i1 = static_cast<int*>(region.get_address());
	for (int i=0; i<1000; i++) {
		*i1 = rand() % 10 + 1;
		std::cout << "Write " << *i1 << " to shared memory" << '\n';
		sleep(1);
	}
}
