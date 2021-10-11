#ifndef COURSE__SERVER_H
#define COURSE__SERVER_H

#pragma once

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QTextEdit>
#include <QTime>
#include <QVBoxLayout>

class FieldServer;
class FieldEnemy;

class Server : public QWidget
{
private:
Q_OBJECT
	FieldServer* fieldServerPtr = nullptr;
	FieldEnemy* fieldEnemyPtr = nullptr;
	QTextEdit* ptrTextResponse = nullptr;
	QTcpServer* qTCPServerPtr = nullptr;
	QTcpSocket* qTCPSocketPtr = nullptr;
	quint16 sizeNextBlock = 0;

public:
	explicit Server(quint16 nPort, QWidget* parent = nullptr);
	void sendClient(const QString& sendString);
	void logWrite(const QString& string);
	void makeShot();

private slots:
	void slotReadClient();
	void slotConnected();
	void slotDisconnected();
};

#endif //COURSE__SERVER_H
