#include "CommSharedMemory.hpp"


MemoryCoordinator::MemoryCoordinator(std::string Role)
{
    if(Role == "Admin")
    {
        //std::cout << "Memory space reservation" << std::endl;
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

        //std::cout << "Mutex step" << std::endl;
        bip::shared_memory_object::remove(MUTEX_NAME_C3_SEGMENT);
        bip::named_mutex C3_table_mutex{
                        bip::open_or_create, 
                        MUTEX_NAME_C3_SEGMENT};
        bip::shared_memory_object::remove(MUTEX_NAME_ACHIEVABLE_SEGMENT);
        bip::named_mutex achievable_mutex{
                        bip::open_or_create, 
                        MUTEX_NAME_ACHIEVABLE_SEGMENT};
        bip::shared_memory_object::remove(MUTEX_NAME_RELEASE_SEGMENT);
        bip::named_mutex release_hw_mutex{
                        bip::open_or_create, 
                        MUTEX_NAME_RELEASE_SEGMENT};
        bip::shared_memory_object::remove(MUTEX_NAME_DONE_SEGMENT);
        bip::named_mutex done_mutex{
                        bip::open_or_create, 
                        MUTEX_NAME_DONE_SEGMENT};

        //Initialize shared memory STL-compatible allocator
        const ShmemAllocator alloc_inst_C3_table     (C3_table_segment.get_segment_manager());
        const ShmemAllocator alloc_inst_achievable   (achievable_segment.get_segment_manager());
        const ShmemAllocator alloc_inst_release_hw   (release_hw_segment.get_segment_manager());
        const ShmemAllocator alloc_inst_done         (done_segment.get_segment_manager());

        //std::cout << "Vector ptr step" << std::endl;
        C3_table_Vptr   = C3_table_segment.construct<SharedVector>(VECTOR_NAME_C3_SEGMENT)
                                            (alloc_inst_C3_table);//first ctor parameter
        achievable_Vptr = achievable_segment.construct<SharedVector>(VECTOR_NAME_ACHIEVABLE_SEGMENT)
                                    (alloc_inst_achievable);//first ctor parameter
        release_hw_Vptr = release_hw_segment.construct<SharedVector>(VECTOR_NAME_RELEASE_SEGMENT)
                                            (alloc_inst_release_hw);//first ctor parameter
        done_Vptr       = done_segment.construct<SharedVector>(VECTOR_NAME_DONE_SEGMENT)
                                                (alloc_inst_done);//first ctor parameter                                
    }
    else 
    {   if(Role == "User")
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
            
            //std::cout << "Mutex step" << std::endl;
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

            //std::cout << "Vector step" << std::endl;
            C3_table_Vptr   = C3_table_segment.find<SharedVector>(VECTOR_NAME_C3_SEGMENT).first;
            achievable_Vptr = achievable_segment.find<SharedVector>(VECTOR_NAME_ACHIEVABLE_SEGMENT).first;
            release_hw_Vptr = release_hw_segment.find<SharedVector>(VECTOR_NAME_RELEASE_SEGMENT).first;
            done_Vptr       = done_segment.find<SharedVector>(VECTOR_NAME_DONE_SEGMENT).first;

            //std::cout << "Int Pointer step" << std::endl;
            C3_table_ptr     = C3_table_Vptr->data();
            achievable_ptr   = achievable_Vptr->data();
            release_hw_ptr   = release_hw_Vptr->data();
            done_ptr         = done_Vptr->data();
        }
        else
        {
            std::cerr << "Wrong argument to construct Memory Coordinator" << std::endl;
        }
    }
}



//EM, TODO: DELETE IT
void Box::print()
{
    std::cout << "length    = " << length << std::endl;
    std::cout << "breadth   = " << breadth << std::endl;
    std::cout << "height    = " << height << std::endl;
}