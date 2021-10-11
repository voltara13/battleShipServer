#ifndef COURSE__FIELD_H
#define COURSE__FIELD_H

#include <QGenericMatrix>
#include <QPoint>
#include <QRandomGenerator>
#include <QTextEdit>

#define ROW_INDEX rx()
#define COLUMN_INDEX ry()
constexpr int dimensions = 10;

class Server;
//Перечисление типа попадания
enum class ModeShot
{
	hit,
	kill
};
//Структура ячейки
struct Cell
{
	int value;
	bool isShot;
	Cell(int value = 0, int isShot = false) : value(value), isShot(isShot) {}
};
//Класс поля
class Field
{
protected:
	//Объявляем указатель на сервер
	Server* serverPtr = nullptr;

	explicit Field(Server* serverPtr);
	void addShip(QPoint first, QPoint second, const int& length);
	void goDown(int& rowIndex, int& columnIndex);
	bool goUp(int& rowIndex, int& columnIndex);
	virtual void setDefault();

public:
	int countHit = 0;

	void printField();

	QGenericMatrix<dimensions, dimensions, Cell> matrix;
};

#endif //COURSE__FIELD_H
