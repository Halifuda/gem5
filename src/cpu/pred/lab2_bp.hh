#ifndef __CPU_PRED_LAB2_BP_HH__
#define __CPU_PRED_LAB2_BP_HH__

#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/Lab2PerceptronBP.hh"

namespace gem5 {
namespace branch_prediction {
class Lab2PerceptronBP : public BPredUnit {
public:
  Lab2PerceptronBP(const Lab2PerceptronBPParams &params);

  bool lookup(ThreadID tid, Addr branch_addr, void *&bp_history) override;
  void updateHistories(ThreadID tid, Addr pc, bool uncond, bool taken,
                       Addr target, void *&bp_history) override;
  void update(ThreadID tid, Addr pc, bool taken, void *&bp_history,
              bool squashed, const StaticInstPtr &inst, Addr target) override;
  void squash(ThreadID tid, void *&bp_history) override;
};
} // namespace branch_prediction
} // namespace gem5

#endif // __CPU_PRED_LAB2_BP_HH__
