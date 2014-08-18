LINK_DIRECTORIES(/usr/local/wormnet/libs)

PROJECT(FlashPolicy)

SET(FLASHPOLICY_SRC main.cpp PolicyApp.cpp PolicySocketHandler.cpp)
ADD_EXECUTABLE (flashpolicy ${FLASHPOLICY_SRC})

INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(/usr/local/wormnet/include)


TARGET_LINK_LIBRARIES(flashpolicy wormnet raknet pthread tinyxml iconv event z ct glog)