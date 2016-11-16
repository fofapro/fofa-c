aux_source_directory(. DIR_SRCS)
set(EXTRA_LIBS
	../lib/libcurl_imp
	../lib/libiconv
	)

set(EXTRA_DLLS
	../lib/libcharset1.dll
	../lib/libcurl.dll
	../lib/libeay32.dll
	../lib/libiconv2.dll
	../lib/libintl3.dll
	../lib/ssleay32.dll
	../lib/zlib1.dll
	)
	
include_directories(../include)
link_libraries(${EXTRA_DLLS})
add_library(fofa ${DIR_SRCS})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/api/build)
