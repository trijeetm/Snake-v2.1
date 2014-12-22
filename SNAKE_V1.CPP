/*
Program:	Snake
Created By:	Trijeet Mukhopadhyay (XII-J)
Changelog:	Bonus food, extra lives added
		Classes, Highscores
*/

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <fstream.h>
#include <iomanip.h>

#define tc textcolor
#define tb textbackground

char input;
int snx=40,sny=25,j=1,i,game=1,lives=5;
const char TL=218,TR=191,BL=192,BR=217,HL=196,VL=179;

class player
{
protected:
	char nick[20];
	int lev, score;
public:
	player()
	{
		score=0;
	}
	int level()
	{
		return lev;
	}
	int rscore()
	{
		return score;
	}
	char* name()
	{
		return nick;
	}
	void changescore(int x)
	{
		score+=x;
	}
	void display()
	{
		cout<<setw(25)<<nick<<setw(10)<<lev<<setw(10)<<score<<endl;
	}
	void enterdetails();
	void writescores();
	void allscores();
	void playerscores();
	void levelscores();
};

class food
{
protected:
	int x,y,bfx,bfy,elx,ely;
public:
	int returnx()
	{
		return x;
	}
	int returny()
	{
		return y;
	}
	int returnbfx()
	{
		return bfx;
	}
	int returnbfy()
	{
		return bfy;
	}
	int returnelx()
	{
		return elx;
	}
	int returnely()
	{
		return ely;
	}
	void reset_bonus()
	{
		bfx=0;
		bfy=0;
		elx=0;
		ely=0;
	}
	void gen_food();
	void gen_bonusfood();
	void gen_extralife();
};

struct bars
{
	char type;
	int size,x,y,hit;
};

class barrier
{
protected:
	bars bar[50];
	bars HIT;
public:
	void bargen (int, int, int);
	int barcheck (int, int);
};

void move (char);
void SNAKE (int);
void intro();
void rules();
void beep (int, int);
void infobar (int, int, char[], int, int);
void DispCh(int C,int R,char Ch);
void DispCh(int C,int R,char Ch[]);
void Color(int TC,int TB);
void Recta(int C1,int R1,int C2,int R2);
int VMenu(int C,int R,char Men[][20],int Noo);
int HMenu(int C,int R,char Men[][20],int Noo);
int menu();

player p;
food f;
barrier b;

void main()
{
	_setcursortype(_NOCURSOR);
	textmode(C4350);
	intro();
	window(1,1,80,50);
	clrscr();
	int menuloop=1;
	do 
	{
		int cont=menu();
		if ((cont==1)||(cont==2))
		{
			game=1;
			menuloop=0;
		}
		else if (cont==4)
		{
			game=0;
			menuloop=0;
		}
		else if (cont==3)
		{
			game=1;
			menuloop=1;
		}
	} while(menuloop==1);
	window(1,1,80,50);		
	while ( (game==1)&&(lives>0)&&(j==1) )
	{
		rules();
		window(1,1,80,50);
		tb(BLACK);
		int i=1;
		lives=5;
		j=1;
		snx=40;
		sny=25;
		clrscr();
		i=1;
		p.enterdetails();
		clrscr();
		gotoxy (snx,sny);
		textcolor(RED);
		cprintf ("\351");
		b.bargen (snx,sny,p.level());
		gotoxy(snx,sny);
		for (;i<=(p.level());i++)
		{
			int scorefactor=0;
			scorefactor=i*(p.level()/10);
			p.changescore(scorefactor);
			f.gen_food();
			if ( ((i%15)-1==0) && (i!=1) )
			{
				f.gen_extralife();
				infobar(p.level(),((p.level())-i+1),"EXTRA LIFE SPAWNED!!!",p.rscore(),lives);
			}
			else if ( ((i%5)-1==0) && (i!=1) )
			{
				f.gen_bonusfood();
				infobar(p.level(),((p.level())-i+1),"BONUS LEVEL!!!",p.rscore(),lives);
			}
			else
			{
				gotoxy(f.returnbfx(),f.returnbfy());
				cout<<" ";
				gotoxy(f.returnelx(),f.returnely());
				cout<<" ";
				f.reset_bonus();
				infobar(p.level(),((p.level())-i+1)," ",p.rscore(),lives);
			}
			gotoxy(snx,sny);
			while (j==1)
			{
				input=getch();
				move(input);
				if ((snx==f.returnx())&&(sny==f.returny()))
				{
					p.changescore(10);
					beep(2000,100);
					break;
				}
				if (b.barcheck(snx,sny)==1)
				{
					lives--;
					infobar(p.level(),((p.level())-i+1),"LIFE LOST!!! RESETTING...",p.rscore(),lives);
					beep(5000,500);
					gotoxy(snx,sny);
					cprintf ("X");
					snx=40;
					sny=25;
					gotoxy (snx,sny);
					textcolor(RED);
					cprintf ("\351");
					if (lives==0)
					{
						i=p.level()*3;
						break;
					}
					delay(1000);
				}
				if ( (f.returnbfx()==snx)&&(f.returnbfy()==sny) )
				{
					p.changescore(50);
					infobar(p.level(),((p.level())+1-i),"BONUS POINTS!!! (+50)",p.rscore(),lives);
					beep(3000,50);
					beep(3050,100);
				}
				if ( (f.returnelx()==snx)&&(f.returnely()==sny) )
				{
					lives++;
					p.changescore(30);
					infobar(p.level(),((p.level())+1-i),"LIVES INCREASED!!! (+1)",p.rscore(),lives);
					beep(2800,50);
					beep(2850,50);
					beep(2950,100);
				}
			}
		}
		clrscr();
		p.changescore(lives*100);
		infobar(p.level(),0,"CONGRATULATIONS!!!",p.rscore(),lives);
		p.writescores();
		if (i==(p.level())+1)
		{
			gotoxy(1,15);
			for (int col=1;col<5;col++)
			{
				textcolor(col);
				textbackground(col+3);
				cprintf(" LEVEL COMPLETE!!! ");
			}
		}
		else
		{
			gotoxy(1,15);
			for (int col=1;col<=5;col++)
			{
				textcolor(col);
				textbackground(col+3);
				cprintf(" LEVEL LOST!!!  ");
			}
		}
		delay(5000);
		getch();
		window(1,30,80,50);
		do 
		{
			int cont=menu();
			if ((cont==1)||(cont==2))
			{
				game=1;
				j=1;
				lives=5;
				menuloop=0;
			}
			else if (cont==4)
			{
				game=0;
				menuloop=0;
			}
			else if (cont==3)
			{
				game=1;
				menuloop=1;
			}
		} while(menuloop==1);
	}
}

void move (char input)
{
	int moveloop=0;
	while (moveloop==0)
	{
		if (kbhit())
			break;
		else
		{
			delay (1600/p.level());
			gotoxy (snx,sny);
			cout<<" ";
			if ( (input=='w') || ((int)input==72) )
			{
				if (sny<=2)
					sny=49;
				else
					sny--;
			}
			else if ( (input=='s') || ((int)input==80) )
			{
				if (sny>=49)
					sny=2;
				else
					sny++;
			}
			else if ( (input=='a') || ((int)input==75) )
			{
				if (snx<=1)
					snx=80;
				else
					snx--;
			}
			else if ( (input=='d') || ((int)input==77) )
			{
				if (snx>=80)
					snx=1;
				else
					snx++;
			}
			else if ((int)input==27)
			{
				i=-1;
				j++;
				lives=0;
			}
			gotoxy (snx,sny);
			textcolor(RED);
			cprintf ("\351");
			gotoxy(snx,sny);
			if ( ((snx==f.returnx())&&(sny==f.returny()))||(b.barcheck(snx,sny)==1)||((snx==f.returnelx())&&(sny==f.returnely()))||((snx==f.returnbfx())&&(sny==f.returnbfy())) )
				break;
		}
	}
}

void food::gen_food()
{
	do {
		x=rand()%80;
		y=rand()%50;
	} while ((x==snx) || (y==sny) || (b.barcheck(x,y)==1) || (y<2) || (x<1));
	gotoxy (x,y);
	textcolor (BLUE);
	cprintf ("+");
}

void food::gen_bonusfood()
{
	do {
		bfx=rand()%80;
		bfy=rand()%50;
	} while ((bfx==snx) || (bfy==sny) || (b.barcheck(bfx,bfy)==1) || (bfy<2) || (bfx<1));
	gotoxy (bfx,bfy);
	textcolor (GREEN);
	cprintf ("+");
}

void food::gen_extralife()
{
	do {
		elx=rand()%80;
		ely=rand()%50;
	} while ((elx==snx) || (ely==sny) || (b.barcheck(elx,ely)==1) || (ely<2) || (elx<1));
	gotoxy (elx,ely);
	textcolor (LIGHTRED+BLINK);
	cprintf ("%c",254);
}

void barrier::bargen (int sx, int sy, int level)
{
	int i=0,sizelim;
	if (level<11)
		sizelim=25;
	else if (level<21)
		sizelim=20;
	else if (level<31)
		sizelim=18;
	else if (level<41)
		sizelim=12;
	else if (level<51)
		sizelim=10;
	for (int b=1; b<=level; b++)
	{
		do
		{
			bar[i].size=random(sizelim);
		} while (bar[i].size<3);
		if (b%2==0)
		{
			bar[i].type='v';
			do
			{
				bar[i].x=random(81);
			} while ((bar[i].x<1)||(bar[i].x==sx));
			do
			{
				bar[i].y=random(50-bar[i].size);
			} while (bar[i].y<2);
		}
		else
		{
			bar[i].type='h';
			do
			{
				bar[i].y=random(50);
			} while ((bar[i].y<2)||(bar[i].y==sy));
			do
			{
				bar[i].x=random(81-bar[i].size);
			} while (bar[i].x<1);
		}
		for (int j=0,dx=bar[i].x,dy=bar[i].y;j<bar[i].size;j++)
		{
			if (bar[i].type=='v')
			{
				window (1,1,80,50);
				gotoxy (dx,dy);
				textcolor (WHITE);
				cprintf ("X");
				dy++;
			}
			else
			{
				window (1,1,80,50);
				gotoxy (dx,dy);
				textcolor (WHITE);
				cprintf ("X");
				dx++;
			}
		}
		i++;
	}
}

int barrier::barcheck (int checkx, int checky)
{
	HIT.hit=0;
	for (int i=0;i<p.level();i++)
	{
		if (bar[i].type=='v')
		{
			if (bar[i].x==checkx)
			{
				if ((checky>=bar[i].y)&&(checky<=(bar[i].y+bar[i].size-1)))
				{
					HIT=bar[i];
					HIT.hit=1;
					i=p.level()+1;
					return 1;
				}
			}
		}
		else if (bar[i].type=='h')
		{
			if (bar[i].y==checky)
			{
				if ((checkx>=bar[i].x)&&(checkx<=(bar[i].x+bar[i].size-1)))
				{
					HIT=bar[i];
					HIT.hit=1;
					i=p.level()+1;
					return 1;
				}
			}
		}
		else
			return 0;
	}
}

void player::enterdetails()
{
	do {
		cout<<"Enter Player Name (Less than 20 characters)\n";
		gets(nick);
	} while (strlen(nick)>20);
	do {
		cout<<"\nEnter Level (Less than 50)\n";
		cin>>lev;
	} while (lev>50);
}

void player::writescores()
{
	fstream f;
	f.open("Scores",ios::binary|ios::in|ios::out);
	f.seekp(0,ios::end);
	f.write((char*)&p,sizeof(player));
	int nor=f.tellg()/sizeof(player);
	player pj,pjp1;
	for (int i = 0; i < nor-1; i++)
	{
		for (int j = 0; j < nor-i-1; j++)
		{
			f.seekg(j*sizeof(player));
			f.read((char*)&pj,sizeof(player));
			f.read((char*)&pjp1,sizeof(player));
			if (pj.rscore()<pjp1.rscore())
			{
				f.seekp(j*sizeof(player));
				f.write((char*)&pjp1,sizeof(player));
				f.write((char*)&pj,sizeof(player));
			}
		}
	}
	f.close();
}

void player::allscores()
{
	window(1,2,80,2);
	textcolor(RED);
	textbackground(BLUE);
	clrscr();
	cout<<setw(25)<<"Player"<<setw(10)<<"Level"<<setw(10)<<"Score"<<endl;
	window(1,3,80,50);
	textcolor(WHITE);
	textbackground(BLACK);
	clrscr();
	fstream f;
	player boss;
	f.open("Scores",ios::binary|ios::in);
	while (f.read((char*)&boss,sizeof(player)))
		boss.display();
	f.close();
	getch();
}

void player::playerscores()
{
	tb(BLACK);
	window(1,2,80,50);
	clrscr();
	window(1,2,80,2);
	textcolor(BLUE);
	textbackground(BLACK);
	clrscr();
	cout<<"Enter name of player:\n";
	char stud[20];
	gets(stud);
	clrscr();
	textcolor(RED);
	textbackground(BLUE);
	clrscr();
	cout<<setw(25)<<"Player"<<setw(10)<<"Level"<<setw(10)<<"Scores"<<endl;
	window(1,3,80,50);
	textcolor(WHITE);
	textbackground(BLACK);
	clrscr();
	fstream f;
	player boss;
	f.open("Scores",ios::binary|ios::in);
	while (f.read((char*)&boss,sizeof(player)))
		if (strcmp(stud,boss.name())==0)
		{
			boss.display();
		}
	f.close();
	getch();
}

void player::levelscores()
{
	window(1,2,80,50);
	tb(BLACK);
	clrscr();
	window(1,2,80,2);
	textcolor(BLUE);
	textbackground(BLACK);
	clrscr();
	cout<<"Enter level:\n";
	int lev;
	cin>>lev;
	clrscr();
	textcolor(RED);
	textbackground(BLUE);
	clrscr();
	cout<<setw(25)<<"Player"<<setw(10)<<"Level"<<setw(10)<<"Scores"<<endl;
	window(1,3,80,50);
	textcolor(WHITE);
	textbackground(BLACK);
	clrscr();
	fstream f;
	player boss;
	f.open("Scores",ios::binary|ios::in);
	while (f.read((char*)&boss,sizeof(player)))
		if (lev==boss.level())
		{
			boss.display();	
		}
	f.close();
	getch();
}

void SNAKE (int textcol)
{
	textcolor(textcol);
	int x=1, y=1;
	for (y=1;y<=15;y++)
	{
		if (y<=8)
		{
			gotoxy(1,y);
			cprintf("S");
		}
		else if (y>=8)
		{
			gotoxy(15,y);
			cprintf("S");
		}
		gotoxy(17,y);
		cprintf("N");
		gotoxy(31,y);
		cprintf("N");
		gotoxy(33,y);
		cprintf("A");
		gotoxy(47,y);
		cprintf("A");
		gotoxy(49,y);
		cprintf("K");
		gotoxy(65,y);
		cprintf("E");
	}
	for (x=1;x<=15;x++)
	{
		gotoxy(x,1);
		cprintf("S");
		gotoxy(x,8);
		cprintf("S");
		gotoxy(x,15);
		cprintf("S");
		gotoxy(x+32,1);
		cprintf("A");
		gotoxy(x+32,8);
		cprintf("A");
		gotoxy(x+64,1);
		cprintf("E");
		gotoxy(x+64,8);
		cprintf("E");
		gotoxy(x+64,15);
		cprintf("E");
	}
	for (y=1,x=1;y<=15;y++)
	{
		gotoxy(y+16,y);
		cprintf("N");
		if (y<=8)
			x--;
		else if (y>8)
			x++;
		gotoxy(63+x,y);
		cprintf("K");
	}
}

void intro()
{
	window(1,1,80,50);
	textbackground(BLACK);
	clrscr();
	window(1,1,80,15);
	textbackground(1);
	clrscr();
	SNAKE(8);
	window(1,17,80,31);
	textbackground(4);
	clrscr();
	SNAKE(15);
	window(1,33,80,47);
	textbackground(7);
	clrscr();
	SNAKE(9);
	window(5,49,80,49);
	textcolor(WHITE);
	textbackground(0);
	cprintf("   -=\t|| Conceptualized and Developed By Trijeet Mukhopadhyay ||\t=-");
	getch();
	window(1,1,80,50);
	tc(RED+BLINK);
	tb(BLACK);
	clrscr();
	gotoxy(30,20);
	cprintf("\t\tIMPORTANT MESSAGE\t\t");
	gotoxy(10,23);
	tc(RED);
	cprintf("If not in full screen mode, press (Alt+Enter) to go fullscreen");
	getch();
}

void rules()
{
	window(1,1,80,50);
	tb(BLACK);
	tc(WHITE);
	clrscr();
	window(1,1,80,17);
	SNAKE(7);
	window(2,2,80,18);
	SNAKE(8);
	window(3,3,0,19);
	SNAKE(RED);
	window(1,20,75,50);
	cout<<"Rules:\n";
	cout<<"The 'Snake' game consists of a red blip (you) whose motion can be controlled by the player by the following keys or the arrow keys\n";
	cout<<"		W	UP			S	DOWN\n";
	cout<<"		A	LEFT			D	RIGHT\n";
	cout<<"Pressing 'Esc' key will exit the game.\n";
	cout<<"Pressing any other key will pause game.\n";
	cout<<"User shall be asked to input a level they wish to play.. The level shall be generated and the aim of the player is to navigate around the screen, dodging the barriers (walls made of X), while collecting the food and increasing their score...\n";
	cout<<"Here are the elements that the player will encounter in the game:\n";
	cout<<"		é	SNAKE			+	FOOD\n";
	cout<<"		X	BARRIER			þ	EXTRA LIFE\n";
	cout<<"Regular food (+10) is BLUE, while bonus food (+50) is GREEN.\n";
	cout<<"The player begins with 5 lives, each level lasts as long there is food left to be eaten.\n";
	cout<<"Hitting a barrier results in loss of a life, picking up a EXTRA LIFE results in increase of a life and 30 points.\n";
	cout<<"Eating BLUE food increases your score by 10, GREEN FOOD by 50.\n";
	cout<<"A level terminates if the user completes it by eating all the food, or they run out of lives.\n";
	cout<<"Also, the user can abort anytime by pressing the 'Esc' key. Pressing any other key will pause the game.\n\n\n";
	for (int col=1;col<5;col++)
	{
		textcolor(col);
		textbackground(col+3);
		cprintf("  BEST OF LUCK!!  ");
	}
	getch();
	window(1,1,80,50);
	clrscr();
	textcolor(WHITE);
	textbackground(0);
}

void beep (int freq, int t)
{
	sound(freq);
	delay(t);
	nosound();
}

void infobar (int lev, int frem, char msg[], int score, int liv)
{
	window (1,1,80,1);
	textbackground(7);
	clrscr();
	textcolor(LIGHTBLUE);
	cprintf("Level: %d \t\t\t\t\t Food Remaining: %d",lev,frem);
	gotoxy(53,1);
	textcolor(RED+BLINK);
	cprintf("%s",msg);
	window(1,50,80,50);
	clrscr();
	textcolor(GREEN);
	cprintf("Score: %d",score);
	gotoxy(60,1);
	textcolor(RED);
	cprintf("Lives Remaining: %d",liv);
	window(1,1,80,50);
	textbackground(0);
}

int menu()
{
	_setcursortype(_NOCURSOR);
	textmode(C4350);
	textbackground(BLACK);
	clrscr();
	Recta(10,10,48,20);
	Recta(2,2,10,9);
	Recta(1,1,49,24);
	char Menu[][20]={"       Rules       ",
			 		 "       Play!       ",
			 		 "      Scores       ",
			 		 "       Quit        "};
	char SMenu[][20]={" View all scores ",
			   		  " View by level   ",
			   		  " View by player  "};
	int Quit=0;
	int choice=0;
	do
	{
		textbackground(BLACK);
		clrscr();
		int N=HMenu(1,1,Menu,4),SN;
		switch (N)
		{
			case 0:
				choice=1;
				rules();
				break;
			case 1:
				Quit=1;
				choice=1;
				break;
			case 2:
				SN=VMenu(42,2,SMenu,3);
				choice=3;
				Quit=0;
				switch(SN)
				{
					case 0:
						p.allscores();
						break;
					case 1:
						p.levelscores();
						break;
					case 2:
						p.playerscores();
						break;
				}
			case 3:
				choice=4;
				Quit=1;
		}
	} while (!Quit);
	return choice;
}

void DispCh(int C,int R,char Ch)
{
	gotoxy(C,R);cprintf("%c",Ch);
}

void DispCh(int C,int R,char S[])
{
	gotoxy(C,R);cprintf("%s",S);
}

void Color(int TC,int TB)
{
	textcolor(TC);
	textbackground(TB);
}

void Recta(int C1,int R1,int C2,int R2)
{
	DispCh(C1,R1,TL);
	DispCh(C2,R1,TR);
	DispCh(C1,R2,BL);
	DispCh(C2,R2,BR);
	for (int i=C1+1;i<C2;i++)
	{
		DispCh(i,R1,HL);
		DispCh(i,R2,HL);
	}
	for (i=R1+1;i<R2;i++)
	{
		DispCh(C1,i,VL);
		DispCh(C2,i,VL);
	}
}

int VMenu(int C,int R,char Men[][20],int Noo)
{
	Color(LIGHTGRAY,GREEN);
	Recta(C,R,C+strlen(Men[0])+1,R+Noo+1);
	for (int i=0;i<Noo;i++)
		DispCh(C+1,R+i+1,Men[i]);

  	int Exit=0,Sel=0;	

	do
	{
	int Tsel=Sel;
	Color(GREEN,LIGHTGRAY);
	DispCh(C+1,R+Sel+1,Men[Sel]);
	Color(LIGHTGRAY,GREEN);
	char Ch=getch();
	switch(Ch)
	{
	case 71:Sel=0;break;
	case 79:Sel=Noo-1;break;
	case 72:if (Sel==0)
	      Sel=Noo-1;
	    else
	      Sel--;
	    break;
	case 80:if (Sel==Noo-1)
	      Sel=0;
	    else
	      Sel++;
	    break;
	case 27:Sel=-1;
	case 13:Exit=1;
	}
	DispCh(C+1,R+Tsel+1,Men[Tsel]);
	}
	while (!Exit);
	return Sel;
}

int HMenu(int C,int R,char Men[][20],int Noo)
{
	int Exit=0,Sel=0;
	Color(GREEN,LIGHTGRAY);
	for (int i=0;i<Noo;i++)
	DispCh(C+20*i+1,R,Men[i]);
	do
	{
		int Tsel=Sel;
		Color(LIGHTGRAY,GREEN);
		DispCh(C+20*Sel+1,R,Men[Sel]);
		Color(GREEN,LIGHTGRAY);
		char Ch=getch();
		switch(Ch)
		{
		case 71:
			Sel=0;
			break;
		case 79:
			Sel=Noo-1;
			break;
		case 75:
			if (Sel==0)
				Sel=Noo-1;
			else
				Sel--;
			break;
		case 77:
			if (Sel==Noo-1)
				Sel=0;
		    else
			Sel++;
		    break;
		case 27:
			Sel=-1;
		case 13:
			Exit=1;
		}
		DispCh(C+20*Tsel+1,R,Men[Tsel]);
	} while (!Exit);
	return Sel;
}
