#pragma once
#include<clocale>
namespace keyboard {

	void gotoxy(int x, int y);
	char diskSearch(); // Выводим список дисков и выбираем первый доступный диск
	char keyYesOrNo();// Выбор 'Да' 'нет'
	char keyYesOrNoOrAll();
	char keyChoice(); // Выбор поиска
}