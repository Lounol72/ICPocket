// Fichier pour récupérer et afficher les statistiques GitHub
const REPO_OWNER = 'Lounol72';
const REPO_NAME = 'ICPocket';

// Fonction pour formater les nombres avec séparateur de milliers
function formatNumber(num) {
    if (typeof num !== 'number') {
        num = parseInt(num) || 0;
    }
    
    if (num >= 1000000) {
        return (num / 1000000).toFixed(1) + 'M';
    }
    if (num >= 1000) {
        return (num / 1000).toFixed(1) + 'K';
    }
    return num.toString();
}

// Récupérer les statistiques GitHub
async function fetchGitHubStats() {
    try {
        // Récupérer les informations du dépôt
        const repoResponse = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}`);
        const repoData = await repoResponse.json();
        
        // Récupérer les contributeurs
        const contributorsResponse = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/contributors`);
        const contributorsData = await contributorsResponse.json();
        
        // Récupérer les releases pour obtenir les téléchargements
        const releasesResponse = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/releases`);
        const releasesData = await releasesResponse.json();
        
        // Calculer le nombre total de téléchargements
        let totalDownloads = 0;
        releasesData.forEach(release => {
            if (release.assets) {
                release.assets.forEach(asset => {
                    totalDownloads += asset.download_count;
                });
            }
        });
        
        // Mettre à jour les statistiques sur la page
        updateStats({
            stars: repoData.stargazers_count,
            forks: repoData.forks_count,
            contributors: contributorsData.length,
            downloads: totalDownloads
        });
        
    } catch (error) {
        console.error('Erreur lors de la récupération des statistiques GitHub:', error);
    }
}

// Mettre à jour les statistiques sur la page
function updateStats(stats) {
    // Pour chaque type de statistique, mettre à jour l'affichage
    document.querySelectorAll('[data-stat]').forEach(element => {
        const statType = element.getAttribute('data-stat');
        if (stats[statType] !== undefined) {
            element.textContent = formatNumber(stats[statType]);
        }
    });
}

// Récupérer les milestones GitHub pour la roadmap
async function fetchGitHubRoadmap() {
    try {
        // Récupérer les milestones
        const milestonesResponse = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/milestones?state=all&sort=due_date&direction=asc`);
        const milestonesData = await milestonesResponse.json();
        
        // Récupérer les dernières releases
        const releasesResponse = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/releases`);
        const releasesData = await releasesResponse.json();
        
        // Créer une carte des versions déjà sorties
        const releasedVersions = new Set();
        releasesData.forEach(release => {
            releasedVersions.add(release.tag_name);
            // Aussi ajouter sans le 'v' si présent (ex: v1.0.0 -> 1.0.0)
            if (release.tag_name.startsWith('v')) {
                releasedVersions.add(release.tag_name.substring(1));
            }
        });
        
        // Mettre à jour la roadmap
        updateRoadmap(milestonesData, releasedVersions);
        
    } catch (error) {
        console.error('Erreur lors de la récupération de la roadmap GitHub:', error);
    }
}

// Mettre à jour la section roadmap avec les données de GitHub
function updateRoadmap(milestones, releasedVersions) {
    const timelineContainer = document.querySelector('.timeline');
    if (!timelineContainer) return;
    
    // Vider la timeline existante
    timelineContainer.innerHTML = '';
    
    // Maintenant est utilisé pour déterminer les jalons actuels vs futurs
    const now = new Date();
    
    milestones.forEach(milestone => {
        // Déterminer l'état du jalon
        let status = 'pending';
        
        // Si le jalon est fermé ou si le titre contient une version qui est déjà sortie
        if (milestone.state === 'closed' || isVersionReleased(milestone.title, releasedVersions)) {
            status = 'completed';
        } 
        // Si le jalon est ouvert et a une date d'échéance
        else if (milestone.due_on) {
            const dueDate = new Date(milestone.due_on);
            const threeMonthsFromNow = new Date();
            threeMonthsFromNow.setMonth(now.getMonth() + 3);
            
            // Si la date d'échéance est dans les 3 prochains mois, c'est le jalon actuel
            if (dueDate <= threeMonthsFromNow) {
                status = 'current';
            }
        }
        
        // Créer un élément de timeline pour ce jalon
        const timelineItem = document.createElement('div');
        timelineItem.className = `timeline-item ${status}`;
        
        // Formater la date
        let dateText = 'À venir';
        if (milestone.due_on) {
            const dueDate = new Date(milestone.due_on);
            dateText = `${dueDate.toLocaleDateString('fr-FR', { month: 'short', year: 'numeric' })}`;
        }
        
        // Extraire la version du titre (si présente)
        let version = extractVersion(milestone.title);
        let titleWithoutVersion = version ? milestone.title.replace(version, '').trim() : milestone.title;
        if (titleWithoutVersion.startsWith('-')) {
            titleWithoutVersion = titleWithoutVersion.substring(1).trim();
        }
        
        timelineItem.innerHTML = `
            <div class="timeline-date">${dateText}</div>
            <div class="timeline-content">
                <h4>${version || 'Mise à jour'}</h4>
                <p>${titleWithoutVersion || milestone.description || 'Amélioration du jeu'}</p>
                <div class="milestone-progress">
                    <div class="progress-bar">
                        <div class="progress" style="width: ${milestone.closed_issues / (milestone.closed_issues + milestone.open_issues) * 100 || 0}%"></div>
                    </div>
                    <span class="progress-text">${milestone.closed_issues}/${milestone.closed_issues + milestone.open_issues} tâches</span>
                </div>
            </div>
        `;
        
        timelineContainer.appendChild(timelineItem);
    });
    
    // Si aucun jalon n'a été trouvé, afficher un message
    if (milestones.length === 0) {
        timelineContainer.innerHTML = `
            <div class="timeline-empty">
                <p>Aucun jalon trouvé sur GitHub. La roadmap sera mise à jour prochainement.</p>
            </div>
        `;
    }
}

// Vérifier si une version est déjà sortie
function isVersionReleased(title, releasedVersions) {
    const version = extractVersion(title);
    return version && releasedVersions.has(version);
}

// Extraire la version d'un titre de jalon
function extractVersion(title) {
    // Chercher les formats comme "v1.0.0", "1.0.0", "Version 1.0", etc.
    const versionRegex = /\b(v?\d+\.\d+(\.\d+)?)\b|Version\s+(\d+\.\d+(\.\d+)?)/i;
    const match = title.match(versionRegex);
    return match ? match[1] || match[3] : null;
}

// Récupérer les dernières releases pour la section actualités
async function fetchLatestReleases() {
    try {
        // Récupérer les releases du dépôt
        const releasesResponse = await fetch(`https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/releases`);
        const releasesData = await releasesResponse.json();
        
        // Vérifier si des releases existent
        if (releasesData.length === 0) {
            updateNewsSection([
                {
                    date: new Date().toLocaleDateString('fr-FR'),
                    title: "Aucune release disponible pour le moment",
                    description: "Restez à l'écoute pour les futures mises à jour du jeu.",
                    url: `https://github.com/${REPO_OWNER}/${REPO_NAME}/releases`
                }
            ]);
            return;
        }
        
        // Prendre les deux dernières releases
        const latestReleases = releasesData.slice(0, 2).map(release => {
            const publishDate = new Date(release.published_at);
            
            // Extraire la première phrase de la description ou les 100 premiers caractères
            let description = release.body || "";
            if (description.length > 100) {
                const firstSentenceMatch = description.match(/^.+?[.!?]/);
                description = firstSentenceMatch 
                    ? firstSentenceMatch[0] 
                    : description.substring(0, 100) + "...";
            }
            
            return {
                date: publishDate.toLocaleDateString('fr-FR', { day: 'numeric', month: 'short', year: 'numeric' }),
                title: release.name || `Version ${release.tag_name}`,
                description: description,
                url: release.html_url
            };
        });
        
        // Mettre à jour la section actualités
        updateNewsSection(latestReleases);
        
    } catch (error) {
        console.error('Erreur lors de la récupération des releases GitHub:', error);
        // Afficher un message d'erreur dans la section actualités
        updateNewsSection([
            {
                date: new Date().toLocaleDateString('fr-FR'),
                title: "Erreur de connexion à GitHub",
                description: "Impossible de récupérer les dernières actualités. Veuillez réessayer plus tard.",
                url: `https://github.com/${REPO_OWNER}/${REPO_NAME}/releases`
            }
        ]);
    }
}

// Mettre à jour la section actualités avec les données des releases
function updateNewsSection(releases) {
    const newsContainer = document.querySelector('.latest-news');
    if (!newsContainer) return;
    
    // Trouver la zone de contenu après le titre
    let newsContent = newsContainer.querySelector('.news-content');
    
    // Si la zone n'existe pas, la créer
    if (!newsContent) {
        newsContent = document.createElement('div');
        newsContent.className = 'news-content';
        
        // Trouver le titre h3 et insérer après
        const newsTitle = newsContainer.querySelector('h3');
        if (newsTitle) {
            newsTitle.after(newsContent);
        } else {
            newsContainer.appendChild(newsContent);
        }
    }
    
    // Vider le contenu actuel
    newsContent.innerHTML = '';
    
    // Si pas de releases, afficher un message
    if (!releases || !Array.isArray(releases) || releases.length === 0) {
        newsContent.innerHTML = `
            <div class="news-item">
                <div class="news-title">Aucune version disponible</div>
                <p>Aucune version n'a encore été publiée pour ce projet.</p>
            </div>
        `;
        return;
    }
    
    // Ajouter les 2 dernières releases
    releases.slice(0, 2).forEach(release => {
        if (!release) return;
        
        const publishDate = new Date(release.published_at || new Date());
        
        // Extraire la première phrase de la description ou les 100 premiers caractères
        let description = release.body || "Pas de description disponible.";
        if (description.length > 100) {
            const firstSentenceMatch = description.match(/^.+?[.!?]/);
            description = firstSentenceMatch 
                ? firstSentenceMatch[0] 
                : description.substring(0, 100) + "...";
        }
        
        const newsItem = document.createElement('div');
        newsItem.className = 'news-item';
        
        newsItem.innerHTML = `
            <div class="news-date">${publishDate.toLocaleDateString('fr-FR', { day: 'numeric', month: 'short', year: 'numeric' })}</div>
            <div class="news-title">${release.name || `Version ${release.tag_name || '?'}`}</div>
            <p>${description}</p>
            <a href="${release.html_url || '#'}" target="_blank" rel="noopener noreferrer">Voir plus</a>
        `;
        
        newsContent.appendChild(newsItem);
    });
    
    // Ajouter un lien vers toutes les actualités
    const moreNews = document.createElement('a');
    moreNews.className = 'more-news';
    moreNews.href = `news.html`;
    moreNews.textContent = 'Voir toutes les actualités →';
    
    newsContent.appendChild(moreNews);
}

// Écouteurs d'événements pour les données GitHub
document.addEventListener(window.GitHubData.EVENTS.STATS_READY, function(e) {
    updateStats(e.detail);
});

document.addEventListener(window.GitHubData.EVENTS.RELEASES_READY, function(e) {
    updateNewsSection(e.detail);
});

// Appeler les deux fonctions au chargement de la page
document.addEventListener('DOMContentLoaded', function() {
    if (window.GitHubData && window.GitHubData.getData) {
        const data = window.GitHubData.getData();
        if (data) {
            if (data.repo) {
                updateStats({
                    stars: data.repo.stargazers_count || 0,
                    forks: data.repo.forks_count || 0,
                    contributors: data.contributors?.length || 0,
                    downloads: calculateTotalDownloads(data.releases) || 0
                });
            }
            if (data.releases && data.releases.length > 0) {
                updateNewsSection(data.releases);
            }
        }
    }
});

// Fonction pour calculer le total des téléchargements
function calculateTotalDownloads(releases) {
    if (!releases || !Array.isArray(releases)) {
        return 0;
    }
    
    return releases.reduce((total, release) => {
        if (!release.assets || !Array.isArray(release.assets)) {
            return total;
        }
        
        return total + release.assets.reduce((subTotal, asset) => {
            return subTotal + (asset.download_count || 0);
        }, 0);
    }, 0);
} 