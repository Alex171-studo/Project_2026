# Scénario Vidéo (5 minutes max)

Ce document contient le texte exact que tu peux lire pendant que tu enregistres ton écran.
Utilise le script `./demo_video.sh` pour que l'action se déroule automatiquement sous tes yeux.

---

## Introduction (30 secondes)
"Bonjour, nous sommes le groupe Eco-Sensing. Voici la présentation de notre projet de simulateur de réseau de capteurs.
Nous avons développé une solution complète en C, modulaire et optimisée, qui respecte toutes les contraintes du cahier des charges : gestion dynamique de la mémoire, simulation énergétique physique, et persistance binaire des données."

## Partie 1 : Compilation et Architecture (30 secondes)
*(Lance `./demo_video.sh`. Le makefile s'exécute).*

"Comme vous le voyez, le projet compile proprement grâce à un Makefile, sans aucun warning.
Nous avons structuré le code avec des dossiers séparés : `src` pour le code source et `inc` pour les headers, garantissant une modularité exemplaire."

## Partie 2 : La Gestion Mémoire (Le "Crash Test") (2 minutes)
*(Le script lance le programme, initialise X=500, Y=500).*

"Pour démontrer la gestion de la mémoire, nous initialisons un capteur très loin de la base, en position 500, 500.
La distance étant élevée, le coût énergétique dépasse la capacité de la batterie, ce qui force le capteur à stocker les paquets sans pouvoir les envoyer."

*(Le script fait défiler les steps. Le buffer se remplit : 1/5, 2/5...)*

"Vous voyez ici le buffer se remplir progressivement. Chaque paquet est alloué dynamiquement avec `malloc`."

*(Attendre le message "Usage: 5/5")*

"Le buffer est maintenant plein. Voici ce qui se passe si on génère un 6ème paquet :"

*(Le script affiche ALERTE...)*

"**Voilà la preuve.** Le système a détecté la saturation, a identifié le paquet le plus ancien, et a libéré sa mémoire avec `free` avant d'ajouter le nouveau. Aucune fuite mémoire n'est possible."

## Partie 3 : Persistance et Conclusion (1 minute)
*(Le script sauvegarde et quitte).*

"Nous avons également implémenté la sauvegarde binaire. L'état complet du capteur, y compris sa liste chaînée dynamique, a été sérialisé dans `save.bin`.
Nous pouvons recharger cet état à tout moment pour reprendre la simulation."

"En conclusion, Eco-Sensing est un simulateur robuste, qui gère tous les cas limites et respecte scrupuleusement les contraintes physiques et logicielles imposées. Merci."

---
