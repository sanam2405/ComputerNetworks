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
  int sockfd, returnvalue, len = 100, n;
  unsigned int addrlen;
  unsigned short serv_port = 25000;
  char serv_ip[] = "127.0.0.1";

  char text[100];
  
  struct sockaddr_in servaddr;
  addrlen = sizeof (servaddr);
  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons (serv_port);
  inet_aton (serv_ip, &servaddr.sin_addr);
  sockfd = socket (PF_INET, SOCK_STREAM, 0);
  connect (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

  printf("Input a message : ");
  scanf("%[^\n]%*c",text);

  int totalLength = strlen(text);
  int windowSize = 10;
  int iterations = totalLength/windowSize;
  int completeCycles = totalLength/windowSize;
  int left = totalLength%windowSize;
  int serialNumber = 1;
  int currentPos = 0;
  while(iterations--)
  {
      char currentWindow[16];
      
      int firstDigOfSerialNum = serialNumber%10;
      int secondDigOfSerialNum = serialNumber/10;

      currentWindow[0] = (char) (secondDigOfSerialNum+48);
      currentWindow[1] = (char) (firstDigOfSerialNum+48);

      currentWindow[2] = '1';
      currentWindow[3] = '0';

      for(int i = 0; i < 10; i++)
        currentWindow[i+4] = text[currentPos++];

    if(totalLength%windowSize==0 && serialNumber==completeCycles)
        currentWindow[14] = '1';
    else if(totalLength%windowSize==0 && serialNumber!=completeCycles)
        currentWindow[14] = '0';
    else if(left)
        currentWindow[14] = '0';
    else
        currentWindow[14] = '1';
    
    currentWindow[15] = '\0';
    returnvalue = write (sockfd, currentWindow, strlen (currentWindow));
    if (returnvalue != -1)
    printf ("\n* sent successfully window number %d *\n",serialNumber);
    else
    printf ("\n* sent failure *\n");
    serialNumber++;
   
    char rtext[100];
    n = read (sockfd, rtext, len);
    rtext[n] = '\0';
    printf ("\n* %s *\n", rtext);
  }

  if(left)
  {
      char currentWindow[6+left];
      int firstDigOfSerialNum = serialNumber%10;
      int secondDigOfSerialNum = serialNumber/10;

      currentWindow[0] = (char) (secondDigOfSerialNum+48);
      currentWindow[1] = (char) (firstDigOfSerialNum+48);

      currentWindow[2] = '0';
      currentWindow[3] = (char) (left+48);

      for(int i = 0; i < left; i++)
        currentWindow[i+4] = text[currentPos++];

        currentWindow[4+left] = '1';

        currentWindow[5+left] = '\0';
    returnvalue = write (sockfd, currentWindow, strlen (currentWindow));
    if (returnvalue != -1)
    printf ("\n* sent successfully window number %d *\n",serialNumber);
    else
    printf ("\n* sent failure *\n");

    char rtext[100];
    n = read (sockfd, rtext, len);
    rtext[n] = '\0';
    printf ("\n* %s *\n", rtext);
  }
  close (sockfd);
}
