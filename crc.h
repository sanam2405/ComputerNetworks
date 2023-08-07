#ifndef CRCH
#define CRCH
unsigned short crc_16(unsigned char [100],unsigned short);

unsigned short crc_16(unsigned char input[100],unsigned short len)
{
unsigned short crc=0xFFFF;
unsigned short temp,i,j;
unsigned char msg[100];
memcpy(msg,input,len);
for(j=0;j<len;j++)
{
for(i=0;i<=7;i++)
{
temp=(crc>>15)^(msg[j]>>7);
crc<<=1;
if(temp)
{
crc^=0x1021;
}
msg[j]<<=1;
}
}
//printf("%x",crc);
return crc;
}
#endif
