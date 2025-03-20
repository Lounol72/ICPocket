// Fichier pour récupérer et afficher les statistiques GitHub
const REPO_OWNER = 'Lounol72';
const REPO_NAME = 'ICPocket';

// Fonction pour formater les nombres avec séparateur de milliers
function formatNumber(num) {
    return new Intl.NumberFormat().format(num);
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
            release.assets.forEach(asset => {
                totalDownloads += asset.download_count;
            });
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
    // Stars
    const starsElement = document.querySelector('.stat-value[data-stat="stars"]');
    if (starsElement) {
        starsElement.textContent = formatNumber(stats.stars);
    }
    
    // Contributeurs
    const contributorsElement = document.querySelector('.stat-value[data-stat="contributors"]');
    if (contributorsElement) {
        contributorsElement.textContent = formatNumber(stats.contributors);
    }
    
    // Forks
    const forksElement = document.querySelector('.stat-value[data-stat="forks"]');
    if (forksElement) {
        forksElement.textContent = formatNumber(stats.forks);
    }
    
    // Téléchargements
    const downloadsElement = document.querySelector('.stat-value[data-stat="downloads"]');
    if (downloadsElement) {
        downloadsElement.textContent = formatNumber(stats.downloads) + '+';
    }
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

// Charger les statistiques quand la page est prête
document.addEventListener('DOMContentLoaded', function() {
    fetchGitHubStats();
    fetchGitHubRoadmap();
}); 