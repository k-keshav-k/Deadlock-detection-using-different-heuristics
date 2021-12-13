#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

pthread_mutex_t lock;       //mutex lock is used so that only one thraed requests resource 
time_t st;                  //at a time and also there is no requests when deadlock detection algo runs
int deadlock_count = 0;
//FILE *fileptr;
float h1=0;
float h2=0;
float h3=0;
float h4=0;
float h5=0;
int time_of_execution = 0;
time_t dt;

struct thread_data{         //struct to pass parameters to thread function
    int tid;
    int din;
    int thread_number;
    int thread_number_resource;
    int* thread_request_resource;
    int* thread_available_resource;
    int* thread_allocate_resource;
    int* thread_total_resource;
};

struct deadlock_attributes{  //struct to pass parameters to deadlock detection function
    int dint;
    int thread_number;
    int heuristic_number;
    int resource_number;
    int* ava_resources;
    int** allocate_matrix;
    int** r_matrix;
};

void request_resource_for_allocation(struct thread_data* td){       //thread requests resources for allocation
    
    int rtype = rand()%(td->thread_number_resource);       //type of resource to be requested --- rtype (A | B | C ...)
    if (td->thread_request_resource[rtype] == 0) return;   //if that type is already fulfilled for the particular thread, then return
    printf("Time: %2ld\t", time(NULL) - st);               //time of request
    int number_resource_requested = rand()%(td->thread_request_resource)[rtype] + 1;    //number of resources to be requested
    printf("Thread\t%d\thas requested\t%d\tof resource\t%c\t", td->tid+1, number_resource_requested, 'A'+rtype);
    if ((td->thread_available_resource)[rtype] >= number_resource_requested){   //if available resources are more than requested resources
        (td->thread_allocate_resource)[rtype] += number_resource_requested;     //allocate requested resource
        (td->thread_available_resource)[rtype] -= number_resource_requested;    //decrement available resource
        (td->thread_request_resource)[rtype] -= number_resource_requested;      //decrement those resources that are fulfilled
        printf("Request Accepted\n");
    }
    else printf("Request Denied\n");
}

void* thread_function(void* tdata){     //thread function which is called when thread is created
    pthread_detach(pthread_self());
    struct thread_data* td = (struct thread_data*) tdata ;
    /*printf("Thread %d has been created\t", td->tid);
    printf("Requested resources: ");
    for(int i=0;i<(td->thread_number_resource);i++){
        printf("%c : %d\t", 'A'+i, (td->thread_request_resource)[i]);
    }
    printf("\n");*/
    sleep(rand()%3+3);
    while(true){        //infinite loop --- to continue requesting resources
        //sleep(rand()%5+3);
        pthread_mutex_lock(&lock);      //locking this particular section
        request_resource_for_allocation(td);
        pthread_mutex_unlock(&lock);    //unlock

        bool ch = false;                //check if all requests are fulfilled or not
        for(int i=0;i<td->thread_number_resource;i++){
            if (td->thread_request_resource[i] != 0){
                ch = true;
                break;
            }
        }
        if (ch == false){           //if all requests of the particular thread is fulfilled
            sleep(td->din * ((rand()%10)/10 + 0.7));    //thread keeps resources for some time
            pthread_mutex_lock(&lock);
            printf("Thread %d will terminate as it has acquired all resources\n", td->tid+1);   //all allocated resources are freed up
            for(int i=0;i<td->thread_number_resource;i++){
                td->thread_available_resource[i] += td->thread_allocate_resource[i];
                td->thread_allocate_resource[i] = 0;
            }
            for(int i=0;i<td->thread_number_resource;i++){      //thread makes new request array
                td->thread_request_resource[i] = rand()%(td->thread_total_resource[i]);
            }
            printf("Thread %d has requested: ", td->tid+1);
            for(int i=0;i<td->thread_number_resource;i++){
                printf("%c : %d ", 'A'+i, td->thread_request_resource[i]);
            }
            printf("\n");
            pthread_mutex_unlock(&lock);
        }
        sleep(rand()%5+2);
    }
}

void* deadlock_detection(void* data){       //function executed by deadlock detection thread
    while(true){
        struct deadlock_attributes* da = (struct deadlock_attributes*) data;
        int time_taken = time(NULL) - st;  
        if (time_taken%(da->dint) == 0){    //if time interval to check deadlock

            pthread_mutex_lock(&lock);
            printf("***********\n");
            printf("Deadlock logic started\n");
            printf("************\n");

            bool first_deadlock = false;

            while(true){

                printf("Allocation matrix\n");        //printing allocation matrix
                for(int i=0;i<da->thread_number;i++){
                    for(int j=0;j<da->resource_number;j++){
                        printf("%d\t", da->allocate_matrix[i][j]);
                    }
                    printf("\n");
                }  
                printf("\n");

                printf("Request matrix\n");        //printing request matrix
                for(int i=0;i<da->thread_number;i++){
                    for(int j=0;j<da->resource_number;j++){
                        printf("%d\t", da->r_matrix[i][j]);
                    }
                    printf("\n");
                }  
                printf("\n");

                printf("Available array: ");
                for(int i=0;i<da->resource_number;i++){
                    printf("%d\t", da->ava_resources[i]);
                }
                printf("\n");
                printf("\n");

                                   //running deadlock detection algorithm
                bool marked[da->thread_number];
                for(int i=0;i<da->thread_number;i++){
                    bool ch = false;
                    for(int j=0;j<da->resource_number;j++){
                        if ((da->allocate_matrix)[i][j] != 0){
                            ch = true;
                            break;
                        }
                    }
                    if (ch == false) marked[i] = true;
                    else marked[i] = false;
                }
                int w[da->resource_number];
                for(int i=0;i<(da->resource_number);i++){
                    w[i] = (da->ava_resources)[i];
                }
                while(true){
                    bool ch2 = false;
                    for(int i=0;i<(da->thread_number);i++){
                        if (marked[i] == true) continue;
                        bool ch = false;
                        for(int j=0;j<(da->resource_number);j++){
                            if ((da->r_matrix)[i][j] > w[j]){
                                ch = true;
                                break;
                            }
                        }
                        if (ch == false){
                            ch2 = true;
                            marked[i] = true;
                            for(int j=0;j<(da->resource_number);j++){
                                w[j] += (da->allocate_matrix)[i][j];
                            }
                        }
                    }
                    if (ch2 == false) break;
                }
                for(int i=0;i<(da->thread_number);i++){
                    if (marked[i] == false){
                        printf("Thread %d is in deadlock\n", i+1);
                    }
                }

                bool is_deadlock = false;
                //deadlock removal
                //strategy 1 --- preempt all deadlocked threads
                if (da->heuristic_number == 1){
                    printf("\nHeuristic 1 is used\n");
                    for(int i=0;i<da->thread_number;i++){
                        if (marked[i] == false){
                            is_deadlock = true;
                            for(int j=0;j<da->resource_number;j++){
                                da->ava_resources[j] += da->allocate_matrix[i][j];
                                da->r_matrix[i][j] += da->allocate_matrix[i][j];
                                da->allocate_matrix[i][j] = 0;
                            }
                            printf("Thread %d is preempted\n\n", i+1);
                        }
                    }
                }

                //strategy 2 --- preempt any random thread
                else if (da->heuristic_number == 2){
                    printf("\nHeuristic 2 is used\n");
                    for(int i=0;i<da->thread_number;i++){
                        if (marked[i] == false){
                            is_deadlock = true;
                            for(int j=0;j<da->resource_number;j++){
                                da->ava_resources[j] += da->allocate_matrix[i][j];
                                da->r_matrix[i][j] += da->allocate_matrix[i][j];
                                da->allocate_matrix[i][j] = 0;
                            }
                            printf("Thread %d is preempted\n\n", i+1);
                            break;
                        }
                    }
                }

                //strategy 3 --- preempt one that is farthest away from acquiring all resources
                else if (da->heuristic_number == 3){
                    printf("\nHeuristic 3 is used\n");
                    int max_tid = 0;
                    int max = 0;
                    for(int i=0;i<da->thread_number;i++){
                        if (marked[i] == false){
                            is_deadlock = true;
                            int sum = 0;
                            for(int j=0;j<da->resource_number;j++){
                                sum += da->r_matrix[i][j];
                            }
                            if (sum > max){
                                max = sum;
                                max_tid = i;
                            }
                        }
                    }
                    if (is_deadlock == true){
                        for(int j=0;j<da->resource_number;j++){
                            da->ava_resources[j] += da->allocate_matrix[max_tid][j];
                            da->r_matrix[max_tid][j] += da->allocate_matrix[max_tid][j];
                            da->allocate_matrix[max_tid][j] = 0;
                        }
                        printf("Thread %d is preempted\n\n", max_tid+1);
                    }
                }

                //strategy 4 --- preempt one with maximum allocated resources
                else if (da->heuristic_number == 4){
                    printf("\nHeuristic 4 is used\n");
                    int max_tid = 0;
                    int max = 0;
                    for(int i=0;i<da->thread_number;i++){
                        if (marked[i] == false){
                            is_deadlock = true;
                            int sum = 0;
                            for(int j=0;j<da->resource_number;j++){
                                sum += da->allocate_matrix[i][j];
                            }
                            if (sum > max){
                                max = sum;
                                max_tid = i;
                            }
                        }
                    }
                    if (is_deadlock == true){
                        for(int j=0;j<da->resource_number;j++){
                            da->ava_resources[j] += da->allocate_matrix[max_tid][j];
                            da->r_matrix[max_tid][j] += da->allocate_matrix[max_tid][j];
                            da->allocate_matrix[max_tid][j] = 0;
                        }
                        printf("Thread %d is preempted\n\n", max_tid+1);
                    }
                }

                //strategy 5 --- preempt one with minimum allocated resources
                else if (da->heuristic_number == 5){
                    printf("\nHeuristic 5 is used\n");
                    int min_tid = 0;
                    int min = __INT_MAX__;
                    for(int i=0;i<da->thread_number;i++){
                        if (marked[i] == false){
                            is_deadlock = true;
                            int sum = 0;
                            for(int j=0;j<da->resource_number;j++){
                                sum += da->allocate_matrix[i][j];
                            }
                            if (sum < min){
                                min = sum;
                                min_tid = i;
                            }
                        }
                    }
                    if (is_deadlock == true){
                        for(int j=0;j<da->resource_number;j++){
                            da->ava_resources[j] += da->allocate_matrix[min_tid][j];
                            da->r_matrix[min_tid][j] += da->allocate_matrix[min_tid][j];
                            da->allocate_matrix[min_tid][j] = 0;
                        }
                        printf("Thread %d is preempted\n\n", min_tid+1);
                    }
                }

                if (is_deadlock == false){
                    printf("There is no deadlock\n\n");
                    break;
                }
                else {
                    if(first_deadlock == false){
                        first_deadlock = true;
                        deadlock_count++;
                    }
                }
            }

            printf("Allocation matrix\n");        //printing allocation matrix
            for(int i=0;i<da->thread_number;i++){
                for(int j=0;j<da->resource_number;j++){
                    printf("%d\t", da->allocate_matrix[i][j]);
                }
                printf("\n");
            }
            printf("\n");

            printf("Request matrix\n");        //printing request matrix
            for(int i=0;i<da->thread_number;i++){
                for(int j=0;j<da->resource_number;j++){
                    printf("%d\t", da->r_matrix[i][j]);
                }
                printf("\n");
            }  
            printf("\n");

            printf("Available array: ");
            for(int i=0;i<da->resource_number;i++){
                printf("%d\t", da->ava_resources[i]);
            }
            printf("\n");
            printf("\n");

            printf("*************\n");
            printf("Deadlock statistics\n");
            printf("Number of deadlocks: %d and Time: %ld\n", deadlock_count, time(NULL)-dt);
            printf("Time per deadlock: %f\n", ((double)(time(NULL)-dt))/deadlock_count);
            //fprintf(fileptr, "%d\t%ld\t%f", deadlock_count, time(NULL) - st, ((double)(time(NULL)-st))/deadlock_count);
            if (da->heuristic_number == 1) h1 = ((double)(time(NULL)-dt))/deadlock_count;
            else if (da->heuristic_number == 2) h2 = ((double)(time(NULL)-dt))/deadlock_count;
            else if (da->heuristic_number == 3) h3 = ((double)(time(NULL)-dt))/deadlock_count;
            else if (da->heuristic_number == 4) h4 = ((double)(time(NULL)-dt))/deadlock_count;
            else if (da->heuristic_number == 5) h5 = ((double)(time(NULL)-dt))/deadlock_count;
            printf("**************\n");
            if (time_taken%time_of_execution == 0){
                if (da->heuristic_number+1 == 6) da->heuristic_number = 1;
                else da->heuristic_number = da->heuristic_number+1;
                deadlock_count = 0;
                dt = time(NULL);
            } 

            sleep(2);
            pthread_mutex_unlock(&lock);
            sleep(da->dint/2);
        }
    }
}

int main(){

    /*fileptr = fopen("deadlock_statistics.txt", "w+");
    if (fileptr == NULL){
        printf("file failed to open\n");
    }*/

    int number_resources;
    printf("Enter number of resources: ");
    scanf("%d", &number_resources);

    int* total_resources = malloc(sizeof (int)*number_resources);
    for(int i=0;i<number_resources;i++){
        printf("Enter number of resources of type %c : ", 'A'+i);
        scanf("%d", &total_resources[i]);      
    }

    int* available_resource = malloc(sizeof(int)*number_resources);
    for(int i=0;i<number_resources;i++){
        available_resource[i] = total_resources[i];     
    }

    int deadlock_check;
    printf("Enter deadlock check interval: ");
    scanf("%d", &deadlock_check);

    int deadlock_strategy;
    printf("Enter deadlock removal heuristic number: ");
    scanf("%d", &deadlock_strategy);

    printf("Enter time of execution: ");
    scanf("%d", &time_of_execution);

    //fprintf(fileptr, "\nHeuristic %d\n", deadlock_strategy);

    int number_threads;
    printf("Enter number of threads: ");
    scanf("%d", &number_threads);

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    st = time(NULL);
    dt = st;

    int** request_matrix = malloc(sizeof(int*)*number_threads);
    int** allocation_matrix = malloc(sizeof(int*)*number_threads);
    pthread_t tid[number_threads];

    for(int i=0;i<number_threads;i++){
        int check;

        int *allocate_resources = malloc(sizeof(int)*number_resources);
        for(int j=0;j<number_resources;j++){
            allocate_resources[j] = 0;
        }
        allocation_matrix[i] = allocate_resources;

        int* request_resources = malloc(sizeof(int)*number_resources);
        printf("Thread\t%d\thas requested:\t", i+1);
        for(int j=0;j<number_resources;j++){
            request_resources[j] = rand()%(total_resources[j]) + 1;
            printf("%c: %d\t", 'A'+j, request_resources[j]);
        }
        printf("\n");
        request_matrix[i] = request_resources;

        struct thread_data* tdata = malloc(sizeof(struct thread_data));
        tdata->tid = i;
        tdata->thread_number_resource = number_resources;
        tdata->thread_request_resource = request_resources;
        tdata->thread_available_resource = available_resource;
        tdata->thread_allocate_resource = allocate_resources;
        tdata->thread_total_resource = total_resources;
        tdata->din = deadlock_check;
        tdata->thread_number = number_threads;
        check = pthread_create(&tid[i], NULL, thread_function, (void*)tdata);
        if (check){
            printf("\n ERROR: thread creation failed \n");
            exit(1);
        }
    }

    sleep(10);
    int error;
    pthread_t thread_id = number_threads;
    struct deadlock_attributes* da = malloc(sizeof(struct deadlock_attributes));
    da->allocate_matrix = allocation_matrix;
    da->dint = deadlock_check;
    da->ava_resources = available_resource;
    da->r_matrix = request_matrix;
    da->thread_number = number_threads;
    da->resource_number = number_resources;
    da->heuristic_number = deadlock_strategy;
    error = pthread_create(&thread_id, NULL, deadlock_detection, (void *)da);
    if (error){
        printf("\n ERROR: thread creation failed \n");
        exit(1);
    }

    /*for(int i=0;i<number_threads;i++){
        pthread_join(tid[i], NULL);
    }
    pthread_join(thread_id, NULL);*/

    while(true){
        if (time(NULL)-st >= 5*time_of_execution){
            pthread_mutex_lock(&lock);
            printf("Heuristic 1: %f\n", h1);
            printf("Heuristic 2: %f\n", h2);
            printf("Heuristic 3: %f\n", h3);
            printf("Heuristic 4: %f\n", h4);
            printf("Heuristic 5: %f\n", h5);
            float arr[5];
            arr[0] = h1;
            arr[1] = h2;
            arr[2] = h3;
            arr[3] = h4;
            arr[4] = h5;

            float max = 0;
            int max_id = 0;
            for(int i=0;i<5;i++){
                if (arr[i] > max){
                    max = arr[i];
                    max_id = i;
                }
            }

            float min = __INT_MAX__;
            int min_id = 0;
            for(int i=0;i<5;i++){
                if (arr[i] < min){
                    min = arr[i];
                    min_id = i;
                }
            }

            printf("Longest average time between deadlocks: %d\n", max_id+1);
            printf("Shortest average time between deadlocks: %d\n", min_id+1);

            pthread_mutex_unlock(&lock);
            exit(0);
            return 0;
            break;
        }
    }
}