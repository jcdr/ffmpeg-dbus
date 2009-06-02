#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <signal.h>



int level = 21614,width=640,height=480
    ,imrate=25
    ,bitrate=200
    ,qscale=6,ok=0,pid;
char *clip = "127.0.0.1";int *status=1;
int pfd[2];
char buffer[BUFSIZ+1];
void readpipe()
{
int i=0;
if (status==0) 
{
   close(pfd[1]); 
    while (read(pfd[0], buffer, BUFSIZ) != 0)
      { 
        i=i+1;
        printf("%s \n",buffer);
        if (i==100) return;
   
      }

} 
return;
}


void send_ok(DBusMessage* msg,DBusConnection* conn)
{
   DBusMessage* reply;
   DBusMessageIter args;
   dbus_uint32_t serial = 0;

   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &ok)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);

}
void start(DBusMessage* msg,DBusConnection* conn)
{
char carac[15];
char *temp;

DBusMessageIter args;

char * parmList[] = 
   {"/usr/bin/ffmpeg", "-f","video4linux2","-s"}; 
if (status==0)
{
 send_ok(msg,conn);
 return;
}
// create the pipe
   if (pipe(pfd) == -1)
     {
       printf("pipe failed\n");
       return ;
     }
status=0;
 printf("video started \n");

   /* create the child */
   if ((pid = fork()) < 0)
     {
       printf("fork failed\n");
       return ;
     }
 
   if (pid == 0)
     {
       /* child */
           close(pfd[0]); /* close the unused read side */
           dup2(pfd[1], 2); /* connect the write side with stderr */
           dup2(pfd[1],1);  /* connect the write side with stdout */
       	   close(pfd[1]); /* close the write side */
           sprintf(carac, "%d", width);
	   strcat(carac,"x");
	   temp=strdup(carac);
	   sprintf(carac, "%d", height);
	   strcat(temp,carac);
	   parmList[4]=strdup(temp);
	   parmList[5]=strdup("-r");
	   sprintf(carac, "%d", imrate);
	   parmList[6]=strdup(carac);
	   parmList[7]=strdup("-b");
 	   sprintf(carac, "%d", bitrate);
 	   parmList[8]=strdup(carac);
   	   parmList[9]=strdup("-qscale");
	   sprintf(carac, "%d", qscale);
	   parmList[10]=strdup(carac);
	   parmList[11]=strdup("-i");
	   parmList[12]=strdup("/dev/video0");
	   parmList[13]=strdup("-f");
	   parmList[14]=strdup("mjpeg");
	   temp=strdup("udp:");
	   strcat(temp,clip);
	   strcat(temp,":1234");
	   parmList[15]=strdup(temp);
	   parmList[16]=NULL;
	   execvp("ffmpeg", parmList);
	   printf("Return not expected. Must be an execvp() error.\\n");

      
     }
   else
     {
       /* parent */
    send_ok(msg,conn);
   

     }



}

void stop (DBusMessage* msg,DBusConnection* conn)
{  
   kill(pid,SIGKILL);
   printf("it's killed \n");
   printf("\n");
   // create a reply from the message
    send_ok(msg,conn);
    waitpid(pid,&status,0);

}


void image_size_set(DBusMessage* msg,DBusConnection* conn)
{  
   DBusMessageIter args;
   // read the arguments
   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not integer!\n");
   dbus_message_iter_get_basic(&args, &width);
   if (!dbus_message_iter_next(&args))
      fprintf(stderr, "Message has too few arguments!\n"); 
   else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) 
      fprintf(stderr, "Argument is not integer!\n"); 
   else
      dbus_message_iter_get_basic(&args, &height);


   printf("image width set:  %d\n",width);
   printf("image height set:  %d\n",height);
   send_ok(msg,conn);
}


void image_size_get(DBusMessage* msg,DBusConnection* conn)
{
   DBusMessage* reply;
   DBusMessageIter args;
   dbus_uint32_t serial = 0;

   // create a reply from the message
   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &width)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &height)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);
}




void image_rate_set(DBusMessage* msg,DBusConnection* conn)
{
   DBusMessageIter args;
   // read the arguments
   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not integer!\n");
   dbus_message_iter_get_basic(&args, &imrate);
   printf("image rate set:  %d\n",imrate);
    send_ok(msg,conn);

}
void image_rate_get(DBusMessage* msg,DBusConnection* conn)
{

   DBusMessage* reply;
   DBusMessageIter args;
   dbus_uint32_t serial = 0;

   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &imrate)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);

}



void bit_rate_set(DBusMessage* msg,DBusConnection* conn)
{
   DBusMessageIter args;
   // read the arguments
   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not integer!\n");
   dbus_message_iter_get_basic(&args, &bitrate);
   printf("Bit rate set:  %d\n",bitrate);
    send_ok(msg,conn);
}

void bit_rate_get(DBusMessage* msg,DBusConnection* conn)
{

   DBusMessage* reply;
   DBusMessageIter args;
   dbus_uint32_t serial = 0;

   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &bitrate)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);

}


void qscale_set(DBusMessage* msg,DBusConnection* conn)
{
   DBusMessageIter args;
   // read the arguments
   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not integer!\n");
   dbus_message_iter_get_basic(&args, &qscale);
   printf("qscale rate set:  %d\n",qscale);
    send_ok(msg,conn);

}
void qscale_get(DBusMessage* msg,DBusConnection* conn)
{

   DBusMessage* reply;
   DBusMessageIter args;
   dbus_uint32_t serial = 0;

   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &qscale)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);

}

void client_ip_set(DBusMessage* msg,DBusConnection* conn)
{
   DBusMessageIter args;

   // read the arguments
   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not string!\n");
   char *text;
   dbus_message_iter_get_basic(&args, &text);
   if (clip) {
	   free(clip);
   }
   clip = strdup(text);

  printf(clip);
  printf("\n");
    send_ok(msg,conn);
}
void client_ip_get(DBusMessage* msg,DBusConnection* conn)
{

   DBusMessage* reply;
   DBusMessageIter args;
   dbus_uint32_t serial = 0;

   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &clip)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);

}

void do_introspect(DBusMessage* msg,DBusConnection* conn)
{
   DBusMessage* reply;
   dbus_uint32_t serial = 0;
      if (dbus_message_is_method_call(msg, "org.freedesktop.DBus.Introspectable", "Introspect")) 
	{
		printf("must do introspection \n");
	         static const char* introspect_xml =
	  	"<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\"\n"
	 	 "                      \"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">\n"
		  "<node>\n"
		  "  <interface name=\"ch.cett.misse.ffmpeg\">\n"
	  	"    <method name=\"image_rate_get\">\n"
	  	"      <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"start\">\n"
	  	"    <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"stop\">\n"
	  	"    <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"image_size_set\">\n"
                "    <arg name=\"size\"  direction=\"in\" type=\"ii\" /> \n"
	  	"    <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"image_size_get\">\n"
	  	"    <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"image_rate_set\">\n"
                "    <arg name=\"size\"  direction=\"in\" type=\"i\" /> \n"
	  	"    <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"bit_rate_set\">\n"
                "    <arg name=\"size\"  direction=\"in\" type=\"i\" /> \n"
	  	"    <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"bit_rate_get\">\n"
	  	"      <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"qscale_set\">\n"
                "    <arg name=\"size\"  direction=\"in\" type=\"i\" /> \n"
	  	"    <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"qscale_get\">\n"
	  	"      <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"client_ip_set\">\n"
                "    <arg name=\"size\"  direction=\"in\" type=\"s\" /> \n"
	  	"    <arg name=\"port\" direction=\"out\" type=\"i\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"client_ip_get\">\n"
	  	"      <arg name=\"port\" direction=\"out\" type=\"s\"/>\n"
	  	"    </method>\n"
	 	 "  </interface>\n"
	 	 "</node>\n";
		reply = dbus_message_new_method_return(msg);
               if (!dbus_message_append_args (reply,
                                 DBUS_TYPE_STRING, &introspect_xml,
                                 DBUS_TYPE_INVALID))
		{
		      fprintf(stderr, "Out Of Memory!\n");
		      exit(1);
		}

		if (!dbus_connection_send(conn, reply, &serial)) 
		{
      		fprintf(stderr, "Out Of Memory!\n");
      		exit(1);
   		}
   		dbus_connection_flush(conn);

		dbus_message_unref(reply);
	}
      if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "image_rate_get")) 
	{
         image_rate_get(msg, conn);
	} 
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "start")) 
	{
	 start(msg,conn);
         readpipe();
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "stop")) 
	{
	 stop(msg,conn);
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "image_size_set")) 
	{
	 image_size_set(msg,conn);
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "image_size_get")) 
	{
	 image_size_get(msg,conn);
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "image_rate_set")) 
	{
	 image_rate_set(msg,conn);
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "bit_rate_set")) 
	{
	 bit_rate_set(msg,conn);
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "bit_rate_get")) 
	{
	 bit_rate_get(msg,conn);
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "qscale_set")) 
	{
	 qscale_set(msg,conn);
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "qscale_get")) 
	{
	 qscale_get(msg,conn);
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "client_ip_set")) 
	{
	 client_ip_set(msg,conn);
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "client_ip_get")) 
	{
	 client_ip_get(msg,conn);
	}
/*
	else 
	 {
	      fprintf(stderr, "Unknown message: member=%s\n", dbus_message_get_member(msg));
         }
*/
      // free the message
      dbus_message_unref(msg);
}




/**
* Server that exposes a method call and waits for it to be called
*/
void listen()
{
   DBusMessage* msg;
   DBusConnection* conn;
   DBusError err;
   int ret;
   printf("Listening for method calls\n");

   // initialise the error
   dbus_error_init(&err);

   // connect to the bus and check for errors
   conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
   if (dbus_error_is_set(&err)) {
      fprintf(stderr, "Connection Error (%s)\n", err.message);
      dbus_error_free(&err);
   }
   if (NULL == conn) {
      fprintf(stderr, "Connection Null\n");
      exit(1);
   }

   // request our name on the bus and check for errors
   ret = dbus_bus_request_name(conn, "ch.cett.misse.ffmpeg",
   DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
   if (dbus_error_is_set(&err)) {
      fprintf(stderr, "Name Error (%s)\n", err.message);
      dbus_error_free(&err);
   }
   if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
      fprintf(stderr, "Not Primary Owner (%d)\n", ret);
      exit(1);
   }

   // loop, testing for new messages
   while (true)
   {
      // non blocking read of the next available message
      dbus_connection_read_write(conn, 0);
      msg = dbus_connection_pop_message(conn);

      // loop again if we haven't got a message
      if (NULL == msg) {
         sleep(1);
         continue;
      }

      // check this is a method call for the right interface & method

      do_introspect(msg,conn);
   }

}




int main(int argc, char** argv)
{
clip = strdup("127.0.0.1");
listen();

return 0;
}

