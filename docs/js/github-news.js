// Configuration
const REPO_OWNER = 'Lounol72';  // Remplacez par votre nom d'utilisateur GitHub
const REPO_NAME = 'ICPocket';   // Remplacez par le nom de votre dépôt
//const MAX_COMMITS = 9;          // Nombre de commits à afficher (changé de 10 à 9)

// Au début du fichier github-news.js
console.log('github-news.js chargé');

// Fonction pour formater la date
function formatDate(dateString) {
    try {
        const date = new Date(dateString);
        return date.toLocaleDateString('fr-FR', { 
            day: 'numeric', 
            month: 'long', 
            year: 'numeric',
            hour: '2-digit',
            minute: '2-digit'
        });
    } catch (e) {
        console.error('Erreur de formatage de date:', e);
        return dateString || 'Date inconnue';
    }
}

// Fonction de debug pour vérifier le contenu des fichiers JSON
function debugJsonContent() {
    console.log("Débogage du contenu JSON");
    fetch('./data/releases.json')
        .then(response => {
            console.log("Status response releases:", response.status);
            return response.json();
        })
        .then(data => {
            console.log("Contenu du fichier releases.json:", data);
        })
        .catch(error => {
            console.error("Erreur lors du chargement de releases.json:", error);
        });
        
    fetch('./data/commits.json')
        .then(response => {
            console.log("Status response commits:", response.status);
            return response.json();
        })
        .then(data => {
            console.log("Contenu du fichier commits.json:", data);
        })
        .catch(error => {
            console.error("Erreur lors du chargement de commits.json:", error);
        });
}

// Fonction pour afficher les releases avec délai d'animation
function displayReleases(releases) {
    console.log('displayReleases appelé avec:', releases);
    const container = document.getElementById('releases-container');
    
    // Si pas de container ou pas de releases, sortir
    if (!container || !releases || !Array.isArray(releases) || releases.length === 0) {
        container.innerHTML = '<div class="empty-message">Aucune version disponible pour le moment.</div>';
        return;
    }
    
    // Vider le conteneur des releases
    container.innerHTML = '';
    
    // Parcourir les releases et créer les éléments HTML avec délai d'animation
    releases.forEach((release, index) => {
        const releaseDate = new Date(release.published_at || release.created_at);
        const formattedDate = releaseDate.toLocaleDateString('fr-FR');
        
        const releaseElement = document.createElement('div');
        releaseElement.className = 'news-card';
        releaseElement.style.animationDelay = `${index * 0.1}s`;
        releaseElement.innerHTML = `
            <div class="news-header">
                <h3 class="news-title">${release.name || release.tag_name}</h3>
                <div class="news-date">${formattedDate}</div>
            </div>
            <div class="news-content">
                <p>${release.body ? release.body.substring(0, 150) + (release.body.length > 150 ? '...' : '') : 'Pas de description disponible'}</p>
            </div>
            <div class="news-footer">
                <a href="${release.html_url}" target="_blank" class="btn-small"><i class="fas fa-external-link-alt"></i> Détails</a>
                ${release.assets && release.assets.length > 0 
                    ? `<a href="${release.assets[0].browser_download_url}" class="btn-download"><i class="fas fa-download"></i> Télécharger</a>` 
                    : ''}
            </div>
        `;
        
        container.appendChild(releaseElement);
    });
}

// Fonction pour afficher les commits
function displayCommits(commits) {
    if (!commits || !Array.isArray(commits) || commits.length === 0) {
        console.error('Aucun commit à afficher');
        return;
    }
    
    // Filtrer uniquement les commits "Update GitHub data"
    const filteredCommits = commits.filter(commit => 
        !(commit.author.username === 'github-actions[bot]' && 
          commit.message.includes('Update GitHub data'))
    );
    
    console.log(`Affichage de ${filteredCommits.length} commits (${commits.length - filteredCommits.length} commits automatiques filtrés)`);
    
    // Obtenir le conteneur
    const container = document.getElementById('commits-container');
    if (!container) {
        console.error('Conteneur de commits non trouvé');
        return;
    }
    
    // Effacer les éventuels skeletons ou animations de chargement
    container.innerHTML = '';
    
    // Si aucun commit après filtrage, afficher un message
    if (filteredCommits.length === 0) {
        container.innerHTML = '<div class="no-commits">Aucun commit significatif récent</div>';
        return;
    }
    
    // Créer un élément pour chaque commit
    filteredCommits.forEach(commit => {
        const commitElement = document.createElement('div');
        commitElement.className = 'news-card';
        
        // Formater la date
        const commitDate = new Date(commit.date);
        const formattedDate = commitDate.toLocaleDateString('fr-FR', {
            year: 'numeric',
            month: 'long',
            day: 'numeric'
        });
        
        commitElement.innerHTML = `
            <div class="news-header">
                <a href="${commit.url}" target="_blank" class="news-title">${commit.sha}</a>
                <span class="news-date">${formattedDate}</span>
            </div>
            <div class="news-content">${commit.message}</div>
            <div class="news-author">par ${commit.author.name || commit.author.username}</div>
        `;
        
        container.appendChild(commitElement);
    });
}

// Afficher une erreur
function displayError(container, message) {
    if (!container) return;
    
    console.error('Erreur news:', message);
    
    container.innerHTML = `
        <div class="error-message">
            <i class="fas fa-exclamation-triangle"></i>
            <p>Impossible de charger les données depuis GitHub.</p>
            <small>${message}</small>
        </div>
    `;
}

// Écouteurs d'événements pour les données GitHub
document.addEventListener(window.GitHubData.EVENTS.RELEASES_READY, function(e) {
    console.log('Événement RELEASES_READY reçu dans github-news.js');
    displayReleases(e.detail);
});

document.addEventListener(window.GitHubData.EVENTS.COMMITS_READY, function(e) {
    console.log('Événement COMMITS_READY reçu');
    displayCommits(e.detail);
});

// Ajouter aussi un écouteur pour les erreurs
document.addEventListener(window.GitHubData.EVENTS.ERROR, function(e) {
    console.error('Erreur lors du chargement des données:', e.detail);
    const releasesContainer = document.getElementById('releases-container');
    const commitsContainer = document.getElementById('commits-container');
    
    if (releasesContainer) {
        releasesContainer.innerHTML = `
            <div class="error-message">
                <i class="fas fa-exclamation-triangle"></i>
                <p>Erreur lors du chargement des données. Veuillez réessayer plus tard.</p>
            </div>
        `;
    }
    
    if (commitsContainer) {
        commitsContainer.innerHTML = `
            <div class="error-message">
                <i class="fas fa-exclamation-triangle"></i>
                <p>Erreur lors du chargement des données. Veuillez réessayer plus tard.</p>
            </div>
        `;
    }
});

// Initialisation au chargement de la page
document.addEventListener('DOMContentLoaded', function() {
    // Vérifier si les données sont déjà disponibles
    if (window.GitHubData && window.GitHubData.getData) {
        const data = window.GitHubData.getData();
        if (data && data.commits && data.commits.length > 0) {
            console.log('Utilisation des commits déjà chargés');
            displayCommits(data.commits);
        }
    }
});

// Appeler la fonction au chargement de la page
document.addEventListener('DOMContentLoaded', debugJsonContent); 