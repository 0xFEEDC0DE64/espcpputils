set(headers
    src/espcppmacros.h
    src/espcrc32builder.h
    src/esprandom.h
    src/espstrutils.h
    src/futurecpp.h
    src/lockhelper.h
    src/lockingqueue.h
    src/recursivelockhelper.h
    src/taskutils.h
    src/tickchrono.h
    src/wrappers/binary_semaphore.h
    src/wrappers/counting_semaphore.h
    src/wrappers/event_group.h
    src/wrappers/http_client.h
    src/wrappers/mqtt_client.h
    src/wrappers/mutex_semaphore.h
    src/wrappers/queue.h
    src/wrappers/recursive_mutex_semaphore.h
    src/wrappers/websocket_client.h
)

set(sources
    src/espstrutils.cpp
    src/taskutils.cpp
)

set(dependencies
    freertos
    esp_system
#    esp_http_client
    esp_websocket_client
    nghttp
    tcp_transport

    cpputils
    espchrono
    expected
    fmt
)

idf_component_register(
    INCLUDE_DIRS
        src
    SRCS
        ${headers}
        ${sources}
    REQUIRES
        ${dependencies}
)

target_compile_options(${COMPONENT_TARGET}
    PRIVATE
        -fstack-reuse=all
        -fstack-protector-all
        -Wno-unused-function
        -Wno-deprecated-declarations
        -Wno-missing-field-initializers
        -Wno-parentheses
)
