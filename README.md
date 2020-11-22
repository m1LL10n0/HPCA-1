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

Παρατήρησαμε ότι καθώς άλλαζαμε το clock απο command line, στην σειρά 44 μεσα στο `[system.clk_domain]` το clock παρέμενε 1000 ενώ το clock σειρά 58 στο `[system.cpu_cluster.clk_domain]` άλλαζε ώστε:  
**`cpu-freq = (system.clk_domain.clock / system.cpu_cluster.clk_domain.clock)GHz`** 

**b)** [stats.txt](simple_se/stats.txt):
Σειρά | Κώδικας | Περιγραφή
--- | --- | ---
10 | `sim_insts 5028` | Number of instructions simulated
11 | `sim_ops 5834` | Number of ops (including micro ops) simulated
14 | `system.cpu_cluster.cpus.committedInsts 5028` | Number of instructions committed
15 | `system.cpu_cluster.cpus.committedOps 5834` | Number of ops (including micro ops) committed  

Ο αριθμός των εντολών που προσομοιώθηκαν και έγιναν \<\<committed\>\> φαίνεται να είναι ίδιος όμως λόγω του γεγονότος ότι μερικές απο τις εντολές έσπασαν σε περισσότερα αλλά μικρότερα micro ops που γίνονται πιο γρήγορα, ο αριθμός των εντολών που ενσωματώνει τα micro ops είναι μεγαλύτερος.

**c)** [stats.txt](simple_se/stats.txt):
Σειρά | Κώδικας | Περιγραφή
--- | --- | ---
143 | `system.cpu_cluster.cpus.dcache.overall_mshr_misses::total 147` | number of overall MSHR misses in L1 dcache
344 | `system.cpu_cluster.cpus.icache.overall_mshr_misses::total 332` | number of overall MSHR misses in L1 icache
493 |`system.cpu_cluster.l2.overall_accesses::total 479` | number of overall (read+write) accesses  

Όπως μπορούμε να δούμε, ο αριθμός των προσπελάσεων της μνήμης cache L2 είναι:   
**`479 = dcache_mshr_misses + icache_mshr_misses`**    

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

**a)** Τρέχουμε το πρόγραμμά μας, το οποίο είναι ένας πολλαπλασιασμός δύο 1000x1 πινάκων, με τις default ρυθμίσεις
[MinorCPU stats.txt](MinorCPU/stats.txt) | [TimingSimpleCPU stats.txt](TimingSimpleCPU/stats.txt) | Περιγραφή
--- | --- | ---
host_inst_rate 283616 | host_inst_rate 611985 | Simulator instruction rate (inst/s)
host_mem_usage 658964 | host_mem_usage 656660 | Number of bytes of host memory used
host_op_rate 324662 | host_op_rate 695487 | Simulator op (including micro ops) rate (op/s)
host_seconds 4.18 | host_seconds 1.93 | Real time elapsed on the host
host_tick_rate 173701768 | host_tick_rate 927086803 | Simulator tick rate (ticks/s)
sim_freq 1000000000000 | sim_freq 1000000000000 | Frequency of simulated ticks
sim_insts 1185066 | sim_insts 1179251 | Number of instructions simulated
sim_ops 1356598 | sim_ops 1340215 | Number of ops (including micro ops) simulated
sim_seconds 0.000726 | sim_seconds 0.001787 | Number of seconds simulated
sim_ticks 725814000 | sim_ticks 1786530000 | Number of ticks simulated
system.cpu.committedInsts 1185066 | system.cpu.committedInsts 1179251 | Number of instructions committed
system.cpu.committedOps 1356598 | system.cpu.committedOps 1340215 | Number of ops (including micro ops) committed
system.cpu.numCycles 1451628  | system.cpu.numCycles 3573060 | Νumber of cpu cycles simulated

**b)**

Όπως μπορούμε να παρατηρήσουμε, ο αριθμός τον εντολών που προσομοιώθηκαν καθώς και το μέγεθος της μνήμης που χρησιμοποίησαν οι δύο αυτές διαφορετικές εκτελέσεις του προγράμματός μας είναι παρόμοιες καθώς εξαρτώνται σχεδόν αποκλειστικά από το ίδιο το πρόγραμμα το οποίο δεν αλλάζει. Από την άλλη όμως βλέπουμε ότι ο προσομοιωμένος χρόνος εκτέλεσης είναι σημαντικά μικρότερος στο μοντέλο MinorCPU καθώς χρησιμοποιεί pipeline ενώ επίσης το μοντέλο TimingSimpleCPU περιμένει και για την επιστροφή των memory requests από το memory system.

**c)** Εδώ ξανατρέχουμε το πρόγραμμα με 16GHz ρολόι και L2 cache

[MinorCPU stats.txt](MinorCPU%20-l2cache%20-16GHz/stats.txt) | [TimingSimpleCPU stats.txt](TimingSimpleCPU%20-l2cache%20-16GHz/stats.txt) | Περιγραφή
--- | --- | ---
host_inst_rate 281885 | host_inst_rate 604988 | Simulator instruction rate (inst/s)
host_mem_usage 667672 | host_mem_usage 665876 | Number of bytes of host memory used
host_op_rate 322679 | host_op_rate 687529 | Simulator op (including micro ops) rate (op/s)
host_seconds 4.20 | host_seconds 1.95 | Real time elapsed on the host
host_tick_rate 30319772 | host_tick_rate 135201548 | Simulator tick rate (ticks/s)
sim_freq 1000000000000 | sim_freq 1000000000000 | Frequency of simulated ticks
sim_insts 1185066 | sim_insts 1179251 | Number of instructions simulated
sim_ops 1356598 | sim_ops 1340215 | Number of ops (including micro ops) simulated
sim_seconds 0.000127 | sim_seconds 0.000264 | Number of seconds simulated
sim_ticks 127469979 | sim_ticks 263554011 | Number of ticks simulated
system.cpu.committedInsts 1185066 | system.cpu.committedInsts 1179251 | Number of instructions committed
system.cpu.committedOps 1356598 | system.cpu.committedOps 1340215 | Number of ops (including micro ops) committed
system.cpu.numCycles 2023333 | system.cpu.numCycles 4183397 | Νumber of cpu cycles simulated

Όπως περιμέναμε, ο αριθμός εντολών που εκτελέστηκαν αλλά και η χρήση μνήμης παραμένουν σχεδόν τα **ίδια** ενώ ο προσομοιομένως χρόνος μειώνεται δραματικά και στα δύο μοντέλα λόγο της χρήσης **δεύτερου επιπέδου cache** καθώς και **4 φορές γρηγορότερου ρολογιού!**

---
#### Sources:
- [SimpleCPU - gem5.org](https://www.gem5.org/documentation/general_docs/cpu_models/SimpleCPU)
- [MinorCPU - gem5.org](https://www.gem5.org/documentation/general_docs/cpu_models/minor_cpu)
- [gem5 CPU model differences - stackoverflow.com](https://stackoverflow.com/questions/58554232/what-is-the-difference-between-the-gem5-cpu-models-and-which-one-is-more-accurat)
- [Memory Access Types - gem5.org](https://www.gem5.org/documentation/general_docs/memory_system/index.html#access-types)
- [Micro Ops - wikipedia.com](https://en.wikipedia.org/wiki/Micro-operation)

---
#### Σχόλια:  
- Μερικά μέρη της εκφώνησης δεν εξηγούνται καλά όπως το τι _ακριβώς_ πρέπει να συγκρίνουμε στο ερώτημα 2 β) ή στο σε ποια αρχεία αναφέρεστε στο 2 α) στην πρόταση "Χρησιμοποιήστε **τα αρχεία αυτά**..." καθώς όπως είναι γραμμένη τώρα η πρόταση δίνει την εντύπωση ότι πρέπει να ψάξουμε μόνο στα config.ini και config.json 
- Επίσης ίσως για την πρώτη και μόνο εργασία θα ήταν καλύτερο το να αναφέρετε που περίπου πρέπει να ψάξουμε για το κάθε τι καθώς όταν ο μαθητής ανοίγει ένα αρχείο 1500 γραμμών δεν του φαίνεται και πολύ ευχάριστο και ας υπάρχει search function. 
- Παρόλο που μάθαμε αρκετά έπειτα από ψάξιμο στο ίντερνετ σχετικά με το πώς λειτουργεί ο προσομοιωτής, τα αρχεία που χρησιμοποιεί αλλά και τα στατιστικά που παράγει, αρκετά από αυτά όπως μια σύντομη περιγραφή των βασικών στατιστικών που θα μας χρειαστούν αλλά και του πώς συνδέονται τα διάφορα κομμάτια του επεξεργαστή εσωτερικά του προσομοιωτή θα ήταν αρκετά χρήσιμα αλλά θα βοηθούσαν και στην εύκολη κατανόηση!
