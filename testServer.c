#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>


int main ()
{
  short int n = 0, m = 0;
  int len = 100;
  int addrlen;
  unsigned short serv_port = 25000;
  short sockfd, connfd;
  char serv_ip[] = "127.0.0.1";
  char mesg[1000];
  char totalMessage[1000];
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

  int pos = 0;
  while(1)
  {
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
  mesg[n] = '\0';

  printf("\n\t\t\t***Current message is***\n");
  printf("\n\t\t\t***%s***\n",mesg);

  for(int i = 4; i < n-1; i++)
  totalMessage[pos++] = mesg[i];
  if(mesg[n-1]=='1')
  break;
  }

  close (sockfd);
  totalMessage[pos] = '\0';
  printf("\n\t\t\t***The complete message is***\n");
  printf("\n\t\t\t***%s***\n",totalMessage);
  
}