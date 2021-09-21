# ============================================
# defines
# ============================================
list(APPEND DEF_LISTS "_CRT_SECURE_NO_WARNINGS")
list(APPEND DEF_LISTS "IMG_DIR=\"${CMAKE_SOURCE_DIR}/Images\"")



# ============================================
# include paths
# ============================================
if(WIN32)
    list(APPEND INC_PATHS "C:/Program Files/SDL2-2.0.16/include")
    list(APPEND INC_PATHS "C:/Program Files/SDL2_image-2.0.5/include")
else()
    list(APPEND INC_PATHS "/usr/include/SDL2")
endif()
list(APPEND INC_PATHS "Include")


# ============================================
# library paths
# ============================================
if(WIN32)
    list(APPEND LIB_PATHS "C:/Program Files/SDL2-2.0.16/lib/x64")
    list(APPEND LIB_PATHS "C:/Program Files/SDL2_image-2.0.5/lib/x64")
else()
    list(APPEND LIB_PATHS "/usr/lib/x86_64-linux-gnu")
endif()

# ============================================
# library lists
# ============================================
list(APPEND LIB_LISTS "SDL2")
list(APPEND LIB_LISTS "SDL2main")
list(APPEND LIB_LISTS "SDL2_image")


# ============================================
# source lists
# ============================================
list(APPEND SRC_LISTS "Source/main.cpp")
list(APPEND SRC_LISTS "Source/Workspace.cpp")
list(APPEND SRC_LISTS "Source/Window.cpp")
list(APPEND SRC_LISTS "Source/Texture.cpp")
list(APPEND SRC_LISTS "Source/Bitmap.cpp")
list(APPEND SRC_LISTS "Source/Image.cpp")

