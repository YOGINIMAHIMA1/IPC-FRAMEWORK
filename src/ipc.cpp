#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

// Message Queue Functions


int initMessageQueue(key_t key) {
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    return msgid;
}
int sendMessage(int msgid, const Message& message) {
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd");
        return -1;
    }
    return 0;
}

int receiveMessage(int msgid, Message& message, long msg_type) {
    if (msgrcv(msgid, &message, sizeof(message.msg_text), msg_type, 0) == -1) {
        perror("msgrcv");
        return -1;
    }
    return 0;
}

// Shared Memory Functions
int initSharedMemory(key_t key, size_t size) {
    int shmid = shmget(key, size, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    return shmid;
}

void* attachSharedMemory(int shmid) {
    void* shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shmaddr;
}

// Semaphore Functions
int initSemaphore(key_t key, int num_sems) {
    int semid = semget(key, num_sems, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphore to 1 (binary semaphore)
    semctl(semid, 0, SETVAL, 1);
   
    return semid;
}

int semaphoreWait(int semid, int sem_num) {
    struct sembuf sops = {static_cast<unsigned short>(sem_num), -1, 0};
    if (semop(semid, &sops, 1) == -1) {
        perror("semop wait");
        return -1;
    }
    return 0;
}

int semaphoreSignal(int semid, int sem_num) {
    struct sembuf sops = {static_cast<unsigned short>(sem_num), 1, 0};
    if (semop(semid, &sops, 1) == -1) {
        perror("semop signal");
        return -1;
    }
    return 0;
}

// IPC Framework Initialization
void initIPCFramework() {
    key_t key = ftok("progfile", 65);
    int msgid = initMessageQueue(key);
    int shmid = initSharedMemory(key, 1024);
    int semid = initSemaphore(key, 1);

    Message message;
    message.msg_type = 1;
    strcpy(message.msg_text, "Hello from Message Queue!");
    sendMessage(msgid, message);
    receiveMessage(msgid, message, 1);
    std::cout << "Received Message: " << message.msg_text << std::endl;

    char* shared_data = static_cast<char*>(attachSharedMemory(shmid));
    strcpy(shared_data, "Shared Memory Data");

    semaphoreWait(semid, 0);
    std::cout << "Shared Memory Content: " << shared_data << std::endl;
    semaphoreSignal(semid, 0);
}

// Testing Functions
void test_message_queue() {
    key_t key = ftok("progfile", 65);
    int msgid = initMessageQueue(key);

    Message message;
    message.msg_type = 1;
    strcpy(message.msg_text, "Test Message Queue");
    sendMessage(msgid, message);

    receiveMessage(msgid, message, 1);
    std::cout << "Message Queue Test - Received: " << message.msg_text << std::endl;
}

void test_shared_memory() {
    key_t key = ftok("progfile", 65);
    int shmid = initSharedMemory(key, 1024);

    char* shared_data = static_cast<char*>(attachSharedMemory(shmid));
    strcpy(shared_data, "Test Shared Memory");
   
    std::cout << "Shared Memory Test - Content: " << shared_data << std::endl;
}

void test_semaphore() {
    key_t key = ftok("progfile", 65);
    int semid = initSemaphore(key, 1);

    std::cout << "Waiting on semaphore." << std::endl;
    if (semaphoreWait(semid, 0) == -1) {
        std::cerr << "Semaphore wait failed." << std::endl;
        return;
    }
    std::cout << "Semaphore acquired, entering critical section." << std::endl;
    // Simulate some work in the critical section
    sleep(1); // Replace with actual work
    if (semaphoreSignal(semid, 0) == -1) {
        std::cerr << "Semaphore signal failed." << std::endl;
        return;
    }
    std::cout << "Semaphore released, exiting critical section." << std::endl;
}

void test_concurrency() {
    pid_t pid = fork();
    if (pid == 0) {
        std::cout << "Child process starting tests." << std::endl;
        test_message_queue();
        test_shared_memory();
        test_semaphore();
        std::cout << "Child process tests completed." << std::endl;
        exit(0);
    } else if (pid > 0) {
        std::cout << "Parent process waiting for child." << std::endl;
        test_message_queue();
        test_shared_memory();
        test_semaphore();
        wait(NULL);
        std::cout << "Parent process tests completed." << std::endl;
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

void test_integration() {
    std::cout << "Integration test starting." << std::endl;
    initIPCFramework();
    std::cout << "Integration test completed." << std::endl;
