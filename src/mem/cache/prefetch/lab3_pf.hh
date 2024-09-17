#ifndef __MEM_CACHE_PREFETCH_LAB3_PF_HH__
#define __MEM_CACHE_PREFETCH_LAB3_PF_HH__

#include "mem/cache/prefetch/queued.hh"
#include "mem/packet.hh"
#include "params/Lab3Hyperion.hh"

namespace gem5 {
namespace prefetch {
class Lab3Hyperion : public Queued {
public:
  Lab3Hyperion(const Lab3HyperionParams &params);
  ~Lab3Hyperion() = default;
  void calculatePrefetch(const PrefetchInfo &pfi,
                         std::vector<AddrPriority> &addresses,
                         const CacheAccessor &cache) override;
  void notifyFill(const CacheAccessProbeArg &acc) override;
};
} // namespace prefetch
} // namespace gem5

#endif // __MEM_CACHE_PREFETCH_LAB3_PF_HH__
