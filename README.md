# HPCA
### Aristotle University of Thessaloniki 
- *Εργαστήριο Β Ομάδα 3*

[Link to the Repository containing this README](https://github.com/pavlidic/HPCA/)

Όνομα | Επώνυμο | ΑΕΜ
--- | --- | ---
Χρήστος | Παυλίδης | 9480
Αιμίλιος | Δραγκίνης | 9364

---

#### 1ο Ερώτημα:
**starter_se.py**  

Σειρά | Κώδικας | Βασικό Χαρακτηριστικό
--- | --- | ---
83 | `cache_line_size = 64` | cache line size
186 | `parser.add_argument("--cpu", type=str, choices=list(cpu_types.keys()),default="atomic",help="CPU model to use")` | τύπος CPU (τον δίνουμε εμείς απο command line)
189 | `parser.add_argument("--cpu-freq", type=str, default="4GHz")` | συχνότητα λειτουργίας
190 | `parser.add_argument("--num-cores", type=int, default=1,help="Number of CPU cores")` | αριθμός πυρήνων
192 | `parser.add_argument("--mem-type", default="DDR3_1600_8x8",choices=ObjectList.mem_list.get_names(),help = "type of memory to use")` | τύπος μνήμης DRAM
195 | `parser.add_argument("--mem-channels", type=int, default=2,help = "number of memory channels")` | αριθμός καναλιών
199 | `parser.add_argument("--mem-size", action="store", type=str,default="2GB",help="Specify the physical memory size")` | μέγεθος μνήμης

Επίσης, προσθέτουμε και L1 L2 caches των οποίων τα χαρακτηριστικά φαίνονται στο **devices.py**:  
Σειρά | Κώδικας | Βασικό Χαρακτηριστικό
--- | --- | ---
55-62 | `class L1I(L1_ICache):  tag_latency = 1 data_latency = 1    response_latency = 1    mshrs = 4    tgts_per_mshr = 8    size = '48kB'    assoc = 3` | L1 instruction cache
65-73 | `class L1D(L1_DCache):    tag_latency = 2    data_latency = 2    response_latency = 1    mshrs = 16    tgts_per_mshr = 16    size = '32kB'    assoc = 2    write_buffers = 16` | L1 data cache
87-96 | `class L2(L2Cache):    tag_latency = 12    data_latency = 12    response_latency = 5    mshrs = 32    tgts_per_mshr = 8    size = '1MB'    assoc = 16    write_buffers = 8    clusivity='mostly_excl'` | L2 cache

---
#### 2ο Ερώτημα:
**a)** Τα στοιχεία αυτά μπορούν αν επαληθευτούν κοιτάζοντας στο αρχείο **config.ini**:
Σειρά | Κώδικας | Βασικό Χαρακτηριστικό
--- | --- | ---
15 | `cache_line_size=64` | cache line size
21 | `mem_ranges=0:2147483648` | DRAM 2GB
22 | `memories=system.mem_ctrls0.dram system.mem_ctrls1.dram` | 2 channels
162-193 | `[system.cpu_cluster.cpus.dcache]` | όλα τα χαρακτηριστικά της L1 data cahce
908-939 | `[system.cpu_cluster.cpus.icache]` | όλα τα χαρακτηριστικά της L1 instruction cahce
1214-1245 | `[system.cpu_cluster.l2]` | όλα τα χαρακτηριστικά της L2 cahce

Παρατήρησα ότι καθώς άλλαζα το clock απο command line, στην σειρά 44 μεσα στο `[system.clk_domain]` το clock παρέμενε 1000 ενώ το clock σειρά 58 στο `[system.cpu_cluster.clk_domain]` άλλαζε ώστε:  
**`cpu-freq=(system.clk_domain.clock/system.cpu_cluster.clk_domain.clock)GHz`** 

**b) stats.txt**:
Σειρά | Κώδικας | Βασικό Χαρακτηριστικό
--- | --- | ---
14 | `system.cpu_cluster.cpus.committedInsts 5028` | Number of instructions committed
15 | `system.cpu_cluster.cpus.committedOps 5834` | Number of ops (including micro ops) committed  

**c) stats.txt**:
Σειρά | Κώδικας | Βασικό Χαρακτηριστικό
--- | --- | ---
143 | `system.cpu_cluster.cpus.dcache.overall_mshr_misses::total 147` | number of overall MSHR misses in L1 dcache
344 | `system.cpu_cluster.cpus.icache.overall_mshr_misses::total 332` | number of overall MSHR misses in L1 icache
493 |`system.cpu_cluster.l2.overall_accesses::total 479` | number of overall (read+write) accesses  

Όπως μπορούμε να δούμε, ο αριθμός τον προσπελάσεων της μνήμης cache L2 είναι:   
**`479=dcache_mshr_misses+icache_mshr_misses`**    

---
#### 3ο Ερώτημα:
