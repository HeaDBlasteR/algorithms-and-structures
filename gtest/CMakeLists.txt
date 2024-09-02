set(TARGET "gtest")
create_project_lib(${TARGET})

find_package(Threads)

if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(${TARGET} PUBLIC "-pthread")
endif()

if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(${TARGET} "${CMAKE_THREAD_LIBS_INIT}")
endif()
