# ICPocket

## Installation

1. Téléchargez l'archive ICPocket.tar.gz
2. Décompressez l'archive :
   ```sh
   tar -xzf ICPocket.tar.gz -C ./ICPocket
   ```
3. Accédez au répertoire :
   ```sh
   cd ICPocket
   ```
4. Lancez le jeu :
   ```sh
   ./bin/main
   ```

## Ajouter un son

Les fichiers audio doivent être au format MP3. Si vous avez un fichier dans un autre format, vous pouvez le convertir en MP3 en utilisant `ffmpeg` :

1. Installez `ffmpeg` si ce n'est pas déjà fait :

    ```sh
    sudo apt-get install ffmpeg
    ```

2. Convertissez le fichier audio :

    ```sh
    ffmpeg -i assets/audio/background_music.mp4 -q:a 0 -map a assets/audio/background_music.mp3
    ```

## Git

### Explications

1. **Commandes Git** :
    - **Naviguer vers le répertoire du projet** :

        ```sh
        cd ~/ICPocket-main
        ```

    - **Récupérer les dernières modifications du dépôt distant** :

        ```sh
        git pull
        ```

    - **Ajouter des fichiers** :

        ```sh
        git add .
        ```

    - **Commit des modifications** :

        ```sh
        git commit -m "Modification qu'apporte le commit"
        ```

    - **Push après le commit** :

        ```sh
        git push
        ```

## Idées pour le jeu

- Faire une table de types avec de nouveaux types (binaire/machine par exemple...)
- Faire trembler la fenêtre du joueur lorsqu'il subit une attaque
- Loup veut une team de waifus (0 blagues)
- Poké custom (ex : une raspberry qui évolue en PC portable qui évolue en PC gamer)
- Pas d'objets en combats
- Nous quatre formons les quatre membres qui donnent chacun un starter au choix

## Table RACI
[Lien vers la table RACI](https://docs.google.com/spreadsheets/d/18GRBhOJjPINtjTH3fe337HzVoUgaLmINAeefcjlDmGY/edit?usp=sharing)

## GANTT
[Lien vers le GANTT](https://docs.google.com/spreadsheets/d/1JiNN4hBVWgQdRoLmsXpOZefp3q4jUFyiuYVRdjpeE_4/edit?gid=1709744959#gid=1709744959)
