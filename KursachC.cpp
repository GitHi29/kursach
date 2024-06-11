// Лабораторная 1.cpp: главный файл проекта.

#include "stdafx.h" //подключение библиотек
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <clocale>
#include <malloc.h>
#include <string.h>

using namespace System;
using namespace std;
using namespace System::IO;

#define ENTER 13 //Обозначение кнопок
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 55
#define END 49

char dan[8][80]={
 "Какой блогер имеет самое большое кол-во зрителей  ?                  ",
 "Кто создал канал раньше всех ?                                       ",
 "Список \"Технических\" каналов с кол-вом подписчиков свыше 10 000      ",
 "Алфавитный список всех блогеров                                      ",
 "Каналы с разными тематиками, с одинаковым кол-вом подписчиков        ",
 "Количество каналов                                                   ",
 "Диграмма.Процентное соотношение подписчиков                          ",                                                        
 "Выход                                                                "
};  //формирование вопросов меню

char BlankLine[80]=
"                                                              "; // пустая строка для форматирования

int NC;//количество позиций задано глобально

struct z {
	
	char name[20];//ФИО владельца канала
	char vid[20];//Тематика канала 
	long summa;//Кол-во подписчиков
	long vews;//Кол-во просмотров
	char data[11];//Дата
};

struct sp{ 
	char vid[20];
	char fio[20];
	long summa;
	struct sp* sled;
	struct sp* pred;
}; 
struct sp* spisok; //глобальное описание структуры списка

int menu(int); //шаблоны используемых функций
void slojn (struct z*);
void maxim(struct z*);
void first(struct z*);
void text_data(char *, char *);
void kolvo(struct z *);
void alfalist(struct z *);
void vstavka(struct z*, char*);
void listing(struct z*);
void diagram(struct z*); 

int main(array<System::String ^> ^args) 
//открытие файла на локальном уровне
//считывание данных в выделенную память
{
	system("chcp 1251");//Установка кодовой страницы консоли для вывода на русском языке
	system("cls");//очистка консоли
	int i,n;
	FILE *in;
	struct z *subs;
	Console::CursorVisible::set(false); //отключаем видимость курсора
	Console::BufferHeight=Console::WindowHeight; //установка размера буфера по ширине
	Console::BufferWidth=Console::WindowWidth;   //установка размера буфера по высоте
	if((in=fopen("Canal1.txt", "r"))==NULL)
	{
		printf("\n Файл не открыт !"); //вывод ошибки при неудачном открытии файла 
		getch(); exit(0);
	}

	fscanf(in,"%d",&NC);
	subs=(struct z*)malloc(NC*sizeof(struct z)); //выделение памяти для 

	// Вывод заголовков перед данными
	printf("Список владельцев видеоканалах(блогеров):\n\n %-20s %-20s %-15s %-15s %-11s", "ФИО", "Тематика", "Подписчики", "Просмотры", "Дата");

	for(i = 0;i < NC; i++) //считываем данные из списка
		fscanf(in, "%s%s%ld%ld%s", subs[i].name,
		subs[i].vid, &subs[i].summa, &subs[i].vews,
		subs[i].data);
	for(i=0;i<NC;i++) //выводим данные из списка
		printf("\n %-20s %-20s %-15ld %-15ld %-11s",
		subs[i].name,
		subs[i].vid, subs[i].summa,subs[i].vews,
		subs[i].data);
	_getch();

 while(1) //в цикле создаем область для вопросов и окрашиваем их в цвета
		//устанавливаем цвет для букв и строки выбранного вопроса	
{
	Console::BackgroundColor=ConsoleColor::White;
	Console::ForegroundColor=ConsoleColor::DarkMagenta
		;
	Console::Clear ();
	Console::ForegroundColor=ConsoleColor::DarkMagenta;
	Console::BackgroundColor=ConsoleColor::White;
	Console::CursorLeft=10;
	Console::CursorTop=4;
	printf(BlankLine);

	for(i=0;i<7;i++) //цикл вывода вопросов
	{
		Console::CursorLeft=10; //первая точка, откуда будем закрашивать область меню
		Console::CursorTop=i+5;
		printf("%s", dan[i]);
	}
	Console::CursorLeft=10; //последняя точка, где будет заканчиваться выделенная область меню
	Console::CursorTop=12;
	printf(BlankLine);

	n=menu(8); //выбор вопроса в меню

	switch(n) { //обработка выбора вопроса
				case 1: maxim(subs); break;
				case 2:	first(subs); break;
				case 3: listing(subs); break;
				case 4: alfalist(subs); break;
				case 5: slojn(subs); break;
				case 6: kolvo(subs); break;
				case 7: diagram(subs); break;
				case 8: exit(0);
			  }

} //конец while(1)...
return 0;
}//конец main()


//программирование выбора из меню

int menu(int n)
{
	int y1=0,y2=n-1; //устанавливаем выделение для точки
	char c=1;
	while(c!=ESC) { //при нажитии кнопки ESC, меню закроется
				switch(c) { 
							case DOWN: y2=y1;y1++;break;//программироание кнопки DOWN, которая позволяет перемещаться вниз
							case UP: y2=y1;y1--;break;//программироание кнопки UP, которая позволяет перемещаться вверх
							case ENTER: return y1+1;//программироание кнопки ENTER, которая позволяет выбрать вопрос
							case HOME: if(y1!=0)y2=y1;y1=0;break;//программироание кнопки HOME, позволяет переместиться на первый вопрос
							case END: if(y1!=n-1)y2=y1;y1=n-1;break;//программироание кнопки END
						  }
				if(y1>n-1){y2=n-1;y1=0;} //условие,когда срабатывает кнопка DOWN
				if(y1<0){y2=0;y1=n-1;}////условие,когда срабатывает кнопка UP
				Console::ForegroundColor=ConsoleColor::White; //привыборе вопроса, буквы окрашиваются в белый цвет
				Console::BackgroundColor=ConsoleColor::DarkMagenta; //привыборе вопроса, план за буквами окрашивается в темно-пурпурный цвет
				Console::CursorLeft=10;
				Console::CursorTop=y1+5;
				printf("%s", dan[y1]);
				Console::ForegroundColor=ConsoleColor::DarkMagenta;
				Console::BackgroundColor=ConsoleColor::White;
				Console::CursorLeft=10;
				Console::CursorTop=y2+5;
				printf("%s", dan[y2]);
				c=getch();
	}// конец while(c!=ESC)...
	exit(0);	
}

void print_in_box(const char* text)
{
	int len = strlen(text);
	int boxWidth = len + 4; // Ширина рамки = длина текста + 4 (для символов рамки)
    int boxHeight = 3; // Высота рамки = 3 строки
    int screenWidth = Console::WindowWidth; // Ширина экрана консоли
    int screenHeight = Console::WindowHeight; // Высота экрана консоли
    int startX = (screenWidth - boxWidth) / 2; // Начальная координата X для центрирования рамки по горизонтали
    int startY = (screenHeight - boxHeight) / 2; // Начальная координата Y для центрирования рамки по вертикали
    // Вывод рамки сверху
    Console::SetCursorPosition(startX, startY);
    for (int i = 0; i < boxWidth; i++) printf("-");
    
    // Вывод текста внутри рамки
    Console::SetCursorPosition(startX, startY + 1);
    printf("| %s |", text);
    
    // Вывод рамки снизу
    Console::SetCursorPosition(startX, startY + 2);
    for (int i = 0; i < boxWidth; i++) printf("-");
}

//функция поиска максимального кол-ва подписчиков
void maxim(struct z* subs)
{
	int i=0; struct z best;
	char buffer[100];
	strcpy(best.name,subs[0].name);//в переменную структуры записываем нулевой элемент
	best.summa=subs[0].summa;
	for(i=1;i<NC;i++)
		if (subs[i].summa>best.summa) //условие поиска максимального количества
		{
			strcpy(best.name,subs[i].name); //Копируем имя текущего элемента в 'best' 
			best.summa=subs[i].summa;       //Обновляем количество подписчиков в 'best'
		}
		Console::ForegroundColor=ConsoleColor::DarkMagenta;
		Console::BackgroundColor=ConsoleColor::White;
		Console::CursorLeft=10;
		Console::CursorTop=15;
		sprintf(buffer,"Наибольшее кол-во подписчиков %ld имеет блогер %s", best.summa,best.name);
		print_in_box(buffer);
		getch();
}

//функция преобразования даты
void text_data(char *s,char *sd)
{
char s0[3],month[12][9]={
"января","февраля","марта","апреля","мая","июня",
"июля","августа","сентября","октября","ноября","декабря"};
strcpy(s,sd+8);
strcat(s," ");
strncpy(s0,sd+5,2); s0[2]=0;
strcat(s,month[ atoi(s0)-1]);
strcat(s," ");
strncat(s,sd,4);
return;
}



// функция поиска самой ранней даты канала
void first(struct z* subs)
{
int i;
char s[17];
struct z* best=subs;
for(i=1;i<NC;i++)
	if (strcmp(subs[i].data,best->data)<0)
		best=&subs[i];
		text_data(s,best->data);

Console::ForegroundColor=ConsoleColor::DarkMagenta;
Console::BackgroundColor=ConsoleColor::White;
Console::CursorLeft=10;
Console::CursorTop=15;
printf("Самый \"старый\" канал с тематикой %s на %ld подписчиков",
best->vid,best->summa);
Console::CursorLeft=10;
Console::CursorTop=16;
printf("имеет блогер %s",best->name);
Console::CursorLeft=10;
Console::CursorTop=17;
printf("Создан %s ",s);
getch();
}

//Составление алфавитного списка блогеров
void alfalist(struct z* subs ) //Формирование списка
{
int i;
struct sp* nt;
Console::ForegroundColor=ConsoleColor::DarkMagenta;
Console::BackgroundColor=ConsoleColor::White;
Console::Clear();
if(!spisok)
 for(i=0;i<NC;i++)
vstavka(subs,subs[i].name);

	Console::Clear();
	printf("\n Алфавитный список блогеров");
	printf("\n===============================\n");
	for(nt=spisok; nt!=0; nt=nt->sled)
	printf("\n %-20s%ld",nt->fio,nt->summa);

	Console::CursorLeft=50;
	Console::CursorTop=1;

	printf ("Обратный список\n");
	Console::CursorLeft=50;
	Console::CursorTop=2;
	printf("=================================\n");
	Console::CursorTop = 4;
	{
		Console::CursorLeft=50;
		printf("%-20s%ld\n", nt->fio,nt->summa);
	} 

		
		getch();
}


void vstavka(struct z* subs,char* fio)//Вставка в алфавитный список
{
int i;
struct sp *nov,*nt,*z=0;
for(nt=spisok; nt!=0 && strcmp(nt->fio,fio)<0; z=nt, nt=nt->sled);
if(nt && strcmp(nt->fio,fio)==0) return;
nov=(struct sp *) malloc(sizeof(struct sp));
strcpy(nov->fio,fio);
nov->pred=z;
nov->sled=nt;
nov->summa=0;
for(i=0;i<NC;i++)
if(strcmp(subs[i].name,fio)==0)
nov->summa+=subs[i].summa;
if (!z) spisok=nov;
if (nt) nt->pred=nov;
if (z) z->sled=nov;
return;
}

// Вывод алфавитного списка вкладчиков
void listing(struct z* subs)
{
int i;
struct z* nt;
Console::ForegroundColor=ConsoleColor::DarkMagenta;
Console::BackgroundColor=ConsoleColor::White;
Console::Clear();
printf("\n\r Список \"Технических\" каналов с аудиторией свыше 10 тыс. подписчиков");
printf("\n\r=====================================================\n\r");
for(i=0,nt=subs;i<NC;nt++,i++)
 if (nt->summa>10000 && strcmp(nt->vid,"Техника")==0)
printf("\n\r %-20s %ld подписчиков",nt->name,nt->summa);
_getch();
}

//Сложный вопрос.Каналы с разными тематиками, с одинаковым кол-вом подписчиков.
void slojn (struct z* subs) //указатель на текущий канал
{
	struct z* prev = subs;
	Console::BackgroundColor=ConsoleColor::White;
	Console::ForegroundColor=ConsoleColor::DarkMagenta;
	Console::CursorTop=15;
	Console::CursorLeft=10;
	for (int i=0;i<NC;i++,prev++)
		for (int z=i+1;z<NC;z++)
			if (strcmp(prev->vid,subs[z].vid)!=0 && prev->summa==subs[z].summa){ //проверка условия 
				printf ("%-21s %-10s %d\n",prev->name,prev->vid,prev->summa); //вывод информации о первом канале
				Console::CursorLeft=10;
				printf ("%-21s %-10s %d\n",subs[z].name,subs[z].vid,subs[z].summa); //вывод информации о втором канале
				getch ();
				return;
			}
	printf ("Нет каналов с разными тематиками с однинаковым кол-вом подписчиков");
	getch();
}

// функция подсчета каналов
void kolvo(struct z *subs)
{
int i,k=0;
char str[20];
Console::ForegroundColor=ConsoleColor::DarkMagenta;
Console::BackgroundColor=ConsoleColor::White;
Console::Clear();
printf("Есть следующие тематики каналов:\nВлог\nИгры\nКино\nЛитература\nМузыка\nПутешествия\nТехника\n");
printf("Введите тематику канала с большой буквы:");
Console::CursorLeft++; 
Console::CursorVisible::set(true);//включение курсора
scanf("%s", &str);//вводим строку
Console::CursorVisible::set(false);//выключение курсора
 printf("\nСписок каналов по тематике \"%s\":\n", str);
    printf("ФИО                Тематика            Подписчики      Просмотры       Дата\n");
    printf("====================================================================================\n");
    
    for(i = 0; i < NC; i++) {
        if (strcmp(subs[i].vid, str) == 0) {
            printf("%-20s %-20s %-15ld %-15ld %s\n", subs[i].name, subs[i].vid, subs[i].summa, subs[i].vews, subs[i].data);
            k++;
        }
    }
	if(k>0){
    printf("====================================================================================\n");
    printf("Каналов \"%s\" всего: %d\n", str, k);
	}
	else{
		printf("Ошибка: нет каналов по тематике \"%s\"\n", str);
	} 

    getch();
}

//Диаграмма, показывающая соотношение подписчиков
void diagram(struct z *subs)//создание диаграммы

{
struct sp *nt;//перемеенная для работы со списком
int len,i,NColor;//i-для номера строки
long sum = 0 ;//число-100%
char str1[20];
char str2[20];
System::ConsoleColor Color; //Задает константы, которые определяют основной цвет и фоновый цвет консоли
Console::ForegroundColor=ConsoleColor::Black;//цвет символов
Console::BackgroundColor=ConsoleColor::White;//цвет заднего фона
Console::Clear();
for(i=0;i<NC;i++) sum = sum+subs[i].summa ;
if(!spisok)
	for(i=0;i<NC;i++)
		vstavka(subs,subs[i].name); //вставляем из списка названия имен и красим символы в чѐрный цвет
Color=ConsoleColor::Black; NColor=0;
for(nt=spisok,i=0; nt!=0; nt=nt->sled,i++)
{
 sprintf(str1,"%s",nt->fio);
 sprintf(str2,"%3.1f%%",(nt->summa*100./sum));
 Console::ForegroundColor=ConsoleColor::Black;
 Console::BackgroundColor= ConsoleColor::White;
 Console::CursorLeft=5; Console::CursorTop=i+1;
 printf(str1);
 Console::CursorLeft=20;
 printf("%s",str2);
 Console::BackgroundColor=++Color; NColor++;
 Console::CursorLeft=30;
 for(len=0; len<nt->summa*100./sum; len++) printf(" ");
 if(NColor==14){ //выделяем на диаграмму 14 цветов
	Color=ConsoleColor::Black; NColor=0; }
}
getch();
return ;
}

