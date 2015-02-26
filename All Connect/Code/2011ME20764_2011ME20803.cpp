//Running format is: <object file> <ip of server> <port no>
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
#include<vector>
/* Added Code */
#include <time.h> 
#include <sys/time.h> 
/* */

#define FOR(i,n) for (int i = 0; i < n; i++)
#define PB push_back
#define inf 1<<25
#define gamma 0.99999

using namespace std;
/* Complete the function below to print 1 integer which will be your next move 
   */
int N, M, K, player;
int numSim = 25000;
vector<string> board;
int UCB_const = 100;
struct timeval tv1,tv2;
int totalMoves;
/* Added Code */
double time_left = 0;
/* */

float maxTime=49.0;
int maxDepth = 20;

vector<int> legalMoves(vector<string> currBoard);			//returns column numbers in which move can be made
bool gameOver(vector<string> currBoard);		
pair<int,int> score(vector<string> currBoard,int);			//returns score 
double evalf(vector<string> );
void printboard(vector<string> );
void updateClone( vector<string>&, int , int);

int swapMe(int p)
{
	return (3-p);
}

struct node 
{
	vector<string> stateBoard;
	int Nstate;
	int *NstateAction;
	node** child;
	int depth;
	double Q;
	double *Qvalue;
	
	node()
	{
		//cout<<" constructor called\n";
		Nstate=0;
		depth = 0;
		NstateAction = new int[M];
		Qvalue = new double[M];
		child = new node *[M];						// i'th child will contain this board plus move in i'th column
		
		for( int i=0 ; i < M ; i++)
		{
			NstateAction[i] = 0;
			Qvalue[i]=0;
			child[i]=NULL;
		}
	}
	
	node(const node &rhs)
	{
		cout<<"copy called";
		Q = rhs.Q;
		Nstate= rhs.Nstate;
		depth = rhs.depth;
		//NstateAction = new int[M];
		//Qvalue = new double[M];
		//child = new node *[M];
		for( int i=0 ; i < M ; i++)
		{
			NstateAction[i] = rhs.NstateAction[i];
			Qvalue[i] = rhs.Qvalue[i];
		}
	}
	
	node& operator= (const node &rhs)
	{
		cout<<"Assignment called";
		if(this!= &rhs)
		{
			Q = rhs.Q;
			Nstate= rhs.Nstate;
			depth = rhs.depth;
			for( int i=0 ; i < M ; i++)
			{
				NstateAction[i] = rhs.NstateAction[i];
				Qvalue[i] = rhs.Qvalue[i];
			}
		}
	
		return *this;
	}
	
	~node()
	{
		for ( int i = 0; i < M; i++)
		{
			//if(child[i]!= NULL)
				delete child[i];
		}
			
		delete[] child;
		delete[] NstateAction;
		delete[] Qvalue;
		stateBoard.clear();
		vector<string>(stateBoard).swap(stateBoard);
	}
	
	node* getChildNode(int action,int player)				// action is making a move in actions's the column of the current board
	{
		if (child[action] ==NULL)
			child[action] = new node;
		
		child[action]->stateBoard = this->stateBoard;
		updateClone(child[action]->stateBoard,action,player);
		child[action]->depth = this->depth +1;
		return child[action];
	
	}

	void printNode()
	{
		printboard(this->stateBoard);
	}
	
	vector<int> getActions()
	{
		return legalMoves(this->stateBoard);
	}
	
	bool isGameOver()
	{
		return gameOver(this->stateBoard);
	}
	double getreward()
	{
		return evalf(this->stateBoard);
	}
};


int nextAction(node*);
int getBestAction(node*);
double buildTree ( node* , int, int);
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
		w1=4;w2=0;w3=0;	
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


void update(int n, int player)
{
    for(int i = 0; i < N; i++)
        if(board[i][n] == '.')
        {
            if(player == 1)
              board[i][n] = 'X';
            else
              board[i][n] = 'O';
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

int uct(node* state) 
	{
		for (int i = 0 ; i < numSim ; i++) 
		{
			buildTree(state, 0,player);
			if ( i > 1000 )
			{
				if ( (i-1000)%200 ==0)
				{ 
					gettimeofday(&tv2, NULL);
					double elapsed = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
					if(elapsed >= (0.91*time_left/totalMoves))
					{
						//cout<<"time exceeded "<<elapsed<<" sec at iteraion number : "<<i<<endl;
						break;
					}
				}
			}
		}
		
		
		
		return getBestAction(state);
	}

int getBestAction(node* root) 
	{
	//double *qu ;
	//qu = root->Qvalue;
	double  maxVal = -1e30;
	int maxAction = -1;
	
	vector<int> acts = legalMoves(root->stateBoard);
	
	int numAction  = acts.size();
	
		for ( int a = 0; a < numAction; a++ ) 
		{
			double val = root->Qvalue[acts[a]];
			
			if( val > maxVal )
			{
				maxVal = val;
				maxAction = acts[a];
			}
		}	
		return maxAction;
	}

double buildTree( node* state, int depth, int currplayer) 
	{
		if (gameOver(state->stateBoard)) 
		{		
			return state->getreward();
		} 
		else if (state->depth >= maxDepth) {			// leaf node
			//cout<<"depth exceeded";
			return state->getreward();
		}
		// UCT RECURSION:
		else {
			// simulate an action
			int act = nextAction(state);
			double r = state->getreward();
			//node *child = new node();
			//child = state->getChildNode(act,currplayer);		
			
			double q ;
			q =  r + (gamma * buildTree(state->getChildNode(act,currplayer), depth + 1,3-currplayer));

			state->Nstate++;										// state visited
			int sa_count = ++ state->NstateAction[act];				// action tried
			
			// compute rolling average for Q
			double alpha = 1.0/sa_count;
			state->Qvalue[act] += (q - state->Qvalue[act]) * alpha;
			
			return q;
		}
	}

int nextAction(node* n) {
		if (n == NULL) 
		{
			cout<<"State was Null\n";			// never reached.
			return  0;
		}
		else {
			double maxVal = -1e30;
			int maxAction  = -1;
			double numerator = log(n->Nstate);
			
			vector<int>actions = legalMoves(n->stateBoard);
			for (int a = 0; a < actions.size(); ++a) 
			{
				double val;
				val  = n->Qvalue[actions[a]];
				//if this action has never been tried, give it max_value
				if (n->NstateAction[actions[a]] == 0) 
					{
						val = inf;
							//otherwise use UCB1 rule
					}
				else
					{
						val += UCB_const * sqrt(numerator / n->NstateAction[actions[a]]);
					}

				if(val > maxVal )
				{
					maxVal = val;
					maxAction  = actions[a];
				}
			}
			return maxAction;			
		}
}

node* s = NULL;

int nextMove()
{
	
	int m;
	gettimeofday(&tv1, NULL);
	//cout<<"time left is : "<<time_left<<endl;
	
	//cout<<" moves remanining : "<<totalMoves<<endl;
	
	 if(!gameOver(board))
		{
			s = new node();
			s->stateBoard = board;
			clock_t t1;
			t1=clock();
		 	m = uct(s);
		 	delete s;
			updateClone(board,m,player);
		 	//cout<<"time taken for deleting is : " <<(float) ((clock()- t)/CLOCKS_PER_SEC)*1000<<endl;
			//cout<<"\n move is in col "<<m<<endl;
			//cout<<"\n score is "<<evalf(board)<<endl;
			
			totalMoves -=1;
			
		  	return m;
		}
		else
		{
			cout<<"Board is full.Game is over.\n";
			//return -1;
		}
}

/*void nonMove(int m)
{
	if(s!=NULL)
		delete s;
	updateClone(board,m,player);
	//printboard(board);

}*/




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
   
   UCB_const = 1000; 
   //UCB_const = 90*( 2000 / (N*M*K)); 

/* Added code */
    int TL;
    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &TL);    
    
    cout<<"Time Limit: "<<TL<<endl;    
    time_left = TL;

/* */

    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &player);  

    cout<<"Player "<<player<<endl;
    
     double temp_moves;
    
    if(player ==1 )
    {
    	temp_moves = N*M /2.0  + 0.5;
    	totalMoves = temp_moves ;
    	}
    else 
    {
    
    	 totalMoves = (N*M)/2;
    	 }

    string temp = "";
    for(int j = 0; j< M; j++)
        temp += ".";
    FOR(i, N)
        board.PB(temp);
        
     /* Added Code */
        struct timeval  tv1, tv2;
      
        /* */

    if(player == 1)
    {
        //cout << "hello" << endl;
        memset(sendBuff, '0', sizeof(sendBuff)); 
        gettimeofday(&tv1, NULL);
        int temp = nextMove();
        gettimeofday(&tv2, NULL);
        double time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
        time_left -= time_spent;
        
        //cout << "temp" << endl;
        snprintf(sendBuff, sizeof(sendBuff), "%d", temp);
        write(sockfd, sendBuff, strlen(sendBuff));
    }

    while(1)
    {
        int nextColumn;

        memset(recvBuff, '0',sizeof(recvBuff));
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);

        gettimeofday(&tv1, NULL);

        recvBuff[n] = 0;
        sscanf(recvBuff, "%d", &nextColumn);
        //cout << "Rec: "<< nextColumn << endl;
        if(player == 1)
	         update(nextColumn, 2);
        else 
          update(nextColumn, 1);

	    cout << nextColumn << endl;
	    
        memset(sendBuff, '0', sizeof(sendBuff)); 
        
        
        
        int temp = nextMove();
        
        
    
        //cout << "temp" << endl;
        snprintf(sendBuff, sizeof(sendBuff), "%d", temp);
    	

         
        gettimeofday(&tv2, NULL);
        double time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
        time_left -= time_spent;
        

        write(sockfd, sendBuff, strlen(sendBuff));
    }

    return 0;
}
