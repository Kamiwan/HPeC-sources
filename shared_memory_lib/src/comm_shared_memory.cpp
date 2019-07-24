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
#include "comm_shared_memory.hpp"


void MemoryCoordinator::Fill_ShMem_C3_Table(const std::vector<int> &memory)
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex_);   
    for(size_t i = 0; i < memory.size(); i++){
        C3_table_Vptr_->push_back(memory[i]);
    }
}
void MemoryCoordinator::Fill_ShMem_Achievable(const std::vector<int> &memory)
{
    bip::scoped_lock<bip::named_mutex> lock(achievable_mutex_);
    for(size_t i = 0; i < memory.size(); i++){
        achievable_Vptr_->push_back(memory[i]);
    }
}
void MemoryCoordinator::Fill_ShMem_Release_HW(const std::vector<int> &memory)
{
    bip::scoped_lock<bip::named_mutex> lock(release_hw_mutex_);
    for(size_t i = 0; i < memory.size(); i++){
        release_hw_Vptr_->push_back(memory[i]);
    }
}
void MemoryCoordinator::Fill_ShMem_Done(const std::vector<int> &memory)
{
    bip::scoped_lock<bip::named_mutex> lock(done_mutex_);
    for(size_t i = 0; i < memory.size(); i++){
        done_Vptr_->push_back(memory[i]);
    }
}
void MemoryCoordinator::Fill_ShMem_Busy_Tile(const std::vector<int> &memory)
{
    bip::scoped_lock<bip::named_mutex> lock(busy_tile_mutex_);
    for(size_t i = 0; i < memory.size(); i++){
        busy_tile_Vptr_->push_back(memory[i]);
    }
}


int  MemoryCoordinator::Achievable_Read(int app_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(achievable_mutex_);
    achievable_ptr_ = achievable_Vptr_->data(); //Always update offset_ptr before use it
    
    if(achievable_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        return achievable_ptr_[app_index];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to achievable_ptr_ Shared Vector OR app_index = " << app_index << std::endl;
        return -1;
    }
}
void MemoryCoordinator::Achievable_Write(int data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(achievable_mutex_);
    achievable_ptr_ = achievable_Vptr_->data(); //Always update offset_ptr before use it
    
    if(achievable_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        achievable_ptr_[app_index] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to achievable Shared Vector OR app_index = " << app_index << std::endl;
    }
}

int  MemoryCoordinator::Release_HW_Read(int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(release_hw_mutex_);
    release_hw_ptr_ = release_hw_Vptr_->data(); //Always update offset_ptr before use it
   
    if(release_hw_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        return release_hw_ptr_[app_index];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to release_hw Shared Vector OR app_index = " << app_index << std::endl;
        return -1;
    }
}
void MemoryCoordinator::Release_HW_Write(int data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(release_hw_mutex_);
    release_hw_ptr_ = release_hw_Vptr_->data(); //Always update offset_ptr before use it
    
    if(release_hw_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        release_hw_ptr_[app_index] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to release_hw Shared Vector OR app_index = " << app_index << std::endl;
    }
}

int  MemoryCoordinator::Done_Read(int app_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(done_mutex_);
    done_ptr_ = done_Vptr_->data(); //Always update offset_ptr before use it
    
    if(done_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        return done_ptr_[app_index];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to done Shared Vector OR app_index = " << app_index << std::endl;
        return -1;
    }
}
void MemoryCoordinator::Done_Write(int data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(done_mutex_);
    done_ptr_ = done_Vptr_->data(); //Always update offset_ptr before use it
    
    if(done_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        done_ptr_[app_index] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to done Shared Vector OR app_index = " << app_index << std::endl;
    }
}
int  MemoryCoordinator::Busy_Tile_Read(int tile_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(busy_tile_mutex_);
    busy_tile_ptr_ = busy_tile_Vptr_->data(); //Always update offset_ptr before use it
    
    if(busy_tile_ptr_ && tile_index >= 0 && tile_index < TILE_NUMBER) //If shared memory contains data, we proceed
    {
        return busy_tile_ptr_[tile_index];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to busy_tile Shared Vector OR tile_index = " << tile_index << std::endl;
        return -1;
    }
}
void MemoryCoordinator::Busy_Tile_Write(int data, int tile_index)
{
    bip::scoped_lock<bip::named_mutex> lock(busy_tile_mutex_);
    busy_tile_ptr_ = busy_tile_Vptr_->data(); //Always update offset_ptr before use it
    
    if(busy_tile_ptr_ && tile_index >= 0 && tile_index < TILE_NUMBER) //If shared memory contains data, we proceed
    {
        busy_tile_ptr_[tile_index] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to busy_tile Shared Vector OR tile_index = " << tile_index << std::endl;
    }
}




std::vector<int> MemoryCoordinator::C3_Table_Read(int app_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex_); 
    C3_table_ptr_ = C3_table_Vptr_->data(); //Always update offset_ptr before use it
    std::vector<int> res;

    if(C3_table_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        for(int i = 0; i < C3_NB_ATTRIBUTES; i++)
            res.push_back(C3_table_ptr_[(app_index*C3_NB_ATTRIBUTES)+i]);
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
    }

    return res;
}
void MemoryCoordinator::C3_Table_Write(const std::vector<int> &data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex_);
    C3_table_ptr_ = C3_table_Vptr_->data(); //Always update offset_ptr before use it
    
    if(C3_table_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        for(int i = 0; i < C3_NB_ATTRIBUTES; i++)
            C3_table_ptr_[(app_index*C3_NB_ATTRIBUTES)+i] = data[i];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
    }
}


void MemoryCoordinator::Update_ExecTime(int data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex_);
    C3_table_ptr_ = C3_table_Vptr_->data(); //Always update offset_ptr before use it

    if(C3_table_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        C3_table_ptr_[(app_index*C3_NB_ATTRIBUTES)+C3_CURRENT_TEXEC] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
    }
}
void MemoryCoordinator::Update_QoS(int data, int app_index)
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex_);
    C3_table_ptr_ = C3_table_Vptr_->data(); //Always update offset_ptr before use it

    if(C3_table_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        C3_table_ptr_[(app_index*C3_NB_ATTRIBUTES)+C3_CURRENT_QOS] = data;
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
    }
}
int MemoryCoordinator::Read_ExecTime(int app_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex_);
    C3_table_ptr_ = C3_table_Vptr_->data(); //Always update offset_ptr before use it

    if(C3_table_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        return C3_table_ptr_[(app_index*C3_NB_ATTRIBUTES)+C3_CURRENT_TEXEC];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
        return -1;
    }

}
int MemoryCoordinator::Read_QoS(int app_index) 
{
    bip::scoped_lock<bip::named_mutex> lock(C3_table_mutex_);
    C3_table_ptr_ = C3_table_Vptr_->data(); //Always update offset_ptr before use it

    if(C3_table_ptr_ && app_index >= 0 && app_index < APP_NUMBER) //If shared memory contains data, we proceed
    {
        return C3_table_ptr_[(app_index*C3_NB_ATTRIBUTES)+C3_CURRENT_QOS];
    } else
    {
        std::cerr << "SHARED MEMORY ERROR, Null pointer to C3_table Shared Vector OR app_index = " << app_index << std::endl;
        return -1;
    }
}



