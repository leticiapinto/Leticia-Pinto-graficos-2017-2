
#pragma once

typedef void ( *FnPtr_keyboard_callback )( unsigned char key, int x, int y );
typedef void ( *FnPtr_mouse_callback )( int button, int action, int x, int y );
typedef void ( *FnPtr_motion_callback )( int x, int y );
typedef void ( *FnPtr_loop_callback )();
typedef void ( *FnPtr_reshape_callback )(int width, int height);
typedef void ( *FnPtr_idle_callback )();
typedef void ( *FnPtr_load_callback )();