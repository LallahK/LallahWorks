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

const int EMPTY = 0;
const int BLACK = 1;
const int WHITE = 2;
const int OUTER = 3;
const int ALLDIRECTIONS[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
const int BOARDSIZE = 100; 

char* gen_move();
void play_move(char *move);
void game_over();
void run_worker();
void initialise();

int* initialboard(void);
int *legalmoves ();
int legalp (int move, int player);
int validp (int move);
int wouldflip (int move, int dir, int player);
int opponent (int player);
int findbracketingpiece(int square, int dir, int player);
int alphabeta(int player, int alpha, int beta, int depth, int max_depth);

void makemove(int move, int player);
int* makemove2(int move, int player);
void unmakemove(int move, int player, int* brackets);
int* orderedmoves(int player);

void makeflips (int move, int dir, int player);
void share_alphabeta(int rank);
void start_share();
int get_loc(char* movestring);
char* get_move_string(int loc);
void printboard();
char nameof(int piece);
int evalBoard (int player);
int count (int player);
int* copyBoard(int* board);
void Build_mpi_type(int* board, int* player, int* count, int* processes, int* depth, 
    int* alpha, int* beta, MPI_Datatype* send_root); 
void Build_mpi_sendalpha(int *val, int* alpha, int* beta, MPI_Datatype* send_alphabeta);

int distribute_asroot(int player, int count, int* processes, int depth);
int start_dts(int size);
void update_processmoves(int** moves, int proc, int move);
int update_topmove(int** moves, int proc, int max, int val, int max_val);

int my_colour;
int time_limit;
int running;
int rank;
int size;
int *board;
int firstrun = 1;
int g_alpha = 0;
int g_beta = 64;
int g_depth;

#ifdef DEBUG
double global_t;
int move_count = 0;
int prune_count = 0;
#endif
int MAX_PROC;
int MAX_DEPTH = 6;

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
                printf("Sending to server: %s\n", my_move);
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

int awaken_threads() {
    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int move = start_dts(size);

    return move;
}

void run_worker(){
    int running = 1, rank, s_alpha, s_beta;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Bcast(&my_colour, 1, MPI_INT, 0, MPI_COMM_WORLD);

    board = (int *)malloc(BOARDSIZE * sizeof(int));
    while (1) {
        MPI_Bcast(&running, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (running == 1) {
            int player, count, depth, *processes = malloc(sizeof(int) * MAX_PROC), val;
            MPI_Datatype recv_root, send_alphabeta;
            MPI_Status recv_status;

            Build_mpi_type(board, &player, &count, processes, &depth, 
                &s_alpha, &s_beta, &recv_root);
            
            g_alpha = 0;
            g_beta = 64;
            while (1) {
                MPI_Recv(board, 1, recv_root, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &recv_status);

                if (board[0] == -1) break;  
                val = distribute_asroot(player, count, processes, depth);

                if (depth % 2 == 0) {
                    if (s_beta < g_beta) g_beta = s_beta;
                } else {
                    if (s_alpha > g_alpha) g_alpha = s_alpha;
                }           

                Build_mpi_sendalpha(&val, &g_alpha, &g_beta, &send_alphabeta);
                MPI_Send(&val, 1, send_alphabeta, recv_status.MPI_SOURCE, depth, MPI_COMM_WORLD);
                MPI_Type_free(&send_alphabeta);
            }
            
            MPI_Type_free(&recv_root); 
        } else if (running == 0) {
            break;
        }
    }
}

int start_dts(int size) {
    int *processes, end_turn = -1;
    processes = (int*)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        processes[i] = i;
    }

#ifdef DEBUG
    double start, end;
    start = MPI_Wtime();
#endif
    g_alpha = 0;
    g_beta = 64;
    int move = distribute_asroot(my_colour, size, processes, 1);
#ifdef DEBUG
    end = MPI_Wtime();
    global_t += (end - start);
    fprintf(fp, "FINAL MOVE = %d elapsed time(%.5f)\n", move, end - start);
    fflush(fp);
    move_count++;
#endif

    for (int i = 1; i < size; i++) {
        MPI_Send(&end_turn, 1, MPI_INT, processes[i], 0, MPI_COMM_WORLD);
    }

    return move;
}

int distribute_asroot(int player, int count, int* processes, int depth) {
    int *moves, sub_trees, index, s_alpha, s_beta, move_i = 11;
    int max = (depth % 2 == 0) ? 64 : 0;
    int opp = opponent(player), next_depth = depth + 1, val, *brackets;
    MPI_Datatype send_root, send_alphabeta;
    MPI_Status done_status;

    moves = orderedmoves(player);
    sub_trees = moves[0];

    if (sub_trees == 0) {

    } else {
        if (count > sub_trees) {     
            int check = 0, groups = 0, *process_counts, **process_blocks;

            process_blocks = (int**)malloc(sizeof(int*) * sub_trees);
            process_counts = (int*)malloc(sizeof(int) * sub_trees);
            for (int i = 0; i < sub_trees; i++) {
                process_blocks[i] = (int*)malloc(sizeof(int) * MAX_PROC);
                process_counts[i] = 0;
            }

            int n = 0;
            for (int i = 0; i < count; i++) {
                process_blocks[n][process_counts[n]] = processes[i];
                process_counts[n]++;
                n++;
                if (n % sub_trees == 0) {
                    n = 0;
                }
            }
            
            for (int i = 1; i < groups; i++) {
                brackets = makemove2(moves[i], player);

                Build_mpi_type(board, &opp, &(process_counts[i]), process_blocks[i],
                    &next_depth, &g_alpha, &g_beta, &send_root);

                MPI_Send(board, 1, send_root, process_blocks[i][0], 0, MPI_COMM_WORLD);
                unmakemove(moves[i], player, brackets);
                MPI_Type_free(&send_root);
                check++;
            }

            brackets = makemove2(moves[check + 1], player);
            val = distribute_asroot(opp, process_counts[0], process_blocks[0], next_depth);
            unmakemove(moves[check + 1], player, brackets);
            move_i = 1;

            for (int i = 1; i < groups; i++) {
                Build_mpi_sendalpha(&val, &s_alpha, &s_beta, &send_alphabeta);
                MPI_Recv(&val, 1, send_alphabeta, MPI_ANY_SOURCE, next_depth,
                    MPI_COMM_WORLD, &done_status);
                MPI_Type_free(&send_alphabeta);
                index = done_status.MPI_SOURCE + 1;
                if (depth % 2 == 0) {
                    if (val < max) max = val;
                } else {
                    if (val > max) max = val;
                    move_i = index;
                }
            }

            move_i = moves[move_i];
        } else {
            int unity = 1, close = 1, check = 0, n = 0, *alloc_moves = malloc(sizeof(int) * count);

            int *active = malloc(sizeof(int) * count);
            for (int i = 0; i < count; i++) active[i] = 0;

            for (int i = 1; i < count; i++) {
                brackets = makemove2(moves[i], player);
               
                Build_mpi_type(board, &opp, &unity, &(processes[i]),
                    &next_depth, &g_alpha, &g_beta, &send_root);
                
                MPI_Send(board, 1, send_root, processes[i], 0, MPI_COMM_WORLD);
                unmakemove(moves[i], player, brackets);
                MPI_Type_free(&send_root);

                alloc_moves[i] = i;
                active[i] = 1;
                check++;
            }
            printf("Process %d team size %d sub trees %d\n", rank, count, sub_trees);
            while (close) {
                if ((check + 1) % count == 0) {
                    brackets = makemove2(moves[check + 1], player);
                    val = alphabeta(player, g_alpha, g_beta, depth, MAX_DEPTH);
                    unmakemove(moves[check + 1], player, brackets);

                    if (depth % 2 == 0) {
                        if (val < max) max = val;
                    } else {
                        if (val > max) max = val;
                        move_i = check + 1;
                    }
                    check++;
                }
                
                if (count > 1) {
                    Build_mpi_sendalpha(&val, &s_alpha, &s_beta, &send_alphabeta);
                    MPI_Recv(&val, 1, send_alphabeta, MPI_ANY_SOURCE, next_depth,
                        MPI_COMM_WORLD, &done_status);
                    MPI_Type_free(&send_alphabeta);
                    for (int i = 0; i < count; i++) {
                        if (processes[i] == done_status.MPI_SOURCE)
                            n = i;
                    }

                    if (depth % 2 == 0) {
                        if (s_beta < g_beta) g_beta = s_beta;
                    } else {
                        if (s_alpha > g_alpha) g_alpha = s_alpha;
                    } 
                
                    if (depth % 2 == 0) {
                        if (val < max) max = val;
                    } else {
                        if (val > max) max = val;
                        move_i = alloc_moves[n];
                    }
                    active[n] = 0;

                    if (check < sub_trees) {
                        brackets = makemove2(moves[check + 1], player);
                        Build_mpi_type(board, &opp, &unity, &(processes[n]),
                            &next_depth, &g_alpha, &g_beta, &send_root);

                        MPI_Send(board, 1, send_root, processes[n], 0, MPI_COMM_WORLD);
                        unmakemove(moves[check + 1], player, brackets);
                        MPI_Type_free(&send_root);

                        active[n] = 1;
                        alloc_moves[n] = check + 1;
                        check++;
                    }
                    printf("sdsdf\n");
                }
                close = 0;
                for (int i = 0; i < count; i++) {
                    if (active[i] == 1) close = 1;
                }
                if (check < sub_trees) close = 1;
            }
            move_i = moves[move_i];
        }
    }
    if (depth == 1) return move_i;
    return max;
}

void update_processmoves(int** moves, int proc, int move) {
    int j = 0;
    while(moves[proc][j] != -1) j++;
    moves[proc][j] = move;
}

int update_topmove(int** moves, int proc, int max, int val, int max_val) {
    int j = 0, m;
    while (moves[proc][j] == 0) j++;

    if (val > max_val) {
        m = moves[proc][j];
        moves[proc][j] = 0;
        return m;
    } else {
        moves[proc][j] = 0;
        return max;
    }
}

int alphabeta(int player, int alpha, int beta, int depth, int max_depth) {
    int eval, *moves, alphabeta_val, alphabeta_choice, sub_trees, opp, *brackets = NULL;

    opp = opponent(player);
    moves = orderedmoves(player);
    sub_trees = moves[0];

    if (depth == max_depth || sub_trees == 0) {
        eval = evalBoard(my_colour);
        return (eval + sub_trees);
    }

    if (my_colour == player) {
        alphabeta_choice = -64;
        for (int i = 1; i <= sub_trees; i++) {
            brackets = makemove2(moves[i], player);
            alphabeta_val = alphabeta(opp, alpha, beta, depth + 1, max_depth);
            unmakemove(moves[i], player, brackets);

            if (alphabeta_val > alphabeta_choice) {
                alphabeta_choice = alphabeta_val;
            }
            if (alphabeta_choice > alpha) {
                alpha = alphabeta_choice;
            }
            if (alpha >= beta) {
                    break;
            }
        }
    } else {
        alphabeta_choice = 64;
        for (int i= 1; i<= sub_trees; i++) {
            brackets = makemove2(moves[i], player);
            alphabeta_val = alphabeta(opp, alpha, beta, depth + 1, max_depth);
            unmakemove(moves[i], player, brackets);

            if (alphabeta_val < alphabeta_choice) {
                alphabeta_choice = alphabeta_val;
            }
            if (alphabeta_choice < beta) {
                beta = alphabeta_choice;
            }
            if (alpha >= beta) {
                    break;
            }
        }
    }

    if (depth == g_depth) {
        g_alpha = alpha;
        g_beta = beta;
    }

    return alphabeta_choice;
}

void Build_mpi_type(int* board, int* player, int* count, int* processes, int* depth, 
        int* alpha, int* beta, MPI_Datatype* send_root) {
    MPI_Aint a_addr, b_addr;
    MPI_Aint displacements[7] = {0, 1, 1, 1, 1, 1, 1};
    int blocklengths[7] = {100, 1, 1, MAX_PROC, 1, 1, 1};
    MPI_Datatype types[7] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    
    MPI_Get_address(board, &a_addr);
    MPI_Get_address(player, &b_addr);
    displacements[1] = b_addr - a_addr;
    MPI_Get_address(count, &b_addr);
    displacements[2] = b_addr - a_addr;
    MPI_Get_address(processes, &b_addr);
    displacements[3] = b_addr - a_addr;
    MPI_Get_address(depth, &b_addr);
    displacements[4] = b_addr - a_addr;
    MPI_Get_address(alpha, &b_addr);
    displacements[5] = b_addr - a_addr;
    MPI_Get_address(beta, &b_addr);
    displacements[6] = b_addr - a_addr;

    MPI_Type_create_struct(7, blocklengths, displacements, types, send_root);
    MPI_Type_commit(send_root);
}

void Build_mpi_sendalpha(int* val, int *alpha, int* beta, MPI_Datatype* send_alphabeta) {
    MPI_Aint a_addr, b_addr;
    MPI_Aint displacements[3] = {0, 1, 1};
    int blocklengths[3] = {1, 1, 1};
    MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};

    MPI_Get_address(val, &a_addr);
    MPI_Get_address(alpha, &b_addr);
    displacements[1] = b_addr - a_addr;
    MPI_Get_address(beta, &b_addr);
    displacements[2] = b_addr - a_addr;

    MPI_Type_create_struct(3, blocklengths, displacements, types, send_alphabeta);
    MPI_Type_commit(send_alphabeta);
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
    fprintf(fp, "Prunes: %d\n", prune_count);
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
    for (i = 0; i <= sub_trees; i++) {
        n = moves[i];
        ratings[i] = 1;
        if (n < 18 || n > 81) ratings[i] = 2;
        if (n % 10 == 1 || n % 10 == 8) ratings[i] = 2;
        switch (n) {
            case 11 : ratings[i] = 3;
                break;
            case 18 : ratings[i] = 3;
                break;
            case 81 : ratings[i] = 3;
                break;
            case 88 : ratings[i] = 3;
                break;
        }
    }

    int key, m, j;
    for (i = 2; i <= sub_trees; i++) {
        key = ratings[i];
        m = moves[i];
        j = i - 1;
        
        while(j > 0 && ratings[j] < key) {
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
            cnt++;
        }
    }

    if (board[44] == player) cnt += 3;
    if (board[45] == player) cnt += 3;
    if (board[54] == player) cnt += 3;
    if (board[55] == player) cnt += 3;

    if (board[11] == player) cnt += 5;
    if (board[18] == player) cnt += 5;
    if (board[81] == player) cnt += 5;
    if (board[88] == player) cnt += 5;

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




