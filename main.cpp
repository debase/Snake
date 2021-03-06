/*----------------------------------------------------------------------------
LAB EXERCISE 9 - Analog input and PWM
 ----------------------------------------
	Use two potentiometers to adjust the volume and pitch of the output sound wave.
	
	Inputs: potentiometers 1 and 2
	Output: speaker, PC

	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <vector>
#include <termios.h>

/* Use this variable to remember original terminal attributes. */

typedef struct s_pos{
	int x;
	int y;
} pos;

struct termios saved_attributes;
std::vector <pos> Snake;

void
reset_input_mode (void)
{
  tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void
set_input_mode (void)
{
  struct termios tattr;
  char *name;

  /* Make sure stdin is a terminal. */
  if (!isatty (STDIN_FILENO))
    {
      fprintf (stderr, "Not a terminal.\n");
      exit (EXIT_FAILURE);
    }

  /* Save the terminal attributes so we can restore them later. */
  tcgetattr (STDIN_FILENO, &saved_attributes);
  atexit (reset_input_mode);

  /* Set the funny terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}
/*
Define the PWM speaker output
Define analog inputs
Define serial output
*/

	//Write your code here


//Define variables
char map[42][103];
bool food = false;
pos foodPoint;
bool add = false;

bool checkFoodPos()
{
	if (foodPoint.x == 0 || foodPoint.x == 41 || foodPoint.y == 0 || foodPoint.y == 99)
		return false;
	int i = Snake.size();
	int j = 0;
	while (j < i)
	{
		if (foodPoint.x == Snake[j].x && foodPoint.y == Snake[j].y)
			return false;
		++j;
	}
	return true;
}

void setFood()
{
	if (food == false)
	{
		time_t tmp = time(NULL);
		srand(tmp);
		foodPoint.x = rand() % 42;
		foodPoint.y = rand() % 103;
		if (checkFoodPos() == false)
		{
			usleep(500);
			setFood();
		}
		else
		{
			food = true;
			printf("%d   %d\n", foodPoint.x, foodPoint.y);
			map[foodPoint.x][foodPoint.y] = '+';
		}
	}
	else
	{
		map[foodPoint.x][foodPoint.y] = '+';
	}
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
void populateMap()
{
	for (int j = 0; j < 101; j++)
		{
			map[0][j] = '=';
		}
		map[0][100] = '\n';
		map[0][101] = '\r';
		map[0][102] = '\0';
	for (int i = 1; i < 41; i++)
	{
		map[i][0] = '=';
		for (int j = 1; j < 100; j++)
		{
			map[i][j] = ' ';
		}
		map[i][99] = '=';
		map[i][100] = '\n';
		map[i][101] = '\r';
		map[i][102] = '\0';
	}
	for (int j = 0; j < 101; j++)
		{
			map[41][j] = '=';
		}
		map[41][100] = '\n';
		map[41][101] = '\r';
		map[41][102] = '\0';
}

void initSnake()
{
	pos p;
	
	p.x = 20;
	p.y = 50;
	Snake.push_back(p);
	p.x = 21;
	p.y = 50;
	Snake.push_back(p);
	p.x = 22;
	p.y = 50;
	Snake.push_back(p);
	p.x = 23;
	p.y = 50;
	Snake.push_back(p);
	p.x = 24;
	p.y = 50;
	Snake.push_back(p);
	p.x = 25;
	p.y = 50;
	Snake.push_back(p);
	p.x = 26;
	p.y = 50;
	Snake.push_back(p);
	p.x = 27;
	p.y = 50;
	Snake.push_back(p);
	p.x = 28;
	p.y = 50;
	Snake.push_back(p);
}

void displayMap()
{
	char tmp[42 * 103];

	tmp[0] = '\0';
	for(int i = 0; i < 42; i++)
	{
		strcat(tmp, map[i]);
	}
	printf("%s\n", tmp);
}

#define GAME_OVER 	0
#define SUCCESS 	1

char check_collison() 
{
	pos head = Snake[0];
	unsigned int i = 1;
	unsigned int size = Snake.size();

	if (head.x == 0 || head.x == 41 || head.y == 0 || head.y == 99)
		return GAME_OVER;

	while (i < size) 
	{
		if (Snake[i].x == head.x && Snake[i].y == head.y) 
		{
			return GAME_OVER;
		}
		if (head.x == foodPoint.x && head.y == foodPoint.y)
		{
			food = false;
			add = true;
		}
		++i;
	}
	return SUCCESS;
}

#define FORWARD 0
#define LEFT 1
#define RIGHT 2
#define BACK 3

int direction = FORWARD;


void moveSnake()
{
	int i = Snake.size();
	int j = 1;
	pos tmp, tmp2;
	switch (direction)
	{
		case FORWARD:
			tmp = Snake[0];
			while (j < i)
			{
				if (j % 2 == 0)
				{
					tmp = Snake[j];
					Snake[j] = tmp2;
				}
				else
				{
					tmp2 = Snake[j];
					Snake[j] = tmp;
				}
				++j;
			}
			Snake[0].x -= 1;
			break;

		case LEFT:
			tmp = Snake[0];
			while (j < i)
			{
				if (j % 2 == 0)
				{
					tmp = Snake[j];
					Snake[j] = tmp2;
				}
				else
				{
					tmp2 = Snake[j];
					Snake[j] = tmp;
				}
				++j;
			}
			Snake[0].y -= 1;
			break;

		case RIGHT:
			tmp = Snake[0];
			while (j < i)
			{
				if (j % 2 == 0)
				{
					tmp = Snake[j];
					Snake[j] = tmp2;
				}
				else
				{
					tmp2 = Snake[j];
					Snake[j] = tmp;
				}
				++j;
			}
			Snake[0].y += 1;
			break;

		case BACK:
			tmp = Snake[0];
				while (j < i)
			{
				if (j % 2 == 0)
				{
					tmp = Snake[j];
					Snake[j] = tmp2;
				}
				else
				{
					tmp2 = Snake[j];
					Snake[j] = tmp;
				}
				++j;
			}
			Snake[0].x += 1;
			break;

		default:
			break;
	}
	if (add == true)
	{
		if ((j - 1) % 2 == 0)
			Snake.push_back(tmp2);
		else
			Snake.push_back(tmp);
		add = false;
	}
}

#define STDIN 0

void drawSnake()
{
	unsigned int i = 0;

	while(i < Snake.size())
	{
		map[Snake[i].x][Snake[i++].y] = 'O';
	}
}

int main(){
	char test[3];
	test[2] = '\0';
	struct timeval tv;
	fd_set readfds;
 
	
 
	// don't care about writefds and exceptfds:
	

	set_input_mode();
	populateMap();
	initSnake();
	while(1){
		tv.tv_sec = 0;
		tv.tv_usec = 50000;
	 
		FD_ZERO(&readfds);
		FD_SET(STDIN, &readfds);
		select(STDIN+1, &readfds, NULL, NULL, &tv);
	 
		if (FD_ISSET(STDIN, &readfds))
		{
			fgets(test, 2, stdin);
			switch(test[0])
			{
				case 'a':
				direction = LEFT;
				break;

				case 'd':
				direction = RIGHT;
				break;			

				case 'w':
				direction = FORWARD;
				break;			

				case 's':
				direction = BACK;
				break;			
			}
		}
		populateMap();
		setFood();
		moveSnake();
		if (check_collison() == GAME_OVER) 
			break;
		drawSnake();
			//sleep(1);
		displayMap();

	}
	reset_input_mode();
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************
