#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

#define TASK_COMM_LEN 16
#define MAX_LINE_SIZE 80

char LICENSE[] SEC("license") = "GPL";

SEC("uretprobe//bin/bash:readline")
int BPF_KRETPROBE(printret, const void *ret) {
    char str[MAX_LINE_SIZE];
    char comm[TASK_COMM_LEN];
    u32 pid;
    u32 uid;

    if (!ret)
        return 0;

    bpf_get_current_comm(&comm, sizeof(comm));

    pid = bpf_get_current_pid_tgid() >> 32;
    uid = bpf_get_current_uid_gid() & 0xFFFFFFFF;
    bpf_probe_read_user_str(str, sizeof(str), ret);

    bpf_printk("PID %d (UID %d, CMD: %s) read: %s", pid, uid, comm, str);

    return 0;
}

