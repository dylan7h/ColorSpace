# ============================================
# defines
# ============================================
list(APPEND DEF_LISTS "_CRT_SECURE_NO_WARNINGS")


# ============================================
# include paths
# ============================================
list(APPEND INC_PATHS "Include")
list(APPEND INC_PATHS "C:/Program Files/SDL2-2.0.16/include")


# ============================================
# library paths
# ============================================
list(APPEND LIB_PATHS "C:/Program Files/SDL2-2.0.16/lib/x64")


# ============================================
# library lists
# ============================================
list(APPEND LIB_LISTS "SDL2")
list(APPEND LIB_LISTS "SDL2main")


# ============================================
# source lists
# ============================================
list(APPEND SRC_LISTS "Source/main.cpp")
list(APPEND SRC_LISTS "Source/Core.cpp")
list(APPEND SRC_LISTS "Source/Bitmap.cpp")
list(APPEND SRC_LISTS "Source/Conversion.cpp")

