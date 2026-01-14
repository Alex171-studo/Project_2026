# RAPPORT DE PROJET

## 1. DIFFICULTÉS RENCONTRÉES

### Gestion de la Mémoire Dynamique
La principale difficulté a été l'implémentation correcte de la liste chaînée avec allocation dynamique. Il fallait s'assurer qu'aucune fuite mémoire ne se produise, notamment lors de :
- La suppression FIFO du paquet le plus ancien quand le buffer est saturé
- La transmission des paquets vers la station de base
- La libération complète du capteur à la fin de la simulation

**Solution** : Utilisation systématique de `free()` pour chaque `malloc()`, avec un parcours complet de la liste chaînée lors de la libération du capteur.

### Persistance Binaire avec Pointeurs
Sauvegarder une liste chaînée en binaire pose problème car les pointeurs (`suivant`) contiennent des adresses mémoire qui changent à chaque exécution.

**Solution** : Sauvegarde séquentielle des paquets sans les pointeurs, puis reconstruction de la liste chaînée lors du chargement en recréant les liens `suivant` dynamiquement.

### Compréhension du Modèle Énergétique
La formule **E = 0.05 + 0.01 × d²** génère des coûts énergétiques très élevés pour des distances importantes. Par exemple, à position (500, 500), le coût est de **5000.05 J**, ce qui dépasse largement la batterie initiale de 100J.

**Solution** : Implémentation d'un système de messages informatifs qui explique clairement pourquoi la transmission échoue (énergie requise vs disponible), évitant toute confusion sur le comportement du simulateur. Un **arrêt de sécurité** a également été ajouté pour stopper la simulation automatique si elle s'avère physiquement impossible, prévenant toute boucle infinie.

### Synchronisation des Identifiants
Lors du chargement d'un état sauvegardé, le compteur global d'ID pouvait entrer en conflit avec les anciens paquets.

**Solution** : Ajout d'une fonction `fixer_id_compteur()` qui synchronise automatiquement le générateur d'ID avec le plus grand ID chargé en mémoire.

---

## 2. CHOIX TECHNIQUES

### Architecture Modulaire
- **Séparation `src/` et `inc/`** : Headers et sources dans des dossiers distincts pour une meilleure organisation
- **Modules indépendants** : 
  - `sensor.c` : Gestion de la mémoire et génération de paquets
  - `simulation.c` : Physique énergétique et transmission
  - `persistence.c` : Sauvegarde/chargement binaire
  - `main.c` : Interface utilisateur interactive

## 3. LIENS

### 📹 Vidéo de Démonstration
**YouTube** : https://youtu.be/q0B8dfpGDYs



### 💻 Code Source
**GitHub** : https://github.com/Alex171-studo/Project_2026


