#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "implementation.h"

std::string menu = "";

class Commands
{

private:

Hierarchy* current;

public:

void help();
    static void initiateCommand(std::string command, bool &end);
    /*!
    *load image from file
    *
    *if another image is loaded close it
    *
    *throw if file path is incorrect
    */
    static void open(std::string file_path, Image *&image);
    /*!
    *save image in current file path
    *
    *throw if no image is loaded
    */
    static void save(Image *&image);
    /*!
    *save image in new file path
    *
    *throw if no image is loaded
    */
    static void saveas(std::string file_path, Image *&image);
    /*!
    *dither image
    */
    static void dither(Image *&image);
    /*!
    *release image memory
    *
    *set curFilePath to empty string
    */
    static void deleteImage(Image *&image);
    /*!
    *create new image 
    *
    *the format to be used depends on the parameters
    */
    static void createNew(Image *&image, std::vector<std::string> &params);
    /*!
    *crop image 
    */
    static void crop(Image *&image, std::vector<std::string> &params);
    /*!
    *resize image 
    */
    static void resize(Image *&image, std::vector<std::string> &params);
    /*!
    *close everything that is open and exit 
    */
    static void exit(Image *&image);
};

//helpers
std::size_t getDitheringIndex();
void toUpper(std::string &input); //make whole string to upper
void printHelpMenu();

#endif