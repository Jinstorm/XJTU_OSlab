#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <bits/signum.h>

void sig_handler (int sig) {
    if (sig == SIGALRM) {
        puts ("+1s...");
        alarm (1);  // 重新设置1秒定时器
    }
}

int main() {

    struct sigaction sa;
    bzero (&sa, sizeof (sa) );
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    assert (sigaction (SIGALRM, &sa, NULL) != -1);

    alarm(1);

    while (1) {
        sleep (1);
    }
    return 0;
}
