#pragma once

#include "ofCamera.h"
#include "ofEvents.h"

class ofxEasyCam : public ofCamera {
public:
	ofxEasyCam();
	~ofxEasyCam();

	// TODO: this should be ofGetViewRect() eventually
	virtual void begin(ofRectangle viewport = ofGetCurrentViewport());

	//----------------------------------------
	// advanced functions

	void setTarget(const ofVec3f& target);
	void setTarget(ofNode& target);
	ofNode& getTarget();

	void setDistance(float distance);
	float getDistance() const;

	// drag is how quickly the camera picks up and slows down
	// it is a normalized value between 0-1
	void setDrag(float drag);
	float getDrag() const;

	// the translation key is the key used to switch between rotation and translation.
	// translation happens only when the key is pressed.
	void setTranslationKey(char key);
    char getTranslationKey();

	// enable or disable mouse input to navigate
	void enableMouseInput();
	void disableMouseInput();
	bool getMouseInputEnabled();
	
	void enableMouseMiddleButton();
	void disableMouseMiddleButton();
	bool getMouseMiddleButtonEnabled();
	
    void setAutoDistance(bool bAutoDistance);

	void setFixUpwards(bool bFixUpwards);
	bool getFixUpwards();

	void enableRoll();
	void disableRoll();
	bool getRollEnabled();
	
private:
	void setDistance(float distance, bool save);

	ofNode target;
	
	bool bEnableMouseMiddleButton;
	bool bApplyInertia;
	bool bDoTranslate;
	bool bDoRotate;
	bool bValidClick;
	bool bInsideArcball;
	bool bMouseInputEnabled;
	bool bDistanceSet;
    bool bAutoDistance;
	bool bFixUpwards;
	bool bRollEnabled;
	float lastDistance;

	float drag;
	
	float xRot;
	float yRot;
	float zRot;
	
	float moveX;
	float moveY;
	float moveZ;
	
	float sensitivityXY;
	float sensitivityZ;
	float sensitivityRot;
	
	float rotationFactor;

	ofVec2f mouse;
	ofVec2f lastMouse;
	ofVec2f mouseVel;
	
	void updateRotation();
	void updateTranslation();
	void update(ofEventArgs & args);
	void updateMouse();
	
	char doTranslationKey;
		
	ofQuaternion curRot;  
    
	// having the  viewport saved localy will make it easier for all the needed 
	// maths dealing with viewport.
	ofRectangle viewport;
};
