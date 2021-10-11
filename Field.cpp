#include "Field.h"
#include "Server.h"
//Конструктор игрового поля
Field::Field(Server* serverPtr) : serverPtr(serverPtr)
{
	matrix.fill(Cell());
}
//Функция установки корабля в поле
void Field::addShip(QPoint first, QPoint second, const int& length)
{
	for (int row = first.ROW_INDEX; row <= second.ROW_INDEX; ++row)
	{
		matrix(row, first.COLUMN_INDEX) = length;
		for (int rowAround = row - 1; rowAround <= row + 1; ++rowAround)
			for (int columnAround = first.COLUMN_INDEX - 1; columnAround <= first.COLUMN_INDEX + 1; ++columnAround)
				if (rowAround > -1 && rowAround < dimensions && columnAround > -1 && columnAround < dimensions && !matrix(rowAround, columnAround).value)
					matrix(rowAround, columnAround) = -1;
	}

	for (int column = first.COLUMN_INDEX; column <= second.COLUMN_INDEX; ++column)
	{
		matrix(first.ROW_INDEX, column) = length;
		for (int columnAround = column - 1; columnAround <= column + 1; ++columnAround)
			for (int rowAround = first.ROW_INDEX - 1; rowAround <= first.ROW_INDEX + 1; ++rowAround)
				if (columnAround > -1 && columnAround < dimensions && rowAround > -1 && rowAround < dimensions && !matrix(rowAround, columnAround).value)
					matrix(rowAround, columnAround) = -1;
	}
}
//Функция прохождения вниз-вправо для определения уничтожения корабля
void Field::goDown(int& rowIndex, int& columnIndex)
{
	while (true)
	{
		if (
			rowIndex + 1 < dimensions &&
				matrix(rowIndex + 1, columnIndex).value > 0 &&
				matrix(rowIndex + 1, columnIndex).isShot)
			rowIndex++;
		else if (
			columnIndex + 1 < dimensions &&
				matrix(rowIndex, columnIndex + 1).value > 0 &&
				matrix(rowIndex, columnIndex + 1).isShot)
			columnIndex++;
		else
			break;
	}
}
//Функция прохождения вверх-влево для определения уничтожения корабля
bool Field::goUp(int& rowIndex, int& columnIndex)
{
	int count = 1;
	while (true)
	{
		if (
			rowIndex - 1 > -1 &&
				matrix(rowIndex - 1, columnIndex).value > 0 &&
				matrix(rowIndex - 1, columnIndex).isShot)
			rowIndex--,	count++;
		else if (
			columnIndex - 1 > -1 &&
				matrix(rowIndex, columnIndex - 1).value > 0 &&
				matrix(rowIndex, columnIndex - 1).isShot)
			columnIndex--, count ++;
		else
			break;
	}
	return count == matrix(rowIndex, columnIndex).value;
}

void Field::setDefault()
{
	countHit = 0;
	matrix.fill(Cell());
}
