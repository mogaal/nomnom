
# Install.

if (NOT WITHOUT_DOC)
    file (GLOB howto_files "${CMAKE_CURRENT_SOURCE_DIR}/Howto*")
    install (FILES ${howto_files} DESTINATION share/doc/nomnom/)
endif ()


