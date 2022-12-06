#ifndef __CPU_PRED_BANNED_PRED_HH__
#define __CPU_PRED_BANNED_PRED_HH__

#include "base/sat_counter.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/BannedBP.hh"

namespace gem5 {

namespace branch_prediction {

class BannedBP : public BPredUnit {
   public:
    BannedBP(const BannedBPParams &params);
    void uncondBranch(ThreadID tid, Addr pc, void *&bp_history);
    void squash(ThreadID tid, void *bp_history);
    bool lookup(ThreadID tid, Addr branch_addr, void *&bp_history);
    void btbUpdate(ThreadID tid, Addr branch_addr, void *&bp_history);
    void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                bool squashed, const StaticInstPtr &inst, Addr corrTarget);
   
   private:
};

}  // namespace branch_prediction
}  // namespace gem5

#endif  // __CPU_PRED_BANNED_PRED_HH__