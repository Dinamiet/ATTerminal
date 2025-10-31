cmake_minimum_required(VERSION 4.1)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
	Buffers
	GIT_REPOSITORY
		git@github.com:Dinamiet/Buffers.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/Buffers
)

FetchContent_Declare(
	ChecksumHash
	GIT_REPOSITORY
		git@github.com:Dinamiet/ChecksumHash.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/ChecksumHash
)

FetchContent_MakeAvailable(
	Buffers
	ChecksumHash
)
