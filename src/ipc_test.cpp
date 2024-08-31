#include "../include/ipc.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    test_message_queue();
    test_shared_memory();
    test_semaphore();
    test_concurrency();
    test_integration();

    return 0;
}

