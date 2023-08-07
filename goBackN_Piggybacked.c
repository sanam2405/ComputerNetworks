#include <stdio.h>
#include <time.h>
#include <stdlib.h>


// return whether the current frame has been damaged or lost.
int damaged_lost()
{
    return (rand()%5 == 0); //a frame is lost or damaged with probability of 20%.
}

int min(int a, int b)
{
    return (a < b)? a : b;
}

int main()
{
    srand(time(NULL));

    int n_frames1, n_frames2, N1, N2; // N1 & N2 are the sliding window size for senders 1 & 2 respectively
    printf("Enter the number of frames to be sent by sender 1: ");
    scanf("%d", &n_frames1);
    printf("Enter the sliding window size for sender 1: ");
    scanf("%d", &N1);
    printf("Enter the number of frames in sender 2: ");
    scanf("%d", &n_frames2);
    printf("Enter the sliding window size for sender 2: ");
    scanf("%d", &N2);

    int next_frame1 = 1; // stores the next frame to be sent by server 1.
    int next_frame2 = 1;
    int flag = 0; // stores the frame number of lost or damaged frame.

    int send1_done = 0; //a flag to store whether the sender1 has sent all its frames.
    int send2_done = 0;

    printf("\n");

    while(!(send1_done & send2_done)) //while both of them have not become true
    {
        //sender1

        if(!send1_done)
        {
            printf("Frames sent by sender1: ");

            for(int j = next_frame1; j < min(next_frame1+N1, n_frames1+1); j++)
                printf("%da ", j); //suffix a represents frames that are to be sent by sender1.
            
            printf("\n");
        }

        //sender2

        flag = 0;

        for(int j = next_frame1; j < min(next_frame1 + N1, n_frames1+1); j++)
        {

            if(damaged_lost()) //if the current frame is damaged or lost
            {
                flag = j;
                break;
            }
        }

        if(!send1_done)
        {
            //if a frame was lost or damaged, the window slides to start from that frame.
            if(flag)
            {
                printf("\nSender 2:\nFrame %da was lost or damaged\n", flag); //sending NACK.
                next_frame1 = flag;
            }

            else
            {
                printf("\nSender 2:\nAcknowledgment sent for Frames %da to %da\n", next_frame1, min(next_frame1+N1-1, n_frames1));
                next_frame1 += N1;
            }
        }

        //sender 2 sends its packets

        if(!send2_done)
        {
            printf("Frames sent by sender2: ");

            for(int j = next_frame2; j < min(next_frame2+N2, n_frames2+1); j++)
                printf("%db ", j);
            
            printf("\n");
        }

        flag = 0;

        //sender2 receives acknowledgements
        for(int j = next_frame2; j < min(next_frame2 + N2, n_frames2+1); j++)
        {

            if(damaged_lost()) //if the current frame is damaged or lost
            {
                flag = j;
                break;
            }
        }

        if(!send2_done)
        {
            //if a frame was lost or damaged, the window slides to start from that frame.
            if(flag)
            {
                printf("\nSender 1:\nFrame %db was lost or damaged\n", flag); //sending NACK.
                next_frame2 = flag;
            }

            else
            {
                printf("\nSender 1:\nAcknowledgment sent for Frames %db to %db\n", next_frame2, min(next_frame2+N2-1, n_frames2));
                next_frame2 += N2;
            }
        }


        if(next_frame1 > n_frames1)
            send1_done = 1;
        
        if(next_frame2 > n_frames2)
            send2_done = 1;

    }
}