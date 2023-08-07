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

    int n_frames, N; // N = sliding window size
    printf("Enter the number of frames: ");
    scanf("%d", &n_frames);
    printf("Enter the sliding window size: ");
    scanf("%d", &N);

    int i = 1; // stores the frame to be sent next.
    int problem_occurred = 0; // a flag to indicate whether there was a lost or damaged frame.
    
    printf("\n");

    while(i <= n_frames)
    {
        //sender
        printf("Frames sent: ");

        for(int j = i; j < min(i+N, n_frames+1); j++)
            printf("%d ", j);
        
        printf("\n");

        //receiver
        problem_occurred = 0;

        for(int j = i; j < min(i+N, n_frames+1); j++)
        {

            if(damaged_lost()) //if the current frame is damaged or lost
            {
                problem_occurred = 1;
                break;
            }
        }

        //sender again

        if(!problem_occurred) //if everything went fine
        {
            printf("Acknowledgment received for Frames %d to %d\n\n", i, min(i+N-1, n_frames));
            i += N;
            continue;
        }

        //if a frame was lost or damaged, we have to send all the frames in the current window again.
        printf("No acknowledgement received\n\nSending all the frames in the current window again\n");
    }
}