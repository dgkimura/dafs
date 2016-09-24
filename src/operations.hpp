#ifndef __OPERATIONS_HPP_INCLUDED__
#define __OPERATIONS_HPP_INCLUDED__


#include "filesystem.hpp"


namespace dafs
{
    void Delete(FileDescriptor f);

    void Read(FileDescriptor f);

    void Update(FileDescriptor f);

    void Write(FileDescriptor f);
}


#endif
