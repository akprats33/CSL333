//Running format is: <object file> <portno> <total_time_for_each_player> <N> <M> <K> <Mode>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <sys/time.h> 
using namespace std;
#include <bits/stdc++.h>
#define FOR(i,n) for (int i = 0; i < n; i++)
#define SZ(x) ((int)x.size())
#define PB push_back
#define MP make_pair
#define sf(x) scanf("%d",&x)
#define pf(x) printf("%d\n",x)
#define split(str) {vs.clear();istringstream ss(str);while(ss>>(str))vs.push_back(str);}
#define ICONST 100  // Remember this will also be the max K allowed
#define GAMMA 0.99999
#define EPS 0.00000000001
int TL = 50; // Time Limit is 50 seconds

//If player is X, I'm the first player.
//If player is O, I'm the second player.

int N,M,K;

double score_p1 = 0, score_p2 = 0;

char board[1000][1000];
string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

pair<int,int> score()
{
    int arr[2] = {0, 0};
    int k = K;
    for(int i = ICONST; i < ICONST + N; i++)
    {
        FOR(j, M)
        {
            if(board[i][j] == '.')
                continue;
            int l_k = 0;
            // diagonally upwards
            for(;l_k < k; l_k++)
            {
                if(board[i][j] != board[i - l_k][j + l_k])
                    break;
            }
            if(l_k == k)
                arr[board[i][j] == 'O']++;

            l_k = 0;
            // rightside
            for(;l_k < k; l_k++)
            {
                if(board[i][j] != board[i][j + l_k])
                    break;
            }
            if(l_k == k)
                arr[board[i][j] == 'O']++;          

            l_k = 0;
            // diagonally downwards
            for(;l_k < k; l_k++)
            {
                if(board[i][j] != board[i + l_k][j + l_k])
                    break;
            }
            if(l_k == k)
                arr[board[i][j] == 'O']++;

            l_k = 0;
            // downwards
            for(;l_k < k; l_k++)
            {
                if(board[i][j] != board[i + l_k][j])
                    break;
            }
            if(l_k == k)
                arr[board[i][j] == 'O']++;
        }
    }

    return MP(arr[0], arr[1]);
}

int insert_column(int x, int player)
{
    if(x >= M || x < 0)
        return -1;
    if(board[ICONST][x] != '.')
        return -1;
    char Cplayer[2] = {'X', 'O'};
    if(board[ICONST + N - 1][x] == '.')
    {
        board[ICONST + N - 1][x] = Cplayer[player];
        return 0;
    }
    
    for(int i = ICONST; i < ICONST + N; i++)
    {
        if(board[i][x] != '.')
        {
            board[i-1][x] = Cplayer[player];
            return 0;
        }
    }

    return -1;
}

int main(int argc, char *argv[])
{

    
    int mode = 0;

	if(argc != 7)
    {
        printf("\n Usage: %s <portno> <total_time_for_each_player> <N> <M> <K> <Mode>\n",argv[0]);
        return 1;
    } 
    TL = atoi(argv[2]);
    N = atoi(argv[3]);
    M = atoi(argv[4]);
    K = atoi(argv[5]);
    mode = atoi(argv[6]);
    // mode 0 is 2 client game, mode 1 is you play 1st player, mode 2 is you play second player
    if(mode != 0)
    {
        cout << "Do you want to play 1st player or 2nd?(1/2) ";
        int temp;
        cin >> temp;
        if(temp == 2)
            mode++;
    }

    double time_left[] = {TL,TL};

    struct timeval  tv1, tv2;
    
    double time_spent;

    FOR(i, 1000)
        FOR(j, 1000)
            board[i][j] = '.';


    int listenfd = 0, connfd = 0, n = 0;
    int connfd1 = 0;

    struct sockaddr_in serv_addr; 

    char sendBuff[1025]; 
    char recvBuff[1024];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1])); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
    if(mode == 0)
        connfd1 = accept(listenfd, (struct sockaddr*)NULL, NULL); 
    
    memset(sendBuff, '0', sizeof(sendBuff)); 
    snprintf(sendBuff, sizeof(sendBuff), "%d", N);
    write(connfd, sendBuff, strlen(sendBuff)); 
    if(mode == 0)
        write(connfd1, sendBuff, strlen(sendBuff)); 

    sleep(1);

    memset(sendBuff, '0', sizeof(sendBuff)); 
    snprintf(sendBuff, sizeof(sendBuff), "%d", M);
    write(connfd, sendBuff, strlen(sendBuff)); 
    if(mode == 0)
        write(connfd1, sendBuff, strlen(sendBuff)); 

    sleep(1);

    memset(sendBuff, '0', sizeof(sendBuff)); 
    snprintf(sendBuff, sizeof(sendBuff), "%d", K);
    write(connfd, sendBuff, strlen(sendBuff)); 
    if(mode == 0)
        write(connfd1, sendBuff, strlen(sendBuff)); 

    sleep(1);

    memset(sendBuff, '0', sizeof(sendBuff)); 
    snprintf(sendBuff, sizeof(sendBuff), "%d", TL);
    write(connfd, sendBuff, strlen(sendBuff)); 
    if(mode == 0)
        write(connfd1, sendBuff, strlen(sendBuff)); 

    sleep(1);
    
    
    //cout << mode << endl;
    if(mode != 1)
    {
        memset(sendBuff, '0', sizeof(sendBuff)); 
        snprintf(sendBuff, sizeof(sendBuff), "%d", 1);
        write(connfd, sendBuff, strlen(sendBuff)); 
    }
    else
    {
        memset(sendBuff, '0', sizeof(sendBuff)); 
        snprintf(sendBuff, sizeof(sendBuff), "%d", 2);
        write(connfd, sendBuff, strlen(sendBuff)); 
    }
    if(mode == 0)
    {
        memset(sendBuff, '0', sizeof(sendBuff)); 
        snprintf(sendBuff, sizeof(sendBuff), "%d", 2);
        write(connfd1, sendBuff, strlen(sendBuff)); 
    }

    ofstream state("moves.txt");
    pair<int, int> prev_result = MP(0,0);

    FOR(i, N*M)
    {
        memset(recvBuff, '0',sizeof(recvBuff));
        gettimeofday(&tv1, NULL);
        int ret = -1;
        if(mode == 0)
        {
            if(i%2 == 0)
                n = read(connfd, recvBuff, sizeof(recvBuff)-1);
            else
                n = read(connfd1, recvBuff, sizeof(recvBuff)-1);
            recvBuff[n] = 0;        
            sscanf(recvBuff, "%d", &ret);
        }
        else
        {
            if(i%2 == mode%2)
            {
                n = read(connfd, recvBuff, sizeof(recvBuff)-1);
                recvBuff[n] = 0;        
                sscanf(recvBuff, "%d", &ret);
            }
            else
            {
                cout << "Enter a column number for your move: ";
                cin >> ret;
            }
        }
        gettimeofday(&tv2, NULL);
        time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
        time_left[i%2] -= time_spent;

        if((time_left[i%2] < 0 && mode == 0) || (time_left[i%2] < 0 && i%2 == mode%2))
        {
            cout << (i%2+1) << " time has finished" << endl;
            return 0;
        }


        //cout << ret << endl;

        memset(sendBuff, '0', sizeof(sendBuff)); 
        snprintf(sendBuff, sizeof(sendBuff), "%d", ret);

        ret = insert_column(ret, i%2);
        if(ret != 0)
        {
            cout << (i%2+1) << " returned an invalid column value: " << ret << endl;
            return 0;
        }

        pair<int, int> result = score();
        if(result.first != prev_result.first)
        	score_p1 += (pow(GAMMA, i))*(result.first - prev_result.first);
        else if(result.second != prev_result.second)
        	score_p2 += (pow(GAMMA, i))*(result.second - prev_result.second);
        prev_result = result;
        //printf("Straights Made 1 : %d - Straights Made 2 : %d\n", prev_result.first, prev_result.second);

        for(int i1 = ICONST; i1 < ICONST + N; i1++)
        {
            FOR(j, M)
            {
                state << board[i1][j] << " ";
                cout << board[i1][j] << " ";
            }
            state << endl;
            cout << endl;
        }
        

        if(mode == 0)
        {
            if(i%2 == 0)
                write(connfd1, sendBuff, strlen(sendBuff));
            else
                write(connfd, sendBuff, strlen(sendBuff));
        }
        else if(i%2 != mode%2)
        {
            //cout << "Packet Sent" << endl;
            write(connfd, sendBuff, strlen(sendBuff));
        }
        state << "--------------------------" << endl;
        cout << "--------------------------" << endl;
    }
    state.close();

    
    //cout << result.first << " " << result.second << endl;
    printf("Player 1 Score : %.8f - Player 2 Score : %.8f\n", score_p1, score_p2);
    printf("Straights Made 1 : %d - Straights Made 2 : %d\n", prev_result.first, prev_result.second);
    //ofstream out(("score.txt").c_str());
   /* ofstream out("score.txt");
    out << result.first << endl;
    out << result.second << endl;
    out.close();
*/
    if(score_p1 >= score_p2 - EPS && score_p1 <= score_p2 + EPS)
        cout << "Match was Draw" << endl;
    else if(score_p1 > score_p2)
        cout << "Match won by 1"<<endl;
    else
        cout << "Match won by 2"<<endl;


    close(connfd);
    close(connfd1);
    sleep(1);

}
