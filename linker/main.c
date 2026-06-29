#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <errno.h>
#include <elf.h>
#include <fcntl.h>
#include "core/bash.h"
#include "core/detection.h"
#include "core/mod.h"
#include "core/mt.h"
#include "core/check.h"
#include "core/xc.h"
#include "core/exec.h"
#include "core/cow.h"

extern unsigned char bash_data[];
//extern size_t bash_size;

static char *g_command   = NULL;
static size_t g_cmd_len  = 0;

static int saye_pipe(int fd[2]) {
    if (pipe(fd) < 0) return -1;
    fcntl(fd[1], F_SETPIPE_SZ, 1048576); // 1MB
    return 0;
}

static void __attribute__((naked)) trampo_jump(unsigned long entry, unsigned long *stack) {
    __asm__ volatile (
        "mov sp, x1\n"
        "br x0\n"
    );
}

int main(int argc, char *argv[], char *envp[]) {
    XC();
    detection();
    XC();
    validation(argv[0]);
    XC();
    prctl(PR_SET_DUMPABLE, 0);

    g_command = get_last_line_alloc(argv[0]);
    if (!g_command) {
        fprintf(stderr, "无法从 %s 读取命令\n", argv[0]);
        XC();
        exit(1);
    }
    g_cmd_len = strlen(g_command);
    XC();

    Elf64_Ehdr *ehdr = (Elf64_Ehdr*)bash_data;
    if (!check_ehdr(ehdr)) {
        fprintf(stderr, "bash ELF 头检查失败\n");
        exit(1);
    }

    Elf64_Phdr *phdr = (Elf64_Phdr*)(bash_data + ehdr->e_phoff);
    unsigned long bash_base = loadelf_mem(bash_data, bash_size, ehdr, phdr);
    if (bash_base == LOAD_ERR) {
        fprintf(stderr, "加载 bash ELF 失败\n");
        exit(1);
    }

    unsigned long bash_entry = ehdr->e_entry + (ehdr->e_type == ET_DYN ? bash_base : 0);
    int pipefd[2];
    if (saye_pipe(pipefd) < 0) {
        fprintf(stderr, "创建管道失败\n");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork 失败\n");
        exit(1);
    }

    if (pid == 0) {
        close(pipefd[1]);

        if (dup2(pipefd[0], STDIN_FILENO) < 0) _exit(1);
        close(pipefd[0]);

        prctl(PR_SET_DUMPABLE, 0);

        char **p = envp;
        while (*p++ != NULL);
        Elf64_auxv_t *av = (Elf64_auxv_t*)p;

        while (av->a_type != AT_NULL) {
            switch (av->a_type) {
                case AT_PHDR: av->a_un.a_val = bash_base + ehdr->e_phoff; break;
                case AT_PHNUM: av->a_un.a_val = ehdr->e_phnum; break;
                case AT_PHENT: av->a_un.a_val = ehdr->e_phentsize; break;
                case AT_ENTRY: av->a_un.a_val = bash_entry; break;
                case AT_BASE:
                    if (ehdr->e_type == ET_DYN) av->a_un.a_val = bash_base;
                    break;
                case AT_EXECFN: av->a_un.a_val = (unsigned long)"bash"; break;
            }
            av++;
        }

        argv[0] = "bash";
        argv[1] = NULL;

        unsigned long *sp = (unsigned long*)argv - 1;
        sp[0] = 1;  // argc = 1，保证 bash 看到正确的参数布局

        trampo_jump(bash_entry, sp);
        _exit(1);
    } else {
        close(pipefd[0]);

        encrypt(g_command, g_cmd_len);   // 解密命令

        cow_write_str(pipefd, "#!/bin/bash\n{\n");
        cow_write(pipefd, g_command, g_cmd_len);
        cow_write_str(pipefd, "\n} </dev/tty\n");

        close(pipefd[1]);
        int status;
        waitpid(pid, &status, 0);
    }

    secure_zero(g_command, g_cmd_len);
    free(g_command);
    g_command = NULL;

    return 0;
}
