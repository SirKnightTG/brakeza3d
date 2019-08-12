//
// Created by darkhead on 2019-08-10.
//

#include "../../headers/2D/Menu.h"
#include "../cJSON.h"
#include "../../headers/Render/Tools.h"
#include "../../headers/Render/Logging.h"


Menu::Menu()
{
    this->currentOptions = 0;
    this->numOptions = 0;
}

void Menu::getOptionsJSON()
{
    size_t file_size;
    const char *mapsFile;
    mapsFile = Tools::readFile(EngineSetup::getInstance()->CONFIG_FOLDER + EngineSetup::getInstance()->CFG_MENU, file_size);
    cJSON *myDataJSON = cJSON_Parse(mapsFile);
    if (myDataJSON == NULL) {
        Logging::getInstance()->Log("menu.json can't be loaded", "ERROR");
        return;
    }

    cJSON *currentOption = NULL;
    optionsJSON = cJSON_GetObjectItemCaseSensitive(myDataJSON, "options" );
    int sizeOptions = cJSON_GetArraySize(optionsJSON);

    if (sizeOptions > 0) {
        cJSON *firstMapOption = cJSON_GetArrayItem(optionsJSON, 0);
        cJSON *nameOption = cJSON_GetObjectItemCaseSensitive(firstMapOption, "name");

        Logging::getInstance()->Log("menu.json have " + std::to_string(sizeOptions) + " optionsJSON");
    } else {
        Logging::getInstance()->Log("menu.json is empty", "ERROR");
    }

    cJSON_ArrayForEach(currentOption, optionsJSON) {
        cJSON *nameOption = cJSON_GetObjectItemCaseSensitive(currentOption, "name");
        cJSON *imageOnOption = cJSON_GetObjectItemCaseSensitive(currentOption, "image_on");
        cJSON *imageOffOption = cJSON_GetObjectItemCaseSensitive(currentOption, "image_off");

        std::string fullPathImageOn = EngineSetup::getInstance()->IMAGES_FOLDER + imageOnOption->valuestring;
        std::string fullPathImageOff = EngineSetup::getInstance()->IMAGES_FOLDER + imageOffOption->valuestring;

        if (cJSON_IsString(nameOption)) {
            this->addOption(nameOption->valuestring, fullPathImageOn, fullPathImageOff);
            Logging::getInstance()->Log("Menu option JSON detected: " + std::string(nameOption->valuestring));
        }
    }
}

void Menu::addOption(std::string label, std::string image_on, std::string image_off )
{
    Logging::getInstance()->Log("Adding menu option '" + label + "'");
    this->options[numOptions] = new MenuOption(label, image_on, image_off);
    numOptions++;
}

void Menu::drawOptions(SDL_Surface *dst)
{
    int offsetY = 20;
    int offsetX = 50;

    int stepY = 30;
    for( int i = 0 ; i < numOptions ; i++) {
        SDL_Rect r;
        r.x = offsetX;
        r.y = offsetY;

        if (i == currentOptions) {
            SDL_BlitSurface(this->options[i]->image_on, NULL, dst, &r);
        } else {
            SDL_BlitSurface(this->options[i]->image_off, NULL, dst, &r);
        }

        offsetY += stepY;
    }
}