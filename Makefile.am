
#The logging library
lib_LTLIBRARIES = libsonic_common.la


nobase_include_HEADERS = \
sonic/ds_common_types.h         sonic/std_file_utils.h         sonic/std_socket_service.h \
sonic/std_assert.h              sonic/std_int_mapping_utils.h  sonic/std_socket_tools.h \
sonic/std_bit_masks.h           sonic/std_ip_utils.h           sonic/std_struct_utils.h \
sonic/std_bit_ops.h             sonic/std_llist.h              sonic/std_system.h \
sonic/std_cmd_redir.h           sonic/std_mac_utils.h          sonic/std_thread_pool.h \
sonic/std_condition_variable.h  sonic/std_mergesort.h          sonic/std_thread_tools.h \
sonic/std_config_file.h         sonic/std_mutex_lock.h         sonic/std_time_tools.h  \
sonic/std_config_node.h         sonic/std_radical.h            sonic/std_tlv.h  \
sonic/std_directory.h           sonic/std_radix.h              sonic/std_tlv_internal.h \
sonic/std_envvar.h              sonic/std_rbtree.h             sonic/std_type_defs.h  \
sonic/std_error_codes.h         sonic/std_rw_lock.h            sonic/std_user_perm.h \
sonic/std_error_ids.h           sonic/std_select_tools.h       sonic/std_utils.h \
sonic/std_event_service.h       sonic/std_shlib.h              sonic/std_xml_parser.h \
sonic/std_crc32.h

libsonic_common_la_SOURCES = \
src/std_ip_utils.c    src/std_socket_service.cpp  \
src/std_bit_masks.c         src/std_llist.c       src/std_socket_tools.cpp \
src/std_cmd_redir.c         src/std_mac_utils.c   src/std_string_utils.cpp \
src/std_config_file.cpp     src/std_mergesort.c   src/std_system.c  \
src/std_config_node.c       src/std_mutex_lock.c  src/std_thread_pool.cpp \
src/std_directory.cpp       src/std_radical.c     src/std_thread_tools.c \
src/std_event_service.cpp   src/std_radix.c       src/std_time_tools.c \
src/std_event_utils.cpp     src/std_rbtree.c      src/std_user_perm.cpp \
src/std_file_utils.c        src/std_select.c      \
src/std_int_mapping_util.c  src/std_shlib.c       \
src/std_crc32.c

libsonic_common_la_CPPFLAGS = -I$(top_srcdir)/sonic -I$(includedir)/libxml2 -I$(includedir)/sonic
libsonic_common_la_CXXFLAGS = -std=c++11
libsonic_common_la_LDFLAGS = -shared -version-info 1:1:0
libsonic_common_la_LIBADD = -lsonic_logging -lxml2 -lpthread -lrt
