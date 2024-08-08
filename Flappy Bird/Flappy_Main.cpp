#include <iostream>
#include <Windows.h>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <conio.h>
#define salto 32

using namespace std;

void gotoxy(int x, int y) {//ancho 80 alto 25 gotoxy puede actualizarse con una variable
	HANDLE input = GetStdHandle(STD_OUTPUT_HANDLE);//hcon es una variable se le puede manipular el nombre
	COORD coord;//lo mismo con dwPos
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(input, coord);
}

class Game {
public:
	virtual void start() {}
	virtual void update() {}
	virtual void Collision() {}
};

class Pajaro : public Game{
private:
	char tecla, nuevatecla = salto;
	void Dibujo() {
		gotoxy(X, Y); cout << "<(0>0)>";
	}
	void Teclas(){
		if (_kbhit()) {
			tecla = _getch();
		}
		if (tecla == nuevatecla) {
			Y-=2;
			tecla = 0;
		}else {
			Y++;
			tecla = 0;
		}
	}
	void MostrarPuntaje() {
		gotoxy(65, 10); cout << "Tus puntos: " << Puntos;
	}
	void MostrarRecord() {
		gotoxy(65, 15); cout << "Tu Record es: " << Record;
	}
	void IniciarRecord() {
		int contenido;
		ifstream archivo("record.txt");
		if (archivo.is_open()) {
			while (archivo >> contenido) {
				Record = contenido;
			}
		}
	}
public:
	bool Perdio = false;
	const int X = 2;
	int Y = 10, Puntos, Record;
	void Collision(){
		if (Y < 0 || Y > 24)Perdio = true;
	}
	void start() { 
		IniciarRecord(); 
	}
	void update(){
		Collision();
		Dibujo();
		Teclas();
		MostrarPuntaje();
		MostrarRecord();
	}
	void RecordNuevo() {
		ofstream archivo("record.txt", ios::out);
		if (Puntos > Record)archivo << Puntos;
		else archivo << Record;
		archivo.close();
	}
	void GameOver() {
		system("cls");
		gotoxy(20, 5); cout << "Perdiste jaja Manco";
		system("pause");
		Perdio = false;
		system("cls");
	}
	void NewTeclas() {
		cout << "Presione una tecla";
		nuevatecla = _getch();
		system("cls");
	}
}; Pajaro p;

class Tubos : public Game{
private:
	int Y = rand()%17;
	void dibujar() {
		for (int i = Y; i > 0; i--) {
			gotoxy(X, i); cout << "[*]";
		}
		for (int i = Y + 4; i < 25; i++) {
			gotoxy(X, i); cout << "[*]";
		}
	}
	void Movimiento() {
		if (X != 1)X--;
		else {
			X = 60;
			Y = rand() % 17;
		}
	}
public:
	int X;
	void start() {}
	void Collision() {
		if (p.X == X) { 
			if (p.Y > Y-1 && p.Y < Y + 5)p.Puntos++;
			else p.Perdio = true;
			gotoxy(70, 2);
		}
	}
	void update(){
		Collision();
		dibujar();
		Movimiento();
	}
}; Tubos t[3];

class Play : public Game {
private:
	int distancia = 20;
	void DibujoLimite() {
		for (int i = 0; i < 80; i++) {
			gotoxy(i, 0); cout << "*";
		}
		for (int i = 0; i < 80; i++) {
			gotoxy(i, 24); cout << "*";
		}
		for (int i = 0; i < 25; i++) {
			gotoxy(0, i); cout << "*";
		}
		for (int i = 0; i < 25; i++) {
			gotoxy(65, i); cout << "*";
		}
		for (int i = 0; i < 25; i++) {
			gotoxy(80, i); cout << "*";
		}
	}

public:
	void start() { 
		p.start();
		for (int i = 0; i < 3; i++) {
			t[i].X = distancia;
			distancia += 20;
		}
		distancia = 20;
	}
	void update(){
		DibujoLimite();
		p.update();
		for (int i = 0; i < 3; i++) {
			t[i].update();
		}
		Sleep(17);
		system("cls");
		if (p.Perdio == false)update();
		else {
			p.RecordNuevo();
			p.Puntos = 0;
			p.GameOver();
		}
	}
}; Play play;

class Interfaz : public Game {
private:
	char tecla;
	int Posicion = 5;
	void Teclear() {
		if (_kbhit) {
			tecla = _getch();
		}
		if ((tecla == 'w' || tecla == 'W') && (Posicion > 5))Posicion--;
		else if ((tecla == 's' || tecla == 'S') &&( Posicion < 7))Posicion++;
		else if (tecla == 32)Elecciones();
		else tecla = 0;
	}
	void Eleccion() {
		gotoxy(5, Posicion); cout << "->";
		gotoxy(10, 5); cout << "Cambiar Tecla";
		gotoxy(10, 6); cout << "Jugar";
		gotoxy(10, 7); cout << "Creado por:";
	}
	void Elecciones() {
		switch (Posicion) {
			system("cls");
		case 5:p.NewTeclas();
			break;
		case 6: play.start();
			play.update();
			break;
		case 7:gotoxy(25, 20); cout << "Creado por raton gamer";
			break;
			system("cls");
		}
	}
public:
	void update() {
		Eleccion();
		Teclear();
		Sleep(17);
		system("cls");
		update();
	}
}; Interfaz i;

int main() {
	i.update();
	return 0;
}