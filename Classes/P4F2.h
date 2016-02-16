#ifndef __P4F2_H__
#define __P4F2_H__
#include "Tank.h"

class P4F2 : public Tank
{
public:

	CREATE_FUNC(P4F2);

	virtual ~P4F2();

	virtual bool init() override;

protected:

	P4F2();

	virtual std::string getModelFilePath() override;
};

#endif