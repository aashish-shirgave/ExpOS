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

<h5>Stage 1</h5>
<p>The stage 1 was to set up enviroment to run ExpOs.</p> 

<h5>Stage 2</h5>
<p>
This stage was to learn about file system used. We read and learned about the file system in ExpOs.

*  Load/retrieve data and executable files from/to your host (Unix) system into the XSM disk.
   
*  Explain the disk data structures of the XFS file system - INODE table, disk free list and root file.
   
*  Find out the data blocks into which a data/executable file is stored in the XSM disk by examining the INODE table and root file.

</p>

<h5>Stage 4<h5>
<p>This stage was to learn SPL (Systems Programming Language) which allows high level programs to be written for the XSM machine.
We learned about the syntax and specification of the language and wrote a OS startup code and run that on xsm machine.</p>

<h5>stage 5<h5>
<p>This stage was to get familiarised with xsm debugger.</p>


