#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

const int WCell = 30;
const int HCell = 20;
const int W = WCell*18;
const int H = HCell*18;

int dx = 0, dy = -1;
char currentDirection = 'D';
float speed = 1.f;
bool board[W][H] = {0};
int score = 0;
bool youWin = false;

bool gameOver = false;

struct Point{
	int x,y;
	Point(int X, int Y){
		x = X;
		y = Y;
	}
} huntingPoint(10,10);

struct Had {
	std::vector<Point> vec;
	void add(int X, int Y){
		Point p(X,Y);
		vec.insert(vec.end(),p);
	}
	Had(int X, int Y){
		vec.insert(vec.begin(),Point(X,Y));
		
	}  
} had(WCell/2,HCell/2);

Point createPoint(){
	int x, y;
	if (had.vec.size()==((HCell-1)*(WCell-1))){
		youWin = true;
		return Point(0,0);
	}
	while (1){
		x= std::rand()*WCell/RAND_MAX;
		y= std::rand()*HCell/RAND_MAX;
		bool ok = true;
		for (auto &piece : had.vec){
			if (piece.x == x && piece.y == y) ok = false; 
		}
		if (ok) break;
	}
	return Point(x,y);
}
// if snake hits hunting point, true is returned
bool moveSnake(){
	
	Point p = had.vec.back();
	
	//printf("x: %d, y: %d",p.x,p.y);
	p.x += dx;
	p.y += dy;
	if (p.x == -1) p.x = WCell-1;
	else if (p.x == WCell) p.x = 0;
	if (p.y == -1) p.y = HCell-1;
	else if (p.y == HCell) p.y = 0;
	//check for colision
	for (auto &piece : had.vec){
		if (piece.x == p.x && piece.y == p.y ){
			gameOver = true;
		}		
	}
	
	had.add(p.x,p.y);
	if (!(p.x == huntingPoint.x && p.y == huntingPoint.y))	{
		had.vec.erase(had.vec.begin());
		return false;
	}
	else
		return true;
}

int main(){
	
	std::srand(std::time(nullptr));	
	
	Clock clock;	 
	huntingPoint = createPoint();
	RenderWindow window(VideoMode(W,H),"Snake");
	
	Texture tx1, tx2,tx3;
	tx1.loadFromFile("ground2.jpg");
	tx2.loadFromFile("tiles.png");
	tx3.loadFromFile("tiles.png");
	Sprite sp1(tx1), sp2(tx2), sp3(tx3);
	bool eventAlreadyRegistered = false;
		
	while (window.isOpen()){
		Event event;
		float relapsed = clock.getElapsedTime().asSeconds();
		
		while (window.pollEvent(event)){
			if (eventAlreadyRegistered) break;
			switch (event.type) {
				case Event::Closed:
					window.close();
				case Event::KeyPressed:
					if (event.key.code == Keyboard::Down) {
						if (currentDirection != 'U') {
						dx = 0; dy = 1;
						currentDirection = 'D';
						eventAlreadyRegistered = true;
						}
					}
					else if (event.key.code == Keyboard::Left) {
						if (currentDirection != 'R') {
						dx = -1; dy = 0;
						currentDirection = 'L';
						eventAlreadyRegistered = true;
						}
					}
					else if (event.key.code == Keyboard::Up) {
						if (currentDirection != 'D') {
						dx = 0; dy = -1;
						currentDirection = 'U';
						eventAlreadyRegistered = true;
						}
					}
					else if (event.key.code == Keyboard::Right) {
						if (currentDirection != 'L') {
						dx = 1; dy = 0;
						currentDirection = 'R';
						eventAlreadyRegistered = true;
						}
					}
					else break;				
			}
		}
		
		sp2.setTextureRect(IntRect(0,0,18,18));
		if (relapsed > 1.f/(1.f+speed)){
			eventAlreadyRegistered = false;
			if (moveSnake()){
				huntingPoint = createPoint();
				speed += 1.f;
				score += 1;
			}
			if (gameOver || youWin){				
				window.close();
			}			
			window.clear(Color::Blue);
			window.draw(sp1);
			for (int i = 0; i < had.vec.size() ;++i){ //
				sp2.setPosition(had.vec.at(i).x*18,had.vec.at(i).y*18);
				sp2.setTextureRect(IntRect(0,0,18,18));
				window.draw(sp2);
			}
			sp3.setPosition(huntingPoint.x*18,huntingPoint.y*18);
			sp3.setTextureRect(IntRect(36,0,18,18));
			window.draw(sp3);
			
			clock.restart();
				
			window.display();
		}
	}
	RenderWindow win2(VideoMode(800,500),"Game Over");
	
	while(win2.isOpen()){
		Event ev;
		Text text, text2;
		Font font;
		font.loadFromFile("arial.ttf");
		text.setFont(font);
		text2.setFont(font);
		text.setFillColor(Color::Red);
		text.setFillColor(Color::Blue);
		text.setStyle(Text::Bold);
		String st = std::to_string(score);
		if (youWin){
			text.setString("You are the champion!");
		}
		else {
			text.setString("You have lost terribly \nYour score is " + st);	
		}
		
		
		text2.setString("For leaving the game, press enter");
		text2.setCharacterSize(15);
		text2.setPosition(0,80);
		
		while (win2.pollEvent(ev)){
			switch (ev.type){
				case Event::KeyPressed:{
					if (ev.key.code == Keyboard::Return)	{
					win2.close();
					break;
					}
				}
			}
		}
		win2.draw(text);
		win2.draw(text2);
		win2.display();
	}
	return 0;
}
