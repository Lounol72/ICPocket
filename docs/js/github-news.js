// Configuration
const REPO_OWNER = 'Lounol72';  // Remplacez par votre nom d'utilisateur GitHub
const REPO_NAME = 'ICPocket';   // Remplacez par le nom de votre dépôt
const MAX_RELEASES = 6;         // Nombre de releases à afficher
const MAX_COMMITS = 9;          // Nombre de commits à afficher (changé de 10 à 9)

// Fonction pour formater la date
function formatDate(dateString) {
    const date = new Date(dateString);
    return date.toLocaleDateString('fr-FR', { 
        day: 'numeric', 
        month: 'long', 
        year: 'numeric',
        hour: '2-digit',
        minute: '2-digit'
    });
}

// Récupérer et afficher les releases GitHub
async function fetchReleases() {
    const releasesContainer = document.getElementById('releases-container');
    if (!releasesContainer) return;

    try {
        const response = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/releases`);
        if (!response.ok) {
            throw new Error(`Erreur HTTP: ${response.status}`);
        }
        
        const releases = await response.json();
        
        if (releases.length === 0) {
            releasesContainer.innerHTML = `
                <div class="empty-message">
                    <i class="fas fa-info-circle"></i>
                    <p>Aucune version publiée pour le moment.</p>
                </div>
            `;
            return;
        }
        
        releasesContainer.innerHTML = '';
        
        // Limiter le nombre de releases affichées
        const displayReleases = releases.slice(0, MAX_RELEASES);
        
        displayReleases.forEach(release => {
            const releaseDate = formatDate(release.published_at);
            const assetCount = release.assets.length;
            const downloadCount = release.assets.reduce((total, asset) => total + asset.download_count, 0);
            
            // Extraire la description (prendre les 200 premiers caractères)
            let description = release.body || "Pas de description disponible.";
            if (description.length > 200) {
                description = description.substring(0, 200) + '...';
            }
            
            const releaseCard = document.createElement('div');
            releaseCard.className = 'release-card';
            releaseCard.innerHTML = `
                <div class="release-header">
                    <div class="release-tag">${release.tag_name}</div>
                    <div class="release-date">${releaseDate}</div>
                </div>
                <h3 class="release-title">
                    <a href="${release.html_url}" target="_blank" rel="noopener noreferrer">
                        ${release.name || `Version ${release.tag_name}`}
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
        
    } catch (error) {
        console.error('Erreur lors de la récupération des releases:', error);
        releasesContainer.innerHTML = `
            <div class="error-message">
                <i class="fas fa-exclamation-triangle"></i>
                <p>Impossible de charger les versions depuis GitHub.</p>
                <small>${error.message}</small>
            </div>
        `;
    }
}

// Récupérer et afficher les commits GitHub
async function fetchCommits() {
    const commitsContainer = document.getElementById('commits-container');
    if (!commitsContainer) return;

    try {
        const response = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/commits`);
        if (!response.ok) {
            throw new Error(`Erreur HTTP: ${response.status}`);
        }
        
        const commits = await response.json();
        
        if (commits.length === 0) {
            commitsContainer.innerHTML = `
                <div class="empty-message">
                    <i class="fas fa-info-circle"></i>
                    <p>Aucun commit trouvé dans ce dépôt.</p>
                </div>
            `;
            return;
        }
        
        commitsContainer.innerHTML = '';
        
        // Limiter le nombre de commits affichés
        const displayCommits = commits.slice(0, MAX_COMMITS);
        
        displayCommits.forEach(commit => {
            const commitDate = formatDate(commit.commit.author.date);
            
            // Extraire le message de commit (première ligne uniquement)
            let message = commit.commit.message.split('\n')[0];
            if (message.length > 80) {
                message = message.substring(0, 80) + '...';
            }
            
            const commitCard = document.createElement('div');
            commitCard.className = 'commit-card';
            commitCard.innerHTML = `
                <div class="commit-author">
                    <img src="${commit.author ? commit.author.avatar_url : 'https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png'}" 
                         alt="${commit.commit.author.name}" class="author-avatar">
                    <div class="author-info">
                        <div class="author-name">${commit.commit.author.name}</div>
                        <div class="commit-date">${commitDate}</div>
                    </div>
                </div>
                <div class="commit-message">
                    <a href="${commit.html_url}" target="_blank" rel="noopener noreferrer">
                        ${message}
                    </a>
                </div>
                <div class="commit-sha">
                    <i class="fas fa-code-branch"></i>
                    <code>${commit.sha.substring(0, 7)}</code>
                </div>
            `;
            
            commitsContainer.appendChild(commitCard);
        });
        
    } catch (error) {
        console.error('Erreur lors de la récupération des commits:', error);
        commitsContainer.innerHTML = `
            <div class="error-message">
                <i class="fas fa-exclamation-triangle"></i>
                <p>Impossible de charger les commits depuis GitHub.</p>
                <small>${error.message}</small>
            </div>
        `;
    }
}

// Charger les données au chargement de la page
document.addEventListener('DOMContentLoaded', function() {
    // Vérifier si nous sommes sur la page des actualités
    if (document.getElementById('releases-container') || document.getElementById('commits-container')) {
        fetchReleases();
        fetchCommits();
    }
}); 