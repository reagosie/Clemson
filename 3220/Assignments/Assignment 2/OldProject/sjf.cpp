int sjf() {
  same start as FIFO
  once you get a task, check if something is waiting
    check if the waiting task is shorter!
    if waiting task is shorter, start that task
    constantly check the waiting list and incoming tasks for a shorter task than currTask
      IF you preempt something, put the OLD currTask on the wait list with its remaining work left
  Everything else the same as FIFO
}
