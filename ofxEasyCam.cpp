#include "ofEasyCam.h"
#include "ofMath.h"
#include "ofUtils.h"

// when an ofEasyCam is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
float minDifference = 0.1e-5f;

//----------------------------------------
ofEasyCam::ofEasyCam() {
	lastDistance = 0;
	drag = 0.9f;
	sensitivityRot = 1.0f;  // side to side: 1 = 180 deg; 0.5 = 90 deg.
	sensitivityXY = 0.5f;
	sensitivityZ= 0.7f;
	
	bDistanceSet = false; 
	bMouseInputEnabled = false;
	bDoRotate = false;
	bApplyInertia =false;
	bDoTranslate = false;
	bInsideArcball = true;
	bValidClick = false;
	bEnableMouseMiddleButton = true;
	bAutoDistance = true;
	bFixUpwards = true;
	bRollEnabled = false;
	doTranslationKey = 'm';
	
	enableMouseInput();
}

//----------------------------------------
ofEasyCam::~ofEasyCam() {
	disableMouseInput();
}

//----------------------------------------
void ofEasyCam::update(ofEventArgs & args) {
    if (!bDistanceSet && bAutoDistance) {
        setDistance(getImagePlaneDistance(viewport), true);
    }
    if (bMouseInputEnabled) {
		rotationFactor = sensitivityRot * 180 / min(viewport.width, viewport.height);
		if (bMouseInputEnabled) {
			updateMouse();
		}
		if (bDoRotate) {
			updateRotation();
		}
		else if (bDoTranslate) {
			updateTranslation(); 
		}
	}	
}

//----------------------------------------
void ofEasyCam::begin(ofRectangle viewport) {
	this->viewport = viewport;
	ofCamera::begin(viewport);	
}

//----------------------------------------
void ofEasyCam::setTarget(const ofVec3f& targetPoint) {
	target.setPosition(targetPoint);
	lookAt(target);
}

//----------------------------------------
void ofEasyCam::setTarget(ofNode& targetNode) {
	target = targetNode;
	lookAt(target);
}

//----------------------------------------
ofNode& ofEasyCam::getTarget() {
	return target;
}

//----------------------------------------
void ofEasyCam::setDistance(float distance) {
	setDistance(distance, true);
}

//----------------------------------------
void ofEasyCam::setDistance(float distance, bool save) {
	//should this be the distance from the camera to the target?
	if (distance > 0.0f) {
		if (save) {
			lastDistance = distance;
		}
		setPosition(target.getPosition() + (distance * getZAxis()));
		bDistanceSet = true;
	}
}

//----------------------------------------
float ofEasyCam::getDistance() const {
	return target.getPosition().distance(getPosition());
}

//----------------------------------------
void ofEasyCam::setAutoDistance(bool bAutoDistance) {
    this->bAutoDistance = bAutoDistance;
    if (bAutoDistance) {
        bDistanceSet = false;
    }
}

//----------------------------------------
void ofEasyCam::setDrag(float drag) {
	this->drag = drag;
}

//----------------------------------------
float ofEasyCam::getDrag() const {
	return drag;
}

//----------------------------------------
void ofEasyCam::setTranslationKey(char key) {
	doTranslationKey = key;
}

//----------------------------------------
char ofEasyCam::getTranslationKey() {
	return doTranslationKey;
}

//----------------------------------------
void ofEasyCam::enableMouseInput() {
	if (!bMouseInputEnabled) {
		bMouseInputEnabled = true;
		//ofRegisterMouseEvents(this);
		ofAddListener(ofEvents().update , this, &ofEasyCam::update);
	}
}

//----------------------------------------
void ofEasyCam::disableMouseInput() {
	if (bMouseInputEnabled) {
		bMouseInputEnabled = false;
		//ofUnregisterMouseEvents(this);
		ofRemoveListener(ofEvents().update, this, &ofEasyCam::update);
	}
}

//----------------------------------------
bool ofEasyCam::getMouseInputEnabled() {
	return bMouseInputEnabled;
}

//----------------------------------------
void ofEasyCam::enableMouseMiddleButton() {
	bEnableMouseMiddleButton = true;
}

//----------------------------------------
void ofEasyCam::disableMouseMiddleButton() {
	bEnableMouseMiddleButton = false;
}

//----------------------------------------
bool ofEasyCam::getMouseMiddleButtonEnabled() {
	return bEnableMouseMiddleButton;
}

//----------------------------------------
void ofEasyCam::updateTranslation() {
	if (bApplyInertia) {
		moveX *= drag;
		moveY *= drag;
		moveZ *= drag;
		if (ABS(moveX) <= minDifference && ABS(moveY) <= minDifference && ABS(moveZ) <= minDifference) {
			bApplyInertia = false;
			bDoTranslate = false;
		}
	}
	move((getXAxis() * moveX) + (getYAxis() * moveY) + (getZAxis() * moveZ));
}	

//----------------------------------------
void ofEasyCam::updateRotation() {
	if (bApplyInertia) {
		xRot *=drag; 
		yRot *=drag;
		zRot *=drag;
		if (ABS(xRot) <= minDifference && ABS(yRot) <= minDifference && ABS(zRot) <= minDifference) {
			bApplyInertia = false;
			bDoRotate = false;
		}
	}
	curRot = ofQuaternion(xRot, ofCamera::getXAxis(), yRot, ofCamera::getYAxis(), zRot, ofCamera::getZAxis());

	if (bFixUpwards) {
		ofQuaternion rotToUp;
		ofVec3f sideDir = ofCamera::getSideDir() * curRot;
		rotToUp.makeRotate(sideDir, sideDir * ofVec3f(1, 0, 1));
		curRot *= rotToUp;
	}

	setPosition((ofCamera::getGlobalPosition()-target.getGlobalPosition())*curRot +target.getGlobalPosition());
	rotate(curRot);
}

//----------------------------------------
void ofEasyCam::updateMouse() {
	mouse = ofVec2f(ofGetMouseX(), ofGetMouseY());
	if(viewport.inside(mouse.x, mouse.y) && !bValidClick && ofGetMousePressed()) {
		if ((bEnableMouseMiddleButton && ofGetMousePressed(OF_MOUSE_BUTTON_MIDDLE)) || 
				ofGetKeyPressed(doTranslationKey) || 
				ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)) {
			bDoTranslate = true;
			bDoRotate = false;
			bApplyInertia = false;
		} 
		else if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
			bDoTranslate = false;
			bDoRotate = true;
			bApplyInertia = false;

			if (bRollEnabled) {
				float halfWidth = viewport.width / 2.0f;
				float halfHeight = viewport.height / 2.0f;
				if (ofVec2f(mouse.x - viewport.x - halfWidth, mouse.y - viewport.y - halfHeight).length() < min(halfWidth, halfHeight)) {
					bInsideArcball = true;
				} 
				else {
					bInsideArcball = false;
				}
			}
		}
		lastMouse = mouse;
		bValidClick = true;
		bApplyInertia = false;
	}
	
	if (bValidClick) {
		if (!ofGetMousePressed()) {
			bApplyInertia = true;
			bValidClick = false;
		}
		else {
			int vFlip;
			if (isVFlipped()) {
				vFlip = -1;
			}
			else {
				vFlip =  1;
			}

			mouseVel = mouse - lastMouse;
			
			if (bDoTranslate) {
				moveX = 0.0f;
				moveY = 0.0f;
				moveZ = 0.0f;
				if (ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)) {
					moveZ = mouseVel.y * sensitivityZ * (getDistance() + FLT_EPSILON) / viewport.height;				
				} 
				else {
					moveX = -mouseVel.x * sensitivityXY * (getDistance() + FLT_EPSILON) / viewport.width;
					moveY = vFlip * mouseVel.y * sensitivityXY * (getDistance() + FLT_EPSILON) / viewport.height;
				}
			}
			else {
				xRot = 0.0f;
				yRot = 0.0f;
				zRot = 0.0f;
				if (bInsideArcball) {
					xRot = vFlip * -mouseVel.y * rotationFactor;
					yRot = -mouseVel.x * rotationFactor;
				}
				else {
					ofVec2f center(viewport.width / 2, viewport.height / 2);
					zRot = - vFlip * ofVec2f(mouse.x - viewport.x - center.x, mouse.y - viewport.y - center.y).angle(lastMouse - ofVec2f(viewport.x, viewport.y) - center);
				}
			}
			lastMouse = mouse;
		}
	}
}

//----------------------------------------
void ofEasyCam::setFixUpwards(bool bFixUpwards) {
	this->bFixUpwards = bFixUpwards;
}

//----------------------------------------
bool ofEasyCam::getFixUpwards() {
	return bFixUpwards;
}

//----------------------------------------
void ofEasyCam::enableRoll() {
	bRollEnabled = true;
}

//----------------------------------------
void ofEasyCam::disableRoll() {
	bRollEnabled = false;
}

//----------------------------------------
bool ofEasyCam::getRollEnabled() {
	return bRollEnabled;
}