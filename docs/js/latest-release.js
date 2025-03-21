// Fonction pour afficher la dernière release en détail
function displayLatestRelease(releases) {
    if (!releases || !Array.isArray(releases) || releases.length === 0) {
        return;
    }
    
    // Récupérer la dernière release (la première du tableau)
    const latestRelease = releases[0];
    
    // Obtenir le conteneur
    const container = document.getElementById('latest-release');
    if (!container) return;
    
    // Cacher l'animation de chargement
    const loadingAnimation = container.querySelector('.loading-animation');
    if (loadingAnimation) {
        loadingAnimation.style.display = 'none';
    }
    
    // Récupérer et afficher le conteneur de détails
    const detailsContainer = container.querySelector('.release-details');
    if (!detailsContainer) return;
    
    // Formater la date
    const releaseDate = new Date(latestRelease.published_at || latestRelease.created_at);
    const formattedDate = releaseDate.toLocaleDateString('fr-FR', {
        year: 'numeric',
        month: 'long',
        day: 'numeric'
    });
    
    // Convertir le texte markdown en HTML (version simplifiée)
    let description = latestRelease.body || "Aucune description disponible.";
    description = description
        .replace(/\r\n|\n/g, '<br>')
        .replace(/\*\*(.*?)\*\*/g, '<strong>$1</strong>')
        .replace(/\*(.*?)\*/g, '<em>$1</em>')
        .replace(/\[(.*?)\]\((.*?)\)/g, '<a href="$2" target="_blank">$1</a>');
    
    // Préparer le HTML pour les assets (fichiers à télécharger)
    let assetsHTML = '';
    if (!latestRelease.assets || latestRelease.assets.length === 0) {
        assetsHTML = `
            <div class="no-assets-message">
                <p>Les fichiers de téléchargement seront disponibles prochainement.</p>
                <a href="${latestRelease.html_url}" class="btn-primary" target="_blank">
                    <i class="fab fa-github"></i> Voir sur GitHub
                </a>
            </div>
        `;
    } else {
        // Code existant pour générer la liste des assets
        assetsHTML = '<div class="release-assets">';
        latestRelease.assets.forEach(asset => {
            assetsHTML += `
                <a href="${asset.browser_download_url}" class="download-asset">
                    <i class="fas fa-download"></i> ${asset.name}
                    <span class="asset-info">
                        <span class="asset-size">${formatFileSize(asset.size)}</span>
                        <span class="download-count"><i class="fas fa-cloud-download-alt"></i> ${asset.download_count}</span>
                    </span>
                </a>
            `;
        });
        assetsHTML += '</div>';
    }
    
    // Construire le HTML complet
    detailsContainer.innerHTML = `
        <div class="release-header">
            <div class="release-title-container">
                <h3 class="release-title">${latestRelease.name || latestRelease.tag_name}</h3>
                <span class="release-tag">${latestRelease.tag_name}</span>
            </div>
            <div class="release-meta">
                <span class="release-date"><i class="far fa-calendar-alt"></i> ${formattedDate}</span>
                <a href="${latestRelease.html_url}" target="_blank" class="view-on-github">
                    <i class="fab fa-github"></i> Voir sur GitHub
                </a>
            </div>
        </div>
        
        <div class="release-body">
            ${description}
        </div>
        
        ${assetsHTML}
    `;
    
    detailsContainer.style.display = 'block';
}

// Fonction utilitaire pour formater la taille des fichiers
function formatFileSize(bytes) {
    if (bytes === 0) return '0 Bytes';
    
    const k = 1024;
    const sizes = ['Bytes', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    
    return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
}

// Écouter l'événement de chargement des releases
document.addEventListener(window.GitHubData.EVENTS.RELEASES_READY, function(e) {
    console.log('Événement RELEASES_READY reçu dans latest-release.js');
    displayLatestRelease(e.detail);
});

// Initialisation au chargement de la page si les données sont déjà disponibles
document.addEventListener('DOMContentLoaded', function() {
    console.log('DOM chargé pour latest-release.js');
    if (window.GitHubData && window.GitHubData.getData) {
        const data = window.GitHubData.getData();
        if (data && data.releases && data.releases.length > 0) {
            console.log('Utilisation des releases déjà chargées');
            displayLatestRelease(data.releases);
        }
    }
}); 