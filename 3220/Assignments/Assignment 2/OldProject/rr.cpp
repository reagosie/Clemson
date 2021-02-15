int rr() {
  same start as FIFO
  once you get a task, check if something is waiting
    build waitlist with same indexes as tasks
    so task A needs to reside in waitList[0], task B in waitList[1], etc.
  only work on a task for 1 time unit, then go to waitList[currTask+1]
    when you reach the end of waitList, start over at [0]
  after working on a task for 1 time unit, check if it is done
  when a task is done, remove it from the waitList and perform completed task routines
}
