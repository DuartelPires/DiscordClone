cmake_minimum_required(VERSION 3.16)
project(MeuHelloWorldGRPC CXX)

set(CMAKE_CXX_STANDARD 17)

# Encontra o gRPC, Protobuf e as flags do Abseil instaladas no Arch
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(absl REQUIRED COMPONENTS flags flags_parse log strings)

find_program(PROTOC_PATH protoc)
find_program(GRPC_CPP_PLUGIN_PATH grpc_cpp_plugin)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS chat.proto)

set(GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.cc")
set(GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.h")

add_custom_command(
    OUTPUT "${GRPC_SRCS}" "${GRPC_HDRS}"
    COMMAND ${PROTOC_PATH}
    ARGS --grpc_out="${CMAKE_CURRENT_BINARY_DIR}"
         --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_PATH}"
         -I "${CMAKE_CURRENT_SOURCE_DIR}"
         "${CMAKE_CURRENT_SOURCE_DIR}/chat.proto"
    DEPENDS chat.proto
)

# Configura o Servidor
add_executable(meu_servidor servidor.cc ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS})
target_include_directories(meu_servidor PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(meu_servidor PRIVATE gRPC::grpc++ protobuf::libprotobuf absl::flags absl::flags_parse absl::log absl::strings)

# Configura o Cliente
add_executable(meu_cliente cliente.cc ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS})
target_include_directories(meu_cliente PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(meu_cliente PRIVATE gRPC::grpc++ protobuf::libprotobuf absl::flags absl::flags_parse absl::log absl::strings)
