import m5
from m5.objects import *
from new_cache import *

# args parsing
from optparse import OptionParser

parser = OptionParser()
parser.add_option('--o3')
parser.add_option('--inorder')
parser.add_option('--cpu_clock', type = "string", default = "2GHz")
parser.add_option('--ddr3_1600_8x8')
parser.add_option('--ddr3_2133_8x8')
parser.add_option('--ddr4_2400_16x4')
parser.add_option('--ddr4_2400_8x8')
parser.add_option('--lpddr2_s4_1066_1x32')
parser.add_option('--wideio_200_1x128')
parser.add_option('--lpddr3_1600_1x32')
parser.add_option('--test_exe', type = "string", default = "hello")
(options, args) = parser.parse_args()

root = Root(full_system = False, system = System())
root.system.clk_domain = SrcClockDomain()
root.system.clk_domain.clock = '2GHz'
root.system.clk_domain.voltage_domain = VoltageDomain()

root.system.mem_mode = 'timing'
root.system.mem_ranges = [AddrRange ('2048MB')]
root.system.mem_ctrl = MemCtrl()

# dram type choosing
if options.ddr3_1600_8x8:
    root.system.mem_ctrl.dram = DDR3_1600_8x8()
elif options.ddr3_2133_8x8:
    root.system.mem_ctrl.dram = DDR3_2133_8x8()
elif options.ddr4_2400_16x4:
    root.system.mem_ctrl.dram = DDR4_2400_16x4()
elif options.ddr4_2400_8x8:
    root.system.mem_ctrl.dram = DDR4_2400_8x8()
elif options.lpddr2_s4_1066_1x32:
    root.system.mem_ctrl.dram = LPDDR2_S4_1066_1x32()
elif options.wideio_200_1x128:
    root.system.mem_ctrl.dram = WideIO_200_1x128()
elif options.lpddr3_1600_1x32:
    root.system.mem_ctrl.dram = LPDDR3_1600_1x32()
else:
    root.system.mem_ctrl.dram = DDR4_2400_16x4()          #default setting


root.system.mem_ctrl.dram.range = root.system.mem_ranges[0]

# cpu type choosing
if options.o3:
    root.system.cpu = DerivO3CPU()
elif options.inorder:
    root.system.cpu = MinorCPU()
else:
    root.system.cpu = TimingSimpleCPU()

# config cpu-specific clk
root.system.cpu_clk_domain = SrcClockDomain()
root.system.cpu_clk_domain.clock = options.cpu_clock
root.system.cpu_clk_domain.voltage_domain = VoltageDomain()
root.system.cpu.clk_domain = root.system.cpu_clk_domain

root.system.cpu.icache = L1ICache()
root.system.cpu.dcache = L1DCache()
root.system.l2cache = L2Cache()

root.system.membus = SystemXBar()

root.system.cpu.icache.cpu_side = root.system.cpu.icache_port
root.system.cpu.dcache.cpu_side = root.system.cpu.dcache_port
root.system.l2bus = L2XBar()
root.system.cpu.icache.mem_side = root.system.l2bus.cpu_side_ports
root.system.cpu.dcache.mem_side = root.system.l2bus.cpu_side_ports
root.system.l2cache.cpu_side = root.system.l2bus.mem_side_ports
root.system.l2cache.mem_side = root.system.membus.cpu_side_ports

root.system.mem_ctrl.port = root.system.membus.mem_side_ports
root.system.cpu.createInterruptController()
root.system.system_port = root.system.membus.cpu_side_ports

# limit max instructions
root.system.cpu.max_insts_any_thread = 100000000

# choose exe path and args
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
