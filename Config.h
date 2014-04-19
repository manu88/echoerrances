//
//  Config.h
//  audioLib
//
//  Created by Manuel Deneu on 15/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef audioLib_Config_h
#define audioLib_Config_h

#ifdef __APPLE__
#define HRTF_FILEPATH "/Users/manueldeneu/HrtfDatabase/"
#endif // __APPLE__

#ifdef __WIN32__
#define HRTF_FILEPATH "C:\dossiers personnels\paul\programation\cpp\echoerrance\HrtfDatabase"
#endif // __WIN32__

// debug mode
#define PD_DEBUG


// taille des impulses
#define HRTF_LENGTH               400

// Force l'utilisation d'un mode générique non-optimisé des algos
#define USE_BASIC_COMPUTATION


#endif
