#include "base/bitfield.hh"
#include "base/intmath.hh"
#include "cpu/pred/banned.hh"

namespace gem5 {

namespace branch_prediction {

BannedBP::BannedBP(const BannedBPParams &params) : BPredUnit(params) {}

/*
 * For an unconditional branch we set its history such that
 * everything is set to taken. I.e., its choice predictor
 * chooses the taken array and the taken array predicts taken.
 */
void BannedBP::uncondBranch(ThreadID tid, Addr pc, void *&bpHistory) {

}

void BannedBP::squash(ThreadID tid, void *bpHistory) {

}

/*
 * Here we lookup the actual branch prediction. We use the PC to
 * identify the bias of a particular branch, which is based on the
 * prediction in the choice array. A hash of the global history
 * register and a branch's PC is used to index into both the taken
 * and not-taken predictors, which both present a prediction. The
 * choice array's prediction is used to select between the two
 * direction predictors for the final branch prediction.
 */
bool BannedBP::lookup(ThreadID tid, Addr branchAddr, void *&bpHistory) {
    return false;
}

void BannedBP::btbUpdate(ThreadID tid, Addr branchAddr, void *&bpHistory) {

}

/* Only the selected direction predictor will be updated with the final
 * outcome; the status of the unselected one will not be altered. The choice
 * predictor is always updated with the branch outcome, except when the
 * choice is opposite to the branch outcome but the selected counter of
 * the direction predictors makes a correct final prediction.
 */
void BannedBP::update(ThreadID tid, Addr branchAddr, bool taken,
                      void *bpHistory, bool squashed, const StaticInstPtr &inst,
                      Addr corrTarget) {

}

}  // namespace branch_prediction

}  // namespace gem5
