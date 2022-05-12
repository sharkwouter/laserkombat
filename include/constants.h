#ifndef CONSTANTS_H
#define CONSTANTS_H

#if defined(NXDK) || defined(__vita__)
    inline constexpr int IMAGE_WIDTH = 32;
#else
    inline constexpr int IMAGE_WIDTH = 40;
#endif

inline constexpr int COLUMNS = 20;
inline constexpr int ROWS = 15;

inline constexpr int WINDOW_WIDTH = COLUMNS*IMAGE_WIDTH;
inline constexpr int WINDOW_HEIGHT = ROWS*IMAGE_WIDTH;

inline constexpr int BEAM_PERSISTANCE = 4;		//beam lasts for this many 0.025 second intervals

inline constexpr char* NAME = (char*) "TANK";
inline constexpr char* TITLE = (char*) "Laser Kombat";

#endif // CONSTANTS_H
