# rewrite the code with AT&T syntax 
.code16

# the size of system module after compilation 
.equ SYSSIZE, 0x3000 

# bss => Block Started by Symbol (未初始化数据段)
  .global _start, begtext, begdata, begbss, endtext, enddata, endbss 
  .text
  begtext: 
  .data 
  begdata: 
  .bss 
  begbss: 
  .text 

  .equ SETUPLEN, 4 # nr of setup-sectors 
  .equ BOOTSEG, 0x07c0 # original address of boot-sector 
  .equ INITSEG, 0x9000 # we move boot here here - out of way 
  .equ SETUPSEG, 0x9020     # setup starts here 
  # SETUPSEG - INITSEG = 0x20(Segments) 
  # => 0x200(B) => 512(B) => 1 harddisk-sector 
  .equ SYSSEG, 0x1000       # system loaded at 0x10000 (65536) 
  .equ ENDSEG, SYSSEG + SYSSIZE # where to stop loading 

# ROOT_DEV: 0x000 - same type of floppy as boot. 
#           0x301 - first partition on first driver etc 
#           0xabc (Old naming style of linux, changed since kernel 0.95) 
#                 a:  1 - memory 
#                     2 - ? 
#                     3 - harddisk 
#                     4 - ttyx 
#                     5 - tty 
#                     6 - parallel port 
#                     7 - pipe 
#                 b:  b % 5 == 0 - The whole (b / 5)th harddisk 
#                     b % 5 == 1 - The 1st partition of the (b / 5)th harddisk 
#                     ...
#                     b % 5 == 4 - The 4th partition of the (b / 5)th harddisk 
  .equ ROOT_DEV, 0x301 
  ljmp $BOOTSEG, $_start 
_start: 
  mov $BOOTSEG, %ax 
  mov %ax, %ds 
  mov $INITSEG, %ax 
  mov %ax, %es 
  mov $256, %cx 
  sub %si, %si 
  sub %di, %di 
  rep                       # repeat (cx) times 
  movsw                     # mov string word 
  ljmp $INITSEG, $go 
go: 
  mov %cs, %ax              # I guess: cs == INITSEG == 0x9000 
  mov %ax, %ds 
  mov %ax, %es 
# put stack at 0x9ff00 => 0x9000:0xff00 
  mov %ax, %ss 
  mov $0xff00, %sp          # arbitrary value >> 512 => ss:sp >> 0x90200 
                            # or sp  >> (0x200 + 0x200 * 4 + sizeof(stack))
                            # which one is right? 

# load the setup-sectors directly
# Note that 'es' is already set up. 

load_setup: 
# read 4 sectors starting from 2nd sector to 0x90200 using BIOS interrupt(INT 0x13) 
# if errors occurred, reset the driver, and try again 
  mov $0x0000, %dx          # driver 0, head 0 
  mov $0x0002, %cx          # sector 2, track 0 
  mov $0x0200, %bx          # address = 512, in INITSEG 
  .equ AX, 0x0200 + SETUPLEN 
  mov $AX, %ax # service 2, nr of sectors => ah = 0x02 -> read sectors to memory 
# the usage of (INT 0x13) to read sectors to memory 
#   ah = 0x02 => read sectors to memory 
#   al => sectors need reading 
#   ch => low 8 bits of cylinder 
#   cl => (0-5) - starting sector; (6-7) - index of cylinder 
#   dh => cylinder head sector num 
#   dl => driver num 
# es:bs -> data buf
# if reading failed, CF_Flag will be set "1". (CF == Carry Flag) 
  int $0x13                 # read it 
  jnc ok_load_setup         # jnc => jump if CF not set 
  mov 0x0000, %dx 
  mov 0x0000, %ax           # reset the diskette 
  int $0x13 
  jmp load_setup 

ok_load_setup: 
# Get disk driver parameters, specifically nr of sectors/track 
# the usage of (INT 0x13) to get disk driver parameters: 
#   ah = 0x08 
#   dl => the index of driver 
# return value: 
#   if errors occurred, CF_Flag will be set. 
#   ah = 0, al = 0
#   bl => type of driver: AT/PS2 
#   ch => low 8bits of max track num 
#   cl => (0-5) - the max sectors per track; (6-7) - high 2bits of max track num  
  mov $0x00, %dl 
  mov $0x0800, %ax 
  int $0x13 
  mov $0x00, %ch 
  # seg cs 
  mov %cx, %cs:sectors + 0  # saving the sectors per track to "sectors" 
  mov $INITSEG, %ax 
  mov %ax, %es              # because the (INT 0x13) changed the es(extra segment), change back to 0x9000 
  
# Print some inane message   
  mov $0x03, %ah            # to read cursor pos 
  xor %bh, %bh              # make bh = 0, xor is faster than assignment statement 
  int $0x10

  mov $24, %cx              # 24 characters => '\nLoading system ...\n\n'
  mov $0x0007, %bx          # page 0, attribute 7 (normal) 
  mov $msg1, %bp            # defined below 
  mov $0x1301, %ax          # write string, move cursor 
  int $0x10 

# ok, we've written the message, now we want to load the system (at 0x10000) 
  mov $SYSSEG, %ax 
  mov %ax, %es  # segment at 0x10000 
  call read_it  # read the system module from disk to memory, parameter: es 
  call kill_motor # kill the motor of the driver so that we can know the status of driver 

# After that we check which root-device to use. If the device is 
# defined (!= 0), nothing is done and the given device is used. 
# Otherwise, either /dev/PS0 (2, 28) or /dev/at0 (2, 8), depending 
# on the number of sectors that the BIOS reports currently. 
  # /dev/PS0 ->  1.44M A driver (0x021c) 
  # /dev/at0 -> 1.2M A driver (0x0208) 
  mov %cs:root_dev + 0, %ax 
  cmp $0, %ax 
  jne root_defined 
  mov %cs:sectors + 0, %bx 
  mov $0x0208, %ax          # 1.2M A driver 
  cmp $15, %bx 
  je  root_defined 
  mov $0x021c, %ax          # 1.44M A driver  
  cmp $18, %bx 
  je  root_defined 
undef_root: 
  jmp undef_root            # endless loop 
root_defined: 
  mov %ax, %cs:root_dev + 0 # saving the root_dev 

# after that (everything loaded), we jump to 
# the setup_routine loaded directly after 
# the bootblock: 
  ljmp $SETUPSEG, $0  # jump to $SETUPLEN:$0 => 0x0920:0x0000 

# This routine loads the system at address 0x10000, making sure 
# no 64kB boundaried are crossed. We try to load it as fast as 
# possible, loading whole tracks whenever we can. 
# 
# in: es - starting address segment (normally 0x1000)
# 
sread: .word 1 + SETUPLEN   # sectors having been read of current track 
head: .word 0               # current head 
track: .word 0              # current track 

read_it: 
  mov %es, %ax 
  test $0x0fff, %ax 
die: 
  jne die                   # make sure es is at 64kB boundary(0x10000) 
  xor %bx, %bx              # bx is starting address within segment 

rp_read:  
  mov %es, %ax 
  cmp $ENDSEG, %ax          # Have we loaded all yet? 
  jb  ok1_read              # jump if below 
  ret 

ok1_read: 
# Calculate the nr of sectors remained in the current track, and store the value in ax.  
# Then, figure out whether the size will exceed the limitation of 64kB if having read 
# the remaining sectors. 
# If exceeds, (64kB - offset) size will be read this round, thus calculating the nr of 
# sectors. 
  mov %cs:sectors+0, %ax    # get the nr of sectors per track 
  sub sread, %ax            # substract the nr of sectors having been read 
  mov %ax, %cx              # cx => nr of sectors remained 
  shl $9, %cx               # sift left the cx by 9 bits => cx = cx * 512 
  add %bx, %cx              # cx = cx + offset of the segment 
  jnc ok2_read              # if CF_Flag not set => not exceeds the limitation of 64kB 
  je  ok2_read              # ????
  # if exceeds the limitation of 64kB, then calculate how long to read, and then convert 
  # to nr of sectors 
  xor %ax, %ax              # make ax = 0 
  sub %bx, %ax              # ax = 0 - bx = 64kB - offset 
  shr $9, %ax               # convert size to nr of sectors 
ok2_read: 
  call read_track 
  mov %ax, %cx              # cx = ax => nr of sectors having been read in this round 
  add sread, %ax            # ax = sread + ax => update the sread 
  cmp %cs:sectors + 0, %ax  # if there remained sectors in the track, jump to ok3_read to read the sectors on head_1 of the same track 
  jne ok3_read 
  mov $1, %ax
  sub head, %ax             # if head == 0, jump to ok4_read to read the sectors on head_1 
                            # (head_before + head_after == 1) 
  jne ok4_read 
  incw track                # else go to read the next track 

ok4_read: 
  mov %ax, head             # change the head from 0 to 1 or from 1 to 0 
  xor %ax, %ax              # clear the ax -- sectors having been read 

ok3_read: 
  mov %ax, sread            # save the nr of sectors having been read 
  shl $9, %cx               # sift left the cx by 9 bits => cx = cx * 512 (cx: nr of sectors having been read in this round 
  add %cx, %bx              # update the offset of the segment 
  jnc rp_read               # if not exceeds the limitation of 64kB, then jump to rp_read, continuing to read data 
                            # else adjust the current segment, preparing for reading the next segment 
  mov %es, %ax              # let the es pointer the next 64kB segment(+ 64kB = + 0x1000) 
  add $0x1000, %ax 
  mov %ax, %es 
  xor %bx, %bx              # clear the offset of segment 
  jmp rp_read 

# read specified nr of sectors from starting sector to es:bx 
# usage of (INT 0x13, while ah = 2) 
#   al => nr of sectors to be read 
#   es:bx => the starting address of buffer 
read_track: 
  push %ax 
  push %bx 
  push %cx 
  push %dx 
  mov track, %dx            # get the current track num 
  mov sread, %cx            # get the nr of sectors having been read 
  inc %cx                   # cl => the starting address of sector to be read 
  mov %dl, %ch              # ch = dl = track => the current track num 
  mov head, %dx             # get the current head num (0 or 1) 
  mov %dl, %dh              # dh = dl => the current head num 
  mov $0, %dl               # dl => driver num 
  and $0x0100, %dx          # make sure the head num less then 2
  mov $2, %ah 
  int $0x13 
  jc bad_rt                 # if errors occurred, then jump to bad_rt 
  pop %dx 
  pop %cx 
  pop %bx 
  pop %ax 
  ret 

# execute the driver_reset operation, and then jump back to read_track for another try 
bad_rt: 
  mov $0, %ax 
  mov $0, %dx 
  int $0x13 
  pop %dx 
  pop %cx 
  pop %bx 
  pop %ax 
  jmp read_track 
  

# This procedure turns off the floppy driver motor, so that we enter the kernel in a 
# known state, and don't have to worry about it later 
kill_motor: 
  push %dx 
  mov $0x3f2, %dx           # the floppy control port, write-only 
  mov $0, %al               # A driver, close FDC, disable DMA and interrupt request, kill the motor 
  outsb                     # output the value of al to the port specified by dx 
  pop %dx 
  ret 


sectors: 
  .word 0 

msg1: 
  .byte 13, 10 
  .ascii "Loading system ..." 
  .byte 13, 10, 13, 10 

  .org 508 
root_dev: 
  .word ROOT_DEV 
boot_flag: 
  .word 0xaa55 

  .text 
  endtext: 
  .data 
  enddata: 
  .bss 
  endbss: 

