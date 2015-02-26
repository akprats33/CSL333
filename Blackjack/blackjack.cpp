#include<iostream>
#include <cstdlib>
#include<vector>
#include<random>
#include<ctime>
#include<fstream>
#define n 20

#define stand 0
#define hit 1
#define doubledown 2
#define split 3

using namespace std;

int avgCard;
typedef int card;
int getHandValue(vector<card>);
int mapping(int);

double mem_reward[42][22][21][21][2][2];

void printHand(vector<card> h)
{
	cout<<"\nHand goes as follows :";
	for ( int i=0; i < h.size(); i++)
	{
		cout<<h[i]<<" | ";
	}
	cout<<"\n";
}

class state
{
	public:
	int s_num;
	vector<card> hand;

	void givecard(int m)
	{
		hand.push_back(m);
	}
	int getState()
	{
		if ((hand.size() == 2) && hand[0] == hand[1])				// pair card
		{
			switch( hand[0] )
			{
				case 1:
					s_num = 41;
					break;
				case 2:
					s_num = 32;
					break;
				case 3:
					s_num = 33;
					break;
				case 4:
					s_num = 34;
					break;
				case 5:
					s_num = 35;
					break;
				case 6:
					s_num = 36;
					break;
				case 7:
					s_num = 37;
					break;
				case 8:
					s_num = 38;
					break;
				case 9:
					s_num = 39;
					break;
				case 10:
					s_num = 40;
					break;
			}
		}
		else if(hand.size()==2 && (hand[0]==1 || hand[1]==1))				// soft hand
		{
			if( hand[0]==1 && hand[1]==1)
				s_num = 41;
			else if(hand[0]==1)
			{
				s_num = 23 + hand[1] -2;
			}
			else
				s_num = 23 + hand[0] -2;
		}
		else
		{
			s_num = getHandValue(hand);
			if (s_num > 21)
				s_num = 22;
		}
		return s_num;
	}
};

int getStatePlayer(vector<card> hand)
	{
		int s_num=0;
		if ((hand.size() == 2) && hand[0] == hand[1])				// pair card
		{
			switch( hand[0] )
			{
				case 1:
					s_num = 41;
					break;
				case 2:
					s_num = 32;
					break;
				case 3:
					s_num = 33;
					break;
				case 4:
					s_num = 34;
					break;
				case 5:
					s_num = 35;
					break;
				case 6:
					s_num = 36;
					break;
				case 7:
					s_num = 37;
					break;
				case 8:
					s_num = 38;
					break;
				case 9:
					s_num = 39;
					break;
				case 10:
					s_num = 40;
					break;
			}
		}
		else if(hand.size()==2 && (hand[0]==1 || hand[1]==1))				// soft hand
		{
			if( hand[0]==1 && hand[1]==1)
				s_num = 41;
			else if(hand[0]==1)
			{
				s_num = 23 + hand[1] -2;
			}
			else
				s_num = 23 + hand[0] -2;
		}
		else
		{
			s_num = getHandValue(hand);
			if (s_num > 21)
				s_num = 22;
		}
		return s_num;
	}

class dealerState
{
	public:
	int s_num;
	vector<card> hand;

	int getState()
	{
		s_num=getHandValue(hand);
		if(s_num > 21)
			s_num=22;
		return s_num;
	}
};

int getStateDealer(vector<card> hand)
	{
		int s_num;
		s_num=getHandValue(hand);
		if(s_num > 21)
			s_num=22;
		return s_num;
	}

dealerState deal;

void initialize();
int getHandValue(vector<card>);
double fp , non_fp;

const int n_state=42;

double t_hit[4][n_state][n_state] ;

int getHandValue(vector<card> h)
{
	//sum of cards on hand
	int sum=0,ace_count=0;

	for( int i=0;i < h.size();i++)
	{
		if(h[i] == 1)
		{
			ace_count++;
				continue;
			//cout<<"Ace found";
		}
		sum += h[i];
	}
	if(sum > 10 && ace_count!=0)
		sum = sum + ace_count;
	else if( ace_count!=0)
		sum= sum + 11 + ace_count-1;

	return sum;
}

int getaces(vector<card> h)
{
	//sum of cards on hand
	int sum=0,ace_count=0,ace;

	for( int i=0;i < h.size();i++)
	{
		if(h[i] == 1)
		{
			ace_count++;
				continue;
			//cout<<"Ace found";
		}
		sum += h[i];
	}
	if(sum > 10 && ace_count!=0)
		ace=0;
	else if( ace_count!=0)
		ace=1;
	else
		ace=0;

	return ace;
}

int getHand(state s)
{
	return getHandValue(s.hand);
}

int statenumTocard[n_state][2];
void initialize()							//initializes transition probabilities
{

	statenumTocard[5][0]=2;statenumTocard[5][1]=3;//;,3};
	statenumTocard[6][0]=2;statenumTocard[6][1]=4;
	statenumTocard[7][0]=2;statenumTocard[7][1]=5;
	statenumTocard[8][0]=2;statenumTocard[8][1]=6;
	statenumTocard[9][0]=2;statenumTocard[9][1]=7;
	statenumTocard[10][0]=2;statenumTocard[10][1]=8;
	statenumTocard[11][0]=2;statenumTocard[11][1]=9;
	statenumTocard[12][0]=2;statenumTocard[12][1]=10;
	statenumTocard[13][0]=4;statenumTocard[13][1]=9;
	statenumTocard[14][0]=5;statenumTocard[14][1]=9;
	statenumTocard[15][0]=7;statenumTocard[15][1]=8;
	statenumTocard[16][0]=7;statenumTocard[16][1]=9;
	statenumTocard[17][0]=8;statenumTocard[17][1]=9;
	statenumTocard[18][0]=10;statenumTocard[18][1]=8;
	statenumTocard[19][0]=10;statenumTocard[19][1]=9;
	statenumTocard[20][0]=4;statenumTocard[20][1]=9;			// + 7
	statenumTocard[21][0]=5;statenumTocard[21][1]=10;			// +6
	statenumTocard[23][0]=2;statenumTocard[23][1]=1;
	statenumTocard[24][0]=1;statenumTocard[24][1]=3;
	statenumTocard[25][0]=1;statenumTocard[25][1]=4;
	statenumTocard[26][0]=1;statenumTocard[26][1]=5;
	statenumTocard[27][0]=1;statenumTocard[27][1]=6;
	statenumTocard[28][0]=1;statenumTocard[28][1]=7;
	statenumTocard[29][0]=1;statenumTocard[29][1]=8;
	statenumTocard[30][0]=1;statenumTocard[30][1]=9;
	statenumTocard[31][0]=1;statenumTocard[31][1]=10;
	statenumTocard[32][0]=2;statenumTocard[32][1]=2;
	statenumTocard[33][0]=3;statenumTocard[33][1]=3;
	statenumTocard[34][0]=4;statenumTocard[34][1]=4;
	statenumTocard[35][0]=5;statenumTocard[35][1]=5;
	statenumTocard[36][0]=6;statenumTocard[36][1]=6;
	statenumTocard[37][0]=7; statenumTocard[37][1]=7;
	statenumTocard[38][0]=8;statenumTocard[38][1]=8;
	statenumTocard[39][0]=9;statenumTocard[39][1]=9;
	statenumTocard[40][0]=10; statenumTocard[40][1]=10;
	statenumTocard[41][0]=1;statenumTocard[41][1]=1;

	for( int i=0 ; i < n_state;i++)
	{
		for(int j=0 ; j < n_state ; j++)
		{
			//t_stand[i][j]=0;
			t_hit[0][i][j] = 0;
			t_hit[1][i][j] = 0;
		}
	}

	for (int i=5;i < n_state; i++)
	{
//		t_stand[i][i] = 1;
		t_hit[0][i][i] = 1;
	}

	for (int i=5;i <= 20; i++)
		{

			for (int j=5;j <= 22; j++)
			{
				if( j<= i)
				{

					t_hit[1][i][j]=0;
				}
				else if (j-i <10 )
				{

					t_hit[1][i][j]=non_fp;
				}
				else if (j-i==10)
				{
					t_hit[1][i][j]=fp;
				}
				else
				{
					t_hit[1][i][j]=0;
				}
			}
//			if(i<=10)
//			{
//				t_hit[1][i][i + 29]=non_fp;
//			}
		}
	t_hit[1][13][22] = fp + non_fp;
	t_hit[1][14][22] = fp + 2*non_fp;
	t_hit[1][15][22] = fp + 3*non_fp;
	t_hit[1][16][22] = fp + 4*non_fp;
	t_hit[1][17][22] = fp + 5*non_fp;
	t_hit[1][18][22] = fp + 6*non_fp;
	t_hit[1][19][22] = fp + 7*non_fp;
	t_hit[1][20][22] = fp + 8*non_fp;

	t_hit[1][21][21]=1;						// state 21 is absorbing
	t_hit[1][22][22]=1;						// state above 21 is absorbing

	for(int i=23; i<=31;i++)				// soft hand
	{
		switch(i)
		{
			case 23:							//A 2 --> 13
			{

				for(int j=14;j <= 21;j++)
				{
					t_hit[1][i][j] = non_fp;

				}
				t_hit[1][i][12]=non_fp;
				t_hit[1][i][13]=fp;					// a face card --> 13 + 10
				break;
			}
			case 24:							//A 3
				{
					for(int j=15;j <= 21;j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][12]=non_fp;				// A 3 8
					t_hit[1][i][13]=non_fp;					// A 3 9
					t_hit[1][i][14] = fp;				// A 3 10
					break;

				}
			case 25:							//A 4
				{
					for(int j=16;j <= 21;j++)
					{
						t_hit[1][i][j]=non_fp;

					}
					t_hit[1][i][12]=non_fp;				// A 4 7
					t_hit[1][i][13]=non_fp;					// A 4 8
					t_hit[1][i][14] = non_fp;				// A 4 9
					t_hit[1][i][15]= fp;					// A 4 10
					break;
				}
			case 26:							//A 5
				{

					for(int j=17;j <= 21;j++)
					{
						t_hit[1][i][j]=non_fp;

					}
					for(int j=12;j < 16; j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][16] = fp;					//A 5 10
					break;
				}
			case 27:							// A 6 --> hand = 17
					{
						for(int j=18;j <= 21;j++)
						{
							t_hit[1][i][j]=non_fp;

						}
						for(int j=12;j < 17;j++)
						{
							t_hit[1][i][j]=non_fp;
						}
						t_hit[1][i][17] = fp;					//A 6 10
						break;
					}

			case 28:							// A 7 --> hand =18
					{
						for(int j=19;j <= 21;j++)
						{
							t_hit[1][i][j]=non_fp;

						}

						for(int j=12;j < 18;j++)
						{
							t_hit[1][i][j]=non_fp;
						}

						t_hit[1][i][18] = fp;					//A 7 10
						break;
					}
			case 29:						// A 8 --> hand= 19
					{
						for(int j=19;j <= 21;j++)
						{
							t_hit[1][i][j]=non_fp;
						}

						for(int j=12;j < 19;j++)
						{
							t_hit[1][i][j]=non_fp;
						}
						t_hit[1][i][19] = fp;			// A 8 10
						break;
					}
			case 30:					// A 9-- > hand = 20
					{
						for(int j=20;j <= 21;j++)
						{
							t_hit[1][i][j]=non_fp;

						}
						for(int j=12;j < 20;j++)
						{
							t_hit[1][i][j]=non_fp;
						}
						t_hit[1][i][20] = fp;			// A 9 10
						break;
					}
			case 31:									// A 10
					{
							t_hit[1][31][31]=1;
							break;
					}

		}

	}



	for(int i=32 ; i<=41 ;i++)						// pair cards
	{
		switch(i)
		{
		case 32:								//22
		{
			for(int j = 6;j <= 13;j++)
			{
				t_hit[1][i][j]=non_fp;
			}
			t_hit[1][i][14] = fp;
			t_hit[1][i][15] = non_fp;
			break;
		}
		case 33:								//33
				{
					for(int j=8;j <= 15;j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][16]=fp;
					t_hit[1][i][17]=non_fp;
					break;
				}
		case 34:								//44
				{
					for(int j=10;j <= 17;j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][18]=fp;
					t_hit[1][i][19]=non_fp;
					break;
				}
		case 35:							//55
				{
					for(int j=12;j <= 19;j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][20]=fp;
					t_hit[1][i][21]=non_fp;
					break;
				}
		case 36:							//66
				{
					for(int j=13;j <= 21;j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][22]=fp;
					break;
				}
		case 37:							//77
				{
					for(int j=15;j <= 21;j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][22] = fp + 2*non_fp;
					break;
				}
		case 38:							//88
				{
					for(int j=17;j <= 21;j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][22]=fp + 4*non_fp;
					break;
				}
		case 39:							//99
				{
					for(int j=19;j <= 21;j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][22]=fp + 6*non_fp;
					break;
				}
		case 40:							//1010
				{
					for(int j=21;j <= 21;j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][22]= fp + 8*non_fp;
					break;
				}
		case 41:							//AA-->12
				{

					for(int j=13;j <= 21;j++)
					{
						t_hit[1][i][j]=non_fp;
					}
					t_hit[1][i][12]=fp;
					break;
				}
			}
		}
	}

int policy[n_state][11] ;

double rewardM(vector<card> pl ,vector<card> dl);
double reward(vector<card> pl ,vector<card> dl);

double hitreward(vector<card> pl ,vector<card> dl , int d_ini)
{
	int p = getHandValue(pl);
	int num=getStatePlayer(pl);

	if (p > 21 )
		return -100.0;
	else if (policy[num][d_ini]== stand)
		return rewardM(pl,dl);
	else if (policy[num][d_ini]== hit)
	{
		double sum=0;
		for(int i=1;i<=10;i++)
		{
			vector<card> temp ;
			temp= pl;
			temp.push_back(i);

			if(i==10)
				sum += fp*hitreward(temp,dl,d_ini);
			else
				sum += non_fp*hitreward(temp, dl,d_ini);
		}
			return sum;
	}
}

double v[n][n_state];
double splitting(vector<card> pl, vector<card> dl,int d_ini,int i,int j);
int main( int argc, char* argv[] )
{
	if(argc !=2 )
	{
		cout<<"Usage : "<<argv[0]<<"<face_card_probability>";
		exit(0);
	}
	else
	{
		fp = atof(argv[1]);
		if(fp > 1)
		{
			cout<<"Wrong input for probability";
			exit(0);
		}
	}
	non_fp = (1 - fp)/9.0;
	cout<<"Face card probability is :"<<fp<<endl;

	avgCard = (5.0 + 5.0*fp);

	state player;

	initialize();
	cout<<endl;

	for( int i=0;i<42;i++)
		{
			for( int j=0;j<22;j++)
			{
				for( int k=0;k<21;k++)
				{
					for( int l=0;l<21;l++)
					{
						for( int m=0;m<2;m++)
						{
							mem_reward[i][j][k][l][m][0] = 3.14159265358979;
							mem_reward[i][j][k][l][m][1] = 3.14159265358979;
						}
					}
				}
			}
		}

	for(int q=1;q<=10;q++)
	{
		for ( int i=5 ; i< n_state ;i++)
			{
				int j = mapping(i);
				if(j <= (21-avgCard))
					policy[i][q] = hit;
				else if (j==21)
						policy[i][q] =stand;
				else if (j==22)
								policy[i][q] = -1;
				else
					policy[i][q] = stand;
			}
	}
	cout<<"Intial policy is :\n";
	for ( int i=5 ; i< n_state ;i++)
	{
		cout<<"policy [ "<<i<<" ] :"<<policy[i][1]<<endl;
	}

	state s2;

	ofstream ofile;
	ofile.open("policy.txt",ios::out | ios::trunc);

	int i,j=5,lim,a;														// a is the action20

	for( i=0; i < n_state ;i++)
		{
			v[0][i]=0;
		}
	int deal_initial;

	for( deal_initial=1; deal_initial <= 10 ; deal_initial++)
	{
		cout<<" Dealer card :"<<deal_initial<<endl;
		deal.hand.clear();
		deal.hand.push_back(deal_initial);

		for( i=1; i< n ;i++)
		{
			//cout<<"\titeration no: " <<i<<endl;
			for(j = 5; j < n_state ; j++)
				{
					if(j==22)
						continue;
					player.hand.clear();
					s2.hand.clear();
					player.givecard(statenumTocard[j][0]);
					player.givecard(statenumTocard[j][1]);
					if( j==20)
						player.givecard(7);
					if(j==21)
						player.givecard(6);
					s2.hand=player.hand;
					int m;
					lim =4 ;
					if(player.hand.size() > 2)
						lim=2;
					int maxAction=-1;
					double maxUtil= - 2e30;
					for (a=0; a < lim ;a++)
						{
							double nextUtil=-2e32;
							if(a == hit)
							{

								nextUtil=0;
								for (int nc=1; nc<=10 ; nc++)
								{
									s2.givecard(nc);
									m=s2.getState();

									nextUtil +=  t_hit[1][j][m] *( hitreward(s2.hand,deal.hand,deal_initial) + v[i-1][m] );
									s2.hand.pop_back();
								}
								//cout<<"inside hit is :"<<nextUtil<<endl;
								s2.hand.clear();
								s2.hand=player.hand;
								if(j==17)
									cout<<"reward for 17 is hit :"<<nextUtil<<endl;
							}
							else if (a == stand)			//a=0
							{
								nextUtil=0;
								nextUtil +=  (( rewardM(player.hand,deal.hand) + v[i-1][j] ));
								if(j==17)
									cout<<"reward for 17 is stand :"<<nextUtil<<endl;
								//cout<<"inside stand is :"<<nextUtil<<endl;
							}
							else if (a== doubledown)
							{
								nextUtil=0;
								for (int nc=1; nc<=10 ; nc++)
								{
									s2.givecard(nc);
									m=s2.getState();
									nextUtil +=  t_hit[1][j][m] *( 2.2*(rewardM(s2.hand,deal.hand)) +   v[i-1][m] );

									s2.hand.pop_back();
								}
								s2.hand.clear();
									s2.hand=player.hand;
								//cout<<"inside doubling is :"<<nextUtil<<endl;
							}
							else if((player.hand[0]==player.hand[1]) && a==split )					// split
							{
								nextUtil=0;
								nextUtil += splitting(player.hand,deal.hand,deal_initial,i,j);
//								cout<<"player cards are : "<<player.hand[0]<<"and :"<<player.hand[1];
//									cout<<"inside splitting is :"<<nextUtil<<endl;
							}

							if(nextUtil > maxUtil)
							{
									maxUtil = nextUtil;
									maxAction = a;
//									if(maxAction ==3)
//									{
//										cout<<"split for state :"<<j<<endl;
//									}
							}
						}

					//cout<<"maximum action is : "<<maxAction<<endl;
					policy[j][deal_initial] = maxAction;
//					if(maxAction ==3)
//					{
//							cout<<"got split----------------------------------------"<<endl;
//					}
					v[i][j] = maxUtil;
					//cout<<" \n value "<<j<<" :"<<v[i][j];
				}
		}
		for( int av=0; av < n_state ;av++)
				{
					v[0][av]=0;
				}

	}
	for( j = 5 ; j < n_state ;j++)
	{
		cout<<" \n value "<<j<<" :"<<v[n-1][j];
	}

	string t;
	for( j=5 ; j<n_state ;j++)
		{
		if(j==22 || j==31)
			continue;
		if(j>=23 && j<=30)
		{
			ofile<<"A"<<(j-23)+2<<"\t";
		}
		else if(j>=32 && j<=40)
		{
			ofile<<(j-30)<<(j-30 )<<"\t";
		}
		else if(j==41)
		{
			ofile<<"AA\t";
		}
		else
			ofile<<j<<"\t";
		for ( int q=2; q<=10 ;q++)
			{
				if(policy[j][q]==0)
					t="S";
				else if(policy[j][q]==1)
					t="H";
				else if(policy[j][q]==2)
					t="D";
				else if(policy[j][q]==3)
					t="P";


				ofile<< t<<" ";
			}
		if(policy[j][1]==0)
			t="S";
		else if(policy[j][1]==1)
			t="H";
		else if(policy[j][1]==2)
			t="D";
		else if(policy[j][1]==3)
		{
			t="P";
			//cout<<"split for state :"<<j<<endl;
		}
		ofile<< t<<"\n";

	}
	//cout<<"average card value is "<<avgCard;
	ofile.close();
}

double rewardM( vector<card> pl ,vector<card> dl)
{
	int p = getHandValue(pl);
	int d = getHandValue(dl);
	int ps = pl.size();
	int ds=  dl.size();
	int p_state = getStatePlayer(pl);
	int d_state = getStateDealer(dl);
	int paces=getaces(pl);
	int daces=getaces(dl);
	//cout<<"player state :"<<p_state<<"and dealer state :"<<d_state<<endl;
	if (p > 21 )
		return -100.0;
	else if (d > 21)
		return 105.0;
	else if (d==21 && dl.size()==2)
		return -200;
	else if ( d >= 17 && d > p)
		return -100;
	else if (d >= 17 && d==p)
		return 2;
	else if (d >= 17 && d<p)
		return 105;
	else if (d < 17)
	{
	if (mem_reward[p_state][d_state][ps][ds][paces][daces] == 3.14159265358979 )
	{
		double sum=0;				//else return mem_reward[p][d][soft aces];
		for(int i=1;i<=10;i++)
		{
			vector<card> temp ;
			temp=dl;
			temp.push_back(i);

			if(i==10)
				sum += fp*rewardM(pl , temp);
			else
				sum += non_fp*rewardM(pl, temp);
		}
		mem_reward[p_state][d_state][ps][ds][paces][daces]= sum;
//		mem_reward[p_state][d_state][0][0] =sum;
	}
	else
		return mem_reward[p_state][d_state][ps][ds][paces][daces] ;
	}
}

int mapping(int snum)
{
	if(snum>=5 && snum<=22)
		return snum;
	else if(snum>=32 && snum<=40)
		return ((snum- 32)*2 + 4);
	else if (snum==41)
			return 12;
	else if(snum >= 23 && snum <=30)
		return ((snum-23) + 13);
	else
		return -1;
}

double splitting(vector<card> pl, vector<card> dl,int d_ini,int i,int j)

{
	pl.pop_back();
	vector<card>player2 = pl;
	vector<card>s2 = pl;
	double nextUtil=0;

	for(int o = 0 ; o < 2 ; o++)
	{
		int tempUtil=0;
		for (int nc=1; nc<=10 ; nc++)
		{
			s2.push_back(nc);
			int m=getStatePlayer(s2);

			if (policy[m][d_ini] == stand)
			{
				tempUtil=0;
				tempUtil +=  ( rewardM(s2,dl) + v[i-1][m] );
			}
			else if (policy[m][d_ini]== hit)
				{tempUtil=0;
					for (int nc=1; nc<=10 ; nc++)
						{
							s2.push_back(nc);
							int next=getStatePlayer(s2);

							tempUtil +=  t_hit[1][m][next] *( hitreward(s2,dl,d_ini) + v[i-1][next] );
							s2.pop_back();
						}
				}
			else if (policy[m][d_ini] == doubledown)
			{tempUtil=0;
				for (int nc=1; nc<=10 ; nc++)
				{
					s2.push_back(nc);
					int next=getStatePlayer(s2);
					tempUtil +=  t_hit[1][m][next] *( 2*(rewardM(s2,dl)) +   v[i-1][next] );

					s2.pop_back();
				}
			}
			if(nc==10)
				nextUtil += (fp * tempUtil);
			else
				nextUtil += (non_fp * tempUtil);
		}
		s2.clear();
		s2=player2;
	}

	return nextUtil;
}

double reward(vector<card> pl ,vector<card> dl)
{
	int p = getHandValue(pl);
	int d = getHandValue(dl);
	if (p > 21 )
		return -100.0;
	else if (d > 21)
		return 140.0;
	else if (d==21 && dl.size()==2)
		return -180;
	else if ( d >= 17 && d > p)
		return -100;
	else if (d >= 17 && d==p)
		return 10;
	else if (d >= 17 && d<p)
		return 100;
	else if (d < 17)
	{
		double sum=0;
		for(int i=1;i<=10;i++)
		{
			vector<card> temp = dl;
			temp.push_back(i);

			if(i==10)
				sum += fp*reward(pl , temp);
			else
				sum += non_fp*reward(pl, temp);
		}
			return sum;
	}
}
