#ifndef CONSTANTS_H
#define CONSTANTS_H

#if defined(NXDK)
    inline constexpr int WINDOW_WIDTH = 640;
    inline constexpr int WINDOW_HEIGHT = 480;
#elif defined(__vita__)
    inline constexpr int WINDOW_WIDTH = 960;
    inline constexpr int WINDOW_HEIGHT = 544;
#else
    inline constexpr int WINDOW_WIDTH = 800;
    inline constexpr int WINDOW_HEIGHT = 600;
#endif

inline constexpr int COLUMNS = 20;
inline constexpr int ROWS = 15;
inline constexpr int IMAGE_WIDTH = 40;

inline constexpr int BEAM_PERSISTANCE = 4;		//beam lasts for this many 0.025 second intervals

inline constexpr char* NAME = (char*) "TANK";
inline constexpr char* TITLE = (char*) "Laser Kombat";

#endif // CONSTANTS_H
