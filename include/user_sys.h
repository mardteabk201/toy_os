#ifndef	_USER_SYS_H
#define	_USER_SYS_H

void user_delay(unsigned long);
void call_sys_write(char * buf);
int call_sys_fork();
void call_sys_exit();

#endif  /*_USER_SYS_H */
