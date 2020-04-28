// 1. Save user stack value for later use set up kernal stack

alias userSP R0;
userSP=SP;
SP = [PROCESS_TABLE + ( [SYSTEM_STATUS_TABLE + 1] * 16) + 11]*512 - 1;

// 2. Set the MODE FLAG in the process table to system call number of exec. 

[PROCESS_TABLE + ( [SYSTEM_STATUS_TABLE + 1] * 16) + 9] = 9;

// 3. Get the argument (name of the file) from user stack.

alias physicalPageNum R1;
alias offset R2;
alias filenameAddr R3;
physicalPageNum = [PTBR + 2 * ((userSP - 4)/ 512)];
offset = (userSP - 4) % 512;
filenameAddr = (physicalPageNum * 512) + offset;
alias filename R4;
filename=[filenameAddr];

// 4. Search memory copy of the inode table for file also check file in exec format

alias index R5;
alias flag R6;
flag = 0;
index = 0;				//inode table page 59
while (index < 960) do
	if ([INODE_TABLE + index+1] == filename) then
		if ([INODE_TABLE + index] == EXEC) then
			flag = 1;
			break;
		endif;
	endif;
	index = index + 16;
endwhile;

if (flag == 0) then
	alias physicalAddrRetVal R7;
	physicalAddrRetVal = ([PTBR + 2 * ((userSP - 1) / 512)] * 512) + ((userSP - 1) % 512);
	[physicalAddrRetVal] = -1;
else

// 5. If the file is present, save the inode index of the file into a register for future use.

	alias fileindex R7;
	fileindex = (INODE_TABLE + index);

// 6. Call the Exit Process function in process manager module to deallocate the resources and pages of the current process.

	multipush(R1,R2,R3,R4,R5,R6,R7);
	R1 = 3; //calling exit
	R2 = [SYSTEM_STATUS_TABLE + 1];
	call MOD_1;
	multipop(R1,R2,R3,R4,R5,R6,R7);

// 7. Get user area page from process table. 
//    Reclam the same page by incrementing the memory free list entry of user area page 
//    and decrementing the MEM_FREE_COUNT field in the system status table. 

	alias UserAreaPage R8;
	UserAreaPage = [PROCESS_TABLE + ([SYSTEM_STATUS_TABLE + 1] * 16) + 11];
	[SYSTEM_STATUS_TABLE+2] = [SYSTEM_STATUS_TABLE+2] - 1;
	[MEMORY_FREE_LIST + UserAreaPage] = [MEMORY_FREE_LIST + UserAreaPage] + 1;

// 8. Set the SP to the start of the user area page to intialize the kernel stack of the new process.

	SP = UserAreaPage*512 - 1;

// 9. New process uses the PID of the terminated process. 
//    Update the STATE field to RUNNING and store inode index obtained above in the inode index field in the process table.

	[PROCESS_TABLE + ( [SYSTEM_STATUS_TABLE + 1] * 16) + 4] = RUNNING;
	[PROCESS_TABLE + ( [SYSTEM_STATUS_TABLE + 1] * 16) + 7] = fileindex;

// 10. Allocate new pages and set the page table entries for the new process.

	alias ptbr R8;
	ptbr = [PROCESS_TABLE + ( [SYSTEM_STATUS_TABLE + 1] * 16) + 14];

    // i. Set library pages entry

	[ptbr+0] = 63;
	[ptbr+1] = "0100";
	[ptbr+2] = 64;
	[ptbr+3] = "0100";


    // ii.Invoke the Get Free Page function to allocate 2 stack and 2 heap pages. Validate corresp. entries in Page table
    
	alias counter R9;
	counter = 0;
	while (counter < 2) do			//Heap
		multipush(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		R1 = 1;
		call MOD_2;
		multipop(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		[ptbr + 4 + counter*2] = R0;
		[ptbr + 4 + counter*2 + 1] = "0110";
		counter = counter + 1;
	endwhile;
    
	counter = 0;
	while (counter < 2) do			//Stack
		multipush(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		R1 = 1;
		call MOD_2;
		multipop(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		[ptbr + 16 + counter*2] = R0;
		[ptbr + 16 + counter*2 + 1] = "0110";
		counter = counter + 1;
	endwhile;


   // iii. Find out the number of blocks occupied by the file from inode table. Allocate same number of code pages and 
   //      update page table entries.  
   //

	if([fileindex + 8] != -1) then
		multipush(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		R1 = 1;
		call MOD_2;
		multipop(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		[ptbr + 8] = R0;
		[ptbr + 9] = "0100";
		loadi(R0,[fileindex + 8]);
	endif;
	if([fileindex + 9] != -1) then
		multipush(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		R1 = 1;
		call MOD_2;
		multipop(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		[ptbr + 10] = R0;
		[ptbr + 11] = "0100";
		loadi(R0,[fileindex + 9]);
	endif;
	if([fileindex + 10] != -1) then
		multipush(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		R1 = 1;
		call MOD_2;
		multipop(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		[ptbr + 12] = R0;
		[ptbr + 13] = "0100";
		loadi(R0,[fileindex + 10]);
	endif;
	if([fileindex + 11] != -1) then
		multipush(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		R1 = 1;
		call MOD_2;
		multipop(R1,R2,R3,R4,R5,R6,R7,R8,R9);
		[ptbr + 14] = R0;
		[ptbr + 15] = "0100";
		loadi(R0,[fileindex + 11]);
	endif;
	[[ptbr + 16]*512]=[[ptbr + 8]*512+1];
	SP = 8*512;
	[PROCESS_TABLE + ( [SYSTEM_STATUS_TABLE + 1] * 16) + 9] = 0;
endif;
ireturn;
