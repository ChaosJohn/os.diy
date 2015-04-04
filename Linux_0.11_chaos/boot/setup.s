#     setup.s 

# setup.s is responsible for getting the system data from the BIOS. 
# and putting them into the appropriate places in system memory. 
# Both setup.s and system has been loaded by the bootblock(bootsect.s).
#
# This code asks the bios for memory/disk/other parameters, and 
# puts them in a "safe" place: 0x90000-0x901FF, i.e. where the 
# boot-block used to be. It is then up to the protected mode 
# system to read them from there before the area is overwritten 
# for buffer-blocks. 

# rewrite the code with AT&T syntax 
.code16

# NOTE! These had better be the same as in bootsect.s! 
  .equ INITSEG, 0x9000 # we move boot here here - out of way 
  .equ SETUPSEG, 0x9020     # setup starts here and this is the current segment 
  # SETUPSEG - INITSEG = 0x20(Segments) 
  # => 0x200(B) => 512(B) => 1 harddisk-sector 
  .equ SYSSEG, 0x1000       # system loaded at 0x10000 (65536) 

  .global _start, begtext, begdata, begbss, endtext, enddata, endbss 
  .text 
  begtext: 
  .data 
  begdata: 
  .bss 
  begbss: 
  .text 

  ljmp $SETUPSEG, $_start 
_start: 
# ok, the read went well so we get current cursor position and save it for posterity.  
  mov $INITSEG, %ax           # this is done in bootsect already, but do it again for sure 
  mov $ax, %ds 
# uage of (INT 0x10) while ah = 0x03 
# function: 
#   Get cursor position and shape 
# input: 
#   bh => Page Number 
# return: 
#   ax = 0 
#   ch => Start Scan Line 
#   cl => End Scan Line 
#   dh => Row 
#   dl => Column 
  mov $0x03, %ah              # read cursor pos 
  xor %bh, %bh                # set bh to 0 => fast 
  int $0x10                   # save it in known place, con_init fetches it from 0x90000 
  mov %dx, %ds:0              # save the row and column infomation to 0x90000 

# Get memory size (extended mem, kB) 
# usage of (INT 0x15) while ah = 0x88 
# function: 
#   Get the size of extended memory 
# return: 
#   if ok, ax => the size of extended memory from 0x1000000(1M) 
#   if error, ax => error code 
  mov $0x88, %ah 
  int $0x15 
  mov %ax, %ds:2 

# Get video-card data: 
# usage of (INT 0x10) while ah = 0x0f 
# function: 
#   Get current video mode 
# return: 
#   ah => column size of characters 
#   al => video mode 
#   bh => the current display page number 
  mov $0x0f, %ah 
  int $0x10 
  mov %bx, %ds:4              # bh => display page 
  mov %ax, %ds:6              # al => video mode; ah => window width 

# Check for EGA/VGA and some config parameters 
# usage of (INT 0x10) while ah = 0x12 and bl = 0x10 
# function: 
#   alternate function select(PS, VGA, EGA, MCGA) - Get EGA info 
# return: 
#   bh => video state 
#         - 00h color mode in effect (I/O port 3Dxh) 
#         - 01h mono mode in effect (I/O port 3Bxh) 
#   bl => installed memory (00h - 64K, 01h - 128K, 02h - 192K, 03h - 256K) 
#   cx => video card features 
#         - ch => feature connector bits
#         - cl => switch settings 
  mov $0x12, %ah 
  mov $0x10, %bl 
  int $0x10 
  mov %ax, %ds:8              # 0x90008 => ?? 
  mov %bx, %ds:10             # 0x9000A => installed memeory 
  mov %cx, %ds:12             # 0x9000C => video card features 

# Get hd0 data 
  mov $0x0000, %ax 
  mov %ax, %ds 
  lds %ds:4*0x41, %si 
  mov $INITSEG, %ax 
  mov %ax, %es 
  mov $0x0080, %di 
  mov $0x10, %cx              # mov 10 byte => movsb 
  rep 
  movsb 

# Get hd1 data 
  mov $0x0000, %ax 
  mov %ax, %ds 
  lds %ds:4*0x46, %si 
  mov $INITSEG, %ax 
  mov %ax, %es 
  mov $0x0090, %di 
  mov $0x10, %cx 
  rep 
  movsb 
