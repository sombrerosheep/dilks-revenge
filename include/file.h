#ifndef DREV_FILE_H
#define DREV_FILE_H

///
/// @brief reads the entirety of a file and returns a pointer to the data
///
/// @param filePath file path to the file to load
/// @param mode mode to open the file with. "r" or "rb"
/// @return unsigned char* buffer of the file's contents.
///         returned memory must be freed with SDL_free
///
unsigned char *ReadEntireFile(const char *filePath, const char *mode);

#endif // DREV_FILE_H
