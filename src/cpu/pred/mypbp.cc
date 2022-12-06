#include "cpu/pred/mypbp.hh"

#include "base/bitfield.hh"
#include "base/intmath.hh"

namespace gem5 {
namespace branch_prediction {

MyPerceptronBP::MyPerceptronBP(const MyPerceptronBPParams &params)
    : BPredUnit(params),
      globalHistoryReg(params.numThreads, ~0ULL),
      globalHistoryBits(params.globalHistoryBits),
      perceptronsSize(params.perceptronsSize),
      historyRegMask(mask(globalHistoryBits)),
      perceptronsMask(perceptronsSize - 1),
      threshold(2 * globalHistoryBits + 14),
      weightBits(1 + ceilLog2(threshold)),
      perceptrons(perceptronsSize,
                  std::vector<int64_t>(globalHistoryBits, params.initWeight)) {
    if (!isPowerOf2(perceptronsSize)) fatal("Invalid choice predictor size.\n");
}

/**
 * We set the perceptron output to 0 for unconditional branch.
 */
void MyPerceptronBP::uncondBranch(ThreadID tid, Addr pc, void *&bp_history) {
    BPHistory *history = new BPHistory;
    history->globalHistoryReg = globalHistoryReg[tid];
    history->perceptronOutput = 0;
    bp_history = static_cast<void *>(history);
    updateGlobalHistory(tid, true);
}
void MyPerceptronBP::squash(ThreadID tid, void *bp_history) {
    BPHistory *history = static_cast<BPHistory *>(bp_history);
    globalHistoryReg[tid] = history->globalHistoryReg;

    delete history;
}

bool MyPerceptronBP::lookup(ThreadID tid, Addr branch_addr, void *&bp_history) {
    uint64_t ghist = globalHistoryReg[tid];
    unsigned perceptronIdx =
        ((branch_addr >> instShiftAmt) ^ ghist) & perceptronsMask;

    assert(perceptronIdx < perceptronsSize);

    int64_t output = 0;
    for (int i = 0; i < globalHistoryBits - 1; ++i) {
        int64_t weight = perceptrons[perceptronIdx][i];
        uint64_t histbit = ghist & (1ULL << i);
        output += (histbit != 0 ? weight : -weight);
    }
    // bias weight
    output += perceptrons[perceptronIdx][globalHistoryBits - 1];

    BPHistory *history = new BPHistory;
    history->globalHistoryReg = globalHistoryReg[tid];
    history->perceptronOutput = output;
    bp_history = static_cast<void *>(history);

    return output >= 0;
}
void MyPerceptronBP::btbUpdate(ThreadID tid, Addr branch_addr,
                               void *&bp_history) {
    // set the newest history bit to 0
    globalHistoryReg[tid] &= (historyRegMask & ~1ULL);
}

void MyPerceptronBP::update(ThreadID tid, Addr branch_addr, bool taken,
                            void *bp_history, bool squashed,
                            const StaticInstPtr &inst, Addr corrTarget) {
    assert(bp_history);

    BPHistory *history = static_cast<BPHistory *>(bp_history);
    if (squashed) {
        globalHistoryReg[tid] = (history->globalHistoryReg << 1) | taken;
        return;
    }

    uint64_t ghist = history->globalHistoryReg;
    unsigned perceptronIdx =
        ((branch_addr >> instShiftAmt) ^ ghist) & perceptronsMask;

    bool correct = taken == (history->perceptronOutput >= 0);
    // train
    if (!correct || abs(history->perceptronOutput) < threshold) {
        for (int i = 0; i < globalHistoryBits; ++i) {
            uint64_t histbit = ghist & (1ULL << i);
            bool histbool = (histbit != 0);
            // bias bit
            if (i == globalHistoryBits - 1) histbool = true;
            perceptrons[perceptronIdx][i] += (histbool == taken ? 1 : -1);
        }
    }

    delete history;
}

void MyPerceptronBP::updateGlobalHistory(ThreadID tid, bool taken) {
    globalHistoryReg[tid] =
        taken ? (globalHistoryReg[tid] << 1) | 1 : (globalHistoryReg[tid] << 1);
    globalHistoryReg[tid] &= historyRegMask;
}

}  // namespace branch_prediction
}  // namespace gem5