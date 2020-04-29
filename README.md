# os_project

This is project for Operating System Course.
By
Aashish Shirgave ( 111703058 )
Swapnil Sawant ( 111703054 )

<h4>To run the project</h4>

```bash
git clone https://gitlab.com/aashish.shirgave/os_project.git
cd os_project
```
Install prerequisites such as gcc, make, readline, flex/lex, bison/yacc


*  For Debian-based linux distros:

```bash
sudo apt-get install libreadline-dev flex bison make gcc
```


*  For Red Hat Linux based distros:

```bash
sudo yum install readline-devel flex flex-devel byacc make gcc
```


*  For Arch Linux:

```bash
sudo pacman -S readline flex bison make gcc
```

<p>Now we will load the the code to virtual disk in the ExpOS </p>

```bash
cd xfs-interface
```

<p>We will use xfs interface to load code to disk.<p>

```bash
./xfs-interface
```

<p>Now u will see prompt for xfs interface with #. write following command there.

```bash
run ../batch.txt
```
<p> If you want to run each stage, so to upload code of that stage use

```bash
../run ../stage_<stage_number>/batch.txt
```
<p>Now exit xfs interface </p>

```bash
exit
```

<p> Now to run the virtual machine. We need to go to xsm folder </p>

```bash
cd ..
cd xsm
```

<p>Now to run the experimental string machine with timer interrupt enabled </p>

```bash
./xsm --timer 10
```
<p>You can use the timer value other than 10</p>
<p>It will run the machine </p>

<h4> What we have done in each Stage </p>

<h5>Stage 1 : Setting up the System </h5>
<p>The stage 1 was to set up enviroment to run ExpOs.</p> 

<h5>Stage 2 : Understanding the Filesystem</h5>
<p>
This stage was to learn about file system used. We read and learned about the file system in ExpOs.

*  Load/retrieve data and executable files from/to your host (Unix) system into the XSM disk.
   
*  Explain the disk data structures of the XFS file system - INODE table, disk free list and root file.
   
*  Find out the data blocks into which a data/executable file is stored in the XSM disk by examining the INODE table and root file.

</p>
<h5>Stage 3 : Bootstrap Loader</h5>
<p>
In this stage our learning ojective was

*  Use the XSM Instruction set to write a small OS startup code.

*  Load your OS startup code into the boot block of the disk and get this code executed on bootstrap.

We wrote small assembly code and loaded as os stratup code.
</p>




<h5>Stage 4 : Learning the SPL Language</h5>
<p>This stage was to learn SPL (Systems Programming Language) which allows high level programs to be written for the XSM machine.
We learned about the syntax and specification of the language and wrote a OS startup code and run that on xsm machine.</p>

<h5>Stage 5 : XSM Debugging</h5>
<p>This stage was to get familiarised with xsm debugger.</p>

<h5>Stage 6 : Bootstrap Loader</h5>
<p>
In this stage our learning ojective was

*     Learn how to set up the address space for an application.

*    Run an init program in user mode from the OS startup code.

In this stage we learned how to setup paging table for process.

</p>

<h5>Stage 7 : ABI and EXEC Format</h5>
<p>
In this stage our learning ojective was

*     Familiarise with the Application Binary Interface(ABI) of eXpOS.

*      Modify the INIT program to comply with the eXpOS ABI.
 

</p>

<h5>Stage 8 : Handling Timer Interrupt </h5>
<p>
In this stage our learning ojective was

*        Run the XSM machine with Timer enabled.

*       Familiarise with timer interrupt handling.

Wrote very basic code for timer interrupt and ran code with timer interrupt enabled.
</p>

<h5>Stage 9 : Handling kernel stack</h5>
<p>
In this stage our objective was to    

*  Get introduced to setting up process table entry for a user program.
   
*  Familiarise with the management of kernel stack in hardware interrupt handlers.

Following are some points to be considered for this


*  eXpOS requires that when the OS enters an interrupt handler that runs in kernel mode, the interrupt handler must switch to a different stack. This requirement is to prevent user level “hacks” into the kernel through the stack. In the previous stage, though you entered the timer interrupt service routine in the kernel mode, you did not change the stack. In this stage, this will be done.

*  To isolate the kernel from the user stack, the OS kernel must maintain two stacks for a program - a user stack and a kernel stack. In eXpOS, one page called the user area page is allocated for each process. A part of the space in this page will be used for the kernel stack (some other process information also will be stored in this page).

*  Whenever there is a transfer of program control from the user mode to kernel during interrupts (or exceptions), the interrupt handler will change the stack to the kernel stack of the program (that is, the SP register must point to the top of the kernel stack of the program). Before the machine returns to user mode from the interrupt, the user stack must be restored (that is, the SP register must point to the top of the user stack of the program).

*  Once we have two stacks for a user program, we need to design some data structure in memory to store the SP values of the two stacks. This is because the SP register of the machine can store only one value.

*  eXpOS requires you to maintain a Process Table, where data such as value of the kernel stack pointer, user stack pointer etc. pertaining to each process is stored.

*  For now, we just have one user program in execution. Hence we will need just one process table entry to be created. Each process table entry contains several fields. But for now, we are only interested in storing only 1) user stack pointer and 2) the memory page allocated as user area for the program.

*  The process table starts at page number 56 (address 28672). The process has space for 16 entries, each having 16 words. Each entry holds information pertaining to one user process. Since we have only one process, we will use the first entry (the first 16 words starting at address 28672). Among these, we will be updating only entries for user stack pointer (word 13) and user area page number (word 11) in this stage. 

</p>

<h5>Stage 10 : Console output</h5>
<p>
Objectives of this stage are
   
*   Familiarise with the low level system call interface in eXpOS.
  
*   Familiarise with the console output mechanism in eXpOS.


In this stage, we will modify the program such that the result is printed directly to the terminal. The terminal print is acheived by issuing a write system call from the user program. The write system call is serviced by interrupt routine 7. 
</p>

<h5>Stage 11 : Introduction to ExpL</h5>
<p>
In this stage we falimiliarised about Experimental language ExpL to write applicaion programs
</p>

<h5>Stage 12 : Introduction to Multiprogramming </h5>
<p>
In this stage our learning ojective was

*          Familiarise with the idle process.

*          Program the timer interrupt handler to concurrently schedule two processes.


In this stage we will set up pagging table table for two process init and idle.We will write timer interrupt handler to schedule the two processes init process and idle process concurrently.  We will do context switch using process table, kernal stack etc. in timer interrupt routine.
</p>


<h5>Stage 13 : Boot Module </h5>
<p>
In this stage our learning ojective was

* Introduction to XSM Module Programming.

* Implementation of Boot Module.

Boot module will be called from os startup code.
</p>


<h5>Stage 8 : Handling Timer Interrupt </h5>
<p>
In this stage our learning ojective was

* Run the XSM machine with Timer enabled.

* Familiarise with timer interrupt handling.

Wrote very basic code for timer interrupt and ran code with timer interrupt enabled.
</p>

