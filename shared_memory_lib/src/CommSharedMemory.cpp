#include "CommSharedMemory.hpp"


int  MemoryCoordinator::achievable_Read(const int &app_index)
{
    boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(achievable_mutex);
    return achievable_ptr[app_index];
}
void MemoryCoordinator::achievable_Write(const int &data, const int &app_index)
{
    boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(achievable_mutex);
    achievable_ptr[app_index] = data;
}

int  MemoryCoordinator::release_hw_Read(const int &app_index)
{
    boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(release_hw_mutex);
    return release_hw_ptr[app_index];
}
void MemoryCoordinator::release_hw_Write(const int &data, const int &app_index)
{
    boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(release_hw_mutex);
    release_hw_ptr[app_index] = data;
}

int  MemoryCoordinator::done_Read(const int &app_index)
{
    boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(done_mutex);
    return done_ptr[app_index];
}
void MemoryCoordinator::done_Write(const int &data, const int &app_index)
{
    boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(done_mutex);
    done_ptr[app_index] = data;
}

