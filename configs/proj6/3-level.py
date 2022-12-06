import m5

from m5.objects import *

from new_cache import *

from optparse import OptionParser

parser = OptionParser()
parser.add_option('--cachec', type = "string", default = "default?")
parser.add_option('--test_exe', type = "string", default = "hello")
(options, args) = parser.parse_args()

root = Root(full_system = False, system = System())

root.system.clk_domain = SrcClockDomain()
root.system.clk_domain.clock = '2GHz'
root.system.clk_domain.voltage_domain = VoltageDomain()

root.system.mem_mode = 'timing'
root.system.mem_ranges = [AddrRange('2GB')]
root.system.mem_ctrl = MemCtrl()
root.system.mem_ctrl.dram = DDR4_2400_16x4()
root.system.mem_ctrl.dram.range = root.system.mem_ranges[0]

root.system.cpu = O3CPU()

root.system.cpu_clk_domain = SrcClockDomain()
root.system.cpu_clk_domain.clock = '2GHz'
root.system.cpu_clk_domain.voltage_domain = VoltageDomain()
root.system.cpu.clk_domain = root.system.cpu_clk_domain

root.system.membus = SystemXBar()

root.system.cpu.icache = L1ICache()
root.system.cpu.dcache = L1DCache()
root.system.l2cache = L2Cache()
root.system.l2bus = L2XBar()

if options.cachec == "spf":
    root.system.cpu.dcache.prefetcher = MySP()
    root.system.cpu.dcache.prefetcher.prefetch_on_access = True
    root.system.cpu.dcache.prefetch_on_access = True
    root.system.cpu.dcache.prefetch_on_pf_hit = True
elif options.cachec == "dcp":
    root.system.cpu.dcache.prefetcher = DCPTPrefetcher()
    root.system.cpu.dcache.prefetcher.prefetch_on_access = True
    root.system.cpu.dcache.prefetch_on_access = True
    root.system.cpu.dcache.prefetch_on_pf_hit = True
elif options.cachec == "mysamp":
    root.system.cpu.dcache.prefetcher = MySamp()
    root.system.cpu.dcache.prefetcher.prefetch_on_access = True
    root.system.cpu.dcache.prefetch_on_access = True
    root.system.cpu.dcache.prefetch_on_pf_hit = True
elif options.cachec == "ampm":
    root.system.cpu.dcache.prefetcher = AMPMPrefetcher()
    root.system.cpu.dcache.prefetcher.prefetch_on_access = True
    root.system.cpu.dcache.prefetch_on_access = True
    root.system.cpu.dcache.prefetch_on_pf_hit = True

if options.cachec != "default":
    root.system.cpu.dcache.prefetcher.prefetch_on_access = True
    root.system.cpu.dcache.prefetch_on_access = True
    root.system.cpu.dcache.prefetch_on_pf_hit = True

root.system.cpu.icache.cpu_side = root.system.cpu.icache_port
root.system.cpu.icache.mem_side = root.system.l2bus.cpu_side_ports
root.system.cpu.dcache.cpu_side = root.system.cpu.dcache_port
root.system.cpu.dcache.mem_side = root.system.l2bus.cpu_side_ports

root.system.l2cache.cpu_side = root.system.l2bus.mem_side_ports
root.system.l2cache.mem_side = root.system.membus.cpu_side_ports

root.system.mem_ctrl.port = root.system.membus.mem_side_ports

root.system.cpu.createInterruptController()
root.system.system_port = root.system.membus.cpu_side_ports

root.system.cpu.max_insts_any_thread = 10000000

if options.test_exe == "hello":
    exe_path_args = ['tests/test-progs/hello/bin/arm/linux/hello']
elif options.test_exe == "2mm":
    exe_path_args = ['tests/test_bench/2MM/2mm_base']
elif options.test_exe == "bfs":
    exe_path_args = ['tests/test_bench/BFS/bfs',
    '-f',
    'tests/test_bench/BFS/USA-road-d.NY.gr']
elif options.test_exe == "bzip2":
    exe_path_args = ['tests/test_bench/bzip2/bzip2_base.amd64-m64-gcc42-nn',
    'tests/test_bench/bzip2/input.source',
    '280']
elif options.test_exe == "mcf": 
    exe_path_args = ['tests/test_bench/mcf/mcf_base.amd64-m64-gcc42-nn',
    'tests/test_bench/mcf/inp.in']

root.system.workload = SEWorkload.init_compatible(exe_path_args[0])

process = Process()
process.cmd = exe_path_args
root.system.cpu.workload = process
root.system.cpu.createThreads()

m5.instantiate()
exit_event = m5.simulate()
print('Exiting @ tick {} because {}'.format(m5.curTick(), exit_event.getCause()))

