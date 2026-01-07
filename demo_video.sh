#!/bin/bash

# Script d'automatisation pour la vidéo de démo "Eco-Sensing"
# Ce script lance le programme et simule les entrées utilisateur avec des pauses
# pour que tu aies le temps de parler ou que la vidéo soit fluide.

# 1. Compilation propre
echo "--- Étape 1 : Compilation ---"
make clean
make
echo "--- Lancement de la Démo ---"
sleep 2

# 2. Exécution du scénario "Saturation Mémoire"
# ( echo "1"; sleep 1; ... ) permet d'envoyer les commandes une par une avec du délai.

(
    # Choisir "New Simulation"
    echo "1" 
    sleep 2
    
    # Position X = 500 (Loin -> Coût élevé -> Pas d'envoi)
    echo "500"
    sleep 1
    # Position Y = 500
    echo "500"
    sleep 2

    # Step 1 : Génération paquet 1
    echo "5"
    sleep 1.5

    # Step 2 : Génération paquet 2
    echo "5"
    sleep 1.5

    # Step 3 : Génération paquet 3
    echo "5"
    sleep 1.5

    # Step 4 : Génération paquet 4
    echo "5"
    sleep 1.5

    # Step 5 : Génération paquet 5 (Buffer Plein après ça)
    echo "5"
    sleep 2

    # Step 6 : Génération paquet 6 -> ALERTE (Preuve Vidéo)
    echo "5"
    sleep 4

    # Save State
    echo "6"
    sleep 2

    # Exit
    echo "7"
    sleep 1
    # Confirm Save
    echo "1"

) | ./eco_sensing

echo ""
echo "--- Fin de la Démo ---"
