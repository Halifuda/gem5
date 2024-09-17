#include "cpu/pred/lab2_bp.hh"

namespace gem5 {
namespace branch_prediction {
Lab2PerceptronBP::Lab2PerceptronBP(const Lab2PerceptronBPParams &params)
    : BPredUnit(params) {}

bool Lab2PerceptronBP::lookup(ThreadID tid, Addr branch_addr,
                              void *&bp_history) {
  return true;
}
void Lab2PerceptronBP::updateHistories(ThreadID tid, Addr pc, bool uncond,
                                       bool taken, Addr target,
                                       void *&bp_history) {}
void Lab2PerceptronBP::update(ThreadID tid, Addr pc, bool taken,
                              void *&bp_history, bool squashed,
                              const StaticInstPtr &inst, Addr target) {}
void Lab2PerceptronBP::squash(ThreadID tid, void *&bp_history) {}
} // namespace branch_prediction
} // namespace gem5
