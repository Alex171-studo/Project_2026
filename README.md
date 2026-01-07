# Eco-Sensing: Documentation Complète du Projet

Ce document présente l'architecture, la structure et le fonctionnement détaillé du simulateur de réseau de capteurs "Eco-Sensing".

## 1. Vue d'ensemble
Le projet est un simulateur écrit en C modulaire qui modélise le comportement d'un capteur IoT autonome. Il gère :
- Une mémoire dynamique limitée (Liste chaînée, Max 5 paquets).
- Une simulation énergétique précise (transmission radio).
- La persistance des données (sauvegarde binaire).
- Un système de log pour le suivi ("Crash Test").

## 2. Structure du Projet
Nous avons adopté une architecture modulaire stricte pour séparer les responsabilités :
```
EcoSensing/
├── Makefile            # Script de compilation automatique
├── inc/                # Fichiers d'en-tête (.h) - Contrats et Structures
│   ├── sensor.h
│   ├── simulation.h
│   └── persistence.h
└── src/                # Fichiers sources (.c) - Implémentation
    ├── main.c
    ├── sensor.c
    ├── simulation.c
    └── persistence.c
```

## 3. Détail des Fichiers

### 3.1 Module Capteur (`sensor.h` / `sensor.c`)
Ce module est le cœur de la gestion mémoire.
- **Structures** :
    - `Packet` : Un nœud de liste chaînée contenant l'ID, la valeur, le timestamp et un pointeur `next`.
    - `Sensor` : Contient l'état du capteur (batterie, position) et la tête de la liste de paquets (`bufferHead`).
- **Fonctions Clés** :
    - `createSensor(x, y)` : `malloc` le capteur.
    - `producePacket(sensor)` : C'est la fonction la plus critique.
        1. Alloue un `Packet` avec `malloc`.
        2. Vérifie si le buffer est plein (5 paquets).
        3. Si Plein : **Supprime le premier paquet** (FIFO) et appelle `free()` pour éviter les fuites mémoire. Affiche l'ALERTE requise.
        4. Ajoute le nouveau paquet à la fin.

### 3.2 Module Simulation (`simulation.h` / `simulation.c`)
Ce module gère la "Physique" et le déroulement du temps.
- **Logique** :
    - `calculateTransmissionEnergy` : Applique la formule $E = 0.05 + 0.01 \times d^2$.
    - `attemptTransmission` : Parcourt la liste des paquets. Si la batterie est suffisante, le paquet est envoyé (supprimé de la mémoire avec `free`) et la batterie est décrémentée.
- **Logging** :
    - `logToCrashFile` : Écrit dans `log.txt` l'état instantané (Temps, Batterie, Buffer Usage) pour le "Crash Test".

### 3.3 Module Persistance (`persistence.h` / `persistence.c`)
Ce module gère la sauvegarde binaire (Binaire, pas texte).
- **Problème** : On ne peut pas sauvegarder les pointeurs (`Packet* next`) directement car les adresses changent à chaque exécution.
- **Solution** :
    - **Sauvegarde (`saveState`)** : On écrit d'abord la structure `Sensor` brute, puis on parcourt la liste chaînée et on écrit le contenu de chaque `Packet` l'un après l'autre dans le fichier.
    - **Chargement (`loadState`)** :
        1. On lit le `Sensor`.
        2. On sait combien de paquets il avait (`bufferUsage`).
        3. On fait une boucle pour lire ce nombre de paquets et on reconstruit la liste chaînée en refaisant les liens (`malloc` + reconstruction des `next`).

### 3.4 Module Principal (`main.c`)
Point d'entrée et Interface Utilisateur (Menu).
- Offre un menu robuste pour :
    - Initialiser un nouveau capteur.
    - Charger une sauvegarde.
    - Lancer la simulation (Automatique ou Pas-à-pas).
    - Quitter proprement (avec libération mémoire).

## 4. Compilation et Exécution
Le projet utilise un `Makefile` pour simplifier la compilation.

**Commandes :**
1. **Compiler** : Ouvrez un terminal dans le dossier et tapez :
   ```bash
   make
   ```
2. **Exécuter** :
   ```bash
   ./eco_sensing
   ```
3. **Nettoyer** (supprimer les exécutables et objets) :
   ```bash
   make clean
   ```

## 5. Fonctionnement Vidéo (Preuve)
Pour la vidéo demandée :
1. Lancez le programme.
2. Choisissez "New Simulation".
3. Choisissez "Run Single Step" 6 fois de suite.
4. Au 6ème coup, vous verrez le message : `ALERTE : Mémoire saturée...`. C'est la preuve de la gestion mémoire.
5. Vérifiez ensuite le fichier `log.txt`.

Ce projet respecte toutes les contraintes de modularité, de nommage (CamelCase) et de performance.
