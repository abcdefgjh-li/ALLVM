#include <sys/mman.h>

#define PAGE_SIZE 4096
#define ALIGN (PAGE_SIZE - 1)
#define ROUND_PG(x) (((x) + ALIGN) & ~ALIGN)
#define TRUNC_PG(x) ((x) & ~ALIGN)
#define PFLAGS(x) ((((x) & PF_R) ? PROT_READ : 0) | \
                   (((x) & PF_W) ? PROT_WRITE : 0) | \
                   (((x) & PF_X) ? PROT_EXEC : 0))
#define LOAD_ERR ((unsigned long)-1)

#define MAX_CMD 4194304

// 检查 ELF 头
static int check_ehdr(Elf64_Ehdr *ehdr) {
    unsigned char *e = ehdr->e_ident;
    return (e[EI_MAG0]==ELFMAG0 && e[EI_MAG1]==ELFMAG1 && e[EI_MAG2]==ELFMAG2 && 
            e[EI_MAG3]==ELFMAG3 && e[EI_CLASS]==ELFCLASS64 && e[EI_VERSION]==EV_CURRENT &&
            (ehdr->e_type == ET_EXEC || ehdr->e_type == ET_DYN));
}

static unsigned long loadelf_mem(unsigned char *data, size_t data_sz, 
                                  Elf64_Ehdr *ehdr, Elf64_Phdr *phdr) {
    unsigned long minva = (unsigned long)-1, maxva = 0;
    int dyn = (ehdr->e_type == ET_DYN);
    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type != PT_LOAD) continue;
        if (phdr[i].p_offset + phdr[i].p_filesz > data_sz) return LOAD_ERR;
        if (phdr[i].p_vaddr < minva) minva = phdr[i].p_vaddr;
        if (phdr[i].p_vaddr + phdr[i].p_memsz > maxva) maxva = phdr[i].p_vaddr + phdr[i].p_memsz;
    }
    
    if (minva == (unsigned long)-1) return LOAD_ERR;
    
    minva = TRUNC_PG(minva);
    maxva = ROUND_PG(maxva);
    int flags = (dyn ? 0 : MAP_FIXED) | MAP_PRIVATE | MAP_ANONYMOUS;
    unsigned char *base = mmap(dyn ? NULL : (void*)minva, maxva - minva, PROT_NONE, flags, -1, 0);
    if (base == MAP_FAILED) return -1;
    munmap(base, maxva - minva);
    flags = MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE;
    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type != PT_LOAD) continue;
        
        unsigned long off = phdr[i].p_vaddr & ALIGN;
        unsigned long start = (dyn ? (unsigned long)base : 0) + TRUNC_PG(phdr[i].p_vaddr);
        size_t segsz = ROUND_PG(phdr[i].p_memsz + off);
        
        unsigned char *p = mmap((void*)start, segsz, PROT_WRITE, flags, -1, 0);
        if (p == MAP_FAILED) return LOAD_ERR;
        
        memcpy(p + off, data + phdr[i].p_offset, phdr[i].p_filesz);
        if (phdr[i].p_memsz > phdr[i].p_filesz)
            memset(p + off + phdr[i].p_filesz, 0, phdr[i].p_memsz - phdr[i].p_filesz);
        int prot = PFLAGS(phdr[i].p_flags);
        if (mprotect(p, segsz, prot) < 0) return LOAD_ERR;
    }
    
    return (unsigned long)base;
}