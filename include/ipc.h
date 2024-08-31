#ifndef IPC_H
#define IPC_H

#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstring>
#include <unistd.h>

// Message Queue structure
struct Message {
    long msg_type;
    char msg_text[100];
};

// Message Queue Functions
int initMessageQueue(key_t key);
int sendMessage(int msgid, const Message& message);
int receiveMessage(int msgid, Message& message, long msg_type);

// Shared Memory Functions
int initSharedMemory(key_t key, size_t size);
void* attachSharedMemory(int shmid);

// Semaphore Functions
int initSemaphore(key_t key, int num_sems);
int semaphoreWait(int semid, int sem_num);
int semaphoreSignal(int semid, int sem_num);

// IPC Framework Initialization
void initIPCFramework();

// Testing Functions
void test_message_queue();
void test_shared_memory();
void test_semaphore();
void test_concurrency();
void test_integration();

#endif // IPC_H
