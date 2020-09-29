#include <iostream> 
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <unordered_map>
#include <cmath>
using namespace std;  

#define Height 25
#define Width 45
const int SIZE=4;
unordered_map<unsigned long long int, double> score;
unordered_map<unsigned long long int, char> path;

void Start(void);
void Print_board(int [SIZE][SIZE]);
int Digit(int );
void New_num(int [SIZE][SIZE], int );
void Down_or_Right(int [SIZE][SIZE], int , int );
void Up_or_Left(int [SIZE][SIZE], int , int );
int Check_zero(int [SIZE][SIZE], int , int , int , int );
int Check(int [SIZE][SIZE], int , int , int , int );
void Win(void);
int End_or_Not(int [SIZE][SIZE]);
void End(void);
unsigned long long Key(int [4][4]);
double Score(int [4][4]);
double AI(int [SIZE][SIZE], int , int );
void Copy(int [SIZE][SIZE], int [SIZE][SIZE]);

int main(void)
{
	int Puzzle[SIZE][SIZE], temp[SIZE][SIZE], i=0, j=0, flag=1, win=1, block=0, max=0, sec=0;

	char key;

	for(i=0; i<SIZE; i++)
		for(j=0; j<SIZE; j++)
			Puzzle[i][j]=0;

	New_num(Puzzle, 2);

	int n=0;

	while(flag)
	{
		block=0;

		unsigned long long int temp=Key(Puzzle);

		unordered_map<unsigned long long int, char>::const_iterator got = path.find(temp);

		for(i=0; i<SIZE; i++)
			for(j=0; j<SIZE; j++)
			{
				if(!Puzzle[i][j])
					block++;
			
				if(Puzzle[i][j]>max)
					max=Puzzle[i][j];
			}

		if(got==path.end())
		{
			if(max<=1024)
			{
				if(block>=6)
					AI(Puzzle, 0, 3);

				else if(block>=3)
					AI(Puzzle, 0, 4);

				else
					AI(Puzzle, 0, 5);
			}

			else if(max<2048)
			{
				if(block>=8)
					AI(Puzzle, 0, 4);

				else if(block>=4)
					AI(Puzzle, 0, 5);

				else
					AI(Puzzle, 0, 6);
			}

			else
			{
				if(block>=5)
					AI(Puzzle, 0, 3);

				else
					AI(Puzzle, 0, 5);
			}
		}

		n++;

		key=path[temp];

		switch(key)
		{
			case 'i':
				cout << "UP\n";
				break;

			case 'k':
				cout << "DOWN\n";
				break;
				
			case 'j':
				cout << "LEFT\n";
				break;

			case 'l':
				cout << "RIGHT\n";
		}
		
		Print_board(Puzzle);

		if(key=='i' || key=='I')
		{
			if(Check(Puzzle, 1, 0, 0, 0))
				Up_or_Left(Puzzle, 0, 1);

			else
				continue;
		}

		else if(key=='k' || key=='K')
		{
			if(Check(Puzzle, 0, 1, 0, 0))
				Down_or_Right(Puzzle, 0, 1);

			else
				continue;
		}

		else if(key=='j' || key=='J')
		{
			if(Check(Puzzle, 0, 0, 1, 0))
				Up_or_Left(Puzzle, 1, 1);

			else
				continue;
		}

		else if(key=='l' || key=='L')
		{
			if(Check(Puzzle, 0, 0, 0, 1))
				Down_or_Right(Puzzle, 1, 1);

			else
				continue;
		}

		if(End_or_Not(Puzzle))
		{
			flag=0;

			End();

			Print_board(Puzzle);
		}
	}

	cout << n << '\n';

	return 0; 
}

double AI(int board[4][4], int count, int limit)
{
	int i=0, j=0, board_temp[SIZE][SIZE];

	double s=0, max=0;

	char move;

	unsigned long long int temp=Key(board);

	unordered_map<unsigned long long int, double>::const_iterator got = score.find(temp);

	if(got!=score.end())
		return score[temp];

	if(count==limit)
		return Score(board);

	if(End_or_Not(board))
	{
		score[temp]=Score(board);

		return score[temp];
	}

	Copy(board_temp, board);

	if(Check(board_temp, 1, 0, 0, 0))
	{
		Up_or_Left(board_temp, 0, 0);

		for(i=0; i<SIZE; i++)
		{
			for(j=0; j<SIZE; j++)
			{
				if(!board_temp[i][j])
				{
					board_temp[i][j]=2;

					s+=(AI(board_temp, count+1, limit)*0.9);

					board_temp[i][j]=4;

					s+=(AI(board_temp, count+1, limit)*0.1);

					board_temp[i][j]=0;
				}
			}
		}
	}

	max=s;

	s=0;

	move='i';

	Copy(board_temp, board);

	if(Check(board_temp, 0, 1, 0, 0))
	{
		Down_or_Right(board_temp, 0, 0);

		for(i=0; i<SIZE; i++)
		{
			for(j=0; j<SIZE; j++)
			{
				if(!board_temp[i][j])
				{
					board_temp[i][j]=2;

					s+=AI(board_temp, count+1, limit)*0.9;

					board_temp[i][j]=4;

					s+=AI(board_temp, count+1, limit)*0.1;

					board_temp[i][j]=0;
				}
			}
		}
	}

	if(max<s)
	{
		max=s;

		move='k';
	}

	s=0;

	Copy(board_temp, board);

	if(Check(board_temp, 0, 0, 1, 0))
	{
		Up_or_Left(board_temp, 1, 0);

		for(i=0; i<SIZE; i++)
		{
			for(j=0; j<SIZE; j++)
			{
				if(!board_temp[i][j])
				{
					board_temp[i][j]=2;

					s+=AI(board_temp, count+1, limit)*0.9;

					board_temp[i][j]=4;

					s+=AI(board_temp, count+1, limit)*0.1;

					board_temp[i][j]=0;
				}
			}
		}
	}

	if(max<s)
	{
		max=s;

		move='j';
	}

	s=0;

	Copy(board_temp, board);

	if(Check(board_temp, 0, 0, 0, 1))
	{
		Down_or_Right(board_temp, 1, 0);

		for(i=0; i<SIZE; i++)
		{
			for(j=0; j<SIZE; j++)
			{
				if(!board_temp[i][j])
				{
					board_temp[i][j]=2;

					s+=AI(board_temp, count+1, limit)*0.9;

					board_temp[i][j]=4;

					s+=AI(board_temp, count+1, limit)*0.1;

					board_temp[i][j]=0;
				}
			}
		}
	}

	if(max<s)
	{
		max=s;

		move='l';
	}

	if(count==0)
		path[temp]=move;

	return max;
}

double Score(int board[SIZE][SIZE])
{
	double score=0;

	float prize=1;

	int i=0, j=0, count=0, max=0, flag=1;

	for(i=0; i<SIZE; i++)
		for(j=0; j<SIZE; j++)
		{
			if(board[i][j])
			{
				count++;

				score+=board[i][j];
			}

			if(board[i][j]>max)
				max=board[i][j];
		}

	/*	if(board[0][0]==max)
		{
		if(board[1][0]<=max && board[1][0]!=0)
		{	
		flag+=2;

		if(board[2][0]<=board[1][0] && board[2][0]!=0)
		{
		flag+=2;	

		if(board[3][0]<=board[2][0])
		flag+=2;
		}
		}
		}

		if(board[0][0]==max)
		{
		if(board[1][0]<=max && board[0][1]<=max)
		{	
		flag+=0.5;

		if(board[2][0]<=board[1][0] && board[0][2]<=board[0][1])
		{
		flag+=0.5;	

		if(board[3][0]<=board[2][0] && board[0][3]<=board[0][2])
		flag+=0.5;
		}
		}
		}*/
	//	score+=max;

	//return score;
	return score/double(count);
	//return score*flag;
}

unsigned long long Key(int board[4][4])
{ 
	unsigned long long h= 0;
	int logBoard[4][4];
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{	
			if (board[i][j] == 0) 
				logBoard[i][j] = 0; 

			else  
				logBoard[i][j] = int(log2(float(board[i][j])));	
		}
	}

	h = 0; 

	for (int i = 0; i < 4; i++) 
	{	
		for (int j = 0; j < 4; j++) 
		{
			h=h<<4;

			h=h|(logBoard[i][j]);
		}
	}

	return h; 
}

int End_or_Not(int board[SIZE][SIZE])
{
	int i=0, j=0, flag=1;

	return !(Check(board, 1, 0, 0, 0) || Check(board, 0, 1, 0, 0) || Check(board, 0, 0, 1, 0) || Check(board, 0, 0, 0, 1));

	return flag;
}

void Copy(int s[SIZE][SIZE], int o[SIZE][SIZE])
{
	int i=0, j=0;

	for(i=0; i<SIZE; i++)
		for(j=0; j<SIZE; j++)
			s[i][j]=o[i][j];
}

void Start(void)
{
	printf("*********************************************\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                    2048                   *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                 'I' to up                 *\n"
			"*                                           *\n"
			"*                'K' to down                *\n"
			"*                                           *\n"
			"*                'J' to left                *\n"
			"*                                           *\n"
			"*                'L' to right               *\n"
			"*                                           *\n"
			"*                     I                     *\n"
			"*                                           *\n"
			"*                  J  K  L                  *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                'E' to quit                *\n"
			"*                                           *\n"
			"*********************************************\n");

	//	usleep(5000000);
}

void Print_board(int Puzzle[SIZE][SIZE])
{
	int i=0, j=0, k=0, space=0, digit=0;

	for(i=0; i<Height; i++)
	{
		if(i%6==0)
		{
			for(j=0; j<Width; j++)
				cout << '*';

			cout << '\n';
		}

		else if(i%3==0 && i%2!=0)
		{
			for(j=0; j<Width; j++)
			{
				if(j%11==0)
					cout << '*';

				else if(j%11==1)
				{
					digit=Digit(Puzzle[i/6][j/11]);

					space=(10-digit)/2;

					if(digit%2==1)
						space++;

					for(k=0; k<space; k++)
						cout << ' ';

					if(Puzzle[i/6][j/11]==0)
						cout << ' ';

					else
						cout << Puzzle[i/6][j/11];

					j+=space+digit-1;
				}

				else
					cout << ' ';
			}

			cout << '\n';
		}

		else
		{
			for(j=0; j<Width; j++)
			{
				if(j%11==0)
					cout << '*';

				else
					cout << ' ';

				if(j==44)
					cout << '\n';
			}
		}
	}
}

int Digit(int num)
{
	if(num/10==0)
		return 1;

	num/=10;

	if(num/10==0)
		return 2;

	num/=10;

	if(num/10==0)
		return 3;

	else return 4;
}

void New_num(int Puzzle[SIZE][SIZE], int n)
{
	int data[10]={2, 2, 2, 2, 2, 2, 2, 2, 2, 4}, var, num, i, j, k=0;

	int seed = (unsigned)time(NULL);
	srand(seed);

	while(k<n)
	{
		num=(rand()%16);

		i=num/4;

		j=num%4;

		var=data[rand()%10];

		if(Puzzle[i][j]==0)
		{
			Puzzle[i][j]=var;

			k++;
		}
	}
}

void Down_or_Right(int Puzzle[SIZE][SIZE], int right, int flag)
{
	int i=0, j=0, k=0;

	for(i=0; i<SIZE; i++)
	{
		for(j=SIZE-1; j>=0; j--)
		{
			for(k=j-1; k>=0; k--)
			{
				if(right)
				{
					//					if(((*(Puzzle+i*SIZE+j))==(*(Puzzle+i*SIZE+k))) && Check_zero(Puzzle, i, k, j, 1))
					if((Puzzle[i][j]==Puzzle[i][k]) && Check_zero(Puzzle, i, k, j, 1))
					{
						//						(*(Puzzle+i*SIZE+j))*=2;
						Puzzle[i][j]*=2;

						//						(*(Puzzle+i*SIZE+k))=0;
						Puzzle[i][k]=0;

						break;
					}
				}

				else
				{
					//					if(((*(Puzzle+j*SIZE+i))==(*(Puzzle+k*SIZE+i))) && Check_zero(Puzzle, k, i, j, 0))
					if((Puzzle[j][i]==Puzzle[k][i]) && Check_zero(Puzzle, k, i, j, 0))
					{
						//						(*(Puzzle+j*SIZE+i))*=2;
						Puzzle[j][i]*=2;

						//						(*(Puzzle+k*SIZE+i))=0;
						Puzzle[k][i]=0;

						break;
					}
				}
			}
		}
	}

	for(i=0; i<SIZE; i++)
	{
		for(j=SIZE-1; j>=0; j--)
		{
			for(k=j-1; k>=0; k--)
			{
				if(right)
				{
					//					if(*(Puzzle+i*SIZE+j)==0 && *(Puzzle+i*SIZE+k)!=0)
					if(Puzzle[i][j]==0 && Puzzle[i][k]!=0)
					{
						int temp=Puzzle[i][j];

						//						temp=(*(Puzzle+i*SIZE+j));

						//						(*(Puzzle+i*SIZE+j))=(*(Puzzle+i*SIZE+k));
						Puzzle[i][j]=Puzzle[i][k];

						//						(*(Puzzle+i*SIZE+k))=temp;
						Puzzle[i][k]=temp;
					}
				}

				else
				{
					//					if(*(Puzzle+j*SIZE+i)==0 && *(Puzzle+k*SIZE+i)!=0)
					if(Puzzle[j][i]==0 && Puzzle[k][i]!=0)
					{
						int temp=Puzzle[j][i];
						//						temp=(*(Puzzle+j*SIZE+i));

						//						(*(Puzzle+j*SIZE+i))=(*(Puzzle+k*SIZE+i));
						Puzzle[j][i]=Puzzle[k][i];

						//						(*(Puzzle+k*SIZE+i))=temp;
						Puzzle[k][i]=temp;
					}
				}
			}
		}
	}

	if(flag)
		New_num(Puzzle, 1);
}

void Up_or_Left(int Puzzle[SIZE][SIZE], int left, int flag)
{
	int i=0, j=0, k=0;

	for(i=0; i<SIZE; i++)
	{
		for(j=0; j<SIZE; j++)
		{
			for(k=j+1; k<SIZE; k++)
			{
				if(left)
				{
					//					if(((*(Puzzle+i*SIZE+j))==(*(Puzzle+i*SIZE+k))) && Check_zero(Puzzle, i, j, k, 1))
					if((Puzzle[i][j]==Puzzle[i][k]) && Check_zero(Puzzle, i, j, k, 1))
					{
						//						(*(Puzzle+i*SIZE+j))*=2;
						Puzzle[i][j]*=2;

						//						(*(Puzzle+i*SIZE+k))=0;
						Puzzle[i][k]=0;

						break;
					}
				}

				else
				{

					//					if(((*(Puzzle+j*SIZE+i))==(*(Puzzle+k*SIZE+i))) && Check_zero(Puzzle, j, i, k, 0))
					if((Puzzle[j][i]==Puzzle[k][i]) && Check_zero(Puzzle, j, i, k, 0))
					{
						//						(*(Puzzle+j*SIZE+i))*=2;
						Puzzle[j][i]*=2;

						//						(*(Puzzle+k*SIZE+i))=0;
						Puzzle[k][i]=0;

						break;
					}
				}
			}
		}
	}

	for(i=0; i<SIZE; i++)
	{
		for(j=0; j<SIZE; j++)
		{
			for(k=j+1; k<SIZE; k++)
			{
				if(left)
				{
					//					if(*(Puzzle+i*SIZE+j)==0 && *(Puzzle+i*SIZE+k)!=0)
					if(Puzzle[i][j]==0 && Puzzle[i][k]!=0)
					{
						int temp=Puzzle[i][j];

						//						temp=(*(Puzzle+i*SIZE+j));

						//						(*(Puzzle+i*SIZE+j))=(*(Puzzle+i*SIZE+k));
						Puzzle[i][j]=Puzzle[i][k];

						//						(*(Puzzle+i*SIZE+k))=temp;
						Puzzle[i][k]=temp;
					}
				}

				else
				{
					//					if(*(Puzzle+j*SIZE+i)==0 && *(Puzzle+k*SIZE+i)!=0)
					if(Puzzle[j][i]==0 && Puzzle[k][i]!=0)
					{
						int temp=Puzzle[j][i];
						//						temp=(*(Puzzle+j*SIZE+i));

						//						(*(Puzzle+j*SIZE+i))=(*(Puzzle+k*SIZE+i));
						Puzzle[j][i]=Puzzle[k][i];

						//						(*(Puzzle+k*SIZE+i))=temp;
						Puzzle[k][i]=temp;
					}
				}
			}
		}
	}

	if(flag)
		New_num(Puzzle, 1);
}

int Check_zero(int Puzzle[SIZE][SIZE], int i, int j, int k, int dir)
{
	int n=0, flag=1;

	if(dir)
		for(n=j+1; n<k; n++)
		{
			//			if((*(Puzzle+i*SIZE+n))!=0)
			if(Puzzle[i][n]!=0)
				flag=0;
		}

	else
		for(n=i+1; n<k; n++)
		{
			//			if((*(Puzzle+n*SIZE+j))!=0)
			if(Puzzle[n][j]!=0)
				flag=0;
		}

	return flag;
}

int Check(int Puzzle[SIZE][SIZE], int Up, int Down, int Left, int Right)
{
	int i=0, j=0;

	for(i=0+Down; i<SIZE-Up; i++)
		for(j=0+Right; j<SIZE-Left; j++)
		{
			//			if((((*(Puzzle+i*SIZE+j))==0) && ((*(Puzzle+(i+Up-Down)*SIZE+(j+Left-Right)))!=0)) || (((*(Puzzle+i*SIZE+j))==(*(Puzzle+(i+Up-Down)*SIZE+(j+Left-Right)))) && (*(Puzzle+i*SIZE+j))!=0 ))
			if((Puzzle[i][j]==0 && Puzzle[i+Up-Down][j+Left-Right]!=0) || (Puzzle[i][j]==Puzzle[i+Up-Down][j+Left-Right] && Puzzle[i][j]!=0))
				return 1;
		}


	return 0;
}

void End(void)
{
	printf("*********************************************\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                 GAME OVER!                *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*                                           *\n"
			"*********************************************\n");

	//	usleep(5000000);

	//system("clear");
}
