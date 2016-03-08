#ifndef __PROGRESS_VIEW_H__
#define __PROGRESS_VIEW_H__

#include "cocos2d.h"  

class ProgressView : public cocos2d::Node
{
public:

	CREATE_FUNC(ProgressView);

public:
	//����Ѫ������  
	void setBackgroundTexture(const char *pName);
	//����Ѫ��ǰ��  
	void setForegroundTexture(const char *pName);
	//������Ѫ��  
	void setTotalProgress(float total);
	//���õ�ǰѪ��  
	void setCurrentProgress(float progress);
	//�õ���ǰѪ��  
	float getCurrentProgress() const;
	//�õ���Ѫ��  
	float getTotalProgress() const;

	void setForegroundColor(const cocos2d::Color3B &col);

private:
	ProgressView();
	//����ǰ��Ѫ����ʾ�ĳ���  
	void setForegroundTextureRect(const cocos2d::Rect &rect);

private:
	cocos2d::Sprite *m_progressBackground;//����Ѫ��  
	cocos2d::Sprite *m_progressForeground;//ǰ��Ѫ��  
	float m_totalProgress;//��Ѫ��  
	float m_currentProgress;//��ǰѪ��  
	float m_scale;//�Ŵ���  
};

#endif