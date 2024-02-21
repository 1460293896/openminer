#pragma once

#include <CL/opencl.hpp>

#include <common/custom.hpp>
#include <common/error/opencl_error.hpp>
#include <common/log/log.hpp>


namespace common
{
    namespace opencl
    {
        struct BufferData
        {
            cl::Buffer*  buffer{ nullptr };
            cl_mem_flags flags{};
            size_t       size{ 0u };
        };


        template<typename TBuffer>
        struct Buffer : private BufferData
        {
            explicit Buffer(cl_mem_flags _flags,
                            size_t const _size = sizeof(TBuffer))
            : BufferData()
            {
                flags = _flags;
                size = _size;
            }

            inline
            void setSize(size_t const _size)
            {
                size = _size;
            }

            inline
            cl::Buffer* getBuffer()
            {
                return buffer;
            }

            inline
            bool alloc(cl::CommandQueue* const clQueue,
                       cl::Context& clContext)
            {
                if (size == 0u)
                {
                    logErr() << "Cannot alloc! The size is 0!";
                    return false;
                }

                SAFE_DELETE(buffer);
                OPENCL_CATCH(
                    buffer = new(std::nothrow) cl::Buffer(
                        clContext, flags, size));
                IS_NULL(buffer);

                return true;
            }

            inline
            void free()
            {
                SAFE_DELETE(buffer);
            }

            inline
            bool write(TBuffer* src,
                       size_t const _size,
                       cl::CommandQueue* const clQueue)
            {
                OPENCL_ER(
                    clQueue->enqueueWriteBuffer(
                        *buffer,
                        CL_TRUE,
                        0,
                        _size,
                        src));
                return true;
            }
        };
    }
}