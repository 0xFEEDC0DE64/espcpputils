set(headers
    include/binary_semaphore.h
    include/counting_semaphore.h
    include/esprandom.h
    include/event_group.h
    include/http_client.h
    include/lockhelper.h
    include/lockingqueue.h
    include/mutex_semaphore.h
    include/recursivelockhelper.h
    include/recursive_mutex_semaphore.h
    include/websocket_client.h
)

idf_component_register(INCLUDE_DIRS include SRCS ${headers} REQUIRES freertos esp_system esp_http_client esp_websocket_client cpputils)
