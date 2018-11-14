#ifndef COMM_SHARED_MEMORY_HPP
#define COMM_SHARED_MEMORY_HPP

#include <string>

#include "CommShMemConst.hpp"

namespace bip = boost::interprocess;

class MemoryCoordinator {
    private:
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
        bip::offset_ptr<int> C3_table_ptr;
        bip::offset_ptr<int> achievable_ptr;
        bip::offset_ptr<int> release_hw_ptr;
        bip::offset_ptr<int> done_ptr;

        //EM, Named mutexes to protect the shared memory access
        // Default constructor private, instanciation mandatory...
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
                                            
    public:
        //EM, constructor for the memory Admin OR simple User
        MemoryCoordinator(std::string Role)
        {
            if(Role == "Admin")
            {
                bip::shared_memory_object::remove(NAME_C3_SEGMENT);
                C3_table_segment = bip::managed_shared_memory(bip::create_only 
        						,NAME_C3_SEGMENT    //segment name
        						,PAGE_SIZE_BYTES);  //segment size in bytes, 1 PAGE
                bip::shared_memory_object::remove(NAME_ACHIEVABLE_SEGMENT);
                achievable_segment = bip::managed_shared_memory (bip::create_only 
        						,NAME_ACHIEVABLE_SEGMENT //segment name
        						,PAGE_SIZE_BYTES);       //segment size in bytes, 1 PAGE
                bip::shared_memory_object::remove(NAME_RELEASE_SEGMENT);
                release_hw_segment = bip::managed_shared_memory(bip::create_only 
        						,NAME_RELEASE_SEGMENT //segment name
        						,PAGE_SIZE_BYTES);    //segment size in bytes, 1 PAGE
                bip::shared_memory_object::remove(NAME_DONE_SEGMENT);
                done_segment = bip::managed_shared_memory(bip::create_only 
        						,NAME_DONE_SEGMENT //segment name
        						,PAGE_SIZE_BYTES); //segment size in bytes, 1 PAGE

                boost::interprocess::shared_memory_object::remove(MUTEX_NAME_C3_SEGMENT);
                bip::named_mutex C3_table_mutex{
                                bip::create_only, 
                                MUTEX_NAME_C3_SEGMENT};
                boost::interprocess::shared_memory_object::remove(MUTEX_NAME_ACHIEVABLE_SEGMENT);
                bip::named_mutex achievable_mutex{
                                bip::create_only, 
                                MUTEX_NAME_ACHIEVABLE_SEGMENT};
                boost::interprocess::shared_memory_object::remove(MUTEX_NAME_RELEASE_SEGMENT);
                bip::named_mutex release_hw_mutex{
                                bip::create_only, 
                                MUTEX_NAME_RELEASE_SEGMENT};
                boost::interprocess::shared_memory_object::remove(MUTEX_NAME_DONE_SEGMENT);
                bip::named_mutex done_mutex{
                                bip::create_only, 
                                MUTEX_NAME_DONE_SEGMENT};
            }
            if(Role == "User")
            {
                C3_table_segment = bip::managed_shared_memory(bip::open_only 
        				,NAME_C3_SEGMENT //segment name
                        );
                achievable_segment = bip::managed_shared_memory(bip::open_only 
        				,NAME_ACHIEVABLE_SEGMENT //segment name
                        );
                release_hw_segment = bip::managed_shared_memory(bip::open_only
        				,NAME_RELEASE_SEGMENT //segment name
        				);
                done_segment = bip::managed_shared_memory(bip::open_only 
        				,NAME_DONE_SEGMENT //segment name
        				);

                bip::named_mutex C3_table_mutex{
                                bip::open_only, 
                                MUTEX_NAME_C3_SEGMENT};
                bip::named_mutex achievable_mutex{
                                bip::open_only, 
                                MUTEX_NAME_ACHIEVABLE_SEGMENT};
                bip::named_mutex release_hw_mutex{
                                bip::open_only, 
                                MUTEX_NAME_RELEASE_SEGMENT};
                bip::named_mutex done_mutex{
                                bip::open_only, 
                                MUTEX_NAME_DONE_SEGMENT};
            }

            //Initialize shared memory STL-compatible allocator
            const ShmemAllocator alloc_inst_C3_table     (C3_table_segment.get_segment_manager());
            const ShmemAllocator alloc_inst_achievable   (achievable_segment.get_segment_manager());
            const ShmemAllocator alloc_inst_release_hw   (release_hw_segment.get_segment_manager());
            const ShmemAllocator alloc_inst_done         (done_segment.get_segment_manager());

            C3_table_Vptr   = C3_table_segment.construct<SharedVector>(VECTOR_NAME_C3_SEGMENT)
                        					       (alloc_inst_C3_table);//first ctor parameter
            achievable_Vptr = achievable_segment.construct<SharedVector>(VECTOR_NAME_ACHIEVABLE_SEGMENT)
                        					       (alloc_inst_achievable);//first ctor parameter
            release_hw_Vptr = release_hw_segment.construct<SharedVector>(VECTOR_NAME_RELEASE_SEGMENT)
                        					       (alloc_inst_release_hw);//first ctor parameter
            done_Vptr       = done_segment.construct<SharedVector>(VECTOR_NAME_DONE_SEGMENT)
                        					       (alloc_inst_done);//first ctor parameter                                                                                                      

            C3_table_ptr     = C3_table_Vptr->data();
            achievable_ptr   = achievable_Vptr->data();
            release_hw_ptr   = release_hw_Vptr->data();
            done_ptr         = done_Vptr->data();
        }


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