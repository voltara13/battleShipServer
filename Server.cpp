#include "Server.h"
#include "FieldServer.h"
#include "FieldEnemy.h"
//Конструктор сервера
Server::Server(quint16 nPort, QWidget* parent) : QWidget(parent)
{
	//Создаём новое соединение
	qTCPServerPtr = new QTcpServer(this);
	//В случае ошибки - выводим сообщение
	if (!qTCPServerPtr->listen(QHostAddress::Any, nPort))
	{
		QMessageBox::critical(nullptr,
			"Ошибка сервера",
			"Невозможно запустить сервер:"
				+ qTCPServerPtr->errorString());
		qTCPServerPtr->close();
		return;
	}
	//Подключаем сигнал создания нового соединения к слоту создания нового соединения у класса
	connect(qTCPServerPtr, &QTcpServer::newConnection, this, &Server::slotConnected);
	//Создаём элементы окна
	ptrTextResponse = new QTextEdit;
	ptrTextResponse->setReadOnly(true);
	//Создаём поля
	fieldServerPtr = new FieldServer(this);
	fieldEnemyPtr = new FieldEnemy(this);
	auto* ptrQVBoxLayout = new QVBoxLayout(this);
	ptrQVBoxLayout->addWidget(ptrTextResponse);
	setWindowTitle("Сервер морского боя");
	setFixedWidth(500);
	logWrite("Сервер успешно создан. Ожидание нового подключения");
}
//Функция отправки клиенту
void Server::sendClient(const QString& sendString)
{
	//Создаём вспомогательный поток
	QByteArray arrBlock = nullptr;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_6_0);
	out << quint16(0) << sendString;
	out.device()->seek(0);
	out << quint16(arrBlock.size() - sizeof(quint16));
	//Записываем данные в сокет и отправляем
	qTCPSocketPtr->write(arrBlock);
	//Выводим сообщение об успешной передаче
	logWrite("Отправка клиенту: " + sendString);
}
//Функия записи строки в лог
void Server::logWrite(const QString& string)
{
	ptrTextResponse->append(QTime::currentTime().toString() + " " + string);
}
//Слот чтения данных, посылаемых клиентом серверу
void Server::slotReadClient()
{
	//Создаём новый сокет
	QTcpSocket* ptrClientSocket = (QTcpSocket*)sender();
	//Создаём вспомогательный поток, ему задаём версию фреймворка
	QDataStream in(ptrClientSocket);
	in.setVersion(QDataStream::Qt_6_0);
	sizeNextBlock = 0;
	//Создаём строку ответа
	QString response;
	//Читаем то, что нам прислал клиент
	while (true)
	{
		//Если размер следующего блока неизвестен, то
		if (!sizeNextBlock)
		{
			//Если блок меньше чем два байта, то выходим из цикла
			if (ptrClientSocket->bytesAvailable() < sizeof(quint16))
				break;
			in >> sizeNextBlock;
		}
		//Если размер следующего блока больше, чем количество байтов ожидающих чтения, то выходим из цикла
		if (ptrClientSocket->bytesAvailable() < sizeNextBlock)
			break;
		//Создаём вспомогательный буфер, в который мы передаём содержимое потока
		QString buffer;
		in >> buffer;
		//Содержимое буфера передаём в строку ответа
		response.append(buffer);
		//Размер следующего блока неизвестен, пусть он будет равен нулю
		sizeNextBlock = 0;
	}
	logWrite("Клиент прислал: " + response);
	if (response.contains("!HIT!"))
		fieldEnemyPtr->setShot(ModeShot::hit);
	else if (response.contains("!KILL!"))
		fieldEnemyPtr->setShot(ModeShot::kill);
	else if (response.contains("!RESTART!"))
	{
		logWrite("Клиент успешно подключился, игра началась, расстановка кораблей...");
		fieldServerPtr->setDefault();
		fieldEnemyPtr->setDefault();
	}
	else
		fieldServerPtr->setShot(response);

}
void Server::makeShot()
{
	while (fieldEnemyPtr->countHit != 20 && fieldEnemyPtr->countHit != 20)
	{
		QPoint pos = {
			QRandomGenerator::global()->bounded(0, dimensions),
			QRandomGenerator::global()->bounded(0, dimensions)};

		if (fieldEnemyPtr->matrix(pos.ROW_INDEX, pos.COLUMN_INDEX).value == 0)
		{
			fieldEnemyPtr->pos = pos;
			fieldEnemyPtr->matrix(pos.ROW_INDEX, pos.COLUMN_INDEX).value = -1;
			fieldEnemyPtr->matrix(pos.ROW_INDEX, pos.COLUMN_INDEX).isShot = true;
			sendClient(QString("%1 %2").arg(pos.ROW_INDEX).arg(pos.COLUMN_INDEX));
			break;
		}
	}
}
//Слот создания нового соединения
void Server::slotConnected()
{
	//Создаём новый сокет
	qTCPSocketPtr = qTCPServerPtr->nextPendingConnection();
	//Добавляем новому сокету слоты отключения от сервера и слот чтения с клиента
	connect(qTCPSocketPtr, &QAbstractSocket::disconnected, this, &Server::slotDisconnected);
	connect(qTCPSocketPtr, &QAbstractSocket::readyRead,this, &Server::slotReadClient);
}
//Слот завершения соединения
void Server::slotDisconnected()
{
	logWrite("Клиент закрыл соединение. Ожидание нового подключения");
}
