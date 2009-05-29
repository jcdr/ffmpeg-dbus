  #include <stdio.h>
  #include <memory.h>
  #include <unistd.h>

  int main( int argc, char ** argv )
  {

  
   /* create the pipe */
   int pfd[2];
     char buffer[BUFSIZ+1];

char * parmList[]= //{"ls","-l",NULL};
 {"/usr/bin/ffmpeg", "-f","video4linux2","-s","640x480","-r","15","-i","/dev/video0","-vcodec","mjpeg","-f","avi","out.avi",NULL}; 
   if (pipe(pfd) == -1)
     {
       printf("pipe failed\n");
       return 1;
     }

   /* create the child */
   int pid;
   if ((pid = fork()) < 0)
     {
       printf("fork failed\n");
       return 2;
     }

   if (pid == 0)
     {
       /* child */
       close(pfd[0]); /* close the unused read side */
       dup2(pfd[1], 1); /* connect the write side with stdout */
       close(pfd[1]); /* close the write side */
       /* execute the process (wc command) */
       execvp("ffmpeg", parmList);
       printf("ls failed"); /* if execlp returns, it's an error */
       return 3;
     }
   else
     {
       /* parent */
       close(pfd[1]); /* close the unused write side */
       while (read(pfd[0], buffer, BUFSIZ) != 0)
      {
         printf("parent reads ");
   
      }
       close(pfd[0]); /* close the read side */
       /* execute the process (ls command) */
 
       return 4;
     }
   return 0;
  }
