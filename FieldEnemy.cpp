#include "FieldEnemy.h"
#include "Server.h"
//Конструктор игрового поля врага
FieldEnemy::FieldEnemy(Server* serverPtr) : Field(serverPtr) {}
//Функция установки выстрела
void FieldEnemy::setShot(const ModeShot& modeShot)
{
	matrix(pos.ROW_INDEX, pos.COLUMN_INDEX).value = 1;
	countHit++;
	switch (modeShot)
	{
	case ModeShot::hit:
		serverPtr->makeShot();
		break;
	case ModeShot::kill:
		int rowIndexTemp = pos.ROW_INDEX;
		int columnIndexTemp = pos.COLUMN_INDEX;
		goUp(rowIndexTemp, columnIndexTemp);
		QPoint first = {rowIndexTemp, columnIndexTemp};
		goDown(rowIndexTemp, columnIndexTemp);
		QPoint second = {rowIndexTemp, columnIndexTemp};
		addShip(first, second, 1);
		serverPtr->makeShot();
		break;
	}
}
//Функция сброса игрового поля ко значения по-умолчанию
void FieldEnemy::setDefault()
{
	pos = {-1, -1};
	Field::setDefault();
}
