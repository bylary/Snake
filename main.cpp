#include <iostream>
#include <ncursesw/curses.h>
#include <vector>
#include <string>


void ncurses_init_colors(){
	// wiÄcej o kolorach tu https://www.linuxjournal.com/content/programming-color-ncurses

	// musimy ustawiÄ jeĹli bÄdziemy uĹźywaÄ kolorowania konsoli
	start_color();
	// i zdefiniowaÄ pary kolorĂłw ktĂłre bÄdziemy uĹźywaÄ
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
}

void ncurses_config(){
	// definiujemy kolory tekstu ktĂłre uĹźyjemy do kolorowania planszy
	ncurses_init_colors();
	// getch ma interpretowaÄ znaki specjalne takie jak KEY_UP
	keypad(stdscr, TRUE);
	// ustawiamy timeout dla getch (milisekundy)
	// po tym czasie program przejdzie dalej nawet jeĹli nie podasz klawisza
	timeout(500);
	// podczas wpisywania z klawiatury nie powinna siÄ drukowaÄ literka
	noecho();
}

void print_board(std::vector<int> position_y, std::vector<int> position_x, int character, int length, int apple, int apple_x, int apple_y){
	// operujemy na wirtualnym ekranie bÄdÄcym buforem ekranu
	// a nastÄpnie wyĹwietlamy bufor w terminalu funkcjÄ refresh
	
	clear(); // czyĹci wirtualny ekran (lepiej byĹo by czyĹciÄ jeden znak albo jedna linie)
	
	attron(COLOR_PAIR(1)); // ustawiamy wczeĹniej zdefiniowanÄ perÄ kolorĂłw
	// (moĹźna uĹźyÄ mvaddch zamiast dwĂłch funcji)
	//move(position_y, position_x); // skaczemy kursorem do danej pozycji
	//addch(character); // drukujemy podany znak
	move(apple_y,apple_x);
	addch(apple);
	for(int i=0; i < length; i++){
	
		move(position_y[i],position_x[i]);
		addch(character);
		//move(position_y[i],position_x[i]+1);
		//addch(character);
	}
	
	move(0, 0); // aby migajÄcy kursor nam nie przeszkadzaĹ
	attroff(COLOR_PAIR(1)); // przywracamy domyĹlny kolor
	
	refresh(); // wyĹwietlamy zawartoĹÄ wirtualnego ekranu dopiero po refresh
}
int letter(int character){
	
	switch(character){
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
int apple(int apple_x, int apple_y, int snake_x, int snake_y){
	if(apple_x == snake_x && apple_y == snake_y){
		return true;
	}
	
	return false;
}

std::vector<int> snake_vektor(std::vector<int> position, int length){
	
	for(int i = 1; i < length; i++ ){
		position[i-1] = position[i];
	}
	
	return position;
}

int main(void) {
	// inicjalizacja ncurses
	WINDOW * mainwin = initscr();
	ncurses_config();

	int last_character = 'S';
	int last_position_x = 15;
	int last_position_y = 15;
	int upper_limit = 1;
	int bottom_limit = 25;
	int left_limit = 1;
	int right_limit = 50;
	std::vector<int> snake_x;
	std::vector<int> snake_y;
	snake_x.push_back(last_position_x);
	snake_y.push_back(last_position_y+1);
	snake_x.push_back(last_position_x);
	snake_y.push_back(last_position_y);
	//int length  = 1;
	int apple_x = 20;
	int apple_y = 2;
	int apple_character = '$';
	int temporary = '*';
	
	while(1==1){
		
		// sczytujemy literkÄ z klawiatury
		// (jeĹli sÄ tu znaki specjalne musi byÄ int bo nie zmieszczÄ siÄ w char)

		
		int input = getch();
		if(input != ERR){
			switch(input){
				case KEY_UP:
					if(apple(apple_x,apple_y,last_position_x,last_position_y) ==true){
						
						snake_y.insert(snake_y.begin(),snake_y[0]-1);
						snake_x.insert(snake_x.begin(),snake_x[0]);
					
						
					}
					snake_x = snake_vektor(snake_x, snake_x.size());
					snake_y = snake_vektor(snake_y, snake_y.size());
					--last_position_y;
					snake_y[snake_y.size()-1] = last_position_y;
					snake_x[snake_x.size()-1] = last_position_x;
					break;
				case KEY_DOWN:
					if(apple(apple_x,apple_y,last_position_x,last_position_y) ==true){
						
						snake_y.insert(snake_y.begin(),snake_y[0]-1);
						snake_x.insert(snake_x.begin(),snake_x[0]);
						
						
					}
					snake_x = snake_vektor(snake_x, snake_x.size());
					snake_y = snake_vektor(snake_y, snake_y.size());
					++last_position_y;
					snake_y[snake_y.size()-1] = last_position_y;
					snake_x[snake_x.size()-1] = last_position_x;
					break;
				case KEY_LEFT:
					if(apple(apple_x,apple_y,last_position_x,last_position_y) ==true){
					
						snake_y.insert(snake_y.begin(),snake_y[0]);
						snake_x.insert(snake_x.begin(),snake_x[0]+1);
						
						
					}
					snake_x = snake_vektor(snake_x, snake_x.size());
					snake_y = snake_vektor(snake_y, snake_y.size());
					
					--last_position_x;
					snake_y[snake_y.size()-1] = last_position_y;
					snake_x[snake_x.size()-1] = last_position_x;
					break;
				case KEY_RIGHT:
					if(apple(apple_x,apple_y,last_position_x,last_position_y) ==true){
						
						snake_y.insert(snake_y.begin(),snake_y[0]);
						snake_x.insert(snake_x.begin(),snake_x[0]-1);
						
						
					}
					snake_x = snake_vektor(snake_x, snake_x.size());
					snake_y = snake_vektor(snake_y, snake_y.size());
					
					++last_position_x;
					snake_y[snake_y.size()-1] = last_position_y;
					snake_x[snake_x.size()-1] = last_position_x;
					break;
				default:
					break;
			}
		
		}
		
		if(last_position_x < left_limit) ++last_position_x;
		if(last_position_x > right_limit) --last_position_x;
		if(last_position_y < upper_limit) ++last_position_y;
		if(last_position_y > bottom_limit) --last_position_y;
		print_board(snake_y, snake_x, temporary, snake_x.size(), apple_character, apple_x, apple_y);
		
		
		
	}
	
	
	// zakaĹczamy prace ncurses
	delwin(mainwin);
	endwin();
	refresh();
	return EXIT_SUCCESS;
}