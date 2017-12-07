

#pragma once

#include "../Config.h"
#include "../Common.h"


#define GLEW_STATIC
#include <GL/glew.h>

#include <GL/glut.h>

//#include <GLFW/glfw3.h>

#define CLEAR_COLOR 0.529f, 0.807f, 0.92f, 0.0f

namespace engine
{

    class LWindow
    {


        private :

        static LWindow* INSTANCE;

        //GLFWwindow* m_window;

        int m_window;

        int m_width;
        int m_height;

        FnPtr_keyboard_callback m_keyCallback;
        FnPtr_mouse_callback m_mouseCallback;
        FnPtr_motion_callback m_motionCallback;
        FnPtr_loop_callback m_loopCallback;
        FnPtr_reshape_callback m_reshapeCallback;
        FnPtr_idle_callback m_idleCallback;
        FnPtr_load_callback m_loadCallback;

        bool m_initialized;

        public :
        LWindow();
        ~LWindow();
        void release();
        void init();
        static void onKeyCallback( unsigned char key, int x, int y);
        static void onMouseCallback( int button, int state, int x, int y);
        static void onMotionCallBack(int x, int y);
        static void onLoopCallBack();
        static void onReshapeCallBack(int width, int height);
        static void onIdleCallBack();
        static void onLoadCallBack();


        void registerKeyCallback( FnPtr_keyboard_callback callback );
        void registerMouseCallback( FnPtr_mouse_callback callback );

        void registerMotionCallback( FnPtr_motion_callback callback );
        void registerLoopCallback( FnPtr_loop_callback callback );
        void registerReshapeCallback( FnPtr_reshape_callback callback );
        void registerIdleCallback( FnPtr_idle_callback callback );
        void registerLoadCallback( FnPtr_load_callback callback );


        void clear() { glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT ); }

        int width() { return m_width; }
        int height() { return m_height; }

    };

}