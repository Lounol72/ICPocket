FROM ubuntu:latest

# Installer les outils de compilation et les dépendances SDL
RUN apt update && apt install -y \
    build-essential \
    libsdl2-dev \
    libsdl2-ttf-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    check \
    valgrind \
    lcov \
    && rm -rf /var/lib/apt/lists/*

# Définir le répertoire de travail
WORKDIR /app

# Copier le code source dans le conteneur
COPY . .

# Compiler le projet
RUN make

# Compiler et exécuter les tests
CMD make test