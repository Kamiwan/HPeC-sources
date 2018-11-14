#ifndef COMM_SHMEM_CONST_HPP
#define COMM_SHMEM_CONST_HPP

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
typedef std::vector<int, ShmemAllocator> SharedVector;

#define NAME_C3_SEGMENT           "C3_table_segment"
#define NAME_ACHIEVABLE_SEGMENT   "Achievable_segment"
#define NAME_RELEASE_SEGMENT      "Release_hw_segment"
#define NAME_DONE_SEGMENT         "Done_segment"

#define VECTOR_NAME_C3_SEGMENT           "Vector_C3_table_segment"
#define VECTOR_NAME_ACHIEVABLE_SEGMENT   "Vector_Achievable_segment"
#define VECTOR_NAME_RELEASE_SEGMENT      "Vector_Release_hw_segment"
#define VECTOR_NAME_DONE_SEGMENT         "Vector_Done_segment"

#define MUTEX_NAME_C3_SEGMENT           "Mutex_C3_table_segment"
#define MUTEX_NAME_ACHIEVABLE_SEGMENT   "Mutex_Achievable_segment"
#define MUTEX_NAME_RELEASE_SEGMENT      "Mutex_Release_hw_segment"
#define MUTEX_NAME_DONE_SEGMENT         "Mutex_Done_segment"

#define PAGE_SIZE_BYTES      4096


#endif