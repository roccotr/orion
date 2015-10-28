#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <unistd.h>

using namespace boost::interprocess;

int main()
{
	try {
		shared_memory_object shdmem(open_or_create, "Boost", read_only);
		mapped_region region(shdmem, read_only);
		for (int i=0; i<1000; i++) {
			int *i2 = static_cast<int*>(region.get_address());
			std::cout << "Read " << *i2 << " from shared memory" << '\n';
			sleep(1);
		}
	} catch (std::exception const& e) {
		std::cout << "are you sure the shared_writer is working ?" << '\n';
	}
}
