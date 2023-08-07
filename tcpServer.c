#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>


int
main ()
{
  short int n = 0, m = 0;
  int len = 100;
  int addrlen;
  unsigned short serv_port = 25000;
  short sockfd, connfd;
  char serv_ip[] = "127.0.0.1";
  char mesg[1000];
  char *client_add_dotted;
  short client_port;
  struct sockaddr_in servaddr, clientaddr;
  client_add_dotted = (char *) malloc (sizeof (servaddr));
  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    {
      printf ("failed");
      exit (1);
    }
  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  inet_aton (serv_ip, &servaddr.sin_addr);
  servaddr.sin_port = htons (serv_port);
  bind (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
  listen (sockfd, 1);
  connfd = accept (sockfd, (struct sockaddr *) &clientaddr, &addrlen);
  n = read (connfd, mesg, 100);
  if (n != -1)
    printf ("\n\t\t\t***receive success***\n");
  else
    printf ("\n\t\t\t***receive failure***\n");
  m = write (connfd, mesg, n);
  if (m != -1)
    printf ("\n\t\t\t***send success***\n");
  else
    printf ("\n\t\t\t***send failure***\n");
  close (sockfd);
}
