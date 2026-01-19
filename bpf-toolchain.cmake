set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR bpf)

set(CMAKE_C_COMPILER clang)

#set(CMAKE_C_COMPILER_TARGET bpf)
#set(CMAKE_CXX_COMPILER_TARGET bpf)
get_filename_component(BPFVM_TOP_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

# BpfWideArgs pass 插件：突破 BPF 5 参数限制 + 支持返回结构体。
set(_bpf_wideargs "")
if(EXISTS "${BPFVM_TOP_INCLUDE}/build/libBpfWideArgs.so")
    set(_bpf_wideargs_flag "-fpass-plugin=${BPFVM_TOP_INCLUDE}/build/libBpfWideArgs.so")
endif()

# BpfSoftFp pass 插件：把浮点运算（fadd/fmul/fptosi/...）在 IR 层替换成对
# extern __ksym __bpf_fp_<ID> 的调用，经 bpfvm-ld 改写成 BPF_FP_* call
# （src_reg=2，FP 专用通道），绕过 BPF 后端在 ISel 对浮点的拒绝。
# VM 侧 do_softfp 用宿主硬件浮点执行。无 guest 侧 glue。
set(_bpf_softfp "")
if(EXISTS "${BPFVM_TOP_INCLUDE}/build/libBpfSoftFp.so")
    set(_bpf_softfp_flag "-fpass-plugin=${BPFVM_TOP_INCLUDE}/build/libBpfSoftFp.so")
endif()

set(CMAKE_C_FLAGS "-target bpf \
                   -fno-merge-all-constants -O1 \
                   -fno-builtin \
                   -mcpu=v4 \
                   -mllvm -bpf-stack-size=4096 \
                   -Xclang -target-feature -Xclang +alu32 \
                   -Xclang -target-feature -Xclang -dwarfris \
                   -Wno-unused-command-line-argument \
                   ${_bpf_wideargs_flag} \
                   ${_bpf_softfp_flag} \
                   -I${BPFVM_TOP_INCLUDE}/include" CACHE STRING "C compiler flags for BPF" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-e,main" CACHE STRING "Linker flags for BPF executables")

set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE> && bpf-objcopy --set-section-flags .rodata.str1.1=alloc,readonly,data <OBJECT>")
set(AS_C11 "ON")

#set(CMAKE_LINKER /usr/bin/bpf-ld)
#set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
#set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
