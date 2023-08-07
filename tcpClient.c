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
  int sockfd, returnvalue, len = 100, n;
  unsigned int addrlen;
  unsigned short serv_port = 25000;
  char serv_ip[] = "127.0.0.1";
  char text[] = "abcde";
  char rtext[100];
  struct sockaddr_in servaddr;
  addrlen = sizeof (servaddr);
  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons (serv_port);
  inet_aton (serv_ip, &servaddr.sin_addr);
  sockfd = socket (PF_INET, SOCK_STREAM, 0);
  connect (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
  returnvalue = write (sockfd, text, strlen (text));
  if (returnvalue != -1)
    printf ("\n* sent successfully *\n");
  else
    printf ("\n* sent failure *\n");
  n = read (sockfd, rtext, len);
  rtext[n] = '\0';
  printf ("\n* %s *\n", rtext);
  close (sockfd);
}