# IPC_with_c_problem_25

### A. PROBLEM STATEMENT
Design  a  program  using  concepts  of  inter-process  communication  ordinary  pipes  in which  one  process  sends  a  string  message  to  a  second  process,  and  the  second  process reverses the case of each character in the message and sends it back to the first process.

**For  example**
  > *If  the  first  process  sends  the  message  "Hi  There"*
  >
  > *The  second  process  will return "hI tHERE".* 

This will require using two pipes, one for sending the original message from the first to the second process and the other for sending the modified message from the  second  to  the  first  process.  You  can  write  this  program  using  either  UNIX  or Windows pipes.
