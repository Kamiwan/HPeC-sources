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

#include <string>
#include <vector>
#include <iostream>

#include "CommShMemConst.hpp"

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
        void    Fill_ShMem_C3_table(const std::vector<int> &memory);
        void    Fill_ShMem_achievable(const std::vector<int> &memory);
        void    Fill_ShMem_release_hw(const std::vector<int> &memory);
        void    Fill_ShMem_done(const std::vector<int> &memory);
        void    Fill_ShMem_busy_tile(const std::vector<int> &memory);

        /* ..._Read/Write
         * Read or write a single data in the dedicated shared memory
         * @param app_index    the number associated to the app
         * @param data         the data to write (Write)
         * @return             the data stored (Read)
         * 
         * (i) For busy_tile, tile_index must not exceed the number of tiles
        */
        int     achievable_Read(int app_index);
        void    achievable_Write(int data, int app_index);
        int     release_hw_Read(int app_index);
        void    release_hw_Write(int data, int app_index);
        int     done_Read(int app_index);
        void    done_Write(int data, int app_index);
        int     busy_tile_Read(int tile_index);
        void    busy_tile_Write(int data, int tile_index);

        /*C3 contains several parameters per app so Read and Write use an array
          (vector) instead of a single int
        */
        std::vector<int>    C3_table_Read(int app_index);
        void                C3_table_Write(const std::vector<int> &data, int app_index);

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
                bip::shared_memory_object::remove(NAME_BUSY_TILE_SEGMENT);
                busy_tile_segment = bip::managed_shared_memory(bip::create_only 
                                ,NAME_BUSY_TILE_SEGMENT //segment name
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
                bip::shared_memory_object::remove(MUTEX_NAME_BUSY_TILE_SEGMENT);
                bip::named_mutex busy_tile_mutex{
                                bip::open_or_create, 
                                MUTEX_NAME_BUSY_TILE_SEGMENT};

                //Initialize shared memory STL-compatible allocator
                const ShmemAllocator alloc_inst_C3_table     (C3_table_segment.get_segment_manager());
                const ShmemAllocator alloc_inst_achievable   (achievable_segment.get_segment_manager());
                const ShmemAllocator alloc_inst_release_hw   (release_hw_segment.get_segment_manager());
                const ShmemAllocator alloc_inst_done         (done_segment.get_segment_manager());
                const ShmemAllocator alloc_inst_busy_tile    (busy_tile_segment.get_segment_manager());

                //std::cout << "Vector ptr step" << std::endl;
                C3_table_Vptr   = C3_table_segment.construct<SharedVector>(VECTOR_NAME_C3_SEGMENT)
                                                    (alloc_inst_C3_table);//first ctor parameter
                achievable_Vptr = achievable_segment.construct<SharedVector>(VECTOR_NAME_ACHIEVABLE_SEGMENT)
                                            (alloc_inst_achievable);//first ctor parameter
                release_hw_Vptr = release_hw_segment.construct<SharedVector>(VECTOR_NAME_RELEASE_SEGMENT)
                                                    (alloc_inst_release_hw);//first ctor parameter
                done_Vptr       = done_segment.construct<SharedVector>(VECTOR_NAME_DONE_SEGMENT)
                                                        (alloc_inst_done);//first ctor parameter 
                busy_tile_Vptr  = busy_tile_segment.construct<SharedVector>(VECTOR_NAME_BUSY_TILE_SEGMENT)
                                                        (alloc_inst_busy_tile);//first ctor parameter                               
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
                    busy_tile_segment = bip::managed_shared_memory(bip::open_only 
                        ,NAME_BUSY_TILE_SEGMENT //segment name
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
                    bip::named_mutex busy_tile_mutex{
                                    bip::open_only, 
                                    MUTEX_NAME_BUSY_TILE_SEGMENT};

                    //std::cout << "Vector step" << std::endl;
                    C3_table_Vptr   = C3_table_segment.find<SharedVector>(VECTOR_NAME_C3_SEGMENT).first;
                    achievable_Vptr = achievable_segment.find<SharedVector>(VECTOR_NAME_ACHIEVABLE_SEGMENT).first;
                    release_hw_Vptr = release_hw_segment.find<SharedVector>(VECTOR_NAME_RELEASE_SEGMENT).first;
                    done_Vptr       = done_segment.find<SharedVector>(VECTOR_NAME_DONE_SEGMENT).first;
                    busy_tile_Vptr  = busy_tile_segment.find<SharedVector>(VECTOR_NAME_BUSY_TILE_SEGMENT).first;

                    //std::cout << "Int Pointer step" << std::endl;
                    C3_table_ptr     = C3_table_Vptr->data();
                    achievable_ptr   = achievable_Vptr->data();
                    release_hw_ptr   = release_hw_Vptr->data();
                    done_ptr         = done_Vptr->data();
                    busy_tile_ptr    = busy_tile_Vptr->data();
                }
                else
                {
                    std::cerr << "Wrong argument to construct Memory Coordinator" << std::endl;
                }
            }
        }

    private:
        //Segments to create or open access to the shared memory
        bip::managed_shared_memory C3_table_segment;
        bip::managed_shared_memory achievable_segment;
        bip::managed_shared_memory release_hw_segment;
        bip::managed_shared_memory done_segment;
        bip::managed_shared_memory busy_tile_segment;

        //Offset Pointers to access the shared vector
        bip::offset_ptr<SharedVector> C3_table_Vptr;
        bip::offset_ptr<SharedVector> achievable_Vptr;
        bip::offset_ptr<SharedVector> release_hw_Vptr;
        bip::offset_ptr<SharedVector> done_Vptr;
        bip::offset_ptr<SharedVector> busy_tile_Vptr;
        

        //Offset Pointers to USE easily the shared memory
        //Use them ONLY when vectors are full of data
        bip::offset_ptr<int> C3_table_ptr;
        bip::offset_ptr<int> achievable_ptr;
        bip::offset_ptr<int> release_hw_ptr;
        bip::offset_ptr<int> done_ptr;
        bip::offset_ptr<int> busy_tile_ptr;
        

        //Named mutexes to protect the shared memory access
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
        bip::named_mutex busy_tile_mutex{
                        bip::open_or_create
                        , MUTEX_NAME_BUSY_TILE_SEGMENT}; 
};

#endif

