#Setup CMake to run tests
file(GLOB TEST_SRCS "*.cpp")

#Run through each source
foreach(testSrc ${TEST_SRCS})
    #Extract the filename without an extension (NAME_WE)
    get_filename_component(testName ${testSrc} NAME_WE)
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${OpenCV_INCLUDE_DIRS})
    link_directories(${BOOST_LIBRARYDIR})

    #Add compile target
    add_executable(${testName} ${testSrc})

    #link to Boost libraries AND your targets and dependencies
    if (MSVC)
       target_link_libraries(${testName} libobjdtect ${OpenCV_LIBS} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
    else()   
       target_link_libraries(${testName} libobjdtect )
       target_link_libraries(${testName} ${OpenCV_LIBS} )
       # target_link_libraries(${testName} ${Boost_LIBRARIES})
       target_link_libraries( ${testName} ${Boost_system_LIBRARY})
       target_link_libraries( ${testName} ${Boost_thread_LIBRARY})
       target_link_libraries( ${testName} ${Boost_serialization_LIBRARY})
       target_link_libraries( ${testName} ${Boost_FILESYSTEM_LIBRARY})
    endif()
    #I like to move testing binaries into a testBin directory
    set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

    # Finally add it to test execution - 
    # Notice the WORKING_DIRECTORY and COMMAND
    # add_test(NAME ${testName} 
    # WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin 
    # COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName} )
endforeach(testSrc)
