CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#-------------------------------------------------
#
# Load Java
#
#-------------------------------------------------
FIND_PACKAGE(Java)
INCLUDE(UseJava)

file(GLOB JAVA_SOURCE
	ConwayGUI/src/conwaygui/*.java 
	ConwayGUI/src/testing/*.java
	)

file(GLOB JAVA_JARS
	ConwayGUI/lib/*.jar
	)

#-------------------------------------------------
#
# Build Jar
#
#-------------------------------------------------
add_jar(GuiBuild 
	SOURCES
		${JAVA_SOURCE}
	INCLUDE_JARS
		${JAVA_JARS}
	MANIFEST 
		"ConwayGUI/manifest.mf"
	)
