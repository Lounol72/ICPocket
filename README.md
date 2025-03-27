# ICPocket

![ICPocket Logo](docs/img/ICPocket.png)

## À propos du projet
ICPocket est un jeu de combat et d'aventure inspiré de Pokémon, développé par des étudiants de l'Université du Mans. Dans cet univers unique où les créatures numériques et la technologie se mêlent harmonieusement, vous incarnez un jeune développeur qui doit relever le défi de devenir le meilleur dresseur d'ICPocket.

## Les membres 

Lounol72 (Subtil Louis)
Natp24019 (Pasquier Nathan)
AstroZr (Borde Alban)
lOkaal1 (Littre William)

**Version actuelle :** 0.9

## 🌐 Site web
Le site web d'ICPocket propose :
- Une page d'accueil présentant le jeu et permettant son téléchargement
- Une page d'actualités suivant l'évolution du développement
- Des informations sur l'équipe de développement
- Une documentation complète
- Un formulaire de contact

## 📁 Structure du projet
- `src/` - Code source du jeu
  - `battle/` - Système de combat
  - `characters/` - Personnages et créatures
  - `world/` - Environnement de jeu
- `include/` - Fichiers d'en-tête
- `assets/` - Ressources graphiques et sonores
  - `sprites/` - Images des personnages et créatures
  - `audio/` - Musiques et effets sonores
  - `maps/` - Cartes du monde
- `bin/` - Exécutables et fichiers compilés
- `docs/` - Site web et documentation technique

## 🔧 Technologies utilisées
- HTML5/CSS3 pour l'interface web
- JavaScript pour les fonctionnalités interactives
- Intégration avec l'API GitHub pour les actualités du développement

## 🚀 Comment contribuer
1. Consultez notre [GitHub](https://github.com/Lounol72/ICPocket)
2. Explorez la [documentation](https://lounol72.github.io/ICPocket/html/index.html)
3. Contactez l'équipe via notre [formulaire de contact](https://lounol72.github.io/ICPocket/contact.html)

## 👥 Équipe
Développé par une équipe d'étudiants passionnés de l'Université du Mans. Découvrez notre équipe [ici](https://lounol72.github.io/ICPocket/team.html).

## Table RACI
[Lien vers la table RACI](https://docs.google.com/spreadsheets/d/18GRBhOJjPINtjTH3fe337HzVoUgaLmINAeefcjlDmGY/edit?usp=sharing)

## GANTT
[Lien vers le GANTT](https://docs.google.com/spreadsheets/d/1JiNN4hBVWgQdRoLmsXpOZefp3q4jUFyiuYVRdjpeE_4/edit?gid=1709744959#gid=1709744959)

## GANTT Prévisionnel
[Lien vers le GANTT](https://docs.google.com/spreadsheets/d/1AqGpYmEBtP7KtYeO5m7FdebBRN59XQRUxrBPlVJ7CM0/edit?gid=1709744959#gid=1709744959)

## Tester docker

Allumer le serveur docker
```sh
  sudo service docker start
```
Build le docker de test
```sh
  docker build -t icpocket-app .
```
Run l'image docker
```sh
  docker run icpocket-app
```
Supprimer les images docker
```sh
  docker system prune -a
  docker images
```
Stopper le serveur docker
```sh
  sudo service docker stop
```


## 📄 Licence
© 2024 ICPocket. Tous droits réservés.
