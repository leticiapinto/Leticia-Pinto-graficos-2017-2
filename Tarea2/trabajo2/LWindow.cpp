

#include "LWindow.h"

#include <iostream>

using namespace std;

namespace engine
{

    LWindow* LWindow::INSTANCE = NULL;

    LWindow::LWindow()
    {
        m_keyCallback = NULL;
        m_mouseCallback = NULL;
        m_motionCallback = NULL;
        m_loopCallback = NULL;
        m_reshapeCallback = NULL;
        m_idleCallback = NULL;
    }

    void LWindow::init()
    {

        m_width=APP_WIDTH;
        m_height=APP_HEIGHT;

        if ( LWindow::INSTANCE != NULL )
        {
            delete LWindow::INSTANCE;
        }

        LWindow::INSTANCE = this;


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
        LWindow::onLoadCallBack();

        glutDisplayFunc(LWindow::onLoopCallBack);
        glutReshapeFunc(LWindow::onReshapeCallBack);
        glutMouseFunc(LWindow::onMouseCallback);
        glutMotionFunc(LWindow::onMotionCallBack);
        glutKeyboardFunc(LWindow::onKeyCallback);
        glutIdleFunc(LWindow::onIdleCallBack);
        glutMainLoop();
        m_initialized = true;

    }

    LWindow::~LWindow()
    {
        release();
    }

    void LWindow::release()
    {
        //m_window = NULL;
        //glfwTerminate();
    }

    void LWindow::onLoadCallBack()
    {
        if ( LWindow::INSTANCE->m_loadCallback == NULL )
        {
            return;
        }

        LWindow::INSTANCE->m_loadCallback();
    }

    void LWindow::onKeyCallback( unsigned char key, int x, int y )
    {
        if ( LWindow::INSTANCE->m_keyCallback == NULL )
        {
            return;
        }

        LWindow::INSTANCE->m_keyCallback( key, x, y );
    }

    void LWindow::onMouseCallback( int button, int state, int x, int y)
    {
        if ( LWindow::INSTANCE->m_mouseCallback == NULL )
        {
            return;
        }
        
        LWindow::INSTANCE->m_mouseCallback( button, state, x, y );
    }

    void LWindow::onMotionCallBack(int x, int y)
    {
        if ( LWindow::INSTANCE->m_motionCallback == NULL )
        {
            return;
        }
        LWindow::INSTANCE->m_motionCallback( x, y );

    }

    void LWindow::onLoopCallBack()
    {
        if ( LWindow::INSTANCE->m_loopCallback == NULL )
        {
            return;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        LWindow::INSTANCE->m_loopCallback();
        glutSwapBuffers();
    }

    void LWindow::onReshapeCallBack(int width, int height)
    {
        if ( LWindow::INSTANCE->m_reshapeCallback == NULL )
        {
            return;
        }
        LWindow::INSTANCE->m_reshapeCallback( width, height );

    }

    void LWindow::onIdleCallBack()
    {
        if ( LWindow::INSTANCE->m_idleCallback == NULL )
        {
            return;
        }
        LWindow::INSTANCE->m_idleCallback( );

    }

    void LWindow::registerKeyCallback( FnPtr_keyboard_callback callback )
    {
        m_keyCallback = callback;
    }

    void LWindow::registerMouseCallback( FnPtr_mouse_callback callback )
    {
        m_mouseCallback = callback;
    }
    void LWindow::registerMotionCallback( FnPtr_motion_callback callback )
    {
        m_motionCallback = callback;
    }
    void LWindow::registerLoopCallback( FnPtr_loop_callback callback )
    {
        m_loopCallback = callback;
    }
    void LWindow::registerReshapeCallback( FnPtr_reshape_callback callback )
    {
        m_reshapeCallback = callback;
    }
    void LWindow::registerIdleCallback( FnPtr_idle_callback callback )
    {
        m_idleCallback = callback;
    }
    void LWindow::registerLoadCallback( FnPtr_load_callback callback )
    {
        m_loadCallback = callback;
    }


}