set(headers
    include/lockhelper.h
    include/lockingqueue.h
    include/recursivelockhelper.h
    include/esprandom.h
    include/wrappers/binary_semaphore.h
    include/wrappers/counting_semaphore.h
    include/wrappers/event_group.h
    include/wrappers/http_client.h
    include/wrappers/mutex_semaphore.h
    include/wrappers/queue.h
    include/wrappers/recursive_mutex_semaphore.h
    include/wrappers/websocket_client.h
)

idf_component_register(INCLUDE_DIRS include SRCS ${headers} REQUIRES freertos esp_system esp_http_client esp_websocket_client cpputils)
