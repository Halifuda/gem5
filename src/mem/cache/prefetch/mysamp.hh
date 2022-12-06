#ifndef __MEM_CACHE_PREFETCH_MY_SAMPLE_HH__
#define __MEM_CACHE_PREFETCH_MY_SAMPLE_HH__

#include <string>
#include <unordered_map>
#include <vector>

#include "base/sat_counter.hh"
#include "base/types.hh"
#include "mem/cache/prefetch/associative_set.hh"
#include "mem/cache/prefetch/queued.hh"
#include "mem/cache/replacement_policies/replaceable_entry.hh"
#include "mem/cache/tags/indexing_policies/set_associative.hh"
#include "mem/packet.hh"

namespace gem5 {

class BaseIndexingPolicy;
GEM5_DEPRECATED_NAMESPACE(ReplacementPolicy, replacement_policy);
namespace replacement_policy {
class Base;
}
struct MySamplePrefetcherParams;

GEM5_DEPRECATED_NAMESPACE(Prefetcher, prefetch);
namespace prefetch {

class MySample : public Queued {
   protected:
    struct SampleEntry : public TaggedEntry {
        void invalidate() override;

        Addr lastAddr;
        Addr signature;

        SampleEntry() : lastAddr(0), signature(0) {}
    };

    struct PredictEntry {
        SatCounter8 confidence;
        Addr prefetchOffset;

        PredictEntry(const SatCounter8& sc)
            : confidence(sc), prefetchOffset(0) {}
    };

    SatCounter8 initConfidence;
    double threshConf;
    int degree;

    int cacheNumBlocks;

    const struct PCTableInfo {
        const int assoc;
        const int numEntries;

        BaseIndexingPolicy* const indexingPolicy;
        replacement_policy::Base* const replacementPolicy;

        PCTableInfo(int assoc, int num_entries,
                    BaseIndexingPolicy* indexing_policy,
                    replacement_policy::Base* repl_policy)
            : assoc(assoc),
              numEntries(num_entries),
              indexingPolicy(indexing_policy),
              replacementPolicy(repl_policy) {}
    } samplerInfo;

    std::vector<PredictEntry> PredTable;
    AssociativeSet<SampleEntry> Sampler;

    uint64_t signMask;
    /* Fraction of cache-sets/sampler-sets, in bit form. */
    unsigned sampFracBits;

    /** Shift a given address by lBlkSize. Prepare it for indexing Sampler. */
    Addr sampleBlkShift(const Addr addr);
    /** Check if the given address is in sampled-set by sampler. */
    bool isSampled(const Addr addr);
    /** Check if need to prefetch by pc, and do the actual prefetch. */
    void prefetch(const Addr pc, const Addr addr,
                  std::vector<AddrPriority>& addresses);
    /** Update predict table by signature.
     * @param sign signature
     * @param last_addr last addr the signature accessed
     * @param this_addr this addr the signature accessed
     * @param hit_or_evict true: hit, false: evict
     */
    void predUpdate(const Addr sign, const Addr last_addr, const Addr this_addr,
                    bool hit_or_evict);

   public:
    MySample(const MySamplePrefetcherParams& params);
    void calculatePrefetch(const PrefetchInfo& pfi,
                           std::vector<AddrPriority>& addresses) override;
};

}  // namespace prefetch
}  // namespace gem5
#endif  // __MEM_CACHE_PREFETCH_MY_SAMPLE_HH__