#pragma once

#ifdef GEODE_IS_WINDOWS
    #ifdef CUSTOM_OBJECTS_EXPORTING
        #define CUSTOM_OBJECTS_DLL __declspec(dllexport)
    #else
        #define CUSTOM_OBJECTS_DLL __declspec(dllimport)
    #endif
#else
    #define CUSTOM_OBJECTS_DLL __attribute__((visibility("default")))
#endif