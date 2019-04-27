// Copyright 2019 ByteDance Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// =============================================================================

#ifndef BYTEPS_COMMUNICATOR_H
#define BYTEPS_COMMUNICATOR_H

#include <cstdlib> 

#ifdef BYTEPS_USE_MPI

#define OMPI_SKIP_MPICXX
#include "mpi.h"

#endif // BYTEPS_USE_MPI

namespace byteps {
namespace common {

class BytePSComm {

public:
    BytePSComm() { _comm = nullptr; }

    virtual void init(int* rank, int* size, int* local_rank, int* local_size) = 0;
    virtual int sendSignal(int destination, void* data, int len) = 0;
    virtual int recvSignal(int* source, void* data, int len) = 0;
    virtual int broadcastSignal(int root, void* data, int len) = 0;

    int broadcast(int root, void* data, int len);
    int reduce(int root, void* data, int len);

protected:
    int _p2pGPUCopy(void* from, void* to, int len);

    int _rank;
    int _size;
    int _local_rank;
    int _local_size;

    void* _comm;

};

class BytePSCommSocket : public BytePSComm {

public:
    void init(int* rank, int* size, int* local_rank, int* local_size);
    int sendSignal(int destination, void* data, int len);
    int recvSignal(int* source, void* data, int len);
    int broadcastSignal(int root, void* data, int len);

};

#ifdef BYTEPS_USE_MPI

class BytePSCommMPI : public BytePSComm {

public:

    ~BytePSCommMPI() {
        if (_comm) {
            free(_comm);
        }
    }

    void init(int* rank, int* size, int* local_rank, int* local_size);
    int sendSignal(int destination, void* data, int len);
    int recvSignal(int* source, void* data, int len);
    int broadcastSignal(int root, void* data, int len);

};

#endif // BYTEPS_USE_MPI

}
}

#endif // BYTEPS_COMMUNICATOR_H