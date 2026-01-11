# RAPPORT DE PROJET - ECO-SENSING
**Simulateur de Réseau de Capteurs IoT**

---

## 1. DIFFICULTÉS RENCONTRÉES

### Gestion de la Mémoire Dynamique
La principale difficulté a été l'implémentation correcte de la liste chaînée avec allocation dynamique. Il fallait s'assurer qu'aucune fuite mémoire ne se produise, notamment lors de :
- La suppression FIFO du paquet le plus ancien quand le buffer est saturé
- La transmission des paquets vers la station de base
- La libération complète du capteur à la fin de la simulation

**Solution** : Utilisation systématique de `free()` pour chaque `malloc()`, avec un parcours complet de la liste chaînée lors de la libération du capteur. Validation avec Valgrind pour garantir zéro fuite mémoire.

### Persistance Binaire avec Pointeurs
Sauvegarder une liste chaînée en binaire pose problème car les pointeurs (`suivant`) contiennent des adresses mémoire qui changent à chaque exécution.

**Solution** : Sauvegarde séquentielle des paquets sans les pointeurs, puis reconstruction de la liste chaînée lors du chargement en recréant les liens `suivant` dynamiquement.

### Compréhension du Modèle Énergétique
La formule **E = 0.05 + 0.01 × d²** génère des coûts énergétiques très élevés pour des distances importantes. Par exemple, à position (500, 500), le coût est de **5000.05 J**, ce qui dépasse largement la batterie initiale de 100J.

**Solution** : Implémentation d'un système de messages informatifs qui explique clairement pourquoi la transmission échoue (énergie requise vs disponible), évitant toute confusion sur le comportement du simulateur.

---

## 2. CHOIX TECHNIQUES

### Architecture Modulaire
- **Séparation `src/` et `inc/`** : Headers et sources dans des dossiers distincts pour une meilleure organisation
- **Modules indépendants** : 
  - `sensor.c` : Gestion de la mémoire et génération de paquets
  - `simulation.c` : Physique énergétique et transmission
  - `persistence.c` : Sauvegarde/chargement binaire
  - `main.c` : Interface utilisateur interactive

### Nomenclature Française
Respect strict des exigences du cahier des charges : `Capteur`, `Paquet`, `batterie`, `valeur`, `suivant`, `tete`, etc.

### Formule Physique Exacte
Implémentation rigoureuse : **E = E_ELEC + E_AMP × d²**
- `E_ELEC = 0.05 J` (énergie électronique de base)
- `E_AMP = 0.01 J/m²` (amplification selon distance)
- Utilisation de `distance_squared = x² + y²` pour éviter `sqrt()` (optimisation)
- Vérification de la batterie **avant** chaque tentative de transmission

### Messages Informatifs Améliorés
- 📦 Emojis pour identifier rapidement le type de message
- ⚠️ Affichage des valeurs exactes (énergie requise vs disponible)
- ✓ Confirmation visuelle des transmissions réussies
- 🔴 Alertes claires lors de la saturation mémoire

### Makefile Robuste
- Flags `-Wall -Wextra` pour détecter tous les warnings
- Liaison mathématique `-lm` pour les fonctions mathématiques
- Règle `clean` pour nettoyer les fichiers générés
- Compilation modulaire avec fichiers objets séparés

---

## 3. SOLUTIONS APPORTÉES

| Problème | Solution Implémentée |
|----------|---------------------|
| **Buffer saturé** | Suppression FIFO du paquet le plus ancien avec message d'alerte visuel 🔴 |
| **Fuites mémoire** | `free()` systématique à chaque suppression/transmission + validation Valgrind |
| **Persistance** | Sauvegarde binaire avec reconstruction de la liste chaînée au chargement |
| **Simulation réaliste** | Un paquet généré et tentative de transmission à chaque étape |
| **Traçabilité** | Fichier `log.txt` avec état complet à chaque étape (temps, batterie, paquets) |
| **Clarté du comportement** | Messages détaillés montrant coût énergétique et raison des échecs |
| **Confusion utilisateur** | Affichage au démarrage du coût énergétique et prédiction du comportement |

---

## 4. COMPORTEMENT DU SIMULATEUR

### Cas 1 : Position Proche (Transmission Réussie)
**Exemple** : Position (10, 10)
- Coût énergétique : 0.05 + 0.01 × (100 + 100) = **2.05 J**
- Batterie initiale : 100 J
- **Résultat** : ~48 transmissions possibles avant épuisement
- Les paquets sont transmis immédiatement, le buffer reste vide

### Cas 2 : Position Lointaine (Stockage en Mémoire)
**Exemple** : Position (500, 500)
- Coût énergétique : 0.05 + 0.01 × (250000 + 250000) = **5000.05 J**
- Batterie initiale : 100 J
- **Résultat** : ⚠️ Transmission impossible (énergie insuffisante)
- Les paquets sont **stockés en mémoire** dans le buffer
- Quand le buffer atteint 5/5, le système applique **FIFO** (suppression du plus ancien)

> **Note Importante** : Ce comportement est **correct et attendu**. Le simulateur respecte fidèlement la physique énergétique. Les paquets ne sont pas "perdus" mais stockés en attente d'une transmission future (qui n'arrivera jamais si l'énergie reste insuffisante).

---

## 5. DÉMONSTRATION

### Scénario "Crash Test Mémoire"
```bash
./eco_sensing
# 1. Nouvelle Simulation
# Position X: 500.0, Y: 500.0 (distance élevée)
# 5. Exécuter Une Étape (répéter 6 fois)
# → Message "🔴 ALERTE: Mémoire saturée..." apparaît au 6ème paquet
```

### Vérification Complète
- **Compilation** : `make` → 0 erreurs, 0 warnings
- **Mémoire** : `valgrind ./eco_sensing` → 0 fuites
- **Log** : `cat log.txt` → Évolution de la batterie et du buffer
- **Sauvegarde** : `save.bin` créé et rechargeable sans perte de données

---

## 6. LIENS

### 📹 Vidéo de Démonstration
**YouTube** : [Lien à ajouter après création du repository]



### 💻 Code Source
**GitHub** : [https://github.com/Alex171-studo/Project_2026]


