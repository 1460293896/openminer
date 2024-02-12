#pragma once

#include <cuda.h>
#include <cuda_runtime.h>

#include <algo/dag_context.hpp>
#include <resolver/nvidia/progpow_kernel_parameter.hpp>


bool progpowInitMemory(algo::DagContext const& context,
                       resolver::nvidia::progpow::KernelParameters& params);
bool progpowUpdateConstants(uint32_t const* const headerSrc,
                            uint32_t* const headerDst);
bool progpowBuildDag(cudaStream_t stream,
                     uint32_t const dagItemParents,
                     uint32_t const dagNumberItems);
