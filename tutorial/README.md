# tutorial

## how to use
run `./build.sh` then `./run.sh`

## references
* Epiphany SDK Reference <http://adapteva.com/docs/epiphany_sdk_ref.pdf>
* OpenMPI v1.8 documentation <http://www.open-mpi.org/doc/v1.8/>

## content
* 0x: Epihany SDK
* 1x: OpenMPI clustering
* 2x: eSDK + OpenMPI

### 0x: Epiphany SDK
#### Prerequirement
Epiphany SDK (eSDK) is preinstalled in /opt/adapteva/esdk dir.

#### 01. Hello World
simple program to print "hello, world" to console (stdout).

#### 02. eHAL (Epiphany Hardware Abstraction Layer)
> The Epiphany Hardware Abstraction Layer (eHAL) library provides functionality for communicating with the Epiphany chip when the application runs on a host. The host can be a PC or an embedded processor. The communication is performed using memory writes to and reads from shared buffers that the applications on both sides should define.

* main.c: include "e-hal.h", call e_init() and e_finalize()
* build.sh: update gcc options

#### 03. Getting Platform Information

* main.c: after initialization, call e_reset_system() and e_get_platform_info()

#### 04. Workgroup
> A Workgroup is a collection of adjacent eCores on the Epiphany chip(s), normally intended for the parallel execution of a computational task. The workgroup is rectangular and its size and origin are defined at run-time by the host. Its parameters are set by a host library function and are maintained in a workgroup object.

* main.c: create a workgroup of 4x4 (all chips), reset, and close it

#### 05. Load program to workgroup

* *new* e-hello.c: eCore side program (empty, only returning 0)
* main.c: load e-hello to all eCore in workgroup (and start executing)
* build.sh: update gcc option, call e-gcc to compile e-hello and e-objcopy to generate SREC img from elf

#### 06. eLib (Epiphany Hardware Utility Library)
> The Epiphany Hardware Utility library provides functions for configuring and querying the Epiphany hardware resources. These routines automate many common programming tasks that are not provided by the C and C++ languages and are specific to the Epiphany architecture.

* e-hello.c: include "e-lib.h" and call e_get_coreid() (pass nothing to host side)
* build.sh: update e-gcc options

#### 07. Write to eCore Local Memory
> Each mesh node has a local, aliased, range of local memory that is accessible by the mesh node itself starting at address 0x0. On 32KB chip models, it ends at address 0x00007FFF. 
<div />
> Each core on the platform is referenced via a definition of a workgroup. Two global objects are available at each core’s space. One object, called e_group_config, contains the information about the chip type, the workgroup’s position and size, and the core’s position in the containing workgroup.

* *new* addr.h: define eCore local memory address to share data with host
* e-hello.c: get coordinates from core_id and set core_id to memory to share

#### 08. Read from eCore Local Memory from Host

* main.c: sleep to wait eCore computing, and read core_id from each eCore local memory and print it
* build.sh: update gcc option (C99 for loop initial declaration)

### 1x: OpenMPI Clustering
#### Prerequirement
OpenMPI is pre-installed in /opt/openmpi dir.

#### Setting up Clustering
- connect nodes to GbE hub
- create ssh key pairs (public key and private key)
- copy pubkeys to each node
- add pubkeys to ~/.ssh/authorized_keys
- check to login each other without password/passphrase

#### 11. OpenMPI Compiling and Running
simple "hello, world" programm, compiling with OpenMPI compiler *mprcc* and running with *mpirun* program

* build.sh: use mpicc to compile
* run.sh: use mpirun to execute 2 copies (processes) of program in local machine

if you have more nodes, set more values to *-np* option e.g. `-np 4` with 2 Parallella boards (4 CPU)

#### 12. OpenMPI Initialize and Finalize

* main.c: initialize and finalize program with MPI functions

#### 13. OpenMPI Functions

* main.c: get process rank (id) and processor name to print



### 2x: Epiphany SDK + OpenMPI

#### Prerequirement
- slurm-llnn: see <http://cb1300sf.info/archives/1036>

#### 21. Template
A Parallella board has one Epiphany and two CPU core. Calling reset/finalize Epiphany from any processes would causes the system crash. To prevent it, you need to call those functions only once. Here, I used blocking functions; MPI_Send/Recv.

* initialize: primary process (rank:0) calles reset and notify sencondary (rank:1, blocked) to starts.
* finialize: secondary process notifys computing completed to primary (blocked), then finalizes.
<div />
* main.c: use blocking functions to prevent system crash

#### 22. Workgroup


#### 23. Load


#### 24. Slurm Workload Manager

run `./build.sh` then `sbatch ./run.sh`, not `./run.sh`