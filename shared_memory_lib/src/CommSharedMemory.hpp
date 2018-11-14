#ifndef COMM_SHARED_MEMORY_HPP
#define COMM_SHARED_MEMORY_HPP

#include <string>
#include <iostream>

#include "CommShMemConst.hpp"

namespace bip = boost::interprocess;

class MemoryCoordinator {
    public:
        //EM, Segments to create or open access to the shared memory
        bip::managed_shared_memory C3_table_segment;
        bip::managed_shared_memory achievable_segment;
        bip::managed_shared_memory release_hw_segment;
        bip::managed_shared_memory done_segment;

        //EM, Offset Pointers to access the shared vector
        bip::offset_ptr<SharedVector> C3_table_Vptr;
        bip::offset_ptr<SharedVector> achievable_Vptr;
        bip::offset_ptr<SharedVector> release_hw_Vptr;
        bip::offset_ptr<SharedVector> done_Vptr;

        //EM, Offset Pointers to USE easily the shared memory
        //Use them ONLY when vectors are full of data
        bip::offset_ptr<int> C3_table_ptr;
        bip::offset_ptr<int> achievable_ptr;
        bip::offset_ptr<int> release_hw_ptr;
        bip::offset_ptr<int> done_ptr;

        //EM, Named mutexes to protect the shared memory access
        //The default constructor is private, instanciation mandatory...
        bip::named_mutex C3_table_mutex{
				    	bip::open_or_create
				    	, MUTEX_NAME_C3_SEGMENT};
        bip::named_mutex achievable_mutex{
						bip::open_or_create
						, MUTEX_NAME_ACHIEVABLE_SEGMENT};
        bip::named_mutex release_hw_mutex{
						bip::open_or_create
						, MUTEX_NAME_RELEASE_SEGMENT};
        bip::named_mutex done_mutex{
						bip::open_or_create
						, MUTEX_NAME_DONE_SEGMENT};                                    
                                            
        //EM, constructor for the memory Admin OR simple User
        MemoryCoordinator(std::string Role);

    private:

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