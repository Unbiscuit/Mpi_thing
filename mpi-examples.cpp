#include <mpi.h>
#include <cstdlib>
#include <ctime>


void hello(int argc, char** argv){
    int num_processes;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("%d: hello (p=%d)\n", rank, num_processes);
}

void generate_number(int rank, int procs){
    long int rand_mine, rand_prev;
    int rank_next = (rank + 1) % procs;
    int rank_prev = rank == 0 ? procs - 1 : rank -1;
    MPI_Status status;

    srandom(time(NULL) + rank);
    rand_mine = random() / (RAND_MAX/100);
    printf("%d: random is %ld\n", rank, rand_mine);

    if (rank % 2 == 0){
        printf("%d: sending %ld to %d\n", rank, rand_mine, rank_next);
        MPI_Send((void *)&rand_mine, 1, MPI_LONG, rank_next, 1, MPI_COMM_WORLD);

        printf("%d: receiving from %d\n", rank, rank_prev);
        MPI_Recv((void *)&rand_prev, 1, MPI_LONG, rank_prev, 1, MPI_COMM_WORLD, &status);
    }else{
        printf("%d: receiving from %d\n", rank, rank_prev);
        MPI_Recv((void *)&rand_prev, 1, MPI_LONG, rank_prev, 1, MPI_COMM_WORLD, &status);

        printf("%d: sending %ld to %d\n", rank, rand_mine, rank_next);
        MPI_Send((void *)&rand_mine, 1, MPI_LONG, rank_next, 1, MPI_COMM_WORLD);
    }

    printf("%d: >> I had %ld, %d had %ld", rank, rand_mine, rank_prev, rand_prev);
}

void temp(int rank, int procs) {
    long int rand_mine, rand_prev;
    int rank_next = (rank + 1) % procs;
    int rank_prev = rank == 0 ? procs - 1 : rank -1;
    MPI_Request request;
    MPI_Status status;

    srandom(time(NULL) + rank);
    rand_mine = random() / (RAND_MAX/100);
    printf("%d: random is %ld\n", rank, rand_mine);

    MPI_Isendrecv((void *)&rand_mine, 1, MPI_LONG, rank_next, 1,
                  (void *)&rand_prev, 1, MPI_LONG, rank_prev, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);

    printf("%d: >> I had %ld, %d had %ld", rank, rand_mine, rank_prev, rand_prev);
}

void round_robin(int argc, char** argv){
    int num_processes;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("%d: hello (p=%d)\n", rank, num_processes);
    temp(rank, num_processes);
    printf("%d: goodbye\n", rank);
}

int get_i(int argc, char** argv){
    MPI_Init(&argc, &argv);

    const char* cvar_name = "MPI_TAILTHREAD_SUPPRESSION";
    int cvar_index;

    MPI_T_cvar_get_index(cvar_name, &cvar_index);

    printf("Successfully obtained index for control variable %s. Index: %d\n", cvar_name, cvar_index);

    MPI_Finalize();

    return 0;
}

void copy_comm(int argc, char** argv){
    int num_processes;
    int rank;
    MPI_Info info;
    MPI_Comm NEW_COMM;
    MPI_Request request;

    MPI_Info_create(&info);
    MPI_Info_set(info, "WHat", "Why");
    MPI_Info_set(info, "Where", "How");

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("%d: hello (p=%d)\n", rank, num_processes);

    MPI_Comm_idup_with_info(MPI_COMM_WORLD, info, &NEW_COMM, &request);
    MPI_Comm_size(NEW_COMM, &num_processes);
    MPI_Comm_rank(NEW_COMM, &rank);
    int nkeys;
    int flag;
    MPI_Info_get_nkeys(info, &nkeys);

    printf("%d: hello from new communicator (p=%d)\n", rank, num_processes);
    printf("Info associated with the communicator:\n");
    for (int i = 0; i < nkeys; i++) {
        char key[MPI_MAX_INFO_KEY];
        char value[MPI_MAX_INFO_VAL];
        MPI_Info_get_nthkey(info, i, key);
        MPI_Info_get(info, key, MPI_MAX_INFO_VAL, value, &flag);
        printf("%s = %s\n", key, value);
    }
}

void sess(int argc, char** argv){
    MPI_Errhandler errhandler = MPI_ERRHANDLER_NULL;
    MPI_Session session;
    MPI_Session_init(MPI_INFO_NULL, errhandler ,&session);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Process %d is running\n", rank);
    MPI_Session_finalize(&session);
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    round_robin(argc, argv);
    MPI_Finalize();
    return 0;
}
