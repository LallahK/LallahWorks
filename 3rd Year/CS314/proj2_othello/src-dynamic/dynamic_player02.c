/*H**********************************************************************
 *
 *	This is a skeleton to guide development of Othello engines to be used
 *	with the Cloud Tournament Engine (CTE). CTE runs tournaments between
 *	game engines and displays the results on a web page which is available on
 *	campus (!!!INSERT URL!!!). Any output produced by your implementations will
 *	be available for download on the web page.
 *
 *	The socket communication required for DTE is handled in the main method,
 *	which is provided with the skeleton. All socket communication is performed
 *	at rank 0.
 *
 *	Board co-ordinates for moves start at the top left corner of the board i.e.
 *	if your engine wishes to place a piece at the top left corner, the "gen_move"
 *	function must return "00".
 *
 *	The match is played by making alternating calls to each engine's "gen_move"
 *	and "play_move" functions. The progression of a match is as follows:
 *		1. Call gen_move for black player
 *		2. Call play_move for white player, providing the black player's move
 *		3. Call gen move for white player
 *		4. Call play_move for black player, providing the white player's move
 *		.
 *		.
 *		.
 *		N. A player makes the final move and "game_over" is called for both players
 *	
 *	IMPORTANT NOTE:
 *		Any output that you would like to see (for debugging purposes) needs
 *		to be written to file. This can be done using file FP, and fprintf(),
 *		don't forget to flush the stream. 
 *		I would suggest writing a method to make this
 *		easier, I'll leave that to you.
 *		The file name is passed as argv[4], feel free to change to whatever suits you.
 *H***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mpi.h>
#include <time.h>
#include <math.h>
#define _ZOBRIST (2000003)

const int EMPTY = 0;
const int BLACK = 1;
const int WHITE = 2;
const int OUTER = 3;
const int ALLDIRECTIONS[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
const int BOARDSIZE = 100; 

const int REQUEST = 0;
const int WORK_AVAIL = 1;
const int WORK_UNAVAIL = 2;
const int ENERGY = 3;
const int HANDSHAKE = 4;
const int TERMINATE = 5;
const int SHARE = 6;
const int RETURN = 7;

unsigned long zobrist[100][2];
int transposition[_ZOBRIST][3];

const int MAX_BETA = 210;
const int MIN_ALPHA = 0;
int MAX_PROC;
const int MAX_DEPTH = 7;

void init_zobrist();
int zobrist_hash(int* board);
void save_transposition(int* board, int alpha, int beta);
int check_transposition(int *board, int *alpha, int *beta);

char* gen_move();
void play_move(char *move);
void game_over();
void run_worker();
void initialise();

int randomstrategy();
int* initialboard(void);
int *legalmoves ();
int legalp (int move, int player);
int validp (int move);
int wouldflip (int move, int dir, int player);
int opponent (int player);
int findbracketingpiece(int square, int dir, int player);
int alphabeta_init();
int alphabeta(int player, int alpha, int beta, int depth, int max_depth);
void share_alphabeta(int* alpha, int* beta);
void cleanup();

void makemove(int move, int player);
int* makemove2(int move, int player);
void unmakemove(int move, int player, int* brackets);
int* orderedmoves(int player);

void makeflips (int move, int dir, int player);
int get_loc(char* movestring);
char* get_move_string(int loc);
void printboard();
char nameof(int piece);
int evalBoard (int player);
int count (int player);
int* copyBoard(int* board);
void Build_sendboard(MPI_Datatype *sendboard);

int start_dts();
int branch();
int terminated();
void fulfill();
void reject();
int unemployed();
void clear_messages();
void request();
void check_for_job(int* request, int* available);
void recieve();
void get_value(int* max, int* best_move);
void return_value(int max, int best_move);

int my_colour;
int time_limit;
int running;
int rank;
int size;
int *board;
int firstrun = 1;
time_t t;
int global_alpha;
int global_beta;

int size;
int rank;
double energy = 1;
int parent;
int shared;
int first;
int old_depth;

int shared_alpha;
int shared_beta;
int shared_count;

int stack[100];
int stack_size = 0;
int cutoff = 1;
int depth;
int *layers;
int layer;

int vals[100] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 6, 0, 3, 3, 3, 3, 0, 6, 0,
                 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
                 0, 3, 1, 2, 1, 1, 2, 1, 3, 0,
                 0, 3, 1, 1, 5, 5, 1, 1, 3, 0,
                 0, 3, 1, 1, 5, 5, 1, 1, 3, 0,
                 0, 3, 1, 2, 1, 1, 2, 1, 3, 0,
                 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
                 0, 6, 0, 3, 3, 3, 3, 0, 6, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                };

#ifdef DEBUG
double global_t;
int move_count = 0;
int prune_count = 0;
int load_balance = 0;
#endif

FILE *fp;

int main(int argc , char *argv[]) {
    int socket_desc, port, msg_len;
    char *ip, *cmd, *opponent_move, *my_move;
    char msg_buf[15], len_buf[2];
    struct sockaddr_in server;
    ip = argv[1];
    port = atoi(argv[2]);
    time_limit = atoi(argv[3]);
    my_colour = EMPTY;
    running = 1;

    init_zobrist();
    srand((unsigned) time(&t));

    /* starts MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);	/* get current process id */
    MPI_Comm_size(MPI_COMM_WORLD, &size);	/* get number of processes */
    MAX_PROC = size;
    // Rank 0 is responsible for handling communication with the server
    if (rank == 0){

        fp = fopen(argv[4], "w");
        fprintf(fp, "This is an example of output written to file.\n");
        fprintf(fp, "Player Bot number: 01\n");
        fflush(fp);
        initialise();
        socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        if (socket_desc == -1) {
            fprintf(fp, "Could not create socket\n");
            fflush(fp);
            return -1;
        }
        server.sin_addr.s_addr = inet_addr(ip);
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        //Connect to remote server
        if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){

            fprintf(fp, "Connect error\n");
            fflush(fp);
            return -1;
        }
        fprintf(fp, "Connected\n");
        fflush(fp);
        if (socket_desc == -1){
            return 1;
        }
        while (running == 1){
            if (firstrun ==1) {
                char tempColour[1];
                if(recv(socket_desc, tempColour , 1, 0) < 0){
                    fprintf(fp,"Receive failed\n");
                    fflush(fp);
                    running = 0;
                    break;
                }
                my_colour = atoi(tempColour);
                MPI_Bcast(&my_colour, 1, MPI_INT, 0, MPI_COMM_WORLD);
                fprintf(fp,"Player colour is: %d\n", my_colour);
                fflush(fp);
                firstrun = 2;
            }


            if(recv(socket_desc, len_buf , 2, 0) < 0){


                fprintf(fp,"Receive failed\n");
                fflush(fp);
                running = 0;
                break;
            }

            msg_len = atoi(len_buf);


            if(recv(socket_desc, msg_buf, msg_len, 0) < 0){
                fprintf(fp,"Receive failed\n");
                fflush(fp);
                running = 0;
                break;
            }


            msg_buf[msg_len] = '\0';
            cmd = strtok(msg_buf, " ");

            if (strcmp(cmd, "game_over") == 0){
                running = 0;
                fprintf(fp, "Game over\n");
                fflush(fp);
                MPI_Bcast(&running, 1, MPI_INT, 0, MPI_COMM_WORLD);
                break;

            } else if (strcmp(cmd, "gen_move") == 0){
                MPI_Bcast(&running, 1, MPI_INT, 0, MPI_COMM_WORLD);
                my_move = gen_move();
                if (send(socket_desc, my_move, strlen(my_move) , 0) < 0){
                    running = 0;
                    fprintf(fp,"Move send failed\n");
                    fflush(fp);
                }
                printboard();
            } else if (strcmp(cmd, "play_move") == 0){
                opponent_move = strtok(NULL, " ");

                int opp_move = 2;
                MPI_Bcast(&opp_move, 1, MPI_INT, 0, MPI_COMM_WORLD);

                play_move(opponent_move);
                printboard();
                
            }
            
            memset(len_buf, 0, 2);
            memset(msg_buf, 0, 15);
        }
        game_over();
    } else {
        run_worker(rank);
        MPI_Finalize();
    }
    return 0;
}

/*
    Called at the start of execution on all ranks

    The board is a 1D array of integers:
    00  3333333333  09
    10  3000000003  19      0: Empty
    20  3000000003  29      1: Black
    30  3000000003  39      2: White
    40  3000210003  49      3: Border
    50  3000120003  59
    60  3000000003  69
    70  3000000003  79
    80  3000000003  89
    90  3333333333  99

 */
void initialise(){
    int i;
    running = 1;
    board = (int *)malloc(BOARDSIZE * sizeof(int));
    for (i = 0; i<=9; i++) board[i]=OUTER;
    for (i = 10; i<=89; i++) {
        if (i%10 >= 1 && i%10 <= 8) board[i]=EMPTY; else board[i]=OUTER;
    }
    for (i = 90; i<=99; i++) board[i]=OUTER;
    board[44]=WHITE; board[45]=BLACK; board[54]=BLACK; board[55]=WHITE;
}

int randomstrategy() {
    int r, * moves;

    moves = legalmoves(my_colour);
    if (moves[0] == 0){
        return -1;
    }
    srand (time(NULL));
    r = moves[(rand() % moves[0]) + 1];
    free(moves);
    return(r);
}

int awaken_threads() {
    int *moves, move_count, move, *original_board;
    int i;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Bcast(board, 100, MPI_INT, 0, MPI_COMM_WORLD);

    original_board = copyBoard(board);
    moves = orderedmoves(my_colour);
    move_count = moves[0];

    stack_size = 0;
    for (i = 1; i <= move_count; i++) {
        stack[stack_size++] = moves[i];
    }
   
    printf("TOTAL MOVES %d\n", move_count);
    if (move_count == 0) 
        return -1;
    move = start_dts();

    board = original_board;
    depth = 0;

    return move;
}

void run_worker(){
    int running = 1;
    MPI_Bcast(&my_colour, 1, MPI_INT, 0, MPI_COMM_WORLD);

    board = (int *)malloc(BOARDSIZE * sizeof(int));
    while (1) {
        MPI_Bcast(&running, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (running == 1) {
            MPI_Bcast(board, 100, MPI_INT, 0, MPI_COMM_WORLD);
            start_dts();
            cleanup();
        } else if (running == 0) {
            break;
        }
    }
}

int start_dts() {
    int buf_size = MPI_BSEND_OVERHEAD + sizeof(int) * 100, *brackets, move, tree_value;
    int player = my_colour, max, best_move = stack[0], local_shared;
    char* send_buffer = malloc(buf_size);

    layers = malloc(sizeof(int) * cutoff);
    energy = 10;
    layer = 0;
    layers[layer] = 0;
    global_alpha = MIN_ALPHA;
    global_beta = MAX_BETA;
    shared_count = 0;

    MPI_Buffer_attach(send_buffer, buf_size);
    MPI_Barrier(MPI_COMM_WORLD);
    first = 1;
    depth = 0;
 
    while (!terminated()) {
        if (first) {
            player = (depth % 2 == 0) ? 1 : 2;
            max = (player == my_colour) ? MIN_ALPHA : MAX_BETA;
            best_move = stack[0];
            first = 0;
        }

        move = stack[--stack_size];
        brackets = makemove2(move, player);
        if (depth > cutoff) {
            tree_value = alphabeta(player, global_alpha, global_beta, depth, MAX_DEPTH);
        } else {
            local_shared = shared;
            tree_value = branch();
            shared = local_shared;
        }
        unmakemove(move, player, brackets);

        if (player == my_colour) {
            if (tree_value > max) {
                max = tree_value;
                best_move = move;
            }
        } else {
            if (tree_value < max) {
                max = tree_value;
                best_move = move;
            }
        }

        if (shared) {
            get_value(&max, &best_move);
        }
        if (stack_size == 0) {
            while (shared) get_value(&max, &best_move);
            if (!(rank == 0 && depth == 0))
                return_value(max, best_move);
        }
    }

    stack_size = 0;
    MPI_Buffer_detach(&send_buffer, &buf_size);

    free(layers);
    printf("BEST MOVE FROM %d: %d (%d)\n", rank, best_move, max);
    if (rank == 0) return best_move;

    return 1;
}

int branch() {
    int *brackets, *moves, player, tree_value, max_value, move;
    int i, local_shared;

    layers[++layer] = stack_size;
    depth++;
    player = (depth % 2 == 0) ? 1 : 2;
    max_value = (player == my_colour) ? MIN_ALPHA : MAX_BETA;

    moves = orderedmoves(player);
    for (i = 1; i <= moves[0]; i++)
        stack[stack_size++] = moves[i];

    if (moves[0] == 0) {
        layer--;
        return evalBoard(my_colour);
    }

    shared = 0;
    while (!terminated()) {
        move = stack[--stack_size];
        brackets = makemove2(move, player);
        if (depth > cutoff) {
            tree_value = alphabeta(player, global_alpha, global_beta, depth, MAX_DEPTH);
        } else {
            local_shared = shared;
            tree_value = branch();
            shared = local_shared;
        }
        unmakemove(move, player, brackets);

        if (player == my_colour)
            max_value = (tree_value > max_value) ? tree_value : max_value;
        else 
            max_value = (tree_value < max_value) ? tree_value : max_value;
       
        if (shared) {
            get_value(&max_value, &move);
        }

        if (stack_size == layers[layer]) {
            while (shared) get_value(&max_value, &move);
        }
    }
    layer--;
    depth--;

    return max_value;
}


int terminated() {
    int req = 1, avail = 0;

    if (stack_size - layers[layer] > 4) {
        fulfill();
        return 0;
    } else {
        reject();
        if (stack_size != layers[layer]) {
            return 0;
        } else {
            if (size == 1) return 1;
            if (layer != 0) return 1;
            req = 0;
            while (1) {
                clear_messages();
                if (unemployed()) {
                    return 1;
                } else if (!req) {
                    request();
                    req = 1;
                } else {
                    check_for_job(&req, &avail);
                    if (avail) {
                        return 0;
                    }
                }
            }
        }
    }
}

void get_value(int* max, int* best_move) {
    int msg_avail, i, tree_val, move, pos = 0, alpha, beta;
    char buff[1000];

    for (i = 0; i < size; i++) {
        if (i != rank) {

            MPI_Iprobe(i, RETURN + depth, MPI_COMM_WORLD, &msg_avail, MPI_STATUS_IGNORE);
            if (msg_avail) {
                MPI_Recv(buff, 1000, MPI_PACKED, i, RETURN + depth, MPI_COMM_WORLD, 
                    MPI_STATUS_IGNORE);
                MPI_Unpack(buff, 1000, &pos, &tree_val, 1, MPI_INT, MPI_COMM_WORLD);
                MPI_Unpack(buff, 1000, &pos, &move, 1, MPI_INT, MPI_COMM_WORLD);
                MPI_Unpack(buff, 1000, &pos, &alpha, 1, MPI_INT, MPI_COMM_WORLD);
                MPI_Unpack(buff, 1000, &pos, &beta, 1, MPI_INT, MPI_COMM_WORLD);

                shared--;
                if (depth % 2 == 0) {
                    if (tree_val > *max) {
                        *max = tree_val;
                        *best_move = move;
                    }
                } else {
                    if (tree_val < *max) {
                        *max = tree_val;
                        *best_move = move;
                    }
                }
            }
        }
    } 
}

void return_value(int max, int best_move) {
    int pos = 0;
    char buff[1000];

    MPI_Pack(&max, 1, MPI_INT, buff, 1000, &pos, MPI_COMM_WORLD);
    MPI_Pack(&best_move, 1, MPI_INT, buff, 1000, &pos, MPI_COMM_WORLD);
    MPI_Pack(&global_alpha, 1, MPI_INT, buff, 1000, &pos, MPI_COMM_WORLD);
    MPI_Pack(&global_beta, 1, MPI_INT, buff, 1000, &pos, MPI_COMM_WORLD);

    MPI_Bsend(buff, pos, MPI_PACKED, parent, RETURN + depth, MPI_COMM_WORLD);
    depth = old_depth;
}

void fulfill() {
    int msg_avail, i, done = 0, pos = 0, recv;
    int index, count;
    char buff[1000];
    MPI_Status status_p;

    for (i = 0; i < size && !done; i++) {
        if (i != rank) {
            MPI_Iprobe(i, REQUEST, MPI_COMM_WORLD, &msg_avail, &status_p);
            if (msg_avail) {
                MPI_Recv(0, 0, MPI_INT, i, REQUEST, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(0, 0, MPI_INT, i, WORK_AVAIL, MPI_COMM_WORLD);
                MPI_Recv(&recv, 1, MPI_INT, i, HANDSHAKE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (recv == 0) {
                    count = 2;
                    index = stack_size - count;
                    stack_size = stack_size - count;

                    energy = energy / 2;

                    MPI_Pack(board, 100, MPI_INT, buff, 1000, &pos, MPI_COMM_WORLD);
                    MPI_Pack(&count, 1, MPI_INT, buff, 1000, &pos, MPI_COMM_WORLD);
                    MPI_Pack(stack + index, count, MPI_INT, buff, 1000, &pos, MPI_COMM_WORLD);
                    MPI_Pack(&depth, 1, MPI_INT, buff, 1000, &pos, MPI_COMM_WORLD);
                    MPI_Pack(&energy, 1, MPI_DOUBLE, buff, 1000, &pos, MPI_COMM_WORLD);
                    MPI_Pack(&global_alpha, 1, MPI_INT, buff, 1000, &pos, MPI_COMM_WORLD);
                    MPI_Pack(&global_beta, 1, MPI_INT, buff, 1000, &pos, MPI_COMM_WORLD);

                    MPI_Send(buff, pos, MPI_PACKED, i, HANDSHAKE, MPI_COMM_WORLD);
                    //printf("-- %d Sending %d to process %d depth %d\n", rank, count, i, depth);
                    shared++;
                    done = 1;
#ifdef DEBUG
                    load_balance++;
#endif
                } 
            }
        }
    }
    //reject();
}

void reject() {
    int msg_avail;

    for (int i = 0; i < size; i++) {
        if (i != rank) {
            MPI_Iprobe(i, REQUEST, MPI_COMM_WORLD, &msg_avail, MPI_STATUS_IGNORE);
            if (msg_avail) {
                MPI_Recv(0, 0, MPI_INT, i, REQUEST, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(0, 0, MPI_INT, i, WORK_UNAVAIL, MPI_COMM_WORLD);
            }
        }
    }
}

void clear_messages() {
    reject();
}

int unemployed() {
    int msg_avail, i;
    double val;

    if (rank == 0) {
        for (i = 1; i < size; i++) {
            MPI_Iprobe(i, ENERGY, MPI_COMM_WORLD, &msg_avail, MPI_STATUS_IGNORE);
            if (msg_avail) {
                MPI_Recv(&val, 1, MPI_DOUBLE, i, ENERGY, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                energy += val;
            }
        }
    } else if (energy != 0){
        val = energy;
        MPI_Send(&val, 1, MPI_DOUBLE, 0, ENERGY, MPI_COMM_WORLD);
        energy = 0; 
    }

    if (layer != 0) return 1;

    if (rank == 0 && (10 * size) - energy  < exp(-12)) {
        for (i = 1; i < size; i++) 
            MPI_Send(0, 0, MPI_INT, i, TERMINATE, MPI_COMM_WORLD);
        return 1;
    } else {
        MPI_Iprobe(0, TERMINATE, MPI_COMM_WORLD, &msg_avail, MPI_STATUS_IGNORE);
        if (msg_avail) {
            MPI_Recv(0, 0, MPI_INT, 0, TERMINATE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            return 1;
        }
    }
    return 0;
}

void request() {
    int i;

    for (i = 0; i < size; i++) {
        if (i != rank) {
            MPI_Send(0, 0, MPI_INT, i, REQUEST, MPI_COMM_WORLD);
        }
    }
}

void check_for_job(int* request, int* available) {
    int i, msg_avail, pos = 0, done = 0;
    double val;
    char recv_b[1000];
    MPI_Status stat;
    
    *available = 0;
    for (i = 0; i < size; i++) {
        if (i != rank) {
            MPI_Iprobe(i, WORK_AVAIL, MPI_COMM_WORLD, &msg_avail, MPI_STATUS_IGNORE);
            if (msg_avail) {
                if (!done) {
                    old_depth = depth;
                    MPI_Recv(0, 0, MPI_INT, i, WORK_AVAIL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Send(&done, 1, MPI_INT, i, HANDSHAKE, MPI_COMM_WORLD);

                    MPI_Recv(recv_b, 1000, MPI_PACKED, i, HANDSHAKE, MPI_COMM_WORLD, &stat);
                    MPI_Unpack(recv_b, 1000, &pos, board, 100, MPI_INT, MPI_COMM_WORLD);
                    MPI_Unpack(recv_b, 1000, &pos, &stack_size, 1, MPI_INT, MPI_COMM_WORLD);
                    MPI_Unpack(recv_b, 1000, &pos, stack, stack_size, MPI_INT, MPI_COMM_WORLD);
                    MPI_Unpack(recv_b, 1000, &pos, &depth, 1, MPI_INT, MPI_COMM_WORLD);
                    MPI_Unpack(recv_b, 1000, &pos, &val, 1, MPI_DOUBLE, MPI_COMM_WORLD);
                    MPI_Unpack(recv_b, 1000, &pos, &global_alpha, 1, MPI_INT, MPI_COMM_WORLD);
                    MPI_Unpack(recv_b, 1000, &pos, &global_beta, 1, MPI_INT, MPI_COMM_WORLD);

                    energy = energy + val;
                    //printf("++ %d Recieved job size %d depth %d\n", rank, stack_size, depth);

                    first = 1;
                    shared = 0;
                    parent = stat.MPI_SOURCE;
                    *available = 1;
                    done = 1;
                } else {
                    MPI_Recv(0, 0, MPI_INT, i, WORK_AVAIL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Send(&done, 1, MPI_INT, i, HANDSHAKE, MPI_COMM_WORLD);                   
                }
            }
            MPI_Iprobe(i, WORK_UNAVAIL, MPI_COMM_WORLD, &msg_avail, MPI_STATUS_IGNORE);
            if (msg_avail) {
                MPI_Recv(0, 0, MPI_INT, i, WORK_UNAVAIL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }
}

int alphabeta(int player, int alpha, int beta, int local_depth, int max_depth) {
    int eval, *moves, alphabeta_val, alphabeta_choice, sub_trees, opp, *brackets = NULL;

    opp = opponent(player);
    moves = orderedmoves(player);
    sub_trees = moves[0];

    int value = 0, flag, aOG = alpha;
    if (check_transposition(board, &value, &flag)) {
        if (flag == 0) {
            return value;
        } else if (flag == -1) {
            if (value > alpha) alpha = value;
        } else if (flag == 1) {
            if (value < beta) beta = value;
        }
    }

    if (alpha >= beta) return value;

    if (local_depth == max_depth || sub_trees == 0) {
        eval = evalBoard(my_colour);
        return eval + sub_trees * 3;
    }

    if (my_colour == player) {
        alphabeta_choice = MIN_ALPHA;
        for (int i = 1; i <= sub_trees; i++) {
            brackets = makemove2(moves[i], player);
            alphabeta_val = alphabeta(opp, alpha, beta, local_depth + 1, max_depth);
            unmakemove(moves[i], player, brackets);

            if (alphabeta_val > alphabeta_choice) {
                alphabeta_choice = alphabeta_val;
            }
            if (alphabeta_choice > alpha) {
                alpha = alphabeta_choice;
            }
            if (alpha >= beta) {
#ifdef DEBUG
                prune_count++;
#endif
                break;
            }
        }
    } else {
        alphabeta_choice = MAX_BETA;
        for (int i= 1; i<= sub_trees; i++) {
            brackets = makemove2(moves[i], player);
            alphabeta_val = alphabeta(opp, alpha, beta, local_depth + 1, max_depth);
            unmakemove(moves[i], player, brackets);

            if (alphabeta_val < alphabeta_choice) {
                alphabeta_choice = alphabeta_val;
            }
            if (alphabeta_choice < beta) {
                beta = alphabeta_choice;
            }
            if (alpha >= beta) {
#ifdef DEBUG
                prune_count++;
#endif
                break;
            }
        }
    }

    if (local_depth == depth) {
        global_alpha = alpha;
        global_beta = beta;
    }

    value = alphabeta_choice;
    if (value <= aOG) {
        flag = 1;
    } else if (value >= beta) {
        flag = -1;
    } else {
        flag = 0;
    }
    save_transposition(board, value, flag);

    return alphabeta_choice;
}

void share_alphabeta(int *alpha, int *beta) {
    int i, msg_avail, a, b, pos = 0;
    char buff[200];

    for (i = 0; i < size; i++) {
        if (i != rank) {
            MPI_Iprobe(i, SHARE, MPI_COMM_WORLD, &msg_avail, MPI_STATUS_IGNORE);
            if (msg_avail) {
                MPI_Recv(buff, 200, MPI_PACKED, i, SHARE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Unpack(buff, 200, &pos, &a, 1, MPI_INT, MPI_COMM_WORLD);
                MPI_Unpack(buff, 200, &pos, &b, 1, MPI_INT, MPI_COMM_WORLD);

                *alpha = (a > *alpha) ? a : *alpha;
                *beta = (a > *beta) ? a : *beta;
            }
        }
    }

    pos = 0;
    MPI_Pack(alpha, 1, MPI_INT, buff, 200, &pos, MPI_COMM_WORLD);
    MPI_Pack(beta, 1, MPI_INT, buff, 200, &pos, MPI_COMM_WORLD);
    for (i  = 0; i < size; i++) {
        if (rank != i) {
            MPI_Bsend(buff, pos, MPI_PACKED, i, SHARE, MPI_COMM_WORLD);
        }
    }
}

void cleanup() {
    int i, clean = 0, msg_avail;
    MPI_Status status;
    char buff[200];
    while (!clean) {
        clean = 1;
        for (i = 0; i < size; i++) {
            if (i  != rank) {
                MPI_Iprobe(i, SHARE, MPI_COMM_WORLD, &msg_avail, &status);
                if (msg_avail) {
                    MPI_Recv(buff, 200, MPI_PACKED, i, SHARE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
        }
    }
}

static uint64_t get64rand() {
    return
    (((uint64_t) rand() <<  0) & 0x000000000000FFFFull) |
    (((uint64_t) rand() << 16) & 0x00000000FFFF0000ull) |
    (((uint64_t) rand() << 32) & 0x0000FFFF00000000ull) |
    (((uint64_t) rand() << 48) & 0xFFFF000000000000ull);
}

void init_zobrist() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 2; j++) {
            zobrist[i][j] = get64rand();
        }
    }
    for (int i = 0; i < _ZOBRIST; i++) {
        transposition[i][0] = -1;
    }
}

int zobrist_hash(int* board) {
    long h = 0;

    for (int i = 0; i < 100; i++) {
        if (board[i] == 1 || board[i] == 2) {
            h ^= zobrist[i][board[i] - 1];
        }
    }
    h = h & 0xfffffff;
    return h % _ZOBRIST;
}

void save_transposition(int* board, int alpha, int beta) {
    int h = zobrist_hash(board);
    transposition[h][0] = alpha;
    transposition[h][1] = beta;
}

int check_transposition(int* board, int* alpha, int* beta) {
    int h = zobrist_hash(board);
    if (transposition[h][0] == -1) return 0;

    *alpha = transposition[h][0];
    *beta = transposition[h][1];

    return 1;
}

/*
    Called when your engine is required to make a move. It must return
    a string of the form "xy", where x and y represent the row and
    column where your piece is placed, respectively.

    play_move will not be called for your own engine's moves, so you
    must apply the move generated here to any relevant data structures
    before returning.
 */
char* gen_move(){
    int loc;
    char* move;
    if (my_colour == EMPTY){
        my_colour = BLACK;
    }
    loc = awaken_threads();
    if (loc == -1){
        move = "pass\n";
    } else {
        move = get_move_string(loc);
        makemove(loc, my_colour);
    }
    return move;
}

/*
    Called when the other engine has made a move. The move is given in a
    string parameter of the form "xy", where x and y represent the row
    and column where the opponent's piece is placed, respectively.
 */
void play_move(char *move){
    int loc;
    if (my_colour == EMPTY){
        my_colour = WHITE;
    }
    if (strcmp(move, "pass") == 0){
        return;
    }
    loc = get_loc(move);
    makemove(loc, opponent(my_colour));
}

/*
    Called when the match is over. Finalizes and closes MPI
 */
void game_over(){
#ifdef DEBUG
    fprintf(fp, "Average time %.5f\n", (global_t/move_count));
    fprintf(fp, "   Prunes: %d\n", prune_count);
    fprintf(fp, "   Load balances: %d\n", load_balance);
    fflush(fp);
#endif
    MPI_Finalize();
}

/*
    Converts the co-ordinate from an array index (0, 100) to an 
    8x8 board position [(0 -9), (0 - 9)] and converts to string of len(2)

    @param loc: Location of move as an array index

    @return Returns string index of move
*/
char* get_move_string(int loc){
    static char ms[3];
    int row, col, new_loc;
    new_loc = loc - (9 + 2 * (loc / 10));
    row = new_loc / 8;
    col = new_loc % 8;
    ms[0] = row + '0';
    ms[1] = col + '0';
    ms[2] = '\n';
    return ms;
}

/*
    Converts a string of len(2) into an array index (0, 100)

    @return array index of move
*/
int get_loc(char* movestring){
    int row, col;
    row = movestring[0] - '0';
    col = movestring[1] - '0';
    return (10 * (row + 1)) + col + 1;
}

/*
    Creates and populates array move with all the legal moves 

    @param player: player color who is to move

    @return array of moves, with amount of legal moves in moves[0]
*/
int *legalmoves (int player) {
    int move, i, *moves;
    moves = (int *)malloc(32 * sizeof(int));
    moves[0] = 0;
    i = 0;
    for (move=11; move<=88; move++)
        if (legalp(move, player)) {
            i++;
            moves[i]=move;
        }
    moves[0]=i;
    return moves;
}

int* orderedmoves (int player) {
    int *moves, *ratings = malloc(32 * sizeof(int)), sub_trees, i, n, rank;

    moves = legalmoves(player);
    sub_trees = moves[0];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (i = 1; i <= sub_trees; i++) {
        n = moves[i];
        ratings[i] = vals[n];   
    }

    int key, m, j;
    for (i = 2; i <= sub_trees; i++) {
        key = ratings[i];
        m = moves[i];
        j = i - 1;
        
        while(j > 0 && ratings[j] > key) {
            ratings[j + 1] = ratings[j];
            moves[j + 1] = moves[j];
            j--;
        }
        ratings[j + 1] = key;
        moves[j  + 1] = m;
    } 
    return moves;
}  

/*
    Checks if the selected position constitues a legal move
    

    @param move array index of move
    @param player the colour of the player

    @return returns the amount of flipped pieces
*/
int legalp (int move, int player) {
    int i;

    if (!validp(move)) return 0;
    if (board[move]==EMPTY) {
        i=0;
        while (i<=7 && !wouldflip(move, ALLDIRECTIONS[i], player)) i++;
        if (i==8) return 0; else return 1;
    }
    else return 0;
}

/*
    Checks if the position is on the board

    @param move array index of move

    @return returns 1 if the move is on the board
*/
int validp (int move) {
    if ((move >= 11) && (move <= 88) && (move%10 >= 1) && (move%10 <= 8))
        return 1;
    else return 0;
}

/*
    Checks which directions the move would flip if made

    @param move array index of move
    @param dir direction of possible flips
    @param player the colour of the player

    @return The amount of pieces that would be flipped, returns false if
        the direction has an allied piece
*/
int wouldflip (int move, int dir, int player) {
    int c;
    c = move + dir;
    if (board[c] == opponent(player))
        return findbracketingpiece(c+dir, dir, player);
    else return 0;
}

/*
   Iterate through the board in a certain direction to discern
   the end piece in a line, before the intermediate pieces are flipped

   @param square the input index in the array
   @param dir direction of iteration
   @param player the colour of the player

   @return Return the end square or 0 if no end was found in a direction
*/
int findbracketingpiece(int square, int dir, int player) {
    while (board[square] == opponent(player)) square = square + dir;
    if (board[square] == player) return square;
    else return 0;
}

/*
    Get colour of opponent

    @param player the colour of the player

    @return the colour of the opponent
*/
int opponent (int player) {
    switch (player) {
        case 1: return 2;
        case 2: return 1;
        default: printf("illegal player\n"); return 0;
    }
}

int* makemove2(int move, int player) {
    int i, bracketer, c, *ends = malloc(sizeof(int) * 8), dir;
    board[move] = player;
    for (i = 0; i <= 7; i++) {
        dir = ALLDIRECTIONS[i];
        bracketer = wouldflip(move, dir, player);
        if (bracketer) {
            c = move + dir;
            do {
                board[c] = player;
                c = c + dir;
            } while (c != bracketer);
        }
        ends[i] = bracketer;
    }
    return ends;
}

void unmakemove(int move, int player, int* brackets) {
    int opp = opponent(player), i, c, dir;
    board[move] = 0;
    for (i = 0; i <= 7; i++) {
        if (brackets[i]) {
            dir = ALLDIRECTIONS[i];
            c = move + dir;
            do {
                board[c] = opp;
                c = c + dir;
            } while (c != brackets[i]);
        }
    }
    free(brackets);
}

void makemove (int move, int player) {
    int i;
    board[move] = player;
    for (i=0; i<=7; i++) makeflips(move, ALLDIRECTIONS[i], player);
}

void makeflips (int move, int dir, int player) {
    int bracketer, c;
    bracketer = wouldflip(move, dir, player);
    if (bracketer) {
        c = move + dir;
        do {
            board[c] = player;
            c = c + dir;
        } while (c != bracketer);
    }
}

int* copyBoard(int *board) {
    int *board_copy = (int*)malloc(BOARDSIZE * sizeof(int)), i;
    for (i = 0; i < BOARDSIZE; i++) {
        board_copy[i] = board[i];
    }
    return board_copy;
}

int evalBoard(int player) {
    int i, cnt;
    cnt = 0;
    for (i = 11; i <= 88; i++) {
        if (board[i] == player) { 
            cnt += vals[i];
        }
    }

    return cnt;
}

void printboard(){
    int row, col;
    fprintf(fp,"   1 2 3 4 5 6 7 8 [%c=%d %c=%d]\n",
            nameof(BLACK), count(BLACK), nameof(WHITE), count(WHITE));
    for (row=1; row<=8; row++) {
        fprintf(fp,"%d  ", row);
        for (col=1; col<=8; col++)
            fprintf(fp,"%c ", nameof(board[col + (10 * row)]));
        fprintf(fp,"\n");
    }
    fflush(fp);
}

char nameof (int piece) {
    static char piecenames[5] = ".bw?";
    return(piecenames[piece]);
}

int count (int player) {
    int i, cnt;
    cnt=0;
    for (i=1; i<=88; i++)
        if (board[i] == player) cnt++;
    return cnt;
}




