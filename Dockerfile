# Utiliser Ubuntu comme image de base
FROM ubuntu:latest

# Éviter les interactions pendant l'installation
ENV DEBIAN_FRONTEND=noninteractive
ENV TERM=xterm

# Installer les dépendances nécessaires
RUN apt-get update && \
    apt-get install -y \
    gcc \
    make \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev \
    && rm -rf /var/lib/apt/lists/*

# Créer un répertoire de travail
WORKDIR /app

# Copier tous les fichiers nécessaires
COPY . .

# Compiler le projet
RUN make

# Commande par défaut pour exécuter le programme
CMD ["./bin/main"]
