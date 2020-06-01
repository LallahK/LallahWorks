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
int legalp (int* board, int move, int player);
int validp (int move);
int wouldflip (int* board, int move, int dir, int player);
int opponent (int player);
int findbracketingpiece(int* board, int square, int dir, int player);
int randomstrategy();
int* distribute_moves(int rank, int* block, int player);
int* minmax_init(int* moves);
int minmax(int* board, int move, int player, int depth, int max_depth);
int alphabeta(int* board, int move, int player, int alpha, int beta, int depth, int max_depth);
void makemove (int *board, int move, int player);
void makeflips (int* board, int move, int dir, int player);
int get_loc(char* movestring);
char* get_move_string(int loc);
void printboard();
char nameof(int piece);
int evalBoard (int player, int* board);
int count (int player, int * board);
int* copyBoard(int* board);

int my_colour;
int time_limit;
int running;
int rank;
int size;
int *board;
int firstrun = 1;
int g_alpha;
int g_beta;

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

int awaken_threads() {
    int size, *all_moves, block_size, *moves, *minmax_move, move, val;

    MPI_Bcast(board, BOARDSIZE, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    moves = distribute_moves(0, &block_size, my_colour);
    minmax_move = minmax_init(moves);

    all_moves = (int*)malloc(2 * sizeof(int));
    val = minmax_move[0];
    move = minmax_move[1];
    for (int i = 1; i < size; i++) {
        MPI_Recv(all_moves, 2, MPI_INT, MPI_ANY_SOURCE, 0, 
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#ifdef DEBUG
    if (rank == 0) {
        fprintf(fp, "Thread %d's choice: %d-%d compared to %d-%d\n", i, all_moves[1], all_moves[0], move, val);
    }
#endif       
        if (all_moves[0] > val) {
            val = all_moves[0];
            move = all_moves[1];
        }
    }    
#ifdef DEBUG
    if (rank == 0) {
    fprintf(fp, "FINAL CHOICE: %d \n", move);
    fflush(fp); }
#endif
    return move;
}

void run_worker(){
    int running = 1, rank, block, *moves, *minmax_move;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Bcast(&my_colour, 1, MPI_INT, 0, MPI_COMM_WORLD);

    board = (int *)malloc(BOARDSIZE * sizeof(int));
    board[0] = 1;
    while (1) {
        MPI_Bcast(&running, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (running == 1) {
            MPI_Bcast(board, BOARDSIZE, MPI_INT, 0, MPI_COMM_WORLD); 

            moves = distribute_moves(rank, &block, my_colour);
            minmax_move = minmax_init(moves);

            MPI_Send(minmax_move, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
            free(moves);
            free(minmax_move);
        } else if (running == 0) {
            break;
        }
    }
}

int* distribute_moves(int rank, int* block, int player) {
    int sub_trees = 0, i, *moves, *local_moves, local_size;
    int size;

    moves = legalmoves(board, player);
    sub_trees = moves[0];
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_size = sub_trees / size + 1; 
    if (sub_trees % size != 0) local_size++;

    local_moves = (int*)malloc(local_size * sizeof(int));
    if (rank >= sub_trees % size) local_moves[local_size - 1] = -1; 

    int j = 0;
    for (i = 1; i <= sub_trees; i++) {
        if (i% size == rank) {
            local_moves[++j] = moves[i];
        }
    }
    local_moves[0] = j;
    
    *block = local_size;
    free(moves);

    return local_moves;
}

int* minmax_init(int* moves) {
    int *minmax_choice, minmax_val, i;

    minmax_choice = (int*)malloc(2 * sizeof(int));
    minmax_choice[0] = 0;
    minmax_choice[1] = -1;

    g_alpha = -64;
    g_beta = 64;
    for (i = 1; i <= moves[0]; i++) {
#ifdef DEBUG
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank == 0) fprintf(fp, "Move %d, %d %d\n", i, moves[i], moves[0]);
#endif
        minmax_val = alphabeta(board, moves[i], my_colour, g_alpha, g_beta, 1, 7);

        if (minmax_val >= minmax_choice[0]) {
            minmax_choice[0] = minmax_val;
            minmax_choice[1] = moves[i];
        }
    }

    return minmax_choice;
}

int alphabeta(int* board, int move, int player, int alpha, int beta, int depth, int max_depth) {
    int eval, *moves, alphabeta_val, alphabeta_choice, *virtual_board, sub_trees, opp;
    
    virtual_board = copyBoard(board);
    makemove(virtual_board, move, player);

    opp = opponent(player);
    moves = legalmoves(virtual_board, opp);
    sub_trees = moves[0];

    if (depth == max_depth || sub_trees == 0) {
        eval = evalBoard(my_colour, virtual_board);
#ifdef DEBUG
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank == 0) {
        for (int j = 1; j < depth; j++) {
            fprintf(fp, "    ");
        }
        fprintf(fp, "Depth: %d | Player %d | Alpha %d | Beta %d | State %d\n", depth, player, alpha, beta, eval); }
#endif
        return eval;
    }

    if (my_colour == opp) {
        alphabeta_choice = -64;
        for (int i = 1; i <= sub_trees; i++) {
            alphabeta_val=alphabeta(virtual_board, moves[i], opp, alpha, beta, depth + 1, max_depth);
            if (alphabeta_val > alphabeta_choice) {
                alphabeta_choice = alphabeta_val;
            }
            if (alphabeta_choice > alpha) {
                alpha = alphabeta_choice;
            }
            if (alpha >= beta) break;
        }
    } else {
        alphabeta_choice = 64;
        for (int i= 1; i<= sub_trees; i++) {
            alphabeta_val=alphabeta(virtual_board, moves[i], opp, alpha, beta, depth + 1, max_depth);
            if (alphabeta_val < alphabeta_choice) {
                alphabeta_choice = alphabeta_val;
            }
            if (alphabeta_choice < beta) {
                beta = alphabeta_choice;
            }
            if (alpha >= beta) break;
        }
    }

    if (depth == 1) {
        g_alpha = alpha;
        g_beta = beta;
    }
    return alphabeta_choice;
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
        makemove(board, loc, my_colour);
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
    makemove(board, loc, opponent(my_colour));
}

/*
    Called when the match is over. Finalizes and closes MPI
 */
void game_over(){
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
int *legalmoves (int* board, int player) {
    int move, i, *moves;
    moves = (int *)malloc(32 * sizeof(int));
    moves[0] = 0;
    i = 0;
    for (move=11; move<=88; move++)
        if (legalp(board, move, player)) {
            i++;
            moves[i]=move;
        }
    moves[0]=i;
    return moves;
}

/*
    Checks if the selected position constitues a legal move
    

    @param move array index of move
    @param player the colour of the player

    @return returns the amount of flipped pieces
*/
int legalp (int* board, int move, int player) {
    int i;
    if (!validp(move)) return 0;
    if (board[move]==EMPTY) {
        i=0;
        while (i<=7 && !wouldflip(board, move, ALLDIRECTIONS[i], player)) i++;
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
int wouldflip (int* board, int move, int dir, int player) {
    int c;
    c = move + dir;
    if (board[c] == opponent(player))
        return findbracketingpiece(board, c+dir, dir, player);
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
int findbracketingpiece(int* board, int square, int dir, int player) {
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

void makemove (int* board, int move, int player) {
    int i;
    board[move] = player;
    for (i=0; i<=7; i++) makeflips(board, move, ALLDIRECTIONS[i], player);
}

void makeflips (int* board, int move, int dir, int player) {
    int bracketer, c;
    bracketer = wouldflip(board, move, dir, player);
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

int evalBoard(int player, int* board) {
    int eval;
    eval = count(player, board);
    return eval;
}

void printboard(){
    int row, col;
    fprintf(fp,"   1 2 3 4 5 6 7 8 [%c=%d %c=%d]\n",
            nameof(BLACK), count(BLACK, board), nameof(WHITE), count(WHITE, board));
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

int count (int player, int * board) {
    int i, cnt;
    cnt=0;
    for (i=1; i<=88; i++)
        if (board[i] == player) cnt++;
    return cnt;
}




