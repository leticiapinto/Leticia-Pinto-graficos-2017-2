

#include "Window.h"

#include <iostream>

using namespace std;

namespace graphics
{

    Window* Window::INSTANCE = NULL;

    Window::Window()
    {
        m_keyCallback = NULL;
        m_mouseCallback = NULL;
        m_motionCallback = NULL;
        m_loopCallback = NULL;
        m_reshapeCallback = NULL;
        m_idleCallback = NULL;
    }

    void Window::init()
    {

        m_width=APP_WIDTH;
        m_height=APP_HEIGHT;

        if ( Window::INSTANCE != NULL )
        {
            delete Window::INSTANCE;
        }

        Window::INSTANCE = this;


        char fakeParam[] = "fake";
        char *fakeargv[] = { fakeParam, NULL };
        int fakeargc = 1;
        glutInit(&fakeargc, fakeargv);
        glutInitWindowSize(APP_WIDTH, APP_HEIGHT);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
        glutCreateWindow(APP_NAME);

        //glewExperimental = GL_TRUE;
        if ( glewInit() != GLEW_OK )
        {
            m_initialized = false;
            cout<<"fail init glew"<<endl;
            return;
        }
        Window::onLoadCallBack();

        glutDisplayFunc(Window::onLoopCallBack);
        glutReshapeFunc(Window::onReshapeCallBack);
        glutMouseFunc(Window::onMouseCallback);
        glutMotionFunc(Window::onMotionCallBack);
        glutKeyboardFunc(Window::onKeyCallback);
        glutIdleFunc(Window::onIdleCallBack);
        glutMainLoop();
        m_initialized = true;

    }

    Window::~Window()
    {
        release();
    }

    void Window::release()
    {
        //m_window = NULL;
        //glfwTerminate();
    }

    void Window::onLoadCallBack()
    {
        if ( Window::INSTANCE->m_loadCallback == NULL )
        {
            return;
        }

        Window::INSTANCE->m_loadCallback();
    }

    void Window::onKeyCallback( unsigned char key, int x, int y )
    {
        if ( Window::INSTANCE->m_keyCallback == NULL )
        {
            return;
        }

        Window::INSTANCE->m_keyCallback( key, x, y );
    }

    void Window::onMouseCallback( int button, int state, int x, int y)
    {
        if ( Window::INSTANCE->m_mouseCallback == NULL )
        {
            return;
        }
        
        Window::INSTANCE->m_mouseCallback( button, state, x, y );
    }

    void Window::onMotionCallBack(int x, int y)
    {
        if ( Window::INSTANCE->m_motionCallback == NULL )
        {
            return;
        }
        Window::INSTANCE->m_motionCallback( x, y );

    }

    void Window::onLoopCallBack()
    {
        if ( Window::INSTANCE->m_loopCallback == NULL )
        {
            return;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Window::INSTANCE->m_loopCallback();
        glutSwapBuffers();
    }

    void Window::onReshapeCallBack(int width, int height)
    {
        if ( Window::INSTANCE->m_reshapeCallback == NULL )
        {
            return;
        }
        Window::INSTANCE->m_reshapeCallback( width, height );

    }

    void Window::onIdleCallBack()
    {
        if ( Window::INSTANCE->m_idleCallback == NULL )
        {
            return;
        }
        Window::INSTANCE->m_idleCallback( );

    }

    void Window::registerKeyCallback( keyboardcallbackPTR callback )
    {
        m_keyCallback = callback;
    }

    void Window::registerMouseCallback( mouseCallbackPTR callback )
    {
        m_mouseCallback = callback;
    }
    void Window::registerMotionCallback( motionCallbackPTR callback )
    {
        m_motionCallback = callback;
    }
    void Window::registerLoopCallback( loopCallbackPTR callback )
    {
        m_loopCallback = callback;
    }
    void Window::registerReshapeCallback( reshapeCallbackPTR callback )
    {
        m_reshapeCallback = callback;
    }
    void Window::registerIdleCallback( idleCallbackPTR callback )
    {
        m_idleCallback = callback;
    }
    void Window::registerLoadCallback( loadCallbackPTR callback )
    {
        m_loadCallback = callback;
    }


}