LINK_DIRECTORIES(/usr/local/wormnet/libs)

PROJECT(Guard)

AUX_SOURCE_DIRECTORY(./ GUARD_SRC)
ADD_EXECUTABLE (guard ${GUARD_SRC})

INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(/usr/local/wormnet/include)

TARGET_LINK_LIBRARIES(guard wormnet pthread tinyxml event z glog)