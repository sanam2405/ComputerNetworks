#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

typedef enum {frame_arrival, timeout} event_type;

typedef enum {data, acknowledgement} frame_kind;

int current_seq_number = 1;

typedef struct
{
    frame_kind kind;
    int seq; // sequence number
    int ack; //acknowledgement number/ sequence number of the received frame
    int info;
} frame;

frame *interface_buffer;

event_type global_event = frame_arrival;

//returns a random integer between 1 and 10
int get_random()
{
    return (int)(1 + 9.0*rand()/(float)RAND_MAX);
}

void wait_for_event(event_type *event)
{
    while(global_event != frame_arrival){};
    *event = global_event;
}

int get_data()
{
    int data;
    printf("\nEnter packet info (an integer): ");
    scanf("%d", &data);
    return data;
}

frame* make_frame(frame_kind kind, int info, int acknowledgement)
{
    frame* f = (frame*)malloc(sizeof(frame));
    f->kind = kind;

    if(f->kind == data)
        f->seq = current_seq_number++; //assign and then increment
    
    f->ack = acknowledgement;
    f->info = info;

    return f;
}

//Acts as the physical layer/ communication channel only for demonstration purposes
void set_interface_buffer(frame* f)
{
    int receiver();
    
    interface_buffer = f;

    if(f->kind == data)
        receiver();
}


void send_frame(frame* f)
{
    set_interface_buffer(f);
}


frame* receive_frame()
{
    frame *temp = interface_buffer;
    interface_buffer = NULL; //channel is emptied after this frame is let out.
    return temp;
}

int extract_data(frame* f)
{
    return f->info;
}

int sender(frame *previous_frame)
{
    int info_buffer;
    frame *f, *ack_f;
    
    if(global_event == timeout) //if the sender had timed out previously
    {     
        f = previous_frame; //set the current frame to the previous frame
        
        global_event = frame_arrival;   
    }
    else //get a new frame
    {
        info_buffer = get_data(); //simulate getting data from the network channel
        f = make_frame(data, info_buffer, 0); //make a frame
    }
    
    send_frame(f);
    //now we are waiting for the acknowledgement.

    if(global_event == timeout) //this timeout is set by the receiver if it does not receive any frame
    {
        printf("Sender: Frame lost while sending.\n");
        sender(f); // to resend the frame f
        return 0;
    }

    struct timeval start, end; //start and end store the starting and ending times of the timer.
    gettimeofday(&start, NULL); //start the timer.

    if(get_random() >= 7) //30% chance
        sleep(2);
    
    gettimeofday(&end, NULL); //end the timer

    int elapsed_time_s = (end.tv_sec + end.tv_usec*pow(10,-6)) - (start.tv_sec + start.tv_usec*pow(10,-6));

    if(elapsed_time_s > 1) //with 30% probability
    {
        global_event = timeout;
    }

    if(global_event == timeout)
    {
        sender(f); // to re-send the frame f
        free(interface_buffer); //freeing the channel
        return 0;
    }

    ack_f = receive_frame();

    if (ack_f->kind == acknowledgement)
        printf("Sender: Acknowledgement received for packet number %d.\n", ack_f->ack);
    //here we have no else condition because in a noiseless channel, we will successfully receive an acknowledgement.

    free(ack_f);
    free(f);
}


int seq_last_frame = 0; //stores the sequence number of the last frame received by the receiver.

int receiver(void)
{
    event_type event;
    frame* received;
    int info_buffer;
    int ack_no; // equal to the sequence number of the arriving frame

    wait_for_event(&event);

    if(get_random() >= 7) // 30% chance
    {
        global_event = timeout;
        return 0;
    }

    received = receive_frame();
    info_buffer = extract_data(received);
    ack_no = received->seq;

    if(ack_no == seq_last_frame)
        printf("Receiver: Received frame number %d's duplicate.\n", seq_last_frame);
    
    else
    {
        printf("Receiver: %d has been extracted from frame number %d.\n", info_buffer, ack_no);
        seq_last_frame = ack_no;
    }

    //send an acknowledgement frame back to the sender.
    frame* dummy = make_frame(acknowledgement, info_buffer, ack_no);

    global_event = frame_arrival;
    
    send_frame(dummy);
}


int main()
{
    srand(time(NULL));

    int counter = 0;

    while(counter < 10)
    {
        sender(NULL);
        //the receiver is automatically called when a data frame is sent.
        ++counter;
    }
}
