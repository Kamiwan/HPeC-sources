#ifndef COMM_SHARED_MEMORY_H
#define COMM_SHARED_MEMORY_H

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

//Alias an STL compatible allocator of ints that allocates ints from the managed
//shared memory segment.  This allocator will allow to place containers
//in managed shared memory segments
typedef boost::interprocess::allocator<int, boost::interprocess::managed_shared_memory::segment_manager> ShmemAllocator;
//Alias a vector that uses the previous STL-like allocator
typedef std::vector<int, ShmemAllocator> MyVector;


class MemoryCoordinator {
    private:

    //Construct a shared memory
    boost::interprocess::offset_ptr<MyVector> C3_table_ptr;
    boost::interprocess::offset_ptr<MyVector> achievable_ptr;
    boost::interprocess::offset_ptr<MyVector> release_hw_ptr;
    boost::interprocess::offset_ptr<MyVector> done_ptr;

    boost::interprocess::offset_ptr<int> ptr;




    public:

};


//EM, TODO: DELETE IT
class Box {
    private:
        double length;   // Length of a box
        double breadth;  // Breadth of a box
        double height;   // Height of a box
        
    public:
        Box(int a, int b, int c)
        {
            length  = a;
            breadth = b;
            height  = c;
        }
        void print();
};


#endif