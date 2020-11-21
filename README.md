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
starter_se.py  

Σειρά | Κώδικας | Βασικό Χαρακτηριστικό
--- | --- | ---
83 | `cache_line_size = 64` | cache line size
186 | `parser.add_argument("--cpu", type=str, choices=list(cpu_types.keys()),default="atomic",help="CPU model to use")` | τύπος CPU (τον δίνουμε εμείς απο command line)
189 | `parser.add_argument("--cpu-freq", type=str, default="4GHz")` | συχνότητα λειτουργίας
190 | `parser.add_argument("--num-cores", type=int, default=1,help="Number of CPU cores")` | αριθμός πυρήνων
192 | `parser.add_argument("--mem-type", default="DDR3_1600_8x8",choices=ObjectList.mem_list.get_names(),help = "type of memory to use")` | τύπος μνήμης DRAM
195 | `parser.add_argument("--mem-channels", type=int, default=2,help = "number of memory channels")` | αριθμός καναλιών
199 | `parser.add_argument("--mem-size", action="store", type=str,default="2GB",help="Specify the physical memory size")` | μέγεθος μνήμης
