#include <common/chrono.hpp>
#include <common/config.hpp>
#include <common/custom.hpp>
#include <common/cast.hpp>
#include <common/log/log.hpp>
#include <device/device.hpp>
#include <resolver/amd/autolykos_v2.hpp>
#include <resolver/amd/etchash.hpp>
#include <resolver/amd/ethash.hpp>
#include <resolver/amd/firopow.hpp>
#include <resolver/amd/kawpow.hpp>
#include <resolver/amd/progpow.hpp>
#include <resolver/nvidia/autolykos_v2.hpp>
#include <resolver/nvidia/etchash.hpp>
#include <resolver/nvidia/ethash.hpp>
#include <resolver/nvidia/firopow.hpp>
#include <resolver/nvidia/kawpow.hpp>
#include <resolver/nvidia/progpow.hpp>


void device::Device::setAlgorithm(
    algo::ALGORITHM newAlgorithm)
{
    ////////////////////////////////////////////////////////////////////////////
    algorithm = newAlgorithm;
    switch (algorithm)
    {
        case algo::ALGORITHM::SHA256:
        {
            break;
        }
        case algo::ALGORITHM::ETHASH:
        {
            switch (deviceType)
            {
                case device::DEVICE_TYPE::NVIDIA:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverNvidiaEthash;
                    break;
                }
                case device::DEVICE_TYPE::AMD:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverAmdEthash;
                    break;
                }
                case device::DEVICE_TYPE::UNKNOW:
                {
                    break;
                }
            }
            break;
        }
        case algo::ALGORITHM::ETCHASH:
        {
            switch (deviceType)
            {
                case device::DEVICE_TYPE::NVIDIA:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverNvidiaEtchash;
                    break;
                }
                case device::DEVICE_TYPE::AMD:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverAmdEtchash;
                    break;
                }
                case device::DEVICE_TYPE::UNKNOW:
                {
                    break;
                }
            }
            break;
        }
        case algo::ALGORITHM::PROGPOW:
        {
            switch (deviceType)
            {
                case device::DEVICE_TYPE::NVIDIA:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverNvidiaProgPOW;
                    break;
                }
                case device::DEVICE_TYPE::AMD:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverAmdProgPOW;
                    break;
                }
                case device::DEVICE_TYPE::UNKNOW:
                {
                    break;
                }
            }
            break;
        }        case algo::ALGORITHM::KAWPOW:
        {
            switch (deviceType)
            {
                case device::DEVICE_TYPE::NVIDIA:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverNvidiaKawPOW;
                    break;
                }
                case device::DEVICE_TYPE::AMD:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverAmdKawPOW;
                    break;
                }
                case device::DEVICE_TYPE::UNKNOW:
                {
                    break;
                }
            }
            break;
        }
        case algo::ALGORITHM::FIROPOW:
        {
            switch (deviceType)
            {
                case device::DEVICE_TYPE::NVIDIA:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverNvidiaFiroPOW;
                    break;
                }
                case device::DEVICE_TYPE::AMD:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverAmdFiroPOW;
                    break;
                }
                case device::DEVICE_TYPE::UNKNOW:
                {
                    break;
                }
            }
            break;
        }
        case algo::ALGORITHM::AUTOLYKOS_V2:
        {
            switch (deviceType)
            {
                case device::DEVICE_TYPE::NVIDIA:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverNvidiaAutolykosV2;
                    break;
                }
                case device::DEVICE_TYPE::AMD:
                {
                    SAFE_DELETE(resolver);
                    resolver = new (std::nothrow) resolver::ResolverAmdAutolykosV2;
                    break;
                }
                case device::DEVICE_TYPE::UNKNOW:
                {
                    break;
                }
            }
            break;
        }
        default:
        {
            kill(device::KILL_STATE::ALGORITH_UNDEFINED);
            return;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    if (nullptr == resolver)
    {
        kill(device::KILL_STATE::RESOLVER_NULLPTR);
        return;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Set default value
    resolver->deviceId = id;
    resolver->setBlocks(128u);
    resolver->setThreads(128u);
}


void device::Device::setStratum(
    stratum::Stratum* const newStratum)
{
    stratum = newStratum;
}


void device::Device::kill(
    device::KILL_STATE const state)
{
    switch (state)
    {
        case device::KILL_STATE::ALGORITH_UNDEFINED:
        {
            logWarn()
                << "device[" << id << "] " << "Killed by code " << castU32(state)
                << " ALGORITH_UNDEFINED";
            break;
        }
        case device::KILL_STATE::RESOLVER_NULLPTR:
        {
            logWarn()
                << "device[" << id << "] " << "Killed by code " << castU32(state)
                << " RESOLVER_NULLPTR";
            break;
        }
        case device::KILL_STATE::UPDATE_MEMORY_FAIL:
        {
            logWarn()
                << "device[" << id << "] " << "Killed by code " << castU32(state)
                << " UPDATE_MEMORY_FAIL";
            break;
        }
        case device::KILL_STATE::UPDATE_CONSTANT_FAIL:
        {
            logWarn()
                << "device[" << id << "] " << "Killed by code " << castU32(state)
                << " UPDATE_CONSTANT_FAIL";
            break;
        }
        case device::KILL_STATE::KERNEL_EXECUTE_FAIL:
        {
            logWarn()
                << "device[" << id << "] " << "Killed by code " << castU32(state)
                << " KERNEL_EXECUTE_FAIL";
            break;
        }
        case device::KILL_STATE::DISABLE:
        {
            logWarn()
                << "device[" << id << "] " << "Killed by code " << castU32(state)
                << " DISABLE";
            break;
        }
    }
    alive.store(false, boost::memory_order::seq_cst);
}


bool device::Device::isAlive() const
{
    return alive.load(boost::memory_order::relaxed);
}


void device::Device::update(
    bool const memory,
    bool const constants,
    stratum::StratumJobInfo const& newJobInfo)
{
    UNIQUE_LOCK(mtxUpdate);

    jobInfo = newJobInfo;
    jobInfo.nonce += (jobInfo.gapNonce * id);

    needUpdateMemory.store(memory, boost::memory_order::seq_cst);
    needUpdateConstants.store(constants, boost::memory_order::seq_cst);
}


double device::Device::getHashrate()
{
    double hashrate { 0u };
    uint32_t const executeCount { miningStats.getKernelExecutedCount() };

    if (kernelMinimunExecuteNeeded <= executeCount)
    {
        miningStats.stop();
        hashrate = miningStats.getHashrate();
        miningStats.reset();
    }

    return hashrate;
}


stratum::Stratum* device::Device::getStratum()
{
    return stratum;
}


void device::Device::run()
{
    threadDoWork.interrupt();
    threadDoWork = boost::thread{ boost::bind(&device::Device::loopDoWork, this) };
}


void device::Device::waitJob()
{
    boost::mutex localMutex;
    while (jobInfo.epoch == -1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


bool device::Device::updateJob()
{
    common::Chrono chrono{};
    bool expectedMemory { true };
    bool expectedConstants { true };

    ////////////////////////////////////////////////////////////////////////////
    {
        UNIQUE_LOCK(mtxUpdate);
        needUpdateMemory.compare_exchange_weak(expectedMemory,
                                               false,
                                               boost::memory_order::seq_cst);
        needUpdateConstants.compare_exchange_weak(expectedConstants,
                                                  false,
                                                  boost::memory_order::seq_cst);
        if (   true == expectedMemory
            || true == expectedConstants)
        {
            if (   jobInfo.epoch != currentJobInfo.epoch
                || jobInfo.period != currentJobInfo.period)
            {
                miningStats.reset();
            }
            currentJobInfo = jobInfo;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    if (true == expectedMemory)
    {
        chrono.start();
        if (false == resolver->updateMemory(currentJobInfo))
        {
            kill(device::KILL_STATE::UPDATE_MEMORY_FAIL);
            return false;
        }
        chrono.stop();
        logInfo() << "device[" << id << "] Update memory in " << chrono.elapsed(common::CHRONO_UNIT::MS) << "ms";
    }

    ////////////////////////////////////////////////////////////////////////////
    if (true == expectedConstants)
    {
        chrono.start();
        resolver->updateJobId(currentJobInfo.jobIDStr);
        if (false == resolver->updateConstants(currentJobInfo))
        {
            kill(device::KILL_STATE::UPDATE_CONSTANT_FAIL);
            return true;
        }
        chrono.stop();
        logDebug() << "device[" << id << "] Update constants in " << chrono.elapsed(common::CHRONO_UNIT::US) << "us";
    }

    ////////////////////////////////////////////////////////////////////////////
    if (true == expectedMemory)
    {
        // Reset the stats, the memory was rebuilt
        miningStats.reset();
    }

    return (true == expectedMemory || true == expectedConstants);
}


void device::Device::loopDoWork()
{
    ////////////////////////////////////////////////////////////////////////////
    common::Config const& config { common::Config::instance() };
    if (false == config.isEnable(id))
    {
        kill(device::KILL_STATE::DISABLE);
        return;
    }

    ////////////////////////////////////////////////////////////////////////////
    if (false == initialize())
    {
        return;
    }
    if (nullptr == resolver)
    {
        return;
    }

    ////////////////////////////////////////////////////////////////////////////
    alive.store(true, boost::memory_order::relaxed);
    waitJob();

    ////////////////////////////////////////////////////////////////////////////
    // Statistical to compute the hashrate.
    miningStats.setBatchNonce(resolver->getBlocks() * resolver->getThreads());
    miningStats.reset();

    ////////////////////////////////////////////////////////////////////////////
    while (true == isAlive())
    {
        // Check and update the job.
        // Do not compute directly after update device.
        // A new job should spawn during the update.
        if (true == updateJob())
        {
            miningStats.setBatchNonce(resolver->getBlocks() * resolver->getThreads());
            continue;
        }
        else
        {
            if (false == isAlive())
            {
                return;
            }
        }

        // Execute the kernel to compute nonces.
        if (false == resolver->execute(currentJobInfo))
        {
            kill(device::KILL_STATE::KERNEL_EXECUTE_FAIL);
            continue;
        }
        resolver->submit(stratum);
        miningStats.increaseKernelExecuted();

        // Increasing nonce to next kernel.
        currentJobInfo.nonce += miningStats.getBatchNonce();
    }
}
