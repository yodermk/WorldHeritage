cmake_minimum_required(VERSION 3.17)
project(WorldHeritage)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost 1.75.0 REQUIRED)

#find_package(folly REQUIRED)
#set(FOLLY_LIBRARIES Folly::folly)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(WorldHeritage main.cpp Site.cpp Site.h Controller.cpp Controller.h TravelGraphGame.cpp TravelGraphGame.h)
#target_link_libraries(${PROJECT_NAME} ${FOLLY_LIBRARIES})
