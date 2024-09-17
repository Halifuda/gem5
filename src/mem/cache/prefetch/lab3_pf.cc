#include "mem/cache/prefetch/lab3_pf.hh"

namespace gem5 {
namespace prefetch {
Lab3Hyperion::Lab3Hyperion(const Lab3HyperionParams &params) : Queued(params) {}

void Lab3Hyperion::calculatePrefetch(const PrefetchInfo &pfi,
                                     std::vector<AddrPriority> &addresses,
                                     const CacheAccessor &cache) {}

void Lab3Hyperion::notifyFill(const CacheAccessProbeArg &acc) {}
} // namespace prefetch
} // namespace gem5
