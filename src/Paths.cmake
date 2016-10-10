# Multimodal Interaction Project
#
# Candy Catch
#
# You usually have to change here your system paths

# OpenCV/irrKlang first stage
if(WIN32)
	set(OpenCV_DIR "C:/Users/Scottish-fold/Documents/Visual Studio 2015/Projects/OpenCV/opencv/build")
	set(IrrKlang_DIR "C:/Users/Scottish-fold/Documents/Visual Studio 2015/Projects/irrKlang-64bit-1.5.0")
else()
	set(OpenCV_DIR "/usr/lib/opencv")
	set(IrrKlang_DIR "/usr/lib/irrKlang-64bit-1.5.0")
endif()

# OpenCV/irrKlang Include dir
set(OpenCV_INCLUDE_DIRS "${OpenCV_DIR}/include")
set(IrrKlang_INCLUDE_DIRS "${IrrKlang_DIR}/include")

# OpenCV/irrKlang Library dir
set(OpenCV_LIBS "${OpenCV_DIR}/x64/vc14/lib")
set(IrrKlang_LIBS "${IrrKlang_DIR}/lib/Winx64-visualStudio")

# OpenCV/irrKlang Binary dir
if(WIN32)
	set(OpenCV_BINS "${OpenCV_DIR}/x64/vc14/bin")
	set(IrrKlang_BINS "${IrrKlang_DIR}/bin/winx64-visualStudio")
else()
	# TODO You need binarys for iOS?? change here
	set(OpenCV_BINS "${OpenCV_DIR}/x64/vc14/bin")
	set(IrrKlang_BINS "${IrrKlang_DIR}/bin/macosx-gcc")
endif()

# Output dir for Debug Binarys
set(DEBUG_BINARY_DIR "${CMAKE_BINARY_DIR}/Debug")

# Output dir for Release Binarys
set(RELEASE_BINARY_DIR "${CMAKE_BINARY_DIR}/Release")