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
 * .cpp file of MemoryCoordinator class methods.
 * Allow HPeC ROS nodes to communicate using shared memory, the fastest communication
 *************************************************************************************/
#include "CommSharedMemory.hpp"


void MemoryCoordinator::Fill_ShMem_C3_table(const std::vector<int> &memory)
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex);   
    for(size_t i = 0; i < memory.size(); i++){
        C3_table_Vptr->push_back(memory[i]);
    }
}
void MemoryCoordinator::Fill_ShMem_achievable(const std::vector<int> &memory)
{
    bip::scoped_lock<bip::named_mutex> lock(achievable_mutex);
    for(size_t i = 0; i < memory.size(); i++){
        achievable_Vptr->push_back(memory[i]);
    }
}
void MemoryCoordinator::Fill_ShMem_release_hw(const std::vector<int> &memory)
{
    bip::scoped_lock<bip::named_mutex> lock(release_hw_mutex);
    for(size_t i = 0; i < memory.size(); i++){
        release_hw_Vptr->push_back(memory[i]);
    }
}
void MemoryCoordinator::Fill_ShMem_done(const std::vector<int> &memory)
{
    bip::scoped_lock<bip::named_mutex> lock(done_mutex);
    for(size_t i = 0; i < memory.size(); i++){
        done_Vptr->push_back(memory[i]);
    }
}
void MemoryCoordinator::Fill_ShMem_busy_tile(const std::vector<int> &memory)
{
    bip::scoped_lock<bip::named_mutex> lock(busy_tile_mutex);
    for(size_t i = 0; i < memory.size(); i++){
        busy_tile_Vptr->push_back(memory[i]);
    }
}


int  MemoryCoordinator::achievable_Read(int app_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(achievable_mutex);
    achievable_ptr = achievable_Vptr->data(); //Always update offset_ptr before use it
    
    if(achievable_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        return achievable_ptr[app_index];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to achievable_ptr Shared Vector OR app_index = " << app_index << std::endl;
        return -1;
    }
}
void MemoryCoordinator::achievable_Write(int data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(achievable_mutex);
    achievable_ptr = achievable_Vptr->data(); //Always update offset_ptr before use it
    
    if(achievable_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        achievable_ptr[app_index] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to achievable Shared Vector OR app_index = " << app_index << std::endl;
    }
}

int  MemoryCoordinator::release_hw_Read(int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(release_hw_mutex);
    release_hw_ptr = release_hw_Vptr->data(); //Always update offset_ptr before use it
   
    if(release_hw_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        return release_hw_ptr[app_index];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to release_hw Shared Vector OR app_index = " << app_index << std::endl;
        return -1;
    }
}
void MemoryCoordinator::release_hw_Write(int data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(release_hw_mutex);
    release_hw_ptr = release_hw_Vptr->data(); //Always update offset_ptr before use it
    
    if(release_hw_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        release_hw_ptr[app_index] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to release_hw Shared Vector OR app_index = " << app_index << std::endl;
    }
}

int  MemoryCoordinator::done_Read(int app_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(done_mutex);
    done_ptr = done_Vptr->data(); //Always update offset_ptr before use it
    
    if(done_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        return done_ptr[app_index];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to done Shared Vector OR app_index = " << app_index << std::endl;
        return -1;
    }
}
void MemoryCoordinator::done_Write(int data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(done_mutex);
    done_ptr = done_Vptr->data(); //Always update offset_ptr before use it
    
    if(done_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        done_ptr[app_index] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to done Shared Vector OR app_index = " << app_index << std::endl;
    }
}
int  MemoryCoordinator::busy_tile_Read(int tile_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(busy_tile_mutex);
    busy_tile_ptr = busy_tile_Vptr->data(); //Always update offset_ptr before use it
    
    if(busy_tile_ptr && tile_index >= 0 && tile_index < TILE_NUMBER) //If shared memory contains data, we proceed
    {
        return busy_tile_ptr[tile_index];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to busy_tile Shared Vector OR tile_index = " << tile_index << std::endl;
        return -1;
    }
}
void MemoryCoordinator::busy_tile_Write(int data, int tile_index)
{
    bip::scoped_lock<bip::named_mutex> lock(busy_tile_mutex);
    busy_tile_ptr = busy_tile_Vptr->data(); //Always update offset_ptr before use it
    
    if(busy_tile_ptr && tile_index >= 0 && tile_index < TILE_NUMBER) //If shared memory contains data, we proceed
    {
        busy_tile_ptr[tile_index] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to busy_tile Shared Vector OR tile_index = " << tile_index << std::endl;
    }
}




std::vector<int> MemoryCoordinator::C3_table_Read(int app_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex); 
    C3_table_ptr = C3_table_Vptr->data(); //Always update offset_ptr before use it
    std::vector<int> res;

    if(C3_table_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        for(int i = 0; i < C3_NB_ATTRIBUTES; i++)
            res.push_back(C3_table_ptr[(app_index*C3_NB_ATTRIBUTES)+i]);
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
    }

    return res;
}
void MemoryCoordinator::C3_table_Write(const std::vector<int> &data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex);
    C3_table_ptr = C3_table_Vptr->data(); //Always update offset_ptr before use it
    
    if(C3_table_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        for(int i = 0; i < C3_NB_ATTRIBUTES; i++)
            C3_table_ptr[(app_index*C3_NB_ATTRIBUTES)+i] = data[i];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
    }
}


void MemoryCoordinator::Update_ExecTime(int data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex);
    C3_table_ptr = C3_table_Vptr->data(); //Always update offset_ptr before use it

    if(C3_table_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        C3_table_ptr[(app_index*C3_NB_ATTRIBUTES)+C3_CURRENT_TEXEC] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
    }
}
void MemoryCoordinator::Update_QoS(int data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex);
    C3_table_ptr = C3_table_Vptr->data(); //Always update offset_ptr before use it

    if(C3_table_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        C3_table_ptr[(app_index*C3_NB_ATTRIBUTES)+C3_CURRENT_QOS] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
    }
}
int MemoryCoordinator::Read_ExecTime(int app_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex);
    C3_table_ptr = C3_table_Vptr->data(); //Always update offset_ptr before use it

    if(C3_table_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        return C3_table_ptr[(app_index*C3_NB_ATTRIBUTES)+C3_CURRENT_TEXEC];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
        return -1;
    }

}
int MemoryCoordinator::Read_QoS(int app_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex);
    C3_table_ptr = C3_table_Vptr->data(); //Always update offset_ptr before use it

    if(C3_table_ptr && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        return C3_table_ptr[(app_index*C3_NB_ATTRIBUTES)+C3_CURRENT_QOS];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
        return -1;
    }
}



