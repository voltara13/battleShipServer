#ifndef COURSE__FIELDENEMY_H
#define COURSE__FIELDENEMY_H

#include "Field.h"

class Server;
//Класс игрового поля противника
class FieldEnemy : public Field
{
public:
	//Объявляем точку нажатия
	QPoint pos = {-1, -1};

	explicit FieldEnemy(Server* serverPtr);
	void setShot(const ModeShot& modeShot);
	void setDefault() override;
};

#endif //COURSE__FIELDENEMY_H
