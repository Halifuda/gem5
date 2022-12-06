from m5.objects import *

class MyP_BP(MyPerceptronBP):
    globalHistoryBits = 32
    perceptronsSize = 4096
    initWeight = 1

class BiM_BP(BiModeBP):
    globalPredictorSize = 8192
    globalCtrBits = 2
    choicePredictorSize = 8192
    choiceCtrBits = 2

class Local_BP(LocalBP):
    localPredictorSize = 2048
    localCtrBits = 2

class BanBP(BannedBP):
    pass

class T_BP(TournamentBP):
    localPredictorSize = 2048
    localCtrBits = 2
    localHistoryTableSize = 2048
    globalPredictorSize = 8192
    globalCtrBits = 2
    choicePredictorSize = 8192
    choiceCtrBits = 2

class P8k_BP(MultiperspectivePerceptron8KB):
    budgetbits = 8192 * 8 + 2048
    num_local_histories = 48
    num_filter_entries = 0
    imli_mask1 = 0x6
    imli_mask4 = 0x4400
    recencypos_mask = 0x100000090

class P64k_BP(MultiperspectivePerceptron64KB):
    budgetbits = 65536 * 8 + 2048
    num_local_histories = 510
    num_filter_entries = 18025
    imli_mask1 = 0xc1000
    imli_mask4 = 0x80008000
    recencypos_mask = 0x100000090