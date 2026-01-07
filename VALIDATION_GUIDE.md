# Guide de Validation "20/20"

Ce guide t'explique exactement comment utiliser le programme pour démontrer toutes les fonctionnalités requises et obtenir la note maximale.

## 1. Compilation
Ouvre le terminal dans le dossier `Ecosensing` et lance :
```bash
make clean && make
```
*Si aucune erreur ne s'affiche, c'est bon.*

## 2. Démonstration Vidéo (Le "Crash Test" Mémoire)
Le prof veut voir le buffer se remplir et le vieux paquet se faire supprimer (`free`).
Pour que ça arrive, il faut que les paquets ne soient **PAS** envoyés.
**Astuce** : Place le capteur très loin. Le coût d'envoi sera trop élevé par rapport à la batterie, donc l'envoi échouera, et le buffer va se remplir.

**Scénario à filmer :**
1. Lance `./eco_sensing`.
2. **Option 1 (New Simulation)**.
3. Position X : `500.0`. Position Y : `500.0`. (Très loin ! Coût > 100J, donc impossible d'envoyer).
4. Fais **Option 5 (Run Single Step)** 6 fois de suite.
   - Coup 1 à 5 : Tu verras `Packet Generated... Usage: X/5`. Le buffer se remplit. Il dira "Battery low! Cannot transmit".
   - Coup 6 : **BOUM !** Tu verras :
     `ALERTE : Mémoire saturée. Suppression du paquet ID [1] pour libérer de l'espace.`
   👉 **C'est ÇA qu'il faut montrer en vidéo.**

## 3. Démonstration Persistance (Sauvegarde)
1. Fais le scénario ci-dessus (remplis le buffer un peu).
2. **Option 6 (Save State)**. Ça affiche "State saved successfully".
3. **Option 7 (Exit)**.
4. Relance `./eco_sensing`.
5. **Option 2 (Load Previous State)**.
6. **Option 3 (View Sensor Status)**.
   👉 Tu dois retrouver exactement les mêmes paquets et la même batterie qu'avant.

## 4. Crash Test Log automatique
1. Lance une simulation en **Option 4 (Run Simulation - Auto)**.
2. Attend que la batterie meure.
3. Ouvre le fichier `log.txt` (créé automatiquement).
   👉 Il contient ligne par ligne l'évolution.

## Check-list Note Maximale
- [x] **Modularité** : Dossiers `src/` et `inc/` séparés.
- [x] **Pointeurs & Malloc** : Tout est alloué dynamiquement (pas de `tab[100]`).
- [x] **Fuites Mémoire** : `free()` est appelé à chaque suppression.
- [x] **Maths** : Formule exacte avec `sqrt` et `pow` utilisée.
- [x] **Qualité** : Makefile propre, pas de warnings, code commenté en anglais (CamelCase).

**Ton projet est prêt.** 🚀
