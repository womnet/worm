SET(LIBSERVER_SRC App.cpp Config.cpp PlayerSocketHander.cpp)
ADD_LIBRARY (server STATIC ${LIBSERVER_SRC})
INCLUDE_DIRECTORIES(../)
INCLUDE_DIRECTORIES(/usr/local/wormnet/include)
