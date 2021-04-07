set(headers
    src/espcrc32builder.h
    src/esprandom.h
    src/espstrutils.h
    src/lockhelper.h
    src/lockingqueue.h
    src/recursivelockhelper.h
    src/taskutils.h
    src/tickchrono.h
    src/wrappers/binary_semaphore.h
    src/wrappers/counting_semaphore.h
    src/wrappers/event_group.h
    src/wrappers/http_client.h
    src/wrappers/mutex_semaphore.h
    src/wrappers/queue.h
    src/wrappers/recursive_mutex_semaphore.h
    src/wrappers/websocket_client.h
)

set(sources
    src/espchrono_impl.cpp
    src/espstrutils.cpp
    src/taskutils.cpp
)

idf_component_register(INCLUDE_DIRS src SRCS ${headers} ${sources} REQUIRES freertos esp_system esp_http_client esp_websocket_client cpputils espchrono)
