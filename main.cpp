#include "Server.h"
#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Server server(2323);
	server.show();
	return QApplication::exec();
}
