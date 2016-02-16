#ifndef __M24_H__
#define __M24_H__
#include "Tank.h"

class M24 : public Tank
{
public:

	CREATE_FUNC(M24);

	virtual ~M24();

	virtual bool init() override;

protected:

	M24();

	virtual std::string getModelFilePath() override;
};

#endif