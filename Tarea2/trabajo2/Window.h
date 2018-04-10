

#pragma once

#include "../Config.h"
#include "../Common.h"


#define GLEW_STATIC
#include <GL/glew.h>

//#include <GL/glut.h>
#ifdef __APPLE__
    #include "GLUT/glut.h"
#else
    #include <GL/glut.h>
#endif

//#include <GLFW/glfw3.h>

#define CLEAR_COLOR 0.529f, 0.807f, 0.92f, 0.0f

namespace graphics
{

    class Window
    {


        private :

        static Window* INSTANCE;

        //GLFWwindow* m_window;

        int m_window;

        int m_width;
        int m_height;

        keyboardcallbackPTR m_keyCallback;
        mouseCallbackPTR m_mouseCallback;
        motionCallbackPTR m_motionCallback;
        loopCallbackPTR m_loopCallback;
        reshapeCallbackPTR m_reshapeCallback;
        idleCallbackPTR m_idleCallback;
        loadCallbackPTR m_loadCallback;

        bool m_initialized;

        public :
        Window();
        ~Window();
        void release();
        void init();
        static void onKeyCallback( unsigned char key, int x, int y);
        static void onMouseCallback( int button, int state, int x, int y);
        static void onMotionCallBack(int x, int y);
        static void onLoopCallBack();
        static void onReshapeCallBack(int width, int height);
        static void onIdleCallBack();
        static void onLoadCallBack();


        void registerKeyCallback( keyboardcallbackPTR callback );
        void registerMouseCallback( mouseCallbackPTR callback );

        void registerMotionCallback( motionCallbackPTR callback );
        void registerLoopCallback( loopCallbackPTR callback );
        void registerReshapeCallback( reshapeCallbackPTR callback );
        void registerIdleCallback( idleCallbackPTR callback );
        void registerLoadCallback( loadCallbackPTR callback );


        void clear() { glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT ); }

        int width() { return m_width; }
        int height() { return m_height; }

    };

}