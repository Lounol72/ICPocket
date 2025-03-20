// Configuration
const REPO_OWNER = 'Lounol72';  // Remplacez par votre nom d'utilisateur GitHub
const REPO_NAME = 'ICPocket';   // Remplacez par le nom de votre dépôt
const MAX_RELEASES = 6;         // Nombre de releases à afficher
const MAX_COMMITS = 9;          // Nombre de commits à afficher (changé de 10 à 9)

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

// Afficher les releases GitHub
function displayReleases(releases) {
    const releasesContainer = document.getElementById('releases-container');
    if (!releasesContainer) return;

    if (!releases || !Array.isArray(releases) || releases.length === 0) {
        releasesContainer.innerHTML = `
            <div class="empty-message">
                <i class="fas fa-info-circle"></i>
                <p>Aucune version publiée pour le moment.</p>
            </div>
        `;
        return;
    }
    
    console.log(`Affichage de ${releases.length} releases`);
    releasesContainer.innerHTML = '';
    
    releases.forEach(release => {
        if (!release) return;
        
        const releaseDate = formatDate(release.published_at);
        const assetCount = release.assets ? release.assets.length : 0;
        const downloadCount = release.assets ? release.assets.reduce((total, asset) => {
            return total + (asset.download_count || 0);
        }, 0) : 0;
        
        // Extraire la description (prendre les 200 premiers caractères)
        let description = release.body || "Pas de description disponible.";
        if (description.length > 200) {
            description = description.substring(0, 200) + '...';
        }
        
        const releaseCard = document.createElement('div');
        releaseCard.className = 'release-card';
        releaseCard.innerHTML = `
            <div class="release-header">
                <div class="release-tag">${release.tag_name || 'v0.0.0'}</div>
                <div class="release-date">${releaseDate}</div>
            </div>
            <h3 class="release-title">
                <a href="${release.html_url}" target="_blank" rel="noopener noreferrer">
                    ${release.name || `Version ${release.tag_name || '?'}`}
                </a>
            </h3>
            <div class="release-description markdown-body">
                ${description.replace(/\n/g, '<br>')}
            </div>
            <div class="release-stats">
                <div class="release-stat">
                    <i class="fas fa-download"></i> ${downloadCount} téléchargements
                </div>
                <div class="release-stat">
                    <i class="fas fa-file-archive"></i> ${assetCount} fichiers
                </div>
            </div>
            <a href="${release.html_url}" class="view-release-btn" target="_blank" rel="noopener noreferrer">
                Voir les détails <i class="fas fa-external-link-alt"></i>
            </a>
        `;
        
        releasesContainer.appendChild(releaseCard);
    });
}

// Afficher les commits GitHub
function displayCommits(commits) {
    const commitsContainer = document.getElementById('commits-container');
    if (!commitsContainer) return;

    if (!commits || !Array.isArray(commits) || commits.length === 0) {
        commitsContainer.innerHTML = `
            <div class="empty-message">
                <i class="fas fa-info-circle"></i>
                <p>Aucun commit trouvé dans ce dépôt.</p>
            </div>
        `;
        return;
    }
    
    console.log(`Affichage de ${commits.length} commits`);
    commitsContainer.innerHTML = '';
    
    commits.forEach(commit => {
        if (!commit || !commit.commit) return;
        
        const commitDate = formatDate(commit.commit.author?.date);
        const authorName = commit.commit.author?.name || 'Auteur inconnu';
        
        // Extraire le message de commit (première ligne uniquement)
        let message = (commit.commit.message || 'No message').split('\n')[0];
        if (message.length > 80) {
            message = message.substring(0, 80) + '...';
        }
        
        const commitCard = document.createElement('div');
        commitCard.className = 'commit-card';
        commitCard.innerHTML = `
            <div class="commit-author">
                <img src="${commit.author?.avatar_url || 'https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png'}" 
                     alt="${authorName}" class="author-avatar">
                <div class="author-info">
                    <div class="author-name">${authorName}</div>
                    <div class="commit-date">${commitDate}</div>
                </div>
            </div>
            <div class="commit-message">
                <a href="${commit.html_url || '#'}" target="_blank" rel="noopener noreferrer">
                    ${message}
                </a>
            </div>
            <div class="commit-sha">
                <i class="fas fa-code-branch"></i>
                <code>${commit.sha ? commit.sha.substring(0, 7) : '???????'}</code>
            </div>
        `;
        
        commitsContainer.appendChild(commitCard);
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
    console.log('Événement RELEASES_READY reçu');
    displayReleases(e.detail);
});

document.addEventListener(window.GitHubData.EVENTS.COMMITS_READY, function(e) {
    console.log('Événement COMMITS_READY reçu');
    displayCommits(e.detail);
});

document.addEventListener(window.GitHubData.EVENTS.ERROR, function(e) {
    console.log('Événement ERROR reçu');
    const releasesContainer = document.getElementById('releases-container');
    const commitsContainer = document.getElementById('commits-container');
    
    if (releasesContainer) {
        displayError(releasesContainer, e.detail ? e.detail.message : 'Erreur inconnue');
    }
    
    if (commitsContainer) {
        displayError(commitsContainer, e.detail ? e.detail.message : 'Erreur inconnue');
    }
});

// Initialisation au chargement de la page si les données sont déjà disponibles
document.addEventListener('DOMContentLoaded', function() {
    console.log('DOM chargé pour github-news.js');
    if (window.GitHubData && window.GitHubData.getData) {
        const data = window.GitHubData.getData();
        if (data) {
            if (data.releases && data.releases.length > 0) {
                console.log('Utilisation des releases déjà chargées');
                displayReleases(data.releases);
            }
            if (data.commits && data.commits.length > 0) {
                console.log('Utilisation des commits déjà chargés');
                displayCommits(data.commits);
            }
        }
    }
}); 