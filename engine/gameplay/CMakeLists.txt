#--------------------------------------------------------------------------
# Gameplay
#--------------------------------------------------------------------------

SET(files_gameplay
	TransformCmp.h
	TransformCmp.cpp
	CameraCmp.h
	CameraCmp.cpp
	RigidbodyCmp.h
	RigidbodyCmp.cpp
	)

SOURCE_GROUP("gameplay" FILES ${files_gameplay})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(gameplay STATIC ${files_gameplay} ${files_pch})
TARGET_PCH(gameplay ../)
ADD_DEPENDENCIES(gameplay glew enet)
TARGET_LINK_LIBRARIES(gameplay PUBLIC engine exts glew enet soloud)