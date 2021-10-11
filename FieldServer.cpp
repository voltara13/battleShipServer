#include "FieldServer.h"
#include "Server.h"
//Конструктор игрового поля сервера
FieldServer::FieldServer(Server* serverPtr) : Field(serverPtr) {}
//Функция проверки возможности установки корабля
bool FieldServer::checkShip()
{
	for (int row = first.ROW_INDEX; row <= second.ROW_INDEX; ++row)
		if (matrix(row, first.COLUMN_INDEX).value != 0)
			return false;
	for (int column = first.COLUMN_INDEX; column <= second.COLUMN_INDEX; ++column)
		if (matrix(first.ROW_INDEX, column).value != 0)
			return false;
	return true;
}
//Функция установки выстрела
void FieldServer::setShot(const QString& string)
{
	QStringList list = QString(string).split(" ", Qt::SkipEmptyParts);
	int rowIndex = list[0].toInt();
	int columnIndex = list[1].toInt();
	matrix(rowIndex, columnIndex).isShot = true;
	if (matrix(rowIndex, columnIndex).value > 0)
	{
		countHit++;
		int rowIndexTemp = rowIndex;
		int columnIndexTemp = columnIndex;

		goDown(rowIndexTemp, columnIndexTemp);
		if (goUp(rowIndexTemp, columnIndexTemp))
		{
			goDown(rowIndexTemp, columnIndexTemp);
			serverPtr->sendClient("!KILL!");
		}
		else
			serverPtr->sendClient("!HIT!");
	}
	else
		serverPtr->makeShot();
}
//Функция сброса игрового поля ко значения по-умолчанию
void FieldServer::setDefault()
{
	shipList = {4, 3, 2, 1};
	first = {-1, -1};
	second = {-1, -1};
	Field::setDefault();
	int length;
	int angle;
	while (*std::max_element(shipList.begin(), shipList.end()))
	{
		first = {
			QRandomGenerator::global()->bounded(0, dimensions),
			QRandomGenerator::global()->bounded(0, dimensions)};
		second = {
			QRandomGenerator::global()->bounded(0, dimensions),
			QRandomGenerator::global()->bounded(0, dimensions)};
		QLineF line(first, second);
		length = line.length() + 1;
		angle = line.angle();
		if (first.ROW_INDEX > second.ROW_INDEX)
		{
			QPoint temp = first;
			first = second;
			second = temp;
		}
		if (first.COLUMN_INDEX > second.COLUMN_INDEX)
		{
			QPoint temp = first;
			first = second;
			second = temp;
		}
		if (length <= 4 && angle % 90 == 0 && shipList[length - 1] && checkShip())
		{
			qDebug() << (QString("FIRST:: %1;%2 SECOND:: %3;%4 LENGTH:: %5 ANGLE:: %6")
				.arg(first.ROW_INDEX)
				.arg(first.COLUMN_INDEX)
				.arg(second.ROW_INDEX)
				.arg(second.COLUMN_INDEX)
				.arg(length)
				.arg(angle));
			shipList[length - 1]--;
			addShip(first, second, length);
		}
	}
	serverPtr->logWrite("Корабли успешно расстановлены. Ожидание хода");
}






