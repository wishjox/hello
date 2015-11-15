//
//  SimpleRecognizer.h
//  2048
//
//  Created by ZeroYang on 5/7/14.
//
//

#ifndef ___048__SimpleRecognizer__
#define ___048__SimpleRecognizer__

#include "cocos2d.h"


typedef enum {
	SimpleGesturesRight,
	SimpleGesturesLeft,
	SimpleGesturesUp,//
	SimpleGesturesDown,//
	SimpleGesturesError,//
	SimpleGesturesNotSupport
    
} SimpleGestures;

class SimpleRecognizer
{
protected:
	//--- These are variables because C++ doesn't (easily) allow
	//---  constants to be floating point numbers
	double X;
	double Y;
	SimpleGestures result;
    std::vector<cocos2d::Point> points;
    
public:
	SimpleRecognizer();
    
	void beginPoint(cocos2d::Point point);
	void movePoint(cocos2d::Point point);
	SimpleGestures endPoint(cocos2d::Point point);
    
    std::vector<cocos2d::Point>& getPoints();
    
};

#endif /* defined(___048__SimpleRecognizer__) */
