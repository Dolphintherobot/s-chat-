/* Dalton Beauchesne nqr159,11353160
 * John Thoms, gvr812,11357558
 */

#include <list.h>
#include <rtthreads.h>
#include <RttCommon.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define MAXMESSAGE 99

/*#define TEST*/

/*message id's so server can 
 * id who sent the message
 */
#define KEYBOARD 4
#define SENDER 1
#define RECEIVER 2
#define PRINTER 3
#define EXIT 5

/*Group of exit codes*/
#define UNRECOGNIZEDMSG -1
#define MALLOCFAILED -2



char exitCode[6] = "EXIT\n";

/*global thread id's*/
RttThreadId serverId;
RttThreadId  transpid, keypid,
      printpid, recpid;

/*group of semphores */
RttSem sendSem;
RttSem emptySem;
RttSem printerSem;

/*group of data structures required */
LIST * transmitterMessages;
LIST * printerBox;


/*group of addresses*/
struct addrinfo *ours;
struct addrinfo *theirs;
struct addrinfo tempaddr;

/*group of sockets */
int sendSockFd;
int recvSockFd;


struct message {
  char string[2048]; /*string user typed in */
  uint32_t seconds;
  uint32_t microseconds;
  uint32_t id; /*which thread sent the message */
};

struct message* throwAwayMsg;
unsigned int * msgSize;
void * aMsg;

/*group of global variables*/
int maxMessages;
int stdinflags;

/* Performs exit procedures if exit code sent from keyboard */
void exit_program(void) {
 /* free all lists, contain malloced messages */
  ListFree(transmitterMessages, free);
  ListFree(printerBox, free);
  fcntl(0,F_SETFL,stdinflags);
  close(recvSockFd);
  close(sendSockFd);
  exit(0);
}

/*Purpose:get memory for a new message
 * return a pointer to a new Message if success
 * will exit the program if fails
 */
struct message * getNewMessage() {
  struct message * ptr;
  ptr  = (struct message *) calloc(1,sizeof(struct message));
  
  if (!ptr) {
    perror("Allocating memory for a new message failed\n");
    exit(MALLOCFAILED);
  }

  return ptr;

}

/*
 * Purpose to act as the server role in the 5
 * threads we are supposed to create for this 
 * assignment 
 
 */
void server() {
  RttThreadId * from;
  
  /*group of messages*/
  struct message * fromMessage;
  struct message * temp;

  void * data;

  unsigned int size;
  unsigned int * size2;
  int rv;


  /*group of boolean variables */
  int transmitterSleeping;
  int keyboardSleeping;
  int printerSleeping;

  transmitterSleeping = 0;
  keyboardSleeping = 0;
  printerSleeping = 0;
  
  maxMessages = MAXMESSAGE;
  from = malloc(sizeof(RttThreadId));
  size = sizeof(struct message);
  size2 = &size;
 

  while(1) {
      


    /*receive a new message */
    data = (void *) getNewMessage();
    RttReceive(from,data,size2);
    fromMessage = (struct message *) data;

    if (fromMessage -> id == KEYBOARD) {

#ifdef TEST
      printf("Server got message from the keyboard\n");
#endif




      /*put message into sender Queue*/
      RttP(sendSem);
      maxMessages--;
      ListAppend(transmitterMessages,fromMessage);
      RttV(sendSem);

      /*
       * Put message into
       * printer Queue
       */
      temp = getNewMessage();
      memcpy(temp,fromMessage,size);
      RttP(printerSem);
      ListAppend(printerBox,temp);
      RttV(printerSem);

      /*check to see if we reply to keyboard
       * or not 
       */
      maxMessages--;
      if (maxMessages) {
      rv =RttReply(keypid,(void *)throwAwayMsg,size);
        if (rv == RTTNOSUCHTHREAD) {
          perror("Error server thread failed to send \
               reply  message to keyboard, no such thread\n");
          return;
        }
        if (rv == RTTFAILED) {
          perror("Error server thread failed to send \
               reply  message to keyboard\n");
          return;
          
        }
      }    
      else keyboardSleeping  = 1;  
    }

    else if (fromMessage -> id == PRINTER) {
    /*check to see if we wake up the printer or not */

#ifdef TEST
      printf("Server got message from the printer\n");
#endif


      if (ListCount(printerBox)) {
        rv =RttReply(printpid,(void *)throwAwayMsg,size);
        if (rv == RTTNOSUCHTHREAD) {
          perror("Error server thread failed to send \
               reply  message to printer, no such thread\n");
          return;
        }

        if (rv == RTTFAILED) {
          perror("Error server thread failed to send \
               reply  message to printer\n");
          return;
        }
      }
      else printerSleeping = 1;
    }

    else if (fromMessage -> id ==SENDER) {
    
      /*check to see if we can reply to it*/
      if (ListCount(transmitterMessages)) {
        rv =RttReply(transpid,(void *)throwAwayMsg,size);
        if (rv == RTTNOSUCHTHREAD) {
          perror("Error server thread failed to send \
               reply  message to sender, no such thread\n");
          return;
        }

        if (rv == RTTFAILED) {
          perror("Error server thread failed to send \
               reply  message to sender\n");
          return;
        }      
      }
      else transmitterSleeping = 1;  
    }

    else if (fromMessage -> id == RECEIVER) {    
      
#ifdef TEST
      printf("Server got message from the Receiver\n");
#endif


      
      
      /*
       * Put message into
       * printer Queue
       */
      RttP(printerSem);
      ListAppend(printerBox,fromMessage);
      RttV(printerSem);

      rv =RttReply(recpid,(void *)throwAwayMsg,size);
      if (rv == RTTNOSUCHTHREAD) {
        perror("Error server thread failed to send \
               reply  message to receiver, no such thread\n");
        return;
      }
      if (rv == RTTFAILED) {
        perror("Error server thread failed to send \
               reply  message to receiver\n");
        return;
        }
      }

      else if (fromMessage -> id == EXIT) {
      
#ifdef TEST
        printf("Server has received exit message\n");
#endif
        
        
        /*then a thread just reports that Exit code has been  
       *entered 
       */
      free(fromMessage);
      fromMessage = NULL;
      exit_program();
      exit(0);
      return;
    }

    else {
      printf("Error Message id is not recognized \
          by the server\n");
      exit(UNRECOGNIZEDMSG);
    }


    /*check to see if we should reply to 
     * other threads sleeping
     */

    if (keyboardSleeping && maxMessages) {
      keyboardSleeping  = 0 ; 

#ifdef TEST
      printf("Server is waking up the keyboard\n");
#endif


       rv = RttReply(keypid,(void *)throwAwayMsg,size);
       if (rv == RTTNOSUCHTHREAD) {
              perror("Error server thread failed to send \
       reply  message to keyboard, no such thread\n");
          return;
            }
           if (rv == RTTFAILED) {
            perror("Error server thread failed to send \
           reply  message to keyboard\n");
        return;}

    }

    if (printerSleeping && ListCount(printerBox)) {
     
#ifdef TEST
      printf("Server is waking up the printer\n");
#endif


     
      printerSleeping  = 0;
      rv =RttReply(printpid,(void *)throwAwayMsg,size);
        if (rv == RTTNOSUCHTHREAD) {
                perror("Error server thread failed to send \
               reply  message to printer, no such thread\n");
            return;
              }
               if (rv == RTTFAILED) {
                perror("Error server thread failed to send \
               reply  message to printer\n");
            return;
        }
      
    
    }

    if (transmitterSleeping && ListCount(transmitterMessages)) {
      transmitterSleeping  = 0;

#ifdef TEST
      printf("Server is Waking up transmitter\n");
#endif


      rv =RttReply(transpid,(void *)throwAwayMsg,size);
        if (rv == RTTNOSUCHTHREAD) {
                perror("Error server thread failed to send \
               reply  message to sender, no such thread\n");
            return;
              }
               if (rv == RTTFAILED) {
                perror("Error server thread failed to send \
               reply  message to sender\n");
            return;
        }
      
    
    }



  }



  return;
}


/*Purpose:to translate a message from
 * host byte order to network byte order
 *param msg: the message you wish to conver
 */
void encode(struct message *msg)
{

  msg -> seconds = htonl(msg->seconds);
  msg -> microseconds = htonl(msg->microseconds);
  msg -> id =  htonl((uint32_t )msg ->id);
}


/*Purpose:to translate a message from
 * network byte order to host byte order
 *param msg: the message you wish to conver
 */
void decode(struct message *msg)
{

  msg -> seconds = ntohl(msg->seconds);
  msg -> microseconds = ntohl(msg->microseconds);
  msg -> id = ntohl((uint32_t )msg ->id);

}









/*
 * Will act as transmitter */
void transmitter() {

  unsigned int size;
  struct message * m;
  int rv;
  struct message * msg;
  void * ptr;
  unsigned int total;
  size = sizeof(struct message);
       while (1) {
       
          msg = getNewMessage();
    msg -> id = SENDER; 
    ptr = (void *) msg;
    rv = RttSend(serverId,ptr,size,aMsg,msgSize);
          if (rv == RTTNOSUCHTHREAD) {
              perror("Error keyboard thread failed to send \
              message to server, no such thread\n");
          return;
        
    }
           if (rv == RTTFAILED) {
              perror("Error keyboard thread failed to send \
            message to server\n");
          return;
        }

    /*server has got our message can discard it */
    free(msg);
    msg = NULL;

  /*get message from the queue
   * then send it 
   * */

  RttP(sendSem);
  m = (struct message * ) ListFirst(transmitterMessages);
        ListRemove(transmitterMessages);
    #ifdef TEST
  printf("Sender is sending message %s\n",m ->string);
#endif  
  maxMessages++;
        RttV(sendSem);
        encode(m);
       /*message should now be in network byte order */

  /*send the entire message */
      total = 0; 
         while (total < sizeof(struct message))  {
          rv = sendto(sendSockFd,(void *) m,
              sizeof(struct message),0,
                    theirs -> ai_addr,
                   theirs ->ai_addrlen);
 
         total+=rv; 
      
  if (rv <1 ) {
        perror("Sending to other client failed\n");
        return;
      }
  
  }
  /*give the message back to the heap*/

         free(m);
         m = NULL;
       }       
 
      return;
}


/* will act as receiver */
void receiver() {
  struct sockaddr their_addr;
  socklen_t size;
  unsigned int size2;
  struct message * m;
  int rv; 
  void * ptr;
  void * ptr2;
  
  size = sizeof(their_addr);
  size2 = sizeof(struct message);

 
  while(1) {

    m = getNewMessage();
    rv = 1;
     while (recvfrom( recvSockFd,(void *) m,
	sizeof(struct message), 0,&their_addr,&size) == -1) {
       RttUSleep(50);
     }


    decode(m);

#ifdef TEST
  printf("The message was %s\n",m ->string);
#endif
    
     /*send message to server
    */
    m -> id = RECEIVER;
  ptr = (void * )m;
  ptr2 = (void *) throwAwayMsg;
        rv = RttSend(serverId,ptr,size2,ptr2,msgSize);
        if (rv == RTTNOSUCHTHREAD) {
          perror("Error receiver thread failed to send \
        message to server, no such thread\n");
        return;
        }
         if (rv == RTTFAILED) {
          perror("Error receiver thread failed to send \
        message to server\n");
      return;
        
   }

         
  free(m);
  m = NULL;
}  
  
  return;
}



/* Will get keyboard input */
void keyboard_input() {
  
  struct message * m;
  int rv;
  unsigned int size;
  struct timeval tv;
  void * ptr;  
  
  size = sizeof(struct message);
  while (1) {


    m = getNewMessage();
    ptr = (void *) m;
    
    while (read(0,m ->string,sizeof(m ->string)) == -1) {
      RttUSleep(50);
    } 


#ifdef TEST
    printf("Keyboard thread got input of %s\n",m-> string);
#endif
    
    /*then we have input from user*/

    /*record time message was recevied*/
    gettimeofday(&tv,NULL);
    m->seconds = (uint32_t) tv.tv_sec;
    m->microseconds = (uint32_t) tv.tv_usec;
    /* check if exit code was entered */
    if (strcmp(m->string, exitCode) == 0) {
      m->id=EXIT;
    }
    /* otherwise, regular message */
    else
      m ->id = KEYBOARD;

    /*send the message to the server*/   
    rv = RttSend(serverId,ptr,size,aMsg,msgSize);
          if (rv == RTTNOSUCHTHREAD) {
              perror("Error keyboard thread failed to send \
              message to server, no such thread\n");
          return;
        
    }
           if (rv == RTTFAILED) {
              perror("Error keyboard thread failed to send \
            message to server\n");
          return;
        }


    /*at this point the server should have
     * copied our message 
     * so we can discard this memory address and 
     * get another
     */
    free(m);
    m = NULL;
 }
  return;
}


/*
 * Purpose to act as the printer role in the 5
 * threads we are supposed to create for this 
 * assignment 
 */
void printer() {
  struct message* msgPtr ;
  struct message * msg;
  void * ptr;
  unsigned int size;
  int rv;

  size = sizeof(struct message);

  while(1) {
  msg = getNewMessage();
   msg -> id = PRINTER; 
   ptr = (void *) msg;
     /*send the message to the server*/   
    rv = RttSend(serverId,ptr,size,aMsg,msgSize);
          if (rv == RTTNOSUCHTHREAD) {
              perror("Error keyboard thread failed to send \
              message to server, no such thread\n");
          return;
        
    }
           if (rv == RTTFAILED) {
              perror("Error keyboard thread failed to send \
            message to server\n");
          return;
        }


   /* should never be false, but just in case */
    if (ListCount(printerBox)) {
      ListFirst(printerBox);
      msgPtr = (struct message*) ListRemove(printerBox);
    }

    printf("%s - %u %u\n", msgPtr->string, msgPtr->seconds,
        msgPtr -> microseconds);

    free(msgPtr);
    free(msg);
    msgPtr = NULL;
    msg = NULL;
  }
}
int mainp(int argc,char ** argv) {

  int temp, temp1, temp2, temp3, temp4;
  RttSchAttr attr, lowAttr;
  unsigned int size;
#ifdef TEST
  char s[990];
#endif 

if (argc != 4) {
    printf("usage s-chat <RecvPort> <Machine> <SendPort>\n");
    return -1;
  }

   msgSize = &size;

   throwAwayMsg = getNewMessage();
  memset(&tempaddr,0,sizeof(struct addrinfo));
  aMsg = (void *) throwAwayMsg;
 
 /*setting address information to
   * unknown if ipv6 or ipv4
   * make it udp diagram
   * choose my ip addres for me
   */
  tempaddr.ai_flags = AI_PASSIVE;
  tempaddr.ai_family = AF_UNSPEC;
  tempaddr.ai_socktype = SOCK_DGRAM;


  if (getaddrinfo(NULL,argv[1],&tempaddr,&ours) != 0){
    printf("Error getting address info\n");
    return -1;

  }


  /*this should change the ip address to 
   * align with the machine you passed 
   * in as an argument 
   */
  tempaddr.ai_flags = AI_CANONNAME;
  /*tempaddr.ai_canonname = argv[1];*/


  if ( getaddrinfo(argv[2],argv[3],&tempaddr,&theirs) != 0) {
   printf("Error getting address info of theirs\n");
   return -1;
 
  }

  
  sendSockFd = socket(theirs -> ai_family, theirs ->ai_socktype,
      theirs -> ai_protocol);

  if (sendSockFd == -1) {
    perror("Failed to create send socket\n");
    return -1;
  }

  
  recvSockFd = socket(ours -> ai_family, ours ->ai_socktype,
      ours -> ai_protocol);

  if (recvSockFd == -1) {
    perror("Failed to create recieve socket\n");
    return -1;
  }


  if (bind(recvSockFd,ours -> ai_addr,ours -> ai_addrlen) == -1) {
    perror("Failed to bind socket\n");
    return -1;
  }


  /*save old flags for cleanup*/
  stdinflags = fcntl(0,F_GETFL);
  /*set keyboard input to not block on read */
  fcntl(0,F_SETFL, fcntl(0,F_GETFL) | O_NONBLOCK);
  fcntl(recvSockFd,F_SETFL, fcntl(recvSockFd,F_GETFL) | O_NONBLOCK);

  

  if ( RttAllocSem(&sendSem,1,RTTFCFS) == -1) {
    perror("Failed to create sendSem\n");
    return -1;
  }

  if ( RttAllocSem(&emptySem,1,RTTFCFS) == -1) {
    perror("Failed to create emptSem\n");
    return -1;
  }
  
  if ( RttAllocSem(&printerSem,1,RTTFCFS) == -1) {
    perror("Failed to create printerSem\n");
    return -1;
  }


  transmitterMessages = ListCreate();
  printerBox = ListCreate();



  #ifdef TEST
  inet_ntop(ours ->ai_addr -> sa_family,
  ours -> ai_addr,
  s, sizeof s);
  printf("server: got connection from %s\n", s);

  inet_ntop(theirs ->ai_addr -> sa_family,
  theirs -> ai_addr,
  s, sizeof s);
  printf("server: also got connection from %s\n", s);
  #endif



  attr.startingtime = RTTZEROTIME;
  attr.priority = RTTNORM;
  attr.deadline = RTTNODEADLINE;
  
  lowAttr.startingtime = RTTZEROTIME;
  lowAttr.priority = RTTLOW;
  lowAttr.deadline = RTTNODEADLINE;
  
  
  temp3 = RttCreate(&printpid, (void(*)()) printer, 10000, "print", NULL, attr,
    RTTUSR);
  if (temp3 == RTTFAILED)
    perror("RttCreate failed\n");

  temp1 = RttCreate(&transpid, (void(*)()) transmitter, 10000, "trn", NULL, 
    attr, RTTUSR);
  if (temp1 == RTTFAILED)
    perror("RttCreate failed\n");
  
  temp2 = RttCreate(&keypid, (void(*)()) keyboard_input, 10000, "key", NULL,
    attr, RTTUSR);
  if (temp2 == RTTFAILED)
    perror("RttCreate failed\n");

  temp4 = RttCreate(&recpid, (void(*)()) receiver, 100000, "rec", NULL, attr,
    RTTUSR);
  if (temp4 == RTTFAILED)
   perror("RttCreate failed\n");
 

  temp = RttCreate(&serverId, (void(*)()) server, 10000, "srv", NULL, lowAttr,
    RTTUSR);
  if (temp == RTTFAILED)
   perror("RttCreate failed\n"); 



  /* Calling each function in a serparate thread*/
  
 return 0;
}




