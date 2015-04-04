AS = as --32 
LD = ld 
OBJCOPY = objcopy 
LDFLAGS = -m elf_i386 -Ttext 0 

bootsect: bootsect.s 
	@$(AS) -o bootsect.o bootsect.s 
	@$(LD) $(LDFLAGS) -o bootsect bootsect.o 
	@$(OBJCOPY) -R .pdr -R .comment -R.note -S -O binary bootsect 

