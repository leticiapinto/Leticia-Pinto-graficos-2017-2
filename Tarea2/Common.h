
#pragma once

typedef void ( *keyboardcallbackPTR )( unsigned char key, int x, int y );
typedef void ( *mouseCallbackPTR )( int button, int action, int x, int y );
typedef void ( *motionCallbackPTR )( int x, int y );
typedef void ( *loopCallbackPTR )();
typedef void ( *reshapeCallbackPTR )(int width, int height);
typedef void ( *idleCallbackPTR )();
typedef void ( *loadCallbackPTR )();