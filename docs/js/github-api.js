/**
 * Gestionnaire central d'API GitHub - Effectue un seul appel pour obtenir toutes les données
 * et les distribue aux différentes parties du site.
 */

// Configuration
const REPO_OWNER = 'Lounol72';
const REPO_NAME = 'ICPocket';
const CACHE_DURATION = 60 * 60 * 1000; // 1 heure en millisecondes
const MAX_RELEASES = 6;
const MAX_COMMITS = 9;

// Structure pour stocker toutes les données GitHub
let gitHubData = {
    repo: null,
    contributors: [],
    releases: [],
    commits: [],
    lastUpdated: 0
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

// Fonction principale pour récupérer toutes les données GitHub
async function fetchAllGitHubData() {
    // Vérifier si les données en cache sont encore valides
    const cachedData = loadFromCache();
    if (cachedData) {
        gitHubData = cachedData;
        notifyAllDataReady();
        return gitHubData;
    }

    // Initialiser l'objet pour stocker les résultats
    gitHubData = {
        repo: null,
        contributors: [],
        releases: [],
        commits: [],
        lastUpdated: Date.now()
    };

    try {
        // Exécuter toutes les requêtes en parallèle
        const [repoData, contributorsData, releasesData, commitsData] = await Promise.all([
            fetchRepo(),
            fetchContributors(),
            fetchReleases(),
            fetchCommits()
        ]);

        // Stocker les résultats
        gitHubData.repo = repoData;
        gitHubData.contributors = contributorsData;
        gitHubData.releases = releasesData;
        gitHubData.commits = commitsData;
        
        // Enregistrer dans le cache
        saveToCache(gitHubData);
        
        // Notifier que toutes les données sont prêtes
        notifyAllDataReady();
        
        return gitHubData;
    } catch (error) {
        console.error('Erreur lors de la récupération des données GitHub:', error);
        document.dispatchEvent(new CustomEvent(EVENTS.ERROR, { detail: error }));
        return null;
    }
}

// Fonctions individuelles pour chaque type de donnée
async function fetchRepo() {
    const response = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}`);
    if (!response.ok) throw new Error(`Erreur HTTP: ${response.status}`);
    return await response.json();
}

async function fetchContributors() {
    const response = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/contributors`);
    if (!response.ok) throw new Error(`Erreur HTTP: ${response.status}`);
    return await response.json();
}

async function fetchReleases() {
    const response = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/releases`);
    if (!response.ok) throw new Error(`Erreur HTTP: ${response.status}`);
    return await response.json();
}

async function fetchCommits() {
    const response = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/commits`);
    if (!response.ok) throw new Error(`Erreur HTTP: ${response.status}`);
    const commits = await response.json();
    return commits.slice(0, MAX_COMMITS); // Limiter le nombre de commits
}

// Fonctions pour notifier les composants
function notifyAllDataReady() {
    // Notifier pour chaque type de données
    document.dispatchEvent(new CustomEvent(EVENTS.STATS_READY, { 
        detail: { 
            stars: gitHubData.repo?.stargazers_count || 0,
            forks: gitHubData.repo?.forks_count || 0,
            contributors: gitHubData.contributors?.length || 0,
            downloads: calculateTotalDownloads()
        } 
    }));
    
    document.dispatchEvent(new CustomEvent(EVENTS.CONTRIBUTORS_READY, { 
        detail: gitHubData.contributors 
    }));
    
    document.dispatchEvent(new CustomEvent(EVENTS.RELEASES_READY, { 
        detail: gitHubData.releases.slice(0, MAX_RELEASES) 
    }));
    
    document.dispatchEvent(new CustomEvent(EVENTS.COMMITS_READY, { 
        detail: gitHubData.commits 
    }));
    
    // Notifier que tout est prêt
    document.dispatchEvent(new CustomEvent(EVENTS.ALL_DATA_READY, { 
        detail: gitHubData 
    }));
}

// Calculer le nombre total de téléchargements
function calculateTotalDownloads() {
    return gitHubData.releases.reduce((total, release) => {
        return total + release.assets.reduce((assetTotal, asset) => {
            return assetTotal + asset.download_count;
        }, 0);
    }, 0);
}

// Fonctions pour gérer le cache
function saveToCache(data) {
    try {
        localStorage.setItem('github-data', JSON.stringify(data));
    } catch (e) {
        console.warn('Impossible de sauvegarder les données dans localStorage:', e);
    }
}

function loadFromCache() {
    try {
        const cachedData = JSON.parse(localStorage.getItem('github-data'));
        if (cachedData && (Date.now() - cachedData.lastUpdated < CACHE_DURATION)) {
            return cachedData;
        }
    } catch (e) {
        console.warn('Erreur lors de la lecture du cache:', e);
    }
    return null;
}

// Fonction pour obtenir les données (soit du cache, soit par un nouvel appel API)
function getGitHubData() {
    return gitHubData.lastUpdated > 0 ? gitHubData : fetchAllGitHubData();
}

// Exporter les fonctions et constantes
window.GitHubAPI = {
    fetch: fetchAllGitHubData,
    getData: getGitHubData,
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
        fetchAllGitHubData();
    }
}); 