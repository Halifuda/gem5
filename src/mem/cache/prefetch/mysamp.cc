#include "mem/cache/prefetch/mysamp.hh"

#include <cassert>

#include "base/bitfield.hh"
#include "base/intmath.hh"
#include "base/logging.hh"
#include "base/random.hh"
#include "base/trace.hh"
#include "debug/HWPrefetch.hh"
#include "mem/cache/base.hh"
#include "mem/cache/prefetch/associative_set_impl.hh"
#include "mem/cache/replacement_policies/base.hh"
#include "mem/cache/tags/indexing_policies/skewed_associative.hh"
#include "params/MySamplePrefetcher.hh"

namespace gem5 {
GEM5_DEPRECATED_NAMESPACE(Prefetcher, prefetch);
namespace prefetch {

void MySample::SampleEntry::invalidate() {
    TaggedEntry::invalidate();
    // lastAddr = 0;
    // signature = 0;
}

MySample::MySample(const MySamplePrefetcherParams &params)
    : Queued(params),
      initConfidence(params.confidence_counter_bits, params.initial_confidence),
      threshConf(params.confidence_threshold / 100.0),
      degree(params.degree),
      cacheNumSets(params.cache_num_sets),
      samplerInfo(params.sampler_assoc, params.sampler_entries,
                  params.sampler_indexing_policy,
                  params.sampler_replacement_policy),
      PredTable(params.pred_entries, PredictEntry(SatCounter8(initConfidence))),
      Sampler(samplerInfo.assoc, samplerInfo.numEntries,
              samplerInfo.indexingPolicy, samplerInfo.replacementPolicy,
              SampleEntry()),
      signMask(params.pred_entries - 1),
      sampFracBits(params.samp_frac_bits) {
    if (!isPowerOf2(samplerInfo.numEntries))
        fatal("[MySamplePrefetcher] invalid sampler entry number.");
    if (!isPowerOf2(params.pred_entries))
        fatal("[MySamplePrefetcher] invalid predict table entry number.");
    if (floorLog2(cacheNumSets / (samplerInfo.numEntries / samplerInfo.assoc)) != sampFracBits)
        fatal(
            "[MySamplePrefetcher] invalid sample fraction bits "
            "(blksize:%d, "
            "cache: %d, samp: %d).",
            blkSize, cacheNumSets, samplerInfo.numEntries);
    inform("[MySamplePrefetcher] blksize: %d, lblksize: %d", blkSize, lBlkSize);
}

Addr MySample::sampleBlkShift(const Addr addr) { return addr >> lBlkSize; }

bool MySample::isSampled(const Addr addr) {
    return true; // now we try to sample all cache blocks
    /*
    Addr parent_set = addr >> lBlkSize;
    Addr sampler_set = (addr >> (lBlkSize + sampFracBits)) << sampFracBits;
    return (sampler_set ^ parent_set) == 0;
    */
}

void MySample::prefetch(const Addr pc, const Addr addr,
                        std::vector<AddrPriority> &addresses) {
    uint64_t index = pc & signMask;
    if (PredTable[index].confidence.calcSaturation() >= threshConf) {
        Addr target_start = addr + PredTable[index].prefetchOffset;
        for (int d = 0; d < degree; d++) {
            Addr new_addr = target_start + d * blkSize;
            addresses.push_back(AddrPriority(new_addr, 0));
        }
    }
}

void MySample::predUpdate(const Addr sign, const Addr last_addr,
                          const Addr this_addr, bool hit_or_evict) {
    uint64_t index = sign & signMask;
    if (hit_or_evict) {
        PredTable[index].confidence--;
    } else {
        PredTable[index].confidence++;
        PredTable[index].prefetchOffset = this_addr - last_addr;
    }
}

void MySample::calculatePrefetch(const PrefetchInfo &pfi,
                                 std::vector<AddrPriority> &addresses) {
    if (!pfi.hasPC()) {
        DPRINTF(HWPrefetch, "Ignoring request with no PC.\n");
        return;
    }

    uint64_t setnum = samplerInfo.numEntries / samplerInfo.assoc;
    uint64_t setmask = setnum - 1;

    Addr pf_addr = pfi.getAddr();
    Addr sidx = (pf_addr >> lBlkSize);
    Addr pc = pfi.getPC();
    bool is_secure = pfi.isSecure();

    DPRINTF(HWPrefetch, "Prefetch Start at addr=%lx pc=%lx set=%ld secure=%d\n", pf_addr, pc, (sidx >> sampFracBits) & setmask, is_secure);

    if (!isSampled(pf_addr)) {
        /* not sampled address, simply predict + prefetch */
        prefetch(pc, pf_addr, addresses);
        return;
    } else {
        /* sampled address, update sampler */
        SampleEntry *entry = Sampler.findEntry(sidx, is_secure);
        if (entry == nullptr) {
            /* no such entry, evict */
            entry = Sampler.findVictim(sidx);
            DPRINTF(HWPrefetch, "Miss. Victim addr=%lx pc=%lx set=%ld\n", entry->lastAddr, entry->signature, 
                ((entry->lastAddr >> lBlkSize) >> sampFracBits) & setmask);
            predUpdate(entry->signature, entry->lastAddr, pf_addr, false);

            // Insert new entry's data
            entry->lastAddr = pf_addr;
            entry->signature = pc;
            Sampler.insertEntry(sidx, is_secure, entry);        
            Sampler.accessEntry(entry);
            DPRINTF(HWPrefetch, "New entry addr=%lx pc=%lx set=%ld\n", entry->lastAddr, entry->signature, 
                ((entry->lastAddr >> lBlkSize) >> sampFracBits) & setmask);
            /* now entry exists, update sampler */
            // do not update PT
        } else {
            // update the predict table
            predUpdate(entry->signature, 0, 0, true);
            Sampler.accessEntry(entry);
            entry->signature = pc;
            entry->lastAddr = pf_addr;
        }

        // predict & prefetch
        prefetch(pc, pf_addr, addresses);
    }
    return;
}

}  // namespace prefetch
}  // namespace gem5