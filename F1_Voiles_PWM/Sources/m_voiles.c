#include "m_voiles.h"

// Renvoie l'angle souhaité pour les voiles en fonction de l'angle de la girouette
// angle_girouette_abs : angle en valeur absolue entre 0 et 179°
int angle_voiles(int angle_girouette_abs)
{
	return (angle_girouette_abs > 45) ? (angle_girouette_abs-45)*90/135 : 0;
}

// Renvoie une valeur de l'angle de la girouette en valeur absolue 
// Paramètre : int angle_ABI [0;359]
// Retour    : int angle_girouette_abs [0;179]
int angle_girouette_abs(int angle_ABI)
{
	angle_ABI -= 180;
	return (angle_ABI>0) ? -angle_ABI : angle_ABI;
}



