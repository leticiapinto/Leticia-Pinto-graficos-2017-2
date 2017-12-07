


if( WIN44 )

    message( "Trabajo 2 - windows" )
    add_executable( main_5.exe main_5.cpp )

elseif( UNIX AND NOT APPLE )

    message( "Trabajo 2 - linux" )
    add_executable( main_5.out 
                    main_5.cpp 
                    LWindow.cpp 
                    LShaderManager.cpp
                    LVertexBuffer.cpp
                    LIndexBuffer.cpp
                    LVertexArray.cpp
                    LMesh.cpp
                    LMeshBuilder.cpp
                    Angel-yjc.h
                    vec.h 
                    mat-yjc-new.h
                    CheckError.h
                    )

    target_link_libraries( main_5.out
                           ${OPENGL_LIBRARIES} 
                           GLEW::GLEW 
                           ${GLUT_LIBRARY}
                           ${X11_LIBRARIES} 
                           ${CMAKE_THREAD_LIBS_INIT}
                           dl )


elseif( APPLE )

    message( "Trabajo 2 - macOS" )

endif()