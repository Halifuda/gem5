#ifndef __CPU_PRED_MY_PERCEPTRON_PRED_HH__
#define __CPU_PRED_MY_PERCEPTRON_PRED_HH__

#include "base/sat_counter.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/MyPerceptronBP.hh"

namespace gem5 {
namespace branch_prediction {

class MyPerceptronBP : public BPredUnit {
   public:
    MyPerceptronBP(const MyPerceptronBPParams &params);
    void uncondBranch(ThreadID tid, Addr pc, void *&bp_history);
    void squash(ThreadID tid, void *bp_history);
    bool lookup(ThreadID tid, Addr branch_addr, void *&bp_history);
    void btbUpdate(ThreadID tid, Addr branch_addr, void *&bp_history);
    void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                bool squashed, const StaticInstPtr &inst, Addr corrTarget);

   private:
    void updateGlobalHistory(ThreadID tid, bool taken);

    struct BPHistory {
        uint64_t globalHistoryReg;
        int64_t perceptronOutput;
    };

    std::vector<uint64_t> globalHistoryReg;

    unsigned globalHistoryBits;
    unsigned perceptronsSize;

    // historyRegMask = 1 << globalHistoryBits - 1, used for update history
    uint64_t historyRegMask;
    // perceptronsMask = perceptronsSize - 1, used for indexing perceptron
    unsigned perceptronsMask;
    // the threshold, t = 1.93 * globalHistoryBits + 14, we change 1.93 to 2
    unsigned threshold;
    // bits per weights, always 1 + log2(threshold)
    unsigned weightBits;

    std::vector<std::vector<int64_t> > perceptrons;
};

}  // namespace branch_prediction
}  // namespace gem5

#endif  // __CPU_PRED_MY_PERCEPTRON_PRED_HH__