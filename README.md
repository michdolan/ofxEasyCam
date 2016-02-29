# ofxEasyCam
openFrameworks addon from forked: 
[openFrameworks/3d/ofEasyCam.cpp](https://github.com/openframeworks/openFrameworks/blob/master/libs/openFrameworks/3d/ofEasyCam.cpp)
[openFrameworks/3d/ofEasyCam.h](https://github.com/openframeworks/openFrameworks/blob/master/libs/openFrameworks/3d/ofEasyCam.h)

3D cameras can be a bit unwieldy for lay users. It's easy to get disoriented and have a difficult time returning to a comfortable view without a full reset. This modified ofEasyCam aims to help with this, particularly for public installations with a wide audience.

Changes:
- Added the option to fix the camera's up axis to a static (0, 1, 0) (true by default).
- Camera roll when the user clicks outside the arcball can be disabled (and is by default).
- Removed the reset() function.
