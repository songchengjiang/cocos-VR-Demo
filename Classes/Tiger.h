#ifndef __TIGER_H__
#define __TIGER_H__
#include "Tank.h"

class Tiger : public Tank
{
public:

	CREATE_FUNC(Tiger);

	virtual ~Tiger();

	virtual bool init() override;
	virtual void update(float delta) override;

protected:

	Tiger();

	virtual std::string getModelFilePath() override;
};

#endif