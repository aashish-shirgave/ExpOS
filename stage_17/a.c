
// 1. Save user stack value for later use set up kernal stack
alias userSP R1;
userSP = SP;

[PROCESS_TABLE + 16 * currentPid + 13] = SP;             // Save current SP into UPTR
SP = [PROCESS_TABLE + 16 * currentPid + 11] * 512 - 1;   // set SP to start of User page area Kernal stack

// 2. Set the MODE FLAG in the process table to system call number of exec.

[PROCESS_TABLE + 16 * currentPid + 9] = 9;

// 3. Get the argument (name of the file) from user stack.

alias physicalPageNum R2;
alias offset R3;
alias filenameAddr R4;

physicalPageNum = [PTBR + 2 * ((userSP - 4)/ 512)]; // we need to do userSP - 4 see kernal stack managnent
                                                    // .. arg pushed, R0 pushed(where return value is stored), call MOD_9 
offset = (userSP - 4) % 512;
filenameAddr = (physicalPageNum * 512) + offset;

alias filename R5;
filename=[filenameAddr];


// 4. Search memory copy of the inode table for file also check file in exec format

alias cnt R6;
cnt = 0;
while(cnt < MAX_FILE_NUM) do
	if([INODE_TABLE	+ cnt*16 + 1] == filename && [INODE_TABLE + cnt*16 ] == EXEC) then
		break;
	else
		cnt = cnt+1;
endif;
endwhile;
if(cnt == MAX_FILE_NUM) then 
    // file is not present or not in exec format
	[[PTBR + 2*((userSP - 1)/512)]*512 + ((userSP - 1)%512)] = -1; // return value is set to -1
	SP = [PROCESS_TABLE + 16 * currentPid + 13];                   // SP is set to UPTR
	[PROCESS_TABLE + 16 * currentPid + 9] =0;                      // MOD Flag value set to 0
	ireturn;
endif;

// 5. If the file is present, save the inode index of the file into a register for future use.

alias inode_index R2;
inode_index = cnt;

// 6. Call the Exit Process function in process manager module to deallocate the resources and pages of the current process.

alias currentPid R3;
currentPid = [SYSTEM_STATUS_TABLE + 1];

multipush(R0,R1,R2, R3);    // R1 userSP, R2 inode_idnex, R3 currentPid
R1 = 3;
R2 = [SYSTEM_STATUS_TABLE + 1];
call PROCESS_MANAGER;
multipop(R0,R1,R2, R3);

// 7. Get user area page from process table. 
//    Reclam the same page by incrementing the memory free list entry of user area page 
//    and decrementing the MEM_FREE_COUNT field in the system status table. 

alias user_area_page R4;
user_area_page = [PROCESS_TABLE + 16*currentPid + 11];

[MEMORY_FREE_LIST + user_area_page] = [MEMORY_FREE_LIST + user_area_page] + 1;
[SYSTEM_STATUS_TABLE + 2] = [SYSTEM_STATUS_TABLE + 2] -1;

// 8. Set the SP to the start of the user area page to intialize the kernel stack of the new process.

SP = user_area_page*512 -1;


// 9. New process uses the PID of the terminated process. 
//    Update the STATE field to RUNNING and store inode index obtained above in the inode index field in the process table.

    [PROCESS_TABLE + 16 * currentPid + 4] = RUNNING;
    [PROCESS_TABLE + 16 * currentPid + 7] =  (INODE_TABLE + inode_index); // diff

// 10. Allocate new pages and set the page table entries for the new process.

    alias page_table R5;
    page_table = [PROCESS_TABLE + ( [SYSTEM_STATUS_TABLE + 1] * 16) + 14];


    // i. Set library pages entry

    [page_table + 0] = 63;
    [page_table + 1] = "0100";
    [page_table + 2] = 64;
    [page_table + 3] = "0100";
    
    // ii.Invoke the Get Free Page function to allocate 2 stack and 2 heap pages. Validate corresp. entries in Page table

    multipush(R0,R1,R2,R3,R4,R5);
    R1 = 1;
    call MEMORY_MANAGER;
    [page_table + 4]=R0;        // Heap pages
    [page_table + 5]="0110";
    
    R1 = 1;
    call MEMORY_MANAGER;
    [page_table + 6]=R0;
    [page_table + 7]="0110";
    
    R1 = 1;
    call MEMORY_MANAGER;
    [page_table + 16]=R0;       // Stack pages
    [page_table + 17]="0110";
    
    R1 = 1;
    call MEMORY_MANAGER;
    [page_table + 18]=R0;
    [page_table + 19]="0110";
    
    multipop(R0,R1,R2,R3,R4,R5);
    
    // iii. Find out the number of blocks occupied by the file from inode table. Allocate same number of code pages and 
    //      update page table entries.  
    //


    alias fileindex R6;
    fileindex = (INODE_TABLE + inode_index);

	if([fileindex + 8] != -1) then
		multipush(R1,R2,R3,R4,R5,R6);
		R1 = 1;
		call MOD_2;
		multipop(R1,R2,R3,R4,R5,R6);
		[ptbr + 8] = R0;
		[ptbr + 9] = "0100";
		loadi(R0,[fileindex + 8]);
	endif;
	if([fileindex + 9] != -1) then
		multipush(R1,R2,R3,R4,R5,R6);
		R1 = 1;
		call MOD_2;
		multipop(R1,R2,R3,R4,R5,R6);
		[ptbr + 10] = R0;
		[ptbr + 11] = "0100";
		loadi(R0,[fileindex + 9]);
	endif;
	if([fileindex + 10] != -1) then
		multipush(R1,R2,R3,R4,R5,R6);
		R1 = 1;
		call MOD_2;
		multipop(R1,R2,R3,R4,R5,R6);
		[ptbr + 12] = R0;
		[ptbr + 13] = "0100";
		loadi(R0,[fileindex + 10]);
	endif;
	if([fileindex + 11] != -1) then
		multipush(R1,R2,R3,R4,R5,R6);
		R1 = 1;
		call MOD_2;
		multipop(R1,R2,R3,R4,R5,R6);
		[ptbr + 14] = R0;
		[ptbr + 15] = "0100";
		loadi(R0,[fileindex + 11]);
	endif;

// 12. Store the entry point IP (present in the header of first code page) value on top of the user stack.

    [[page_table+16]*512] = [[page_table+8]*512 + 1];
    SP = 8*512;
    [PROCESS_TABLE + ( [SYSTEM_STATUS_TABLE + 1] * 16) + 9] = 0;A

    ireturn;
