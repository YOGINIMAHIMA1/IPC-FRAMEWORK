#                                                                     Bug Report

`Title: Semaphore Not Released Properly in IPC Testing`

#                                                           Description:
In the `ipc_test` program, the semaphore operations do not seem to work as expected. The semaphore is acquired but not properly released, leading to a hang in the critical section where it is supposed to signal and exit.

#                                              Steps to Reproduce:
1. Compile the code using the provided `Makefile`.
2. Run the `ipc_test` executable.
3. Observe the output related to semaphore operations.

#                                              Expected Behavior:
#                                 The output should include:
- "Waiting on semaphore."
- "Semaphore acquired, entering critical section."
- Simulated work in the critical section.
- "Semaphore released, exiting critical section."

#                                           Actual Behavior:
The output may hang at "Waiting on semaphore." or "Semaphore acquired, entering critical section." without proceeding to "Semaphore released, exiting critical section."

#                                      Code Involved:
The issue appears to be related to the semaphore operations defined in `ipc.cpp`, specifically in the `test_semaphore()` function.

#                               Additional Information:
- Semaphore initialization in `initSemaphore()` includes setting the initial semaphore value to 1.
- The program hangs at semaphore wait, indicating a potential issue with semaphore signaling or initialization.

#                            Suggested Fix:
- Verify semaphore initialization and ensure that the semaphore value is correctly set.
- Add more detailed debugging information to `semaphoreWait()` and `semaphoreSignal()` to trace if and where errors occur.
- Ensure that semaphore operations are executed correctly and that resources are not locked indefinitely.

#                             Attachments:
- `ipc.cpp`
- `Makefile`

`Priority: High`

`Status: Closed`
