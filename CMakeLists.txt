cmake_minimum_required(VERSION 3.14.0)
project(Ghostbusters LANGUAGES C)
set(CMAKE_C_STANDARD 11)
#file(GLOB_RECURSE SOURCES src/c2p/*.c src/res/*.c src/misc/*.c include/*.h)
file(GLOB_RECURSE SOURCES src/*.c src/*.h)
#MESSAGE (${SOURCES})
if(ELF2HUNK)
	set(GHOST_EXECUTABLE ghost.elf)
	set(GHOST_OUTPUT_EXECUTABLE ghost.exe)
	add_executable(${GHOST_EXECUTABLE} ${SOURCES})
	target_link_libraries(${GHOST_EXECUTABLE} -Wl,-Map=ghost.map)
	add_custom_command(
		TARGET ${GHOST_EXECUTABLE} POST_BUILD
		COMMAND ${ELF2HUNK} ${GHOST_EXECUTABLE} ${GHOST_OUTPUT_EXECUTABLE}
	)
	add_custom_command(
		TARGET ${GHOST_EXECUTABLE} POST_BUILD
		COMMAND ${OBJDUMP} --disassemble -S ${GHOST_EXECUTABLE} > ghost.s
	)
else()
	SET(GHOST_EXECUTABLE ghost)
	SET(GHOST_OUTPUT_EXECUTABLE ghost)
	SET(PAK_EXECUTABLE convertAssets)
	SET(PAK_OUTPUT_EXECUTABLE convertAssets)
	
	add_executable(${GHOST_EXECUTABLE} ${SOURCES})
endif()

#if(AMIGA)
add_subdirectory(deps/ace ace)
#endif()

if(WIN32)
	find_package(SDL2 CONFIG REQUIRED)
	include_directories("C:\\Users\\Amiga\\AmigaDev\\LandsOfghost\\deps\\vcpkg\\packages\\sdl2_x86-windows\\include\\")
#	target_link_libraries(${GHOST_EXECUTABLE} PRIVATE SDL2::SDL2 SDL2::SDL2main)
	#target_link_libraries(${GHOST_EXECUTABLE} SDL2main)
endif()

# Version stuff
string(TIMESTAMP YEAR "%y")
string(TIMESTAMP DAY "%d")
string(TIMESTAMP MONTH "%m")
MATH(EXPR VER_MAJOR "0 + ${YEAR}")
MATH(EXPR VER_MINOR "0 + ${MONTH}")
MATH(EXPR VER_FIX "0 + ${DAY}")
set(VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_FIX}")

target_compile_definitions(${GHOST_EXECUTABLE} PRIVATE GHOST_VERSION="${VERSION}")
include_directories(${SDL2_INCLUDE_DIRS} include/)
target_link_libraries(${GHOST_EXECUTABLE} ace )  
target_compile_options(${GHOST_EXECUTABLE} PUBLIC -Wall)

if(GAME_DEBUG)
	target_compile_definitions(${GHOST_EXECUTABLE} PRIVATE GAME_DEBUG)
	target_compile_definitions(ace PUBLIC ACE_DEBUG_ALL ACE_DEBUG_UAE)
endif()


set(RES_DIR ${CMAKE_CURRENT_LIST_DIR}/res)
set(DATA_DIR ${CMAKE_CURRENT_BINARY_DIR}/data)
set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

set(TRANSPARENCY_HEX "#AAAAAA")

file(
	MAKE_DIRECTORY
	${DATA_DIR} ${DATA_DIR}/sfx ${DATA_DIR}/intro ${DATA_DIR}/outro ${DATA_DIR}/sprites
	${GEN_DIR}/game_bg ${GEN_DIR}/map ${GEN_DIR}/sprites/map   ${GEN_DIR}/sprites/ghosts  
)

# Palette
set(PLT_PATH ${DATA_DIR})


convertPalette(${GHOST_EXECUTABLE} ${RES_DIR}/bitmaps/germz.gpl ${PLT_PATH}/gemz.plt)
convertPalette(${GHOST_EXECUTABLE} ${RES_DIR}/bitmaps/GB-Classic.gpl ${PLT_PATH}/GB-Classic.plt)
convertPalette(${GHOST_EXECUTABLE} ${RES_DIR}/bitmaps/GB-Modern.gpl ${PLT_PATH}/GB-Modern.plt)
convertPalette(${GHOST_EXECUTABLE} ${RES_DIR}/maps/Ghostbusters.gpl ${PLT_PATH}/maps/GB-Game.plt)

# set(CITYTILES "")
# foreach(i RANGE 75)
# MATH(EXPR offsX "0 + ${i} * 16")
# extractBitmaps(TARGET ${GHOST_EXECUTABLE} SOURCE ${RES_DIR}/maps/City-Sheet16x.png
# DESTINATIONS
# ${GEN_DIR}/maps/${i}.png  ${offsX}   0 16 16
# )
# list(APPEND CITYTILES "${i}.png")
# endforeach()
# 	convertTileset(
# 		TARGET ${GHOST_EXECUTABLE} SIZE 16 PALETTE ${RES_DIR}/bitmaps/GB-Classic.gpl
# 		SOURCE ${GEN_DIR}/maps DESTINATION ${GEN_DIR}/citytiles.png
# 		TILES ${CITYTILES}
# 	)
# convertBitmaps(
# 	TARGET ${GHOST_EXECUTABLE} PALETTE ${RES_DIR}/bitmaps/GB-Classic.gpl
# 	INTERLEAVED SOURCES
# 		${RES_DIR}/bitmaps/GB-Classic.png

# 	DESTINATIONS
# 		${DATA_DIR}/GB-Classic.bm
# )
convertBitmaps(
	TARGET ${GHOST_EXECUTABLE} PALETTE ${RES_DIR}/maps/Ghostbusters.gpl
	INTERLEAVED SOURCES
		${RES_DIR}/bitmaps/GB-Classic.png

	DESTINATIONS
		${DATA_DIR}/GB-Classic.bm

)
set(MAPSPRITES "")
foreach(i RANGE 6)
	MATH(EXPR offsX "0 + ${i} * 16")
	extractBitmaps(TARGET ${GHOST_EXECUTABLE} SOURCE ${RES_DIR}/spritesheet/Sprite-0003-Sheet.png
	DESTINATIONS
	${GEN_DIR}/sprites/map/${i}.png  ${offsX}   0 16 16
	)
	list(APPEND MAPSPRITES "${i}.png")
	
endforeach(i RANGE 6)


foreach(i RANGE 6)
	MATH(EXPR offsX "0 + ${i} * 16")
	MATH(EXPR file "7 + ${i}")
	extractBitmaps(TARGET ${GHOST_EXECUTABLE} SOURCE ${RES_DIR}/spritesheet/Sprite-0003-Sheet.png
	DESTINATIONS
	${GEN_DIR}/sprites/map/${file}.png  ${offsX}   16 16 16
	)
	list(APPEND MAPSPRITES "${file}.png")
	
endforeach(i RANGE 6)

foreach(i RANGE 6)
	MATH(EXPR offsX "0 + ${i} * 16")
	MATH(EXPR file "14 + ${i}")
	extractBitmaps(TARGET ${GHOST_EXECUTABLE} SOURCE ${RES_DIR}/spritesheet/Sprite-0003-Sheet.png
	DESTINATIONS
	${GEN_DIR}/sprites/map/${file}.png  ${offsX}   32 16 16
	)
	list(APPEND MAPSPRITES "${file}.png")
	
endforeach(i RANGE 6)


convertTileset(
		TARGET ${GHOST_EXECUTABLE} SIZE 16 PALETTE ${RES_DIR}/maps/Ghostbusters.gpl
		SOURCE ${GEN_DIR}/sprites/map DESTINATION ${GEN_DIR}/mapsprites.png
		TILES ${CITYTILES}
)

convertBitmaps(
	TARGET ${GHOST_EXECUTABLE} PALETTE ${RES_DIR}/maps/Ghostbusters.gpl MASK_COLOR ${TRANSPARENCY_HEX}
	INTERLEAVED SOURCES 
		
		${GEN_DIR}/mapsprites.png
	DESTINATIONS
		${DATA_DIR}/sprites/mapsprites.bm
	MASKS
		${DATA_DIR}/sprites/mapsprites_masks.bm
)




set(CITYTILES "")
foreach(i RANGE 38)
MATH(EXPR offsX "0 + ${i} * 16")
extractBitmaps(TARGET ${GHOST_EXECUTABLE} SOURCE ${RES_DIR}/maps/City-Sheet.png
DESTINATIONS
${GEN_DIR}/maps/${i}.png  ${offsX}   0 16 16
)
list(APPEND CITYTILES "${i}.png")
endforeach()


foreach(i RANGE 38)
MATH(EXPR offsX "0 + ${i} * 16")
MATH(EXPR file "39 + ${i}")
extractBitmaps(TARGET ${GHOST_EXECUTABLE} SOURCE ${RES_DIR}/maps/City-Sheet.png
DESTINATIONS
${GEN_DIR}/maps/${file}.png  ${offsX}   16 16 16
)
list(APPEND CITYTILES "${file}.png")
endforeach()


foreach(i RANGE 38)
MATH(EXPR offsX "0 + ${i} * 16")
MATH(EXPR file "78 + ${i}")
extractBitmaps(TARGET ${GHOST_EXECUTABLE} SOURCE ${RES_DIR}/maps/City-Sheet.png
DESTINATIONS
${GEN_DIR}/maps/${file}.png  ${offsX}   32 16 16
)
list(APPEND CITYTILES "${file}.png")
endforeach()

foreach(i RANGE 38)
MATH(EXPR offsX "0 + ${i} * 16")
MATH(EXPR file "117 + ${i}")
extractBitmaps(TARGET ${GHOST_EXECUTABLE} SOURCE ${RES_DIR}/maps/City-Sheet.png
DESTINATIONS
${GEN_DIR}/maps/${file}.png  ${offsX}   48 16 16
)
list(APPEND CITYTILES "${file}.png")
endforeach()

foreach(i RANGE 38)
MATH(EXPR offsX "0 + ${i} * 16")
MATH(EXPR file "156 + ${i}")
extractBitmaps(TARGET ${GHOST_EXECUTABLE} SOURCE ${RES_DIR}/maps/City-Sheet.png
DESTINATIONS
${GEN_DIR}/maps/${file}.png  ${offsX}   64 16 16
)
list(APPEND CITYTILES "${file}.png")
endforeach()

convertTileset(
		TARGET ${GHOST_EXECUTABLE} SIZE 16 PALETTE ${RES_DIR}/maps/Ghostbusters.gpl 
		SOURCE ${GEN_DIR}/maps DESTINATION ${GEN_DIR}/citytiles.png
		TILES ${CITYTILES}
	)

convertBitmaps(
	TARGET ${GHOST_EXECUTABLE} PALETTE ${RES_DIR}/maps/Ghostbusters.gpl MASK_COLOR ${TRANSPARENCY_HEX}
	INTERLEAVED SOURCES
		
		${GEN_DIR}/citytiles.png
	DESTINATIONS
		${DATA_DIR}/maps/citytiles.bm
	MASKS
		${DATA_DIR}/maps/citytilesMasked.bm
)


set(GHOSTSPRITES "")
foreach(i RANGE 29)
MATH(EXPR offsX "0 + ${i} * 32")
extractBitmaps(TARGET ${GHOST_EXECUTABLE} SOURCE ${RES_DIR}/spritesheet/ghosts-24x24.png
DESTINATIONS
${GEN_DIR}/sprites/ghosts/${i}.png  ${offsX}   0 32 32
)
list(APPEND GHOSTSPRITES "${i}.png")
endforeach()

convertTileset(
		TARGET ${GHOST_EXECUTABLE} SIZE 32 PALETTE ${RES_DIR}/maps/Ghostbusters.gpl 
		SOURCE ${GEN_DIR}/sprites/ghosts DESTINATION ${GEN_DIR}/ghosts.png
		TILES ${GHOSTSPRITES}
	)

convertBitmaps(
	TARGET ${GHOST_EXECUTABLE} PALETTE ${RES_DIR}/maps/Ghostbusters.gpl MASK_COLOR ${TRANSPARENCY_HEX}
	INTERLEAVED SOURCES
		
		${GEN_DIR}/ghosts.png
	DESTINATIONS
		${DATA_DIR}/sprites/ghosts.bm
	MASKS
		${DATA_DIR}/sprites/ghosts_mask.bm
)


convertBitmaps(
	TARGET ${GHOST_EXECUTABLE} PALETTE ${RES_DIR}/bitmaps/GB-Modern.gpl
	INTERLEAVED SOURCES
		${RES_DIR}/bitmaps/GB-Modern.png
	DESTINATIONS
		${DATA_DIR}/GB-Modern.bm
		
)

# mergeMods(
# 	TARGET ${GHOST_EXECUTABLE}
# 	SOURCES ${RES_DIR}/music/ghostbusters_.mod 
# 	DESTINATIONS ${DATA_DIR}/ghostbusters.mod

# 	SAMPLE_PACK ${DATA_DIR}/samples.samplepack
# )

convertFont(
	TARGET ${GHOST_EXECUTABLE} FIRST_CHAR 32
	SOURCE ${RES_DIR}/fonts/font_germz.png DESTINATION ${DATA_DIR}/germz.fnt
)

convertFont(
	TARGET ${GHOST_EXECUTABLE} FIRST_CHAR 29
	SOURCE ${RES_DIR}/fonts/font_uni54.png DESTINATION ${DATA_DIR}/uni54.fnt
)
# Copy maps and defs
file(
	COPY
		${RES_DIR}/maps/city8x.json ${RES_DIR}/maps/City-Sheet8x.json
	DESTINATION ${DATA_DIR}/maps/
)
# Generating ZIP
set(GHOST_ZIP_NAME "${CMAKE_PROJECT_NAME} ${VER_MAJOR}_${VER_MINOR}_${VER_FIX}")
add_custom_target(generateZip COMMAND
	${CMAKE_COMMAND} -E tar "cf" "${GHOST_ZIP_NAME}.zip" --format=zip
	"${CMAKE_CURRENT_BINARY_DIR}/${GHOST_OUTPUT_EXECUTABLE}" "${DATA_DIR}"
	COMMENT "Generating ${GHOST_ZIP_NAME}.zip"
)


# Generating ADF
set(ADF_DIR "${CMAKE_CURRENT_BINARY_DIR}/adf")
add_custom_target(generateAdf
	COMMAND ${CMAKE_COMMAND} -E make_directory "${ADF_DIR}/s"
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${GHOST_OUTPUT_EXECUTABLE}" "${ADF_DIR}"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${DATA_DIR}" "${ADF_DIR}/data"
	COMMAND ${CMAKE_COMMAND} -E echo "${GHOST_OUTPUT_EXECUTABLE}" > "${ADF_DIR}/s/startup-sequence"
	COMMAND exe2adf.exe -l ${CMAKE_PROJECT_NAME} -a "${CMAKE_PROJECT_NAME}.adf" -d ${ADF_DIR}
	COMMAND ${CMAKE_COMMAND} -E rm -rf "${ADF_DIR}"
	COMMENT "Generating ADF file"
)
