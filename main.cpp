#include <iostream>
#include <ncursesw/curses.h>
#include <vector>
#include <string>
#include <cstdlib>



void ncurses_init_colors() {
	// wiÄcej o kolorach tu https://www.linuxjournal.com/content/programming-color-ncurses

	// musimy ustawiÄ jeĹli bÄdziemy uĹźywaÄ kolorowania konsoli
	start_color();
	// i zdefiniowaÄ pary kolorĂłw ktĂłre bÄdziemy uĹźywaÄ
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
}

void ncurses_config() {
	// definiujemy kolory tekstu ktĂłre uĹźyjemy do kolorowania planszy
	ncurses_init_colors();
	// getch ma interpretowaÄ znaki specjalne takie jak KEY_UP
	keypad(stdscr, TRUE);
	// ustawiamy timeout dla getch (milisekundy)
	// po tym czasie program przejdzie dalej nawet jeĹli nie podasz klawisza
	timeout(5000);
	// podczas wpisywania z klawiatury nie powinna siÄ drukowaÄ literka
	noecho();
}
struct Coord {
	int x,y;
	Coord(int a=0,int b=0):x(a),y(b) {
	}
	void incX() {
		++x;
	}
	void incY() {
		++x;
	}
	void decX() {
		--x;
	}
	void decY() {
		--y;
	}
};
struct Frame {
	int upper_limit,bottom_limit,left_limit,right_limit;
	Frame(int a, int b, int c, int d):upper_limit(a),bottom_limit(b),left_limit(c),right_limit(d) {
	}
};

class MainWindow {
	private:
		int wXR;
		int wYB;
		int wYU;
		int wXL;
		int score = 1;
	public:
		WINDOW * mainwin;

		void window(int yB, int xR, int yU, int xL) {
			wXR = xR;
			wYB = yB;
			wYU = yU;
			wXL = xL;
		}


		void addScore() {
			score +=1;
		}
		void print_board(std::vector<Coord> position, int character,  int appleSkin, Coord apple) {
			mainwin = newwin(wYB,wXR,wYU,wXL);



			for(int h=0; h<wYB-1; h++) {
				mvwprintw(mainwin, h, 0,"#");
				mvwprintw(mainwin, h, wXR-1,"#");

			}

			for(int w=0; w<wXR; w++) {
				mvwprintw(mainwin, 0, w,"#");
				mvwprintw(mainwin, wYB-1, w,"#");
			}



			char const* pchar = std::to_string(score).c_str();

			mvwprintw(mainwin, 0, 1,"SNAKE");
			mvwprintw(mainwin, 0, 7,"SCORE:");
			mvwprintw(mainwin, 0, 13, pchar);

			// operujemy na wirtualnym ekranie bÄdÄcym buforem ekranu
			// a nastÄpnie wyĹwietlamy bufor w terminalu funkcjÄ refresh

			//clear(); // czyĹci wirtualny ekran (lepiej byĹo by czyĹciÄ jeden znak albo jedna linie)

			attron(COLOR_PAIR(1)); // ustawiamy wczeĹniej zdefiniowanÄ perÄ kolorĂłw
			// (moĹźna uĹźyÄ mvaddch zamiast dwĂłch funcji)
			//move(position_y, position_x); // skaczemy kursorem do danej pozycji
			//addch(character); // drukujemy podany znak

			mvwaddch(mainwin,apple.y,apple.x, appleSkin);
			for(int i=0; i < position.size(); i++) {
				mvwaddch(mainwin,position[i].y,position[i].x,character);
			}

			move(0, 0); // aby migajÄcy kursor nam nie przeszkadzaĹfsdfsdfsadfsda
			attroff(COLOR_PAIR(1)); // przywracamy domyĹlny kolor

			refresh(); // wyĹwietlamy zawartoĹÄ wirtualnego ekranu dopiero po refresh
		}
};

class Snake {

	private:
		std::vector<Coord> snake;

	public:
		Snake(Coord vektor) {
			eat(vektor);
		}
		void eat(Coord c) { // push apple
			snake.push_back(c);

		}
		void moveSnake(Coord C) {
			snake.pop_back();
			snake.insert(snake.begin(),C);
		}

		int size() {
			return snake.size();
		}
		std::vector<Coord> vektors() {
			return snake;
		}
		int duplicate(Coord examined,int isSnake=2) {
			for(int i=isSnake; i<size(); i++) {
				if(examined.x==snake[i].x && examined.y == snake[i].y) {
					return true;
				}
			}
			return false;
		}


};

int letter(int character) {

	switch(character) {
		case 'S':
			return 'N';
		case 'N':
			return 'A';
		case 'A':
			return 'K';
		case 'K':
			return 'E';
		case 'E':
			return 'S';
		default:
			break;
	}
	return character;
}
int apple(Coord apple, Coord snakeHead) {
	if(apple.x == snakeHead.x && apple.y == snakeHead.y) {
		return true;
	}

	return false;
}


void check(Coord& pos, Frame limits) {
	if(pos.x < limits.left_limit+1) {
		pos.x = limits.right_limit-2;

	}
	if(pos.x > limits.right_limit-2) {
		pos.x = limits.left_limit+1;

	}
	if(pos.y < limits.upper_limit+1) {
		pos.y = limits.bottom_limit-2;

	}
	if(pos.y > limits.bottom_limit-2) {
		pos.y = limits.upper_limit+1;

	}
}
Coord changeApple(Coord &apple,Frame limits) {
	apple.x = rand() % (limits.right_limit+1-limits.left_limit) + limits.left_limit;
	apple.y = rand() % (limits.bottom_limit+1-limits.upper_limit) + limits.upper_limit;
	check(apple,limits);
	return apple;
}

int main(void) {

	std::cout<<"totalna porazka";

	initscr();
	MainWindow mainwin = MainWindow();


	ncurses_config();


	curs_set(0);
	int last_character = 'S';
	int last_position_x = 15;
	int last_position_y = 15;
	Coord pos(last_position_x, last_position_y);
	Frame limits(0,25,0,50);

	int przegrana = false;
	int last_move = 1;
	mainwin.window(limits.bottom_limit, limits.right_limit, limits.upper_limit,limits.left_limit);
	Coord snake_vektor(last_position_x,last_position_y+1);

	Snake snake(snake_vektor);
	snake.eat(Coord(last_position_x,last_position_y));


	Coord apple1(20,2);


	int apple_character = '@';
	int temporary = '*';

	while(1==1) {
		wclear(mainwin.mainwin);
		int input = getch();
		if(input != ERR) {
			switch(input) {
				case KEY_UP:
					if(apple(apple1, pos)) {
						mainwin.addScore();
						pos.incY();
						pos.decX();
						snake.eat(pos);

						while(snake.duplicate(apple1,0)) {
							changeApple(apple1,limits);
						}


					}
					--pos.y;
					check(pos,limits);
					snake.moveSnake(pos);
					last_move=1;
					break;
				case KEY_DOWN:
					if(apple(apple1,pos)) {
						mainwin.addScore();
						pos.decY();
						snake.eat(pos);

						while(snake.duplicate(apple1,0)) {
							changeApple(apple1,limits);
						}

					}
					++pos.y;
					check(pos,limits);
					snake.moveSnake(pos);
					last_move=2;
					break;
				case KEY_LEFT:
					if(apple(apple1,pos)) {
						mainwin.addScore();
						pos.incX();
						snake.eat(pos);

						while(snake.duplicate(apple1,0)) {
							changeApple(apple1,limits);
						}

					}
					--pos.x;
					check(pos,limits);
					snake.moveSnake(pos);
					last_move=0;
					break;
				case KEY_RIGHT:
					if(apple(apple1,pos)) {
						mainwin.addScore();
						pos.decX();
						snake.eat(pos);
						while(snake.duplicate(apple1,0)) {
							changeApple(apple1,limits);
						}
					}
					++pos.x;
					check(pos,limits);
					snake.moveSnake(pos);
					last_move=4;
					break;
				default:
					break;
			}

		}



		mainwin.print_board(snake.vektors(), temporary,  apple_character, apple1);


		if(snake.duplicate(pos,2)) {
			break;
		}

		if(last_move==1){
			pos.decY();
		}
		if(last_move==2){
			pos.incY();
		}
		if(last_move==0){
			pos.decX();
		}
		if(last_move==4){
			pos.incX();
		}
		check(pos,limits);
		snake.moveSnake(pos);
		timeout(100);
		wrefresh(mainwin.mainwin);



	}
}
