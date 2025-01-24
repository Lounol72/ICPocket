# ICPocket

## Prérequis

Avant de lancer `make`, assurez-vous d'avoir installé toutes les bibliothèques nécessaires :

```sh
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
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

## [Table RACI](https://docs.google.com/spreadsheets/d/18GRBhOJjPINtjTH3fe337HzVoUgaLmINAeefcjlDmGY/edit?usp=sharing)

## [GANTT](https://docs.google.com/spreadsheets/d/1JiNN4hBVWgQdRoLmsXpOZefp3q4jUFyiuYVRdjpeE_4/edit?gid=1709744959#gid=1709744959)
