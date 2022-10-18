#ifndef __M_VOILES_H__
#define __M_VOILES_H__

// Renvoie l'angle souhaité pour les voiles en fonction de l'angle de la girouette
int angle_voiles(int angle_girouette);

// Renvoie une valeur de l'angle de la girouette en valeur absolue 
// Paramètre : int angle_ABI [0;359]
// Retour    : int angle_girouette_abs [0;179]
int angle_girouette_abs(int angle_ABI);

#endif
