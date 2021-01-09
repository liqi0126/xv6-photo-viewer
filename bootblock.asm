
bootblock.o：     文件格式 elf32-i386


Disassembly of section .text:

00007c00 <start>:
# with %cs=0 %ip=7c00.

.code16                       # Assemble for 16-bit mode
.globl start
start:
  cli                         # BIOS enabled interrupts; disable
    7c00:	fa                   	cli    

00007c01 <setupsvga>:
  
setupsvga:
  movb    $0x4f,%ah
    7c01:	b4 4f                	mov    $0x4f,%ah
  movb    $0x01,%al
    7c03:	b0 01                	mov    $0x1,%al
  movw    $0x4115,%cx
    7c05:	b9 15 41 bf 00       	mov    $0xbf4115,%ecx
  movw    $0x1000,%di
    7c0a:	10 cd                	adc    %cl,%ch
  int     $0x10
    7c0c:	10 bb 00 90 8e c3    	adc    %bh,-0x3c717000(%ebx)

  movw    $GUI_BUF,%bx
  movw    %bx, %es

  movb    $0x4f,%ah
    7c12:	b4 4f                	mov    $0x4f,%ah
  movb    $0x02,%al
    7c14:	b0 02                	mov    $0x2,%al
  movw    $0x4115,%bx
    7c16:	bb 15 41 cd 10       	mov    $0x10cd4115,%ebx
  int     $0x10

  # Zero data segment registers DS, ES, and SS.
  xorw    %ax,%ax             # Set %ax to zero
    7c1b:	31 c0                	xor    %eax,%eax
  movw    %ax,%ds             # -> Data Segment
    7c1d:	8e d8                	mov    %eax,%ds
  movw    %ax,%es             # -> Extra Segment
    7c1f:	8e c0                	mov    %eax,%es
  movw    %ax,%ss             # -> Stack Segment
    7c21:	8e d0                	mov    %eax,%ss

00007c23 <seta20.1>:

  # Physical address line A20 is tied to zero so that the first PCs 
  # with 2 MB would run software that assumed 1 MB.  Undo that.
seta20.1:
  inb     $0x64,%al               # Wait for not busy
    7c23:	e4 64                	in     $0x64,%al
  testb   $0x2,%al
    7c25:	a8 02                	test   $0x2,%al
  jnz     seta20.1
    7c27:	75 fa                	jne    7c23 <seta20.1>

  movb    $0xd1,%al               # 0xd1 -> port 0x64
    7c29:	b0 d1                	mov    $0xd1,%al
  outb    %al,$0x64
    7c2b:	e6 64                	out    %al,$0x64

00007c2d <seta20.2>:

seta20.2:
  inb     $0x64,%al               # Wait for not busy
    7c2d:	e4 64                	in     $0x64,%al
  testb   $0x2,%al
    7c2f:	a8 02                	test   $0x2,%al
  jnz     seta20.2
    7c31:	75 fa                	jne    7c2d <seta20.2>

  movb    $0xdf,%al               # 0xdf -> port 0x60
    7c33:	b0 df                	mov    $0xdf,%al
  outb    %al,$0x60
    7c35:	e6 60                	out    %al,$0x60

  # Switch from real to protected mode.  Use a bootstrap GDT that makes
  # virtual addresses map directly to physical addresses so that the
  # effective memory map doesn't change during the transition.
  lgdt    gdtdesc
    7c37:	0f 01 16             	lgdtl  (%esi)
    7c3a:	90                   	nop
    7c3b:	7c 0f                	jl     7c4c <start32+0x1>
  movl    %cr0, %eax
    7c3d:	20 c0                	and    %al,%al
  orl     $CR0_PE, %eax
    7c3f:	66 83 c8 01          	or     $0x1,%ax
  movl    %eax, %cr0
    7c43:	0f 22 c0             	mov    %eax,%cr0

//PAGEBREAK!
  # Complete transition to 32-bit protected mode by using long jmp
  # to reload %cs and %eip.  The segment descriptors are set up with no
  # translation, so that the mapping is still the identity mapping.
  ljmp    $(SEG_KCODE<<3), $start32
    7c46:	ea                   	.byte 0xea
    7c47:	4b                   	dec    %ebx
    7c48:	7c 08                	jl     7c52 <start32+0x7>
	...

00007c4b <start32>:

.code32  # Tell assembler to generate 32-bit code now.
start32:
  # Set up the protected-mode data segment registers
  movw    $(SEG_KDATA<<3), %ax    # Our data segment selector
    7c4b:	66 b8 10 00          	mov    $0x10,%ax
  movw    %ax, %ds                # -> DS: Data Segment
    7c4f:	8e d8                	mov    %eax,%ds
  movw    %ax, %es                # -> ES: Extra Segment
    7c51:	8e c0                	mov    %eax,%es
  movw    %ax, %ss                # -> SS: Stack Segment
    7c53:	8e d0                	mov    %eax,%ss
  movw    $0, %ax                 # Zero segments not ready for use
    7c55:	66 b8 00 00          	mov    $0x0,%ax
  movw    %ax, %fs                # -> FS
    7c59:	8e e0                	mov    %eax,%fs
  movw    %ax, %gs                # -> GS
    7c5b:	8e e8                	mov    %eax,%gs

  # Set up the stack pointer and call into C.
  movl    $start, %esp
    7c5d:	bc 00 7c 00 00       	mov    $0x7c00,%esp
  call    bootmain
    7c62:	e8 ec 00 00 00       	call   7d53 <bootmain>

  # If bootmain returns (it shouldn't), trigger a Bochs
  # breakpoint if running under Bochs, then loop.
  movw    $0x8a00, %ax            # 0x8a00 -> port 0x8a00
    7c67:	66 b8 00 8a          	mov    $0x8a00,%ax
  movw    %ax, %dx
    7c6b:	66 89 c2             	mov    %ax,%dx
  outw    %ax, %dx
    7c6e:	66 ef                	out    %ax,(%dx)
  movw    $0x8ae0, %ax            # 0x8ae0 -> port 0x8a00
    7c70:	66 b8 e0 8a          	mov    $0x8ae0,%ax
  outw    %ax, %dx
    7c74:	66 ef                	out    %ax,(%dx)

00007c76 <spin>:
spin:
  jmp     spin
    7c76:	eb fe                	jmp    7c76 <spin>

00007c78 <gdt>:
	...
    7c80:	ff                   	(bad)  
    7c81:	ff 00                	incl   (%eax)
    7c83:	00 00                	add    %al,(%eax)
    7c85:	9a cf 00 ff ff 00 00 	lcall  $0x0,$0xffff00cf
    7c8c:	00                   	.byte 0x0
    7c8d:	92                   	xchg   %eax,%edx
    7c8e:	cf                   	iret   
	...

00007c90 <gdtdesc>:
    7c90:	17                   	pop    %ss
    7c91:	00 78 7c             	add    %bh,0x7c(%eax)
	...

00007c96 <waitdisk>:
  entry();
}

void
waitdisk(void)
{
    7c96:	55                   	push   %ebp
    7c97:	89 e5                	mov    %esp,%ebp
static inline uchar
inb(ushort port)
{
  uchar data;

  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    7c99:	ba f7 01 00 00       	mov    $0x1f7,%edx
    7c9e:	ec                   	in     (%dx),%al
  // Wait for disk ready.
  while((inb(0x1F7) & 0xC0) != 0x40)
    7c9f:	83 e0 c0             	and    $0xffffffc0,%eax
    7ca2:	3c 40                	cmp    $0x40,%al
    7ca4:	75 f8                	jne    7c9e <waitdisk+0x8>
    ;
}
    7ca6:	5d                   	pop    %ebp
    7ca7:	c3                   	ret    

00007ca8 <readsect>:

// Read a single sector at offset into dst.
void
readsect(void *dst, uint offset)
{
    7ca8:	55                   	push   %ebp
    7ca9:	89 e5                	mov    %esp,%ebp
    7cab:	57                   	push   %edi
    7cac:	53                   	push   %ebx
    7cad:	8b 5d 0c             	mov    0xc(%ebp),%ebx
  // Issue command.
  waitdisk();
    7cb0:	e8 e1 ff ff ff       	call   7c96 <waitdisk>
}

static inline void
outb(ushort port, uchar data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
    7cb5:	ba f2 01 00 00       	mov    $0x1f2,%edx
    7cba:	b8 01 00 00 00       	mov    $0x1,%eax
    7cbf:	ee                   	out    %al,(%dx)
    7cc0:	ba f3 01 00 00       	mov    $0x1f3,%edx
    7cc5:	89 d8                	mov    %ebx,%eax
    7cc7:	ee                   	out    %al,(%dx)
    7cc8:	89 d8                	mov    %ebx,%eax
    7cca:	c1 e8 08             	shr    $0x8,%eax
    7ccd:	ba f4 01 00 00       	mov    $0x1f4,%edx
    7cd2:	ee                   	out    %al,(%dx)
    7cd3:	89 d8                	mov    %ebx,%eax
    7cd5:	c1 e8 10             	shr    $0x10,%eax
    7cd8:	ba f5 01 00 00       	mov    $0x1f5,%edx
    7cdd:	ee                   	out    %al,(%dx)
    7cde:	89 d8                	mov    %ebx,%eax
    7ce0:	c1 e8 18             	shr    $0x18,%eax
    7ce3:	83 c8 e0             	or     $0xffffffe0,%eax
    7ce6:	ba f6 01 00 00       	mov    $0x1f6,%edx
    7ceb:	ee                   	out    %al,(%dx)
    7cec:	ba f7 01 00 00       	mov    $0x1f7,%edx
    7cf1:	b8 20 00 00 00       	mov    $0x20,%eax
    7cf6:	ee                   	out    %al,(%dx)
  outb(0x1F5, offset >> 16);
  outb(0x1F6, (offset >> 24) | 0xE0);
  outb(0x1F7, 0x20);  // cmd 0x20 - read sectors

  // Read data.
  waitdisk();
    7cf7:	e8 9a ff ff ff       	call   7c96 <waitdisk>
}

static inline void
insl(int port, void *addr, int cnt)
{
  asm volatile("cld; rep insl" :
    7cfc:	8b 7d 08             	mov    0x8(%ebp),%edi
    7cff:	b9 80 00 00 00       	mov    $0x80,%ecx
    7d04:	ba f0 01 00 00       	mov    $0x1f0,%edx
    7d09:	fc                   	cld    
    7d0a:	f3 6d                	rep insl (%dx),%es:(%edi)
  insl(0x1F0, dst, SECTSIZE/4);
}
    7d0c:	5b                   	pop    %ebx
    7d0d:	5f                   	pop    %edi
    7d0e:	5d                   	pop    %ebp
    7d0f:	c3                   	ret    

00007d10 <readseg>:

// Read 'count' bytes at 'offset' from kernel into physical address 'pa'.
// Might copy more than asked.
void
readseg(uchar* pa, uint count, uint offset)
{
    7d10:	55                   	push   %ebp
    7d11:	89 e5                	mov    %esp,%ebp
    7d13:	57                   	push   %edi
    7d14:	56                   	push   %esi
    7d15:	53                   	push   %ebx
    7d16:	8b 5d 08             	mov    0x8(%ebp),%ebx
    7d19:	8b 75 10             	mov    0x10(%ebp),%esi
  uchar* epa;

  epa = pa + count;
    7d1c:	89 df                	mov    %ebx,%edi
    7d1e:	03 7d 0c             	add    0xc(%ebp),%edi

  // Round down to sector boundary.
  pa -= offset % SECTSIZE;
    7d21:	89 f0                	mov    %esi,%eax
    7d23:	25 ff 01 00 00       	and    $0x1ff,%eax
    7d28:	29 c3                	sub    %eax,%ebx

  // Translate from bytes to sectors; kernel starts at sector 1.
  offset = (offset / SECTSIZE) + 1;
    7d2a:	c1 ee 09             	shr    $0x9,%esi
    7d2d:	83 c6 01             	add    $0x1,%esi

  // If this is too slow, we could read lots of sectors at a time.
  // We'd write more to memory than asked, but it doesn't matter --
  // we load in increasing order.
  for(; pa < epa; pa += SECTSIZE, offset++)
    7d30:	39 df                	cmp    %ebx,%edi
    7d32:	76 17                	jbe    7d4b <readseg+0x3b>
    readsect(pa, offset);
    7d34:	56                   	push   %esi
    7d35:	53                   	push   %ebx
    7d36:	e8 6d ff ff ff       	call   7ca8 <readsect>
  offset = (offset / SECTSIZE) + 1;

  // If this is too slow, we could read lots of sectors at a time.
  // We'd write more to memory than asked, but it doesn't matter --
  // we load in increasing order.
  for(; pa < epa; pa += SECTSIZE, offset++)
    7d3b:	81 c3 00 02 00 00    	add    $0x200,%ebx
    7d41:	83 c6 01             	add    $0x1,%esi
    7d44:	83 c4 08             	add    $0x8,%esp
    7d47:	39 df                	cmp    %ebx,%edi
    7d49:	77 e9                	ja     7d34 <readseg+0x24>
    readsect(pa, offset);
}
    7d4b:	8d 65 f4             	lea    -0xc(%ebp),%esp
    7d4e:	5b                   	pop    %ebx
    7d4f:	5e                   	pop    %esi
    7d50:	5f                   	pop    %edi
    7d51:	5d                   	pop    %ebp
    7d52:	c3                   	ret    

00007d53 <bootmain>:

void readseg(uchar*, uint, uint);

void
bootmain(void)
{
    7d53:	55                   	push   %ebp
    7d54:	89 e5                	mov    %esp,%ebp
    7d56:	57                   	push   %edi
    7d57:	56                   	push   %esi
    7d58:	53                   	push   %ebx
    7d59:	83 ec 0c             	sub    $0xc,%esp
  uchar* pa;

  elf = (struct elfhdr*)0x10000;  // scratch space

  // Read 1st page off disk
  readseg((uchar*)elf, 4096, 0);
    7d5c:	6a 00                	push   $0x0
    7d5e:	68 00 10 00 00       	push   $0x1000
    7d63:	68 00 00 01 00       	push   $0x10000
    7d68:	e8 a3 ff ff ff       	call   7d10 <readseg>

  // Is this an ELF executable?
  if(elf->magic != ELF_MAGIC)
    7d6d:	83 c4 0c             	add    $0xc,%esp
    7d70:	81 3d 00 00 01 00 7f 	cmpl   $0x464c457f,0x10000
    7d77:	45 4c 46 
    7d7a:	75 50                	jne    7dcc <bootmain+0x79>
    return;  // let bootasm.S handle error

  // Load each program segment (ignores ph flags).
  ph = (struct proghdr*)((uchar*)elf + elf->phoff);
    7d7c:	a1 1c 00 01 00       	mov    0x1001c,%eax
    7d81:	8d 98 00 00 01 00    	lea    0x10000(%eax),%ebx
  eph = ph + elf->phnum;
    7d87:	0f b7 35 2c 00 01 00 	movzwl 0x1002c,%esi
    7d8e:	c1 e6 05             	shl    $0x5,%esi
    7d91:	01 de                	add    %ebx,%esi
  for(; ph < eph; ph++){
    7d93:	39 f3                	cmp    %esi,%ebx
    7d95:	73 2f                	jae    7dc6 <bootmain+0x73>
    pa = (uchar*)ph->paddr;
    7d97:	8b 7b 0c             	mov    0xc(%ebx),%edi
    readseg(pa, ph->filesz, ph->off);
    7d9a:	ff 73 04             	pushl  0x4(%ebx)
    7d9d:	ff 73 10             	pushl  0x10(%ebx)
    7da0:	57                   	push   %edi
    7da1:	e8 6a ff ff ff       	call   7d10 <readseg>
    if(ph->memsz > ph->filesz)
    7da6:	8b 4b 14             	mov    0x14(%ebx),%ecx
    7da9:	8b 43 10             	mov    0x10(%ebx),%eax
    7dac:	83 c4 0c             	add    $0xc,%esp
    7daf:	39 c1                	cmp    %eax,%ecx
    7db1:	76 0c                	jbe    7dbf <bootmain+0x6c>
}

static inline void
stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
    7db3:	01 c7                	add    %eax,%edi
    7db5:	29 c1                	sub    %eax,%ecx
    7db7:	b8 00 00 00 00       	mov    $0x0,%eax
    7dbc:	fc                   	cld    
    7dbd:	f3 aa                	rep stos %al,%es:(%edi)
    return;  // let bootasm.S handle error

  // Load each program segment (ignores ph flags).
  ph = (struct proghdr*)((uchar*)elf + elf->phoff);
  eph = ph + elf->phnum;
  for(; ph < eph; ph++){
    7dbf:	83 c3 20             	add    $0x20,%ebx
    7dc2:	39 de                	cmp    %ebx,%esi
    7dc4:	77 d1                	ja     7d97 <bootmain+0x44>
  }

  // Call the entry point from the ELF header.
  // Does not return!
  entry = (void(*)(void))(elf->entry);
  entry();
    7dc6:	ff 15 18 00 01 00    	call   *0x10018
}
    7dcc:	8d 65 f4             	lea    -0xc(%ebp),%esp
    7dcf:	5b                   	pop    %ebx
    7dd0:	5e                   	pop    %esi
    7dd1:	5f                   	pop    %edi
    7dd2:	5d                   	pop    %ebp
    7dd3:	c3                   	ret    
