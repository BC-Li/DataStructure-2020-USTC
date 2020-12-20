# Lab 3 - Shortest Paths
This lab is originated from 9th DIMACS Implementation Challenge - Shortest Paths. 

Link: http://users.diag.uniroma1.it/challenge9/.

* binpath_full.c: full version including debug and tracking mode. Uncomment `DEFINE XXX` to enable. Time run on i7-9750H, DIMACS dataset is about 12s.
* binpath_accelerated.c: accelerated by finding only from src to dst instead of all of the paths. Time run on i7-9750H, DIMACS dataset is about 5s.
* binpath_clear.c: accelerated and removed all additional features such as `TRACKMEMORY`, easier to read.
## Run 
* Ubuntu environment to get the best speed.
* `$ make` in terminal and run `./binpath-accelerated -in USA-road.d.USA.gr -src 1 -dst 100` for example.
* Get the result.