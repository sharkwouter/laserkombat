#ifndef CONSTANTS_H
#define CONSTANTS_H

#if defined(NXDK)
    inline constexpr int WINDOW_WIDTH = 640;
    inline constexpr int WINDOW_HEIGHT = 480;
    inline constexpr int BLOCK_SIZE = 40;
#elif defined(__vita__)
    inline constexpr int WINDOW_WIDTH = 960;
    inline constexpr int WINDOW_HEIGHT = 544;
    inline constexpr int BLOCK_SIZE = 40;
#elif defined(__PSP__)
    inline constexpr int WINDOW_WIDTH = 480;
    inline constexpr int WINDOW_HEIGHT = 272;
    inline constexpr int BLOCK_SIZE = 16;
#else
    inline constexpr int WINDOW_WIDTH = 480;
    inline constexpr int WINDOW_HEIGHT = 272;
    inline constexpr int BLOCK_SIZE = 16;
#endif

inline constexpr int COLUMNS = 20;
inline constexpr int ROWS = 15;

inline constexpr int BEAM_PERSISTANCE = 4;		//beam lasts for this many 0.025 second intervals

inline constexpr char* TITLE = (char*) "Laser Kombat";

#endif // CONSTANTS_H
