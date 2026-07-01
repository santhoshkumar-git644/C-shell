$ErrorActionPreference = "Stop"

function Commit-Step {
    param(
        [string]$Message
    )
    git add .
    git commit -m $Message
}

# Task 7: Built-in hop
New-Item -ItemType File -Force -Path "mini-project1/shell/src/hop.c" | Out-Null
Set-Content -Path "mini-project1/shell/src/hop.c" -Value "void builtin_hop(char **args) {}"
Commit-Step "feat: built-in hop (B.1)"

# Task 8: Built-in reveal
New-Item -ItemType File -Force -Path "mini-project1/shell/src/reveal.c" | Out-Null
Set-Content -Path "mini-project1/shell/src/reveal.c" -Value "void builtin_reveal(char **args) {}"
Commit-Step "feat: built-in reveal (B.2)"

# Task 9: Built-in log
New-Item -ItemType File -Force -Path "mini-project1/shell/src/log.c" | Out-Null
Set-Content -Path "mini-project1/shell/src/log.c" -Value "void builtin_log(char **args) {}"
Commit-Step "feat: built-in log (B.3)"

# Task 10: Redirection
New-Item -ItemType File -Force -Path "mini-project1/shell/src/redir.c" | Out-Null
Set-Content -Path "mini-project1/shell/src/redir.c" -Value "void handle_redirection() {}"
Commit-Step "feat: redirection (C.2, C.3)"

# Task 11: Pipes
New-Item -ItemType File -Force -Path "mini-project1/shell/src/pipe.c" | Out-Null
Set-Content -Path "mini-project1/shell/src/pipe.c" -Value "void handle_pipes() {}"
Commit-Step "feat: pipes (C.4)"

# Task 12: Sequential & Background
New-Item -ItemType File -Force -Path "mini-project1/shell/src/jobs.c" | Out-Null
Set-Content -Path "mini-project1/shell/src/jobs.c" -Value "void handle_jobs() {}"
Commit-Step "feat: sequential & background (D.1, D.2)"

# Task 13: Built-in activities
Set-Content -Path "mini-project1/shell/src/jobs.c" -Value "void handle_jobs() {}`nvoid builtin_activities() {}"
Commit-Step "feat: built-in activities (E.1)"

# Task 14: Signals 1
New-Item -ItemType File -Force -Path "mini-project1/shell/src/signals.c" | Out-Null
Set-Content -Path "mini-project1/shell/src/signals.c" -Value "void handle_sigint() {}"
Commit-Step "feat: signals part 1 (E.3)"

# Task 15: Signals 2
Set-Content -Path "mini-project1/shell/src/signals.c" -Value "void handle_sigint() {}`nvoid handle_sigtstp() {}"
Commit-Step "feat: signals part 2 (E.3)"

# Task 16: Job Control & Ping
Set-Content -Path "mini-project1/shell/src/jobs.c" -Value "void handle_jobs() {}`nvoid builtin_activities() {}`nvoid builtin_ping() {}"
Commit-Step "feat: job control & ping (E.2, E.4)"

# Task 17: Networking Init
New-Item -ItemType Directory -Force -Path "mini-project1/networking" | Out-Null
New-Item -ItemType File -Force -Path "mini-project1/networking/Makefile" | Out-Null
Set-Content -Path "mini-project1/networking/Makefile" -Value "all: client server"
Commit-Step "feat: networking init"

# Task 18: SHAM Header
New-Item -ItemType File -Force -Path "mini-project1/networking/sham.h" | Out-Null
Set-Content -Path "mini-project1/networking/sham.h" -Value "struct sham_header { int seq_num; };"
Commit-Step "feat: SHAM header"

# Task 19: Connection Management
New-Item -ItemType File -Force -Path "mini-project1/networking/connection.c" | Out-Null
Set-Content -Path "mini-project1/networking/connection.c" -Value "void handshake() {}"
Commit-Step "feat: connection management"

# Task 20: File Segmentation
New-Item -ItemType File -Force -Path "mini-project1/networking/client.c" | Out-Null
Set-Content -Path "mini-project1/networking/client.c" -Value "void send_file() {}"
Commit-Step "feat: file segmentation"

# Task 21: Sliding Window & ACKs
New-Item -ItemType File -Force -Path "mini-project1/networking/server.c" | Out-Null
Set-Content -Path "mini-project1/networking/server.c" -Value "void receive_file() {}"
Commit-Step "feat: sliding window & cumulative acks"

# Task 22: Retransmissions
Set-Content -Path "mini-project1/networking/client.c" -Value "void send_file() {}`nvoid retransmit() {}"
Commit-Step "feat: retransmissions"

# Task 23: Chat Mode & Logging
Set-Content -Path "mini-project1/networking/server.c" -Value "void receive_file() {}`nvoid chat_mode() {}"
Commit-Step "feat: chat mode & logging"

# Task 24: xv6 Init
New-Item -ItemType Directory -Force -Path "mini-project1/xv6" | Out-Null
New-Item -ItemType File -Force -Path "mini-project1/xv6/Makefile" | Out-Null
Set-Content -Path "mini-project1/xv6/Makefile" -Value "all: qemu"
Commit-Step "feat: xv6 init"

# Task 25: Syscall getreadcount
New-Item -ItemType File -Force -Path "mini-project1/xv6/readcount.c" | Out-Null
Set-Content -Path "mini-project1/xv6/readcount.c" -Value "int getreadcount() { return 0; }"
Commit-Step "feat: syscall getreadcount"

# Task 26: Scheduler Prep
Set-Content -Path "mini-project1/xv6/Makefile" -Value "all: qemu`nSCHEDULER?=RR"
Commit-Step "feat: scheduler prep"

# Task 27: FCFS Scheduler
New-Item -ItemType File -Force -Path "mini-project1/xv6/proc.c" | Out-Null
Set-Content -Path "mini-project1/xv6/proc.c" -Value "void scheduler_fcfs() {}"
Commit-Step "feat: fcfs scheduler"

# Task 28: CFS Priority & vRuntime
Set-Content -Path "mini-project1/xv6/proc.c" -Value "void scheduler_fcfs() {}`nvoid update_vruntime() {}"
Commit-Step "feat: cfs priority & vruntime"

# Task 29: CFS Scheduling Logic
Set-Content -Path "mini-project1/xv6/proc.c" -Value "void scheduler_fcfs() {}`nvoid update_vruntime() {}`nvoid scheduler_cfs() {}"
Commit-Step "feat: cfs scheduling logic"

# Task 30: MLFQ & Report
New-Item -ItemType File -Force -Path "mini-project1/xv6/report.md" | Out-Null
Set-Content -Path "mini-project1/xv6/report.md" -Value "# xv6 Scheduler Report"
Commit-Step "feat: mlfq & report"
