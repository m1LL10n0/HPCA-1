# HPCA
## Aristotle University of Thessaloniki 
- *Εργαστήριο Β Ομάδα 3*

[Link to the Repository containing this README](https://github.com/pavlidic/HPCA/)

Όνομα | Επώνυμο | ΑΕΜ
--- | --- | ---
Χρήστος | Παυλίδης | 9480
Αιμίλιος | Δραγκίνης | 9364

---

### 1ο Ερώτημα:
Βασικά χαρακτηριστικά του συστήματος που βρίσκουμε στο **starter_se.py**:  

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
### 2ο Ερώτημα:
**a)** Τα στοιχεία αυτά μπορούν να επαληθευτούν κοιτάζοντας στο αρχείο [config.ini](simple_se/config.ini)
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

**b)** [stats.txt](simple_se/stats.txt):
Σειρά | Κώδικας | Περιγραφή
--- | --- | ---
10 | `sim_insts 5028` | Number of instructions simulated
11 | `sim_ops 5834` | Number of ops (including micro ops) simulated
14 | `system.cpu_cluster.cpus.committedInsts 5028` | Number of instructions committed
15 | `system.cpu_cluster.cpus.committedOps 5834` | Number of ops (including micro ops) committed  

Ο αριθμός των εντολών που προσομοιώθηκαν και έγιναν \<\<committed\>\> φαίνεται να είναι ίδιος όμως λόγω του γεγονότος ότι μερικές απο τις εντολές έσπασαν σε περισσότερα αλλά μικρότερα micro ops που γίνονται πιο γρήγορα, ο αριθμός των εντολών που ενωματώνει τα micro ops είναι μεγαλύτερος.

**c)** [stats.txt](simple_se/stats.txt):
Σειρά | Κώδικας | Περιγραφή
--- | --- | ---
143 | `system.cpu_cluster.cpus.dcache.overall_mshr_misses::total 147` | number of overall MSHR misses in L1 dcache
344 | `system.cpu_cluster.cpus.icache.overall_mshr_misses::total 332` | number of overall MSHR misses in L1 icache
493 |`system.cpu_cluster.l2.overall_accesses::total 479` | number of overall (read+write) accesses  

Όπως μπορούμε να δούμε, ο αριθμός τον προσπελάσεων της μνήμης cache L2 είναι:   
**`479=dcache_mshr_misses+icache_mshr_misses`**    

---
### 3ο Ερώτημα:
Μοντέλα CPU:
- SimpleCPU:  
Ένα απλό μοντέλο cpu χωρίς pipeline που λειτουργεί χωρίς να είναι υπερβολικά ακριβές αλλά είναι αρκετά γρήγορο. Χρήσιμοποιείται συνήθως για να δούμε αν λειτουργεί ένα πρόγραμμα ή για γρήγορη εκκίνηση των Linux  
Αποτελείται απο τρεις κλάσεις:
  1. BaseSimpleCPU:  
  Παρέχει τις βασικές συναρτήσεις κρατάει τα στατιστικά απαραίτητα για την λειτουργεία των άλλων 2
  2. AtomicSimpleCPU:  
  Χρησιμοποιεί atomic memory accesses που επιστρέφουν προσεγγίσεις ως προς τον χρόνο ολοκλήρωσης των accesses
  3. TimingSimpleCPU:  
  Χρήσιμοποιεί timing memory accesses που είναι λεπτομερείς και έχουν μεγάλη ακρίβεια καθώς περιμένουν και χρονομετρούν τα responses
- MinorCPU:  
Δημιουργήθηκε για την μοντελοποίηση καθαρά in-order CPU, έχει σταθερό pipeline ενώ οι δομές δεδομένων και ο τρόπος εκτέλεσης εντολών μπορούν να τροποποιηθούν
- Τέλος υπάρχουν και τα 03CPU και TraceCPU μοντέλα που βασίζονται σε out-of-order CPU

**a)** 
[MinorCPU stats.txt]() | [TimingSimpleCPU stats.txt]()
--- | ---
host_inst_rate 283616 | host_inst_rate 611985
host_mem_usage 658964 | host_mem_usage 656660
host_op_rate 324662 | host_op_rate 695487
host_seconds 4.18 | host_seconds 1.93
host_tick_rate 173701768 | host_tick_rate 927086803
sim_freq 1000000000000 | sim_freq 1000000000000
sim_insts 1185066 | sim_insts 1179251
sim_ops 1356598 | sim_ops 1340215
sim_seconds 0.000726 | sim_seconds 0.001787
sim_ticks 725814000 | sim_ticks 1786530000
system.cpu.committedInsts 1185066 | system.cpu.committedInsts 1179251
system.cpu.committedOps 1356598 | system.cpu.committedOps 1340215
system.cpu.numCycles 1451628  | system.cpu.numCycles 3573060

**b)**  
Όπως μπορούμε να παρατηρήσουμε, ο αριθμός τον εντολών που προσομοιώθηκαν καθώς και το μέγεθος της μνήμης που χρησιμοποίησαν οι δύο αυτές διαφορετικές εκτελέσεις του προγράμματός μας είναι παρόμοιες καθώς εξαρτώνται σχεδόν αποκλειστικά από το ίδιο το πρόγραμμα το οποίο δεν αλλάζει. Από την άλλη όμως βλέπουμε ότι ο προσομοιωμένος χρόνος εκτέλεσης είναι σημαντικά μικρότερος στο μοντέλο MinorCPU καθώς χρησιμοποιεί pipeline ενώ επίσης το μοντέλο TimingSimpleCPU περιμένει και για την επιστροφή των memory requests από το memory system.


---
#### Sources:
- [SimpleCPU](https://www.gem5.org/documentation/general_docs/cpu_models/SimpleCPU)
- [MinorCPU](https://www.gem5.org/documentation/general_docs/cpu_models/minor_cpu)
- [Memory Access Types](https://www.gem5.org/documentation/general_docs/memory_system/index.html#access-types)
- [Micro Ops](https://en.wikipedia.org/wiki/Micro-operation)
