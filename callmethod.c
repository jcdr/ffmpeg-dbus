#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()

{
   pid_t pid;
printf("/dev/video0 \n");
 char *const parmList[] = 
   {"/usr/bin/ffmpeg", "-f","video4linux2 ","-s","640x480 ","-r "," 15"," -i", " /dev/video0 ", "-f", "mjpeg","udp:127.0.0.1:1234",  NULL}; 

   if ((pid = fork()) == -1)
      perror("fork() error");
   else if (pid == 0) {
      execvp("ffmpeg", parmList);

return 0;
   }
}
