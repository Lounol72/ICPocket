/**
 * Gestionnaire de données GitHub - Charge les données depuis les fichiers JSON statiques
 */

// Configuration
const DATA_PATH = 'data/';
const DATA_FILES = {
    repo: 'repo.json',
    contributors: 'contributors.json',
    releases: 'releases.json',
    commits: 'commits.json',
    metadata: 'metadata.json'
};
const MAX_RELEASES = 6;
const MAX_COMMITS = 9;

// Structure pour stocker toutes les données GitHub
let gitHubData = {
    repo: null,
    contributors: [],
    releases: [],
    commits: [],
    lastUpdated: null
};

// Événements personnalisés pour notifier les composants quand les données sont prêtes
const EVENTS = {
    STATS_READY: 'github-stats-ready',
    CONTRIBUTORS_READY: 'github-contributors-ready',
    RELEASES_READY: 'github-releases-ready',
    COMMITS_READY: 'github-commits-ready',
    ALL_DATA_READY: 'github-all-data-ready',
    ERROR: 'github-api-error'
};

// Fonction principale pour charger toutes les données GitHub
async function loadAllGitHubData() {
    try {
        // Récupérer les métadonnées pour afficher quand les données ont été mises à jour
        const metadataResponse = await fetch(`${DATA_PATH}${DATA_FILES.metadata}`);
        const metadata = await metadataResponse.json();
        gitHubData.lastUpdated = new Date(metadata.last_updated);
        
        // Exécuter tous les chargements en parallèle
        const [repoData, contributorsData, releasesData, commitsData] = await Promise.all([
            loadRepoData(),
            loadContributorsData(),
            loadReleasesData(),
            loadCommitsData()
        ]);

        // Stocker les résultats
        gitHubData.repo = repoData;
        gitHubData.contributors = contributorsData;
        gitHubData.releases = releasesData;
        gitHubData.commits = commitsData.slice(0, MAX_COMMITS);
        
        // Notifier que toutes les données sont prêtes
        notifyAllDataReady();
        
        return gitHubData;
    } catch (error) {
        console.error('Erreur lors du chargement des données GitHub:', error);
        document.dispatchEvent(new CustomEvent(EVENTS.ERROR, { detail: error }));
        return null;
    }
}

// Fonctions individuelles pour charger chaque type de donnée
async function loadRepoData() {
    const response = await fetch(`${DATA_PATH}${DATA_FILES.repo}`);
    return await response.json();
}

async function loadContributorsData() {
    const response = await fetch(`${DATA_PATH}${DATA_FILES.contributors}`);
    return await response.json();
}

async function loadReleasesData() {
    const response = await fetch(`${DATA_PATH}${DATA_FILES.releases}`);
    return await response.json();
}

async function loadCommitsData() {
    const response = await fetch(`${DATA_PATH}${DATA_FILES.commits}`);
    return await response.json();
}

// Fonctions pour notifier les composants
function notifyAllDataReady() {
    // Calculer le nombre de téléchargements
    const totalDownloads = gitHubData.releases.reduce((total, release) => {
        return total + release.assets.reduce((subTotal, asset) => subTotal + asset.download_count, 0);
    }, 0);
    
    // Notifier pour les statistiques
    document.dispatchEvent(new CustomEvent(EVENTS.STATS_READY, { 
        detail: { 
            stars: gitHubData.repo?.stargazers_count || 0,
            forks: gitHubData.repo?.forks_count || 0,
            contributors: gitHubData.contributors?.length || 0,
            downloads: totalDownloads
        } 
    }));
    
    // Notifier pour les contributeurs
    document.dispatchEvent(new CustomEvent(EVENTS.CONTRIBUTORS_READY, { 
        detail: gitHubData.contributors 
    }));
    
    // Notifier pour les releases
    document.dispatchEvent(new CustomEvent(EVENTS.RELEASES_READY, { 
        detail: gitHubData.releases.slice(0, MAX_RELEASES) 
    }));
    
    // Notifier pour les commits
    document.dispatchEvent(new CustomEvent(EVENTS.COMMITS_READY, { 
        detail: gitHubData.commits 
    }));
    
    // Notifier que tout est prêt
    document.dispatchEvent(new CustomEvent(EVENTS.ALL_DATA_READY, { 
        detail: gitHubData 
    }));
    
    // Mettre à jour l'indicateur de dernière mise à jour
    updateLastUpdatedIndicator();
}

// Mettre à jour l'indicateur de dernière mise à jour des données
function updateLastUpdatedIndicator() {
    const indicators = document.querySelectorAll('.data-last-updated');
    if (indicators.length > 0 && gitHubData.lastUpdated) {
        const formattedDate = gitHubData.lastUpdated.toLocaleDateString('fr-FR', {
            day: 'numeric',
            month: 'long',
            year: 'numeric',
            hour: '2-digit',
            minute: '2-digit'
        });
        
        indicators.forEach(indicator => {
            indicator.textContent = `Données mises à jour le ${formattedDate}`;
        });
    }
}

// Exporter les fonctions et constantes
window.GitHubData = {
    load: loadAllGitHubData,
    EVENTS: EVENTS
};

// Charger les données au démarrage
document.addEventListener('DOMContentLoaded', function() {
    // Ne charger les données que si un des composants qui les utilise est présent
    const needsGitHubData = document.querySelector('[data-stat]') || 
                           document.querySelector('.team-grid') ||
                           document.querySelector('#releases-container') ||
                           document.querySelector('#commits-container') ||
                           document.querySelector('.latest-news');
                           
    if (needsGitHubData) {
        loadAllGitHubData();
    }
}); 