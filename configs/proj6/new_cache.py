from m5.objects import *


class L1DCache(Cache):
    size = "32kB"
    assoc = 2
    tag_latency = 2
    data_latency = 2
    response_latency = 2
    mshrs = 4
    tgts_per_mshr = 20

    def __init__(self):
        super(L1DCache, self).__init__()


class L1ICache(Cache):
    size = "32kB"
    assoc = 2
    tag_latency = 2
    data_latency = 2
    response_latency = 2
    mshrs = 4
    tgts_per_mshr = 20

    def __init__(self):
        super(L1ICache, self).__init__()


class L2Cache(Cache):
    size = "256kB"
    assoc = 8
    tag_latency = 20
    data_latency = 20
    response_latency = 20
    mshrs = 20
    tgts_per_mshr = 12

    def __init__(self):
        super(L2Cache, self).__init__()


class MySamp(MySamplePrefetcher):
    cache_num_sets = 512
    sampler_entries = '32'
    sampler_assoc = 1
    samp_frac_bits = 4
    confidence_counter_bits = 3
    initial_confidence = 5
    confidence_threshold = 25
    degree = 4
    pred_entries = 4096
    sampler_replacement_policy = LRURP()

class MySP(SignaturePathPrefetcher):
    signature_table_assoc = 1
    signature_table_replacement_policy = LRURP()
