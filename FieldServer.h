#ifndef COURSE__FIELDSERVER_H
#define COURSE__FIELDSERVER_H

#include "Field.h"

class Server;
//Конструктор игрового поля сервера
class FieldServer : public Field
{
private:
	//Объявляем переменные-точки
	QPoint first = { -1, -1};
	QPoint second = { -1, -1};
	//Объявляем массив количества кораблей
	QList<int> shipList = { 4, 3, 2, 1 };

	bool checkShip();

public:
	explicit FieldServer(Server* serverPtr);
	void setShot(const QString& string);
	void setDefault() override;
};

#endif //COURSE__FIELDSERVER_H
