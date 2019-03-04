/* 
 * This file is part of the HPeC distribution (https://github.com/Kamiwan/HPeC-sources).
 * Copyright (c) 2018 Lab-STICC Laboratory.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
/*************************************************************************************
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on: November 12, 2018
 * 
 * MemoryCoordinator class definition.
 * Allow HPeC ROS nodes to communicate using shared memory, the fastest communication.
 * Boost InterProcess library used.
 * 
 * When you use this class to enable IPC, one of your target application must
 * create an object of this class as an "Admin" with the main constructor. Then, this
 * "Admin" must instantiate shared memories before use them by other applications.
 * IPC = Inter Process Communication
 *************************************************************************************/

#ifndef COMM_SHARED_MEMORY_HPP
#define COMM_SHARED_MEMORY_HPP

#include "comm_shmem_const.hpp"

namespace bip = boost::interprocess;

class MemoryCoordinator {
    public:
        /* Fill_ShMem_...
        *  Copy a vector of int in the dedicated shared memory
        *  @param memory the data vector to record
        *  /!\ Use these functions at least once with the Admin (see constructor)
        *      in order to use the shared memory later
        *  /!\ busy_tile must only be an array (vector) of 3 elements because we
        *      have 3 tiles
        */
        void    Fill_ShMem_C3_Table(const std::vector<int> &memory);
        void    Fill_ShMem_Achievable(const std::vector<int> &memory);
        void    Fill_ShMem_Release_HW(const std::vector<int> &memory);
        void    Fill_ShMem_Done(const std::vector<int> &memory);
        void    Fill_ShMem_Busy_Tile(const std::vector<int> &memory);

        /* ..._Read/Write
         * Read or write a single data in the dedicated shared memory
         * @param app_index    the number associated to the app
         * @param data         the data to write (Write)
         * @return             the data stored (Read)
         * 
         * (i) For busy_tile, tile_index must not exceed the number of tiles
        */
        int     Achievable_Read(int app_index);
        void    Achievable_Write(int data, int app_index);
        int     Release_HW_Read(int app_index);
        void    Release_HW_Write(int data, int app_index);
        int     Done_Read(int app_index);
        void    Done_Write(int data, int app_index);
        int     Busy_Tile_Read(int tile_index);
        void    Busy_Tile_Write(int data, int tile_index);

        /*C3 contains several parameters per app so Read and Write use an array
          (vector) instead of a single int
        */
        std::vector<int>    C3_Table_Read(int app_index);
        void                C3_Table_Write(const std::vector<int> &data, int app_index);

        //Useful single data Read/Write functions for C3 table 
        void    Update_ExecTime(int data, int app_index);
        void    Update_QoS(int data, int app_index);
        int     Read_ExecTime(int app_index);
        int     Read_QoS(int app_index);

        //Main Class Constructor
        //!\ Role must be "Admin" OR "User"
        MemoryCoordinator(const std::string &Role)
        {
            if(Role == "Admin")
            {
                //std::cout << "Memory space reservation" << std::endl;
                bip::shared_memory_object::remove(NAME_C3_SEGMENT);
                C3_table_segment_ = bip::managed_shared_memory(bip::create_only 
                                ,NAME_C3_SEGMENT    //segment name
                                ,PAGE_SIZE_BYTES);  //segment size in bytes, 1 PAGE
                bip::shared_memory_object::remove(NAME_ACHIEVABLE_SEGMENT);
                achievable_segment_ = bip::managed_shared_memory (bip::create_only 
                                ,NAME_ACHIEVABLE_SEGMENT //segment name
                                ,PAGE_SIZE_BYTES);       //segment size in bytes, 1 PAGE
                bip::shared_memory_object::remove(NAME_RELEASE_SEGMENT);
                release_hw_segment_ = bip::managed_shared_memory(bip::create_only 
                                ,NAME_RELEASE_SEGMENT //segment name
                                ,PAGE_SIZE_BYTES);    //segment size in bytes, 1 PAGE
                bip::shared_memory_object::remove(NAME_DONE_SEGMENT);
                done_segment_ = bip::managed_shared_memory(bip::create_only 
                                ,NAME_DONE_SEGMENT //segment name
                                ,PAGE_SIZE_BYTES); //segment size in bytes, 1 PAGE
                bip::shared_memory_object::remove(NAME_BUSY_TILE_SEGMENT);
                busy_tile_segment_ = bip::managed_shared_memory(bip::create_only 
                                ,NAME_BUSY_TILE_SEGMENT //segment name
                                ,PAGE_SIZE_BYTES); //segment size in bytes, 1 PAGE

                //std::cout << "Mutex step" << std::endl;
                bip::shared_memory_object::remove(MUTEX_NAME_C3_SEGMENT);
                bip::named_mutex C3_table_mutex_{
                                bip::open_or_create, 
                                MUTEX_NAME_C3_SEGMENT};
                bip::shared_memory_object::remove(MUTEX_NAME_ACHIEVABLE_SEGMENT);
                bip::named_mutex achievable_mutex_{
                                bip::open_or_create, 
                                MUTEX_NAME_ACHIEVABLE_SEGMENT};
                bip::shared_memory_object::remove(MUTEX_NAME_RELEASE_SEGMENT);
                bip::named_mutex release_hw_mutex_{
                                bip::open_or_create, 
                                MUTEX_NAME_RELEASE_SEGMENT};
                bip::shared_memory_object::remove(MUTEX_NAME_DONE_SEGMENT);
                bip::named_mutex done_mutex_{
                                bip::open_or_create, 
                                MUTEX_NAME_DONE_SEGMENT};
                bip::shared_memory_object::remove(MUTEX_NAME_BUSY_TILE_SEGMENT);
                bip::named_mutex busy_tile_mutex_{
                                bip::open_or_create, 
                                MUTEX_NAME_BUSY_TILE_SEGMENT};

                //Initialize shared memory STL-compatible allocator
                const ShmemAllocator alloc_inst_C3_table     (C3_table_segment_.get_segment_manager());
                const ShmemAllocator alloc_inst_achievable   (achievable_segment_.get_segment_manager());
                const ShmemAllocator alloc_inst_release_hw   (release_hw_segment_.get_segment_manager());
                const ShmemAllocator alloc_inst_done         (done_segment_.get_segment_manager());
                const ShmemAllocator alloc_inst_busy_tile    (busy_tile_segment_.get_segment_manager());

                //std::cout << "Vector ptr step" << std::endl;
                C3_table_Vptr_   = C3_table_segment_.construct<SharedVector>(VECTOR_NAME_C3_SEGMENT)
                                                    (alloc_inst_C3_table);//first ctor parameter
                achievable_Vptr_ = achievable_segment_.construct<SharedVector>(VECTOR_NAME_ACHIEVABLE_SEGMENT)
                                            (alloc_inst_achievable);//first ctor parameter
                release_hw_Vptr_ = release_hw_segment_.construct<SharedVector>(VECTOR_NAME_RELEASE_SEGMENT)
                                                    (alloc_inst_release_hw);//first ctor parameter
                done_Vptr_       = done_segment_.construct<SharedVector>(VECTOR_NAME_DONE_SEGMENT)
                                                        (alloc_inst_done);//first ctor parameter 
                busy_tile_Vptr_  = busy_tile_segment_.construct<SharedVector>(VECTOR_NAME_BUSY_TILE_SEGMENT)
                                                        (alloc_inst_busy_tile);//first ctor parameter                               
            }
            else 
            {   if(Role == "User")
                {
                    C3_table_segment_ = bip::managed_shared_memory(bip::open_only 
                            ,NAME_C3_SEGMENT //segment name
                            );
                    achievable_segment_ = bip::managed_shared_memory(bip::open_only 
                            ,NAME_ACHIEVABLE_SEGMENT //segment name
                            );
                    release_hw_segment_ = bip::managed_shared_memory(bip::open_only
                            ,NAME_RELEASE_SEGMENT //segment name
                            );
                    done_segment_ = bip::managed_shared_memory(bip::open_only 
                        ,NAME_DONE_SEGMENT //segment name
                        );
                    busy_tile_segment_ = bip::managed_shared_memory(bip::open_only 
                        ,NAME_BUSY_TILE_SEGMENT //segment name
                        );
                    
                    //std::cout << "Mutex step" << std::endl;
                    bip::named_mutex C3_table_mutex_{
                                    bip::open_only, 
                                    MUTEX_NAME_C3_SEGMENT};
                    bip::named_mutex achievable_mutex_{
                                    bip::open_only, 
                                    MUTEX_NAME_ACHIEVABLE_SEGMENT};
                    bip::named_mutex release_hw_mutex_{
                                    bip::open_only, 
                                    MUTEX_NAME_RELEASE_SEGMENT};
                    bip::named_mutex done_mutex_{
                                    bip::open_only, 
                                    MUTEX_NAME_DONE_SEGMENT};
                    bip::named_mutex busy_tile_mutex_{
                                    bip::open_only, 
                                    MUTEX_NAME_BUSY_TILE_SEGMENT};

                    //std::cout << "Vector step" << std::endl;
                    C3_table_Vptr_   = C3_table_segment_.find<SharedVector>(VECTOR_NAME_C3_SEGMENT).first;
                    achievable_Vptr_ = achievable_segment_.find<SharedVector>(VECTOR_NAME_ACHIEVABLE_SEGMENT).first;
                    release_hw_Vptr_ = release_hw_segment_.find<SharedVector>(VECTOR_NAME_RELEASE_SEGMENT).first;
                    done_Vptr_       = done_segment_.find<SharedVector>(VECTOR_NAME_DONE_SEGMENT).first;
                    busy_tile_Vptr_  = busy_tile_segment_.find<SharedVector>(VECTOR_NAME_BUSY_TILE_SEGMENT).first;

                    //std::cout << "Int Pointer step" << std::endl;
                    C3_table_ptr_     = C3_table_Vptr_->data();
                    achievable_ptr_   = achievable_Vptr_->data();
                    release_hw_ptr_   = release_hw_Vptr_->data();
                    done_ptr_         = done_Vptr_->data();
                    busy_tile_ptr_    = busy_tile_Vptr_->data();
                }
                else
                {
                    std::cerr << "Wrong argument to construct Memory Coordinator" << std::endl;
                }
            }
        }

    private:
        //Segments to create or open access to the shared memory
        bip::managed_shared_memory C3_table_segment_;
        bip::managed_shared_memory achievable_segment_;
        bip::managed_shared_memory release_hw_segment_;
        bip::managed_shared_memory done_segment_;
        bip::managed_shared_memory busy_tile_segment_;

        //Offset Pointers to access the shared vector
        bip::offset_ptr<SharedVector> C3_table_Vptr_;
        bip::offset_ptr<SharedVector> achievable_Vptr_;
        bip::offset_ptr<SharedVector> release_hw_Vptr_;
        bip::offset_ptr<SharedVector> done_Vptr_;
        bip::offset_ptr<SharedVector> busy_tile_Vptr_;
        
        //Offset Pointers to USE easily the shared memory
        //Use them ONLY when vectors are full of data
        bip::offset_ptr<int> C3_table_ptr_;
        bip::offset_ptr<int> achievable_ptr_;
        bip::offset_ptr<int> release_hw_ptr_;
        bip::offset_ptr<int> done_ptr_;
        bip::offset_ptr<int> busy_tile_ptr_;
        
        //Named mutexes to protect the shared memory access
        //The default constructor is private, instanciation mandatory...
        bip::named_mutex C3_table_mutex_{
                        bip::open_or_create
                        , MUTEX_NAME_C3_SEGMENT};
        bip::named_mutex achievable_mutex_{
                        bip::open_or_create
                        , MUTEX_NAME_ACHIEVABLE_SEGMENT};
        bip::named_mutex release_hw_mutex_{
                        bip::open_or_create
                        , MUTEX_NAME_RELEASE_SEGMENT};
        bip::named_mutex done_mutex_{
                        bip::open_or_create
                        , MUTEX_NAME_DONE_SEGMENT}; 
        bip::named_mutex busy_tile_mutex_{
                        bip::open_or_create
                        , MUTEX_NAME_BUSY_TILE_SEGMENT}; 
};

#endif

