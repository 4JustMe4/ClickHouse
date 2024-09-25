#pragma once

#include <uv.h>
#include <memory>
#include <mutex>
#include <thread>
#include <nats.h>
#include <base/types.h>
#include <Common/Logger.h>

#include <Storages/UVLoop.h>

namespace DB
{

namespace Loop
{
    static const UInt8 RUN = 1;
    static const UInt8 STOP = 2;
    static const UInt8 CLOSED = 3;
}

using NATSOptionsPtr = std::unique_ptr<natsOptions, decltype(&natsOptions_Destroy)>;
using LockPtr = std::unique_ptr<std::lock_guard<std::mutex>>;

class NATSHandler
{
public:
    NATSHandler(LoggerPtr log_);

    /// Loop for background thread worker.
    void runLoop();
    void stopLoop();

    UInt8 getLoopState() { return loop_state.load(); }

    NATSOptionsPtr createOptions();

private:
    UVLoop loop;
    LoggerPtr log;

    std::atomic<UInt8> loop_state;
};

}
