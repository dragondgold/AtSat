#include <stdio.h>

#include "thread.h"
#include "msg.h"
#include "config/cansat.h"

void *second_thread(void *arg)
{
    (void) arg;

    printf("2nd thread started, pid: %" PRIkernel_pid "\n", thread_getpid());
    msg_t m;

    while (1) {
        msg_receive(&m);
        printf("2nd: Got msg from %" PRIkernel_pid "\n", m.sender_pid);
        m.content.value++;
        msg_reply(&m, &m);
    }

    return NULL;
}

char second_thread_stack[THREAD_STACKSIZE_MAIN];

int main(void)
{
	cansat_init();

    printf("Starting IPC Ping-pong example...\n");
    printf("1st thread started, pid: %" PRIkernel_pid "\n", thread_getpid());

    msg_t m;

    kernel_pid_t pid = thread_create(second_thread_stack, sizeof(second_thread_stack),
                            THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                            second_thread, NULL, "pong");

    m.content.value = 1;

    while (1) {
        msg_send_receive(&m, &m, pid);
        printf("1st: Got msg with content %u\n", (unsigned int)m.content.value);
    }
}
