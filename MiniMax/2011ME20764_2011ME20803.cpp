#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <bits/stdc++.h>
#include<time.h>
#define FOR(i,n) for (int i = 0; i < n; i++)
#define PB push_back

#define MP make_pair
#define inf 1<<25;

using namespace std;

int N, M, K, player;
vector<string> board;

float maxTime=49.0;
int maxDepth=5;
int swappy[2]={2,1};

vector<int> legalMoves(vector<string> currBoard);	//returns column numbers in which move can be made
bool gameOver(vector<string> currBoard);		
int minimax (vector<string> currBoard ); 
double maxMove(vector<string> currBoard,int ,double , double , int ,clock_t);
double minMove(vector<string> currBoard,int ,double , double , int ,clock_t);
pair<int,int> score(vector<string> currBoard,int);			//returns score 

void updateClone(vector<string> &currBoard,int n,int currplayer)	//fills the first unoccupied row of nth column 
{
    for(int i = 0; i < N; i++)
        if(currBoard[i][n] == '.')
        {
		if (currplayer==1)
			currBoard[i][n] = 'X';
		else if (currplayer==2)
			currBoard[i][n] = 'O';
            return;
        }
}

void printboard(vector<string> currBoard)
{
	int i,j;
	for (i = N-1; i >=0; i--)
	{
       for (j = 0; j< M; j++)
		{
            cout<<currBoard[i][j]<<" ";
		}
	cout<<endl;
	}
} 


double evalf(vector<string> currboard)	//value of 'X' minus value of 'O'
{
    int arr[2] = {0, 0};
    int arr2[2] = {0, 0};
    int arr3[2] = {0, 0};
    int k = K;
    for(int i = 0; i <  N; i++)
    {
        FOR(j, M)
        {
            if(currboard[i][j] == '.')
                continue;
            int l_k = 0;
            // diagonally downwards
            for(;(l_k < k) && (l_k<=i)&& (j + l_k<M) ; l_k++)
            {
                if(currboard[i][j] != currboard[i - l_k][j + l_k])
                    break;
            }
            if(l_k == k)
                arr[currboard[i][j] == 'O']++;
            if((l_k ==(k-1)) && (l_k<=i) && (j + l_k<M))
            {
                if(currboard[i - l_k][j + l_k]=='.')
                        arr2[currboard[i][j] == 'O']++;
            }
            if((l_k ==(k-1)) && (i+1<N) && (j>=1))
            {
                if(currboard[i+1][j-1]=='.')
                        arr2[currboard[i][j] == 'O']++;
            }
            if((l_k == (k-2)) && (l_k+1<=i)&& (j + l_k+1<M) )
            {
                if(currboard[i - l_k][j + l_k]=='.' && (currboard[i - l_k-1][j + l_k+1]=='.'  || currboard[i - l_k-1][j + l_k+1]==currboard[i][j]))
                        arr3[currboard[i][j] == 'O']++;
            }
            if((l_k == (k-2)) && (i+2<N)&& (j>=2) )
            {
                if(currboard[i+1][j-1]=='.' && (currboard[i+2][j-2]=='.' || currboard[i+2][j-2]==currboard[i][j]))
                        arr3[currboard[i][j] == 'O']++;
            }

            l_k = 0;
            // rightside
            for(;(l_k < k) && (j+l_k<M) ; l_k++)
            {
                if(currboard[i][j] != currboard[i][j + l_k])
                    break;
            }
            if(l_k == k)
                arr[currboard[i][j] == 'O']++;
            if((l_k == (k-1)) && (j+l_k<M))
            {
                if(currboard[i][j+l_k]=='.')
                        arr2[currboard[i][j] == 'O']++;
            }
            if((l_k == (k-1)) && (j>=1))
            {
                if(currboard[i][j-1]=='.')
                        arr2[currboard[i][j] == 'O']++;
            }
            if((l_k == (k-2)) && (j+l_k+1<M))
            {
                if(currboard[i][j+l_k]=='.' && (currboard[i][j+l_k+1]=='.' || currboard[i][j+l_k+1]==currboard[i][j]))
                        arr3[currboard[i][j] == 'O']++;
            }
            if((l_k == (k-2)) && (j>=2))
            {
                if(currboard[i][j-1]=='.' && (currboard[i][j-2]=='.' || currboard[i][j-2]==currboard[i][j]))
                        arr3[currboard[i][j] == 'O']++;
            }




            l_k = 0;
            // diagonally upwards
            for(;(l_k < k) && (j+l_k<M) && ( i+l_k < N); l_k++)
            {
                if(currboard[i][j] != currboard[i + l_k][j + l_k])
                    break;
            }
            if(l_k == k)
                arr[currboard[i][j] == 'O']++;
            if((l_k == (k-1)) && (j+l_k<M) && ( i+l_k < N))
            {
                if(currboard[i + l_k][j + l_k]=='.')
                        arr2[currboard[i][j] == 'O']++;
            }
            if((l_k == (k-1)) && (j>=1) && (i>=1))
            {
                if(currboard[i-1][j-1]=='.')
                        arr2[currboard[i][j] == 'O']++;
            }
            if((l_k == (k-2)) && (j+l_k+1<M) && ( i+l_k+1< N))
            {
                if(currboard[i + l_k][j + l_k]=='.' && (currboard[i + l_k+1][j + l_k+1]=='.' || currboard[i + l_k+1][j + l_k+1]==currboard[i][j]))
                        arr3[currboard[i][j] == 'O']++;
            }
            if((l_k == (k-2)) && (j>=2) && ( i>=2))
            {
                if(currboard[i-1][j-1]=='.' && (currboard[i-2][j-2]=='.' || currboard[i-2][j-2]==currboard[i][j]))
                        arr3[currboard[i][j] == 'O']++;
            }

            l_k = 0;
            // upwards
            for(;(l_k < k) && (l_k + i < N); l_k++)
            {
                if(currboard[i][j] != currboard[i + l_k][j])
                    break;
            }
            if(l_k == k)
                arr[currboard[i][j] == 'O']++;
            if((l_k == (k-1)) && (l_k + i < N))
            {
                if(currboard[i + l_k][j]=='.')
                        arr2[currboard[i][j] == 'O']++;
            }
            if((l_k == (k-2)) && (l_k + i +1< N))
            {
                if(currboard[i + l_k][j]=='.' && ( currboard[i + l_k+1][j]=='.' || currboard[i + l_k+1][j]==currboard[i][j]))
                        arr3[currboard[i][j] == 'O']++;
            }


        }
    }
    double w1,w2,w3;
	if(gameOver(currboard))
	{
		w1=2.1;w2=0;w3=0;	
	}
	else
	{
	w1=1.0; w2=0.7; w3=0.4;   //weights
	}                                
	int sign;
	sign= (player==1) ? 1 : (-1);
    return sign * (w1*(arr[0]-arr[1])+ w2*(arr2[0]-arr2[1])+ w3*(arr3[0]-arr3[1]));
} 




bool cutOffTest(vector<string> boardState,clock_t tm,int depth)
{
	float elapsed= (float)(clock() - tm)/CLOCKS_PER_SEC;
	
		return ( depth > maxDepth || (elapsed > 2*maxTime/(M*M*N) )||  gameOver(boardState));
	
}


vector<int> legalMoves(vector<string> currboard)	//returns a vector containing legal moves 
  {
	vector<int> result;
	int i=N-1,j;
	j=M/2+1;
	 for(; j != (M/2);)			//check which of the column is unoccupied
	 {
        if(currboard[i][j] == '.')
        {
            result.push_back(j);
        }
		j=(j+1)%M;
		}
	if(currboard[i][j] == '.' && j== (M/2))
        {
            result.push_back(j);
        }
	return result;
  }

void update(int n)				//for updating main board
{
    for(int i = 0; i < N; i++)
        if(board[i][n] == '.')
        {
			if(player==1)
					board[i][n] = 'X';
			else
				{
					board[i][n] = 'O';
				}
			
            return;
        }
}

  

bool gameOver(vector<string> boardState)
{
	int j,i=N-1;
	for(j = 0; j < M; j++)			//checking if all columns of last row are occupied
		{
			if(boardState[i][j] == '.')
			{
				return false;
			}
		}
	return true;
}

double maxMove(vector<string> currBoard,int depth, double alpha , double beta, int currplayer,clock_t tm)
{
	if (cutOffTest (currBoard,tm,depth))
      		return evalf (currBoard);
	  
	vector<int> successor=legalMoves(currBoard);
	int move;
	double value;
	
	while(successor.size()!=0) {
		move=successor[0];
		successor.erase(successor.begin());
		updateClone(currBoard,move,currplayer);
		value = minMove (currBoard, depth + 1, alpha, beta, swappy[currplayer/2],tm);
		
		if (value > alpha) {
			alpha = value;
		}

		if (alpha > beta) {
			return beta;
		}
	}
	return alpha;
}

double minMove (vector<string> currBoard, int depth, double alpha, double beta,int currplayer,clock_t tm)
   {
    if (cutOffTest (currBoard,tm, depth))
      	return evalf (currBoard);

    vector<int> successor=legalMoves (currBoard);
    int move;
    double value;
    
    while(successor.size()!=0) 
	{
		move=successor[0];
		successor.erase(successor.begin());
		updateClone(currBoard,move,currplayer);
      		value = maxMove (currBoard, depth + 1, alpha, beta,swappy[currplayer/2],tm);

      		if (value < beta)
			{
        		beta = value;
      		}

      		if (beta < alpha) 
		{
        		return alpha;
      		}
    	}
    return beta;
  }

int minimax (vector<string> Theboard) 
	{

    vector<int> successor;					//contains row positions in each column which is not occupied
    int move, bestMove ;

 	
    double value, alpha,beta;
	alpha=-inf;
	beta=inf;

    successor = legalMoves (Theboard);		
	
    while (successor.size()!=0) {			//checking for each successor move

		clock_t t;
		t=clock();								//Timer starts now

        move =successor[0];
		successor.erase(successor.begin());
	
      	updateClone(Theboard,move,player);										//updating moves in clone
	
		value = maxMove (Theboard, 1, alpha, beta,swappy[player/2],t);
		if (value > alpha) 
		{
			alpha = value;
			bestMove = move;		
		}
	}
	 return bestMove;
  }


int nextMove(int p)
{
	

  	int m;

	
	if(!gameOver(board))
	{
 	m=minimax(board);
	cout<<"\n move is in col "<<m<<endl;
	updateClone(board,m,p);
	printboard(board);
	cout<<"\n score is "<<evalf(board)<<endl;
  	return m;
	}
	else
	{
		cout<<"Board is full.Game is over.\n";
		return -1;
	}
	
  	
	
}




int main(int argc, char *argv[])
{
	srand (time(NULL));
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1025];
    struct sockaddr_in serv_addr; 

    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> <port no> \n",argv[0]);
        return 1;
    } 

    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    cout<<"Connect(N,M,K) will start"<<endl;

    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &N);

    cout<<"N: "<<N<<endl;
    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &M);

    cout<<"M: "<<M<<endl;
    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &K);    
    
    cout<<"K: "<<K<<endl;
    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &player);  

    cout<<"Player "<<player<<endl;			//player is client's player number.


    string temp = "";
    for(int j = 0; j< M; j++)
        temp += ".";
    FOR(i, N)
        board.PB(temp);

    if(player == 1)
    {
        //cout << "hello" << endl;
        memset(sendBuff, '0', sizeof(sendBuff)); 
        int temp = nextMove(player);						//client's turn
        //cout << "temp" << endl;
        snprintf(sendBuff, sizeof(sendBuff), "%d", temp);
        write(sockfd, sendBuff, strlen(sendBuff));
		
    }

    while(1)
    {
        int nextColumn;

        memset(recvBuff, '0',sizeof(recvBuff));
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
        recvBuff[n] = 0;
        sscanf(recvBuff, "%d", &nextColumn);
        //cout << "Rec: "<< nextColumn << endl;
	    updateClone(board,nextColumn,swappy[player/2]);			// opponent's turn		
		//cout<<"\nopponent No: "<<swappy[player/2]<<endl;
	    cout << nextColumn << endl;
	    
        memset(sendBuff, '0', sizeof(sendBuff)); 
									//client's player number
        int temp = nextMove(player);
        //cout << "temp" << endl;
        snprintf(sendBuff, sizeof(sendBuff), "%d", temp);
        write(sockfd, sendBuff, strlen(sendBuff));
		
		
    }

    return 0;
}
