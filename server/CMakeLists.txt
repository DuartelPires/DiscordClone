cmake_minimum_required(VERSION 3.16)
project(DiscordServer CXX)

set(CMAKE_CXX_STANDARD 17)

# Encontrar o gRPC primeiro resolve o conflito de dependências do Protobuf no Arch
find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

find_program(PROTOC_PATH protoc)
find_program(GRPC_CPP_PLUGIN_PATH grpc_cpp_plugin)

set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../contract")
set(PROTO_FILE "${PROTO_DIR}/helloworld.proto")

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})
set(GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
set(GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")

add_custom_command(
    OUTPUT "${GRPC_SRCS}" "${GRPC_HDRS}"
    COMMAND ${PROTOC_PATH}
    ARGS --grpc_out="${CMAKE_CURRENT_BINARY_DIR}"
         --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_PATH}"
         -I "${PROTO_DIR}"
         "${PROTO_FILE}"
    DEPENDS ${PROTO_FILE}
)

add_executable(discord_server greeter_server.cc ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS})
target_include_directories(discord_server PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(discord_server PRIVATE gRPC::grpc++ protobuf::libprotobuf)
