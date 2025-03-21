// Configuration des membres de l'équipe avec leur rôle
const TEAM_MEMBERS = {
    'Lounol72': {
        name: 'Louis Subtil',
        role: 'Lead Developer',
        description: 'Responsable de l\'architecture globale du jeu et du développement du moteur de combat.'
    },
    'Natp24109': {
        name: 'Nathan Pasquier',
        role: 'Game Developer & Level Designer',
        description: 'Conception des mécaniques de jeu et implémentation des fonctionnalités de gameplay.'
    },
    'AstroZr': {
        name: 'Alban Borde',
        role: 'Game Developer & Level Designer',
        description: 'Conception des mécaniques de jeu et implémentation des fonctionnalités de gameplay.'
    },
    'lOkaal1': {
        name: 'William Littre',
        role: 'Développeur Frontend & Testeur',
        description: 'Développement des interfaces et assurance qualité du jeu.'
    }
};

// Données de secours au cas où l'API ne fonctionne pas
const FALLBACK_CONTRIBUTORS = [
    {
        "login": "Lounol72",
        "id": 16176398,
        "avatar_url": "https://avatars.githubusercontent.com/u/16176398?v=4",
        "html_url": "https://github.com/Lounol72",
        "contributions": 150
    }
];

// Génération du HTML pour un membre de l'équipe
function createTeamMemberHTML(username, memberInfo, contributorData) {
    // Utiliser l'avatar GitHub si disponible, sinon utiliser un placeholder
    const avatarUrl = contributorData?.avatar_url || 
                     `https://avatars.githubusercontent.com/${username}` || 
                     `https://via.placeholder.com/90?text=${memberInfo.name.charAt(0)}`;
    const githubUrl = contributorData?.html_url || `https://github.com/${username}`;
    const contributions = contributorData?.contributions || 0;
    
    return `
        <div class="team-member">
            <div class="member-avatar">
                <img src="${avatarUrl}" alt="Photo de ${memberInfo.name}" 
                     onerror="this.src='https://via.placeholder.com/90?text=${memberInfo.name.charAt(0)}'">
            </div>
            <div class="member-info">
                <h3>${memberInfo.name}</h3>
                <div class="member-role">${memberInfo.role}</div>
                <p>${memberInfo.description}</p>
                ${contributions ? `<div class="member-contributions"><i class="fas fa-code-commit"></i> ${contributions} contributions</div>` : ''}
                <div class="member-links">
                    <a href="${githubUrl}" target="_blank" rel="noopener noreferrer" title="GitHub"><i class="fab fa-github"></i></a>
                </div>
            </div>
        </div>
    `;
}

// Afficher les membres de l'équipe
function displayTeamMembers(contributors) {
    // Utiliser les données de secours si aucun contributeur n'est fourni
    if (!contributors || !Array.isArray(contributors) || contributors.length === 0) {
        console.warn('Aucun contributeur trouvé, utilisation des données de secours');
        contributors = FALLBACK_CONTRIBUTORS;
    }
    
    const teamGrid = document.querySelector('.team-grid');
    if (!teamGrid) {
        console.error('Élément .team-grid non trouvé');
        return;
    }
    
    console.log(`Affichage de ${contributors.length} contributeurs`);
    
    // Créer un dictionnaire pour un accès rapide aux données des contributeurs
    const contributorsDict = {};
    contributors.forEach(contributor => {
        if (contributor && contributor.login) {
            contributorsDict[contributor.login] = contributor;
        }
    });
    
    // Vider le conteneur
    teamGrid.innerHTML = '';
    
    // Ajouter chaque membre à la grille
    Object.entries(TEAM_MEMBERS).forEach(([username, memberInfo]) => {
        const contributorData = contributorsDict[username] || null;
        const memberHTML = createTeamMemberHTML(username, memberInfo, contributorData);
        
        // Créer un élément div temporaire pour pouvoir insérer le HTML
        const tempDiv = document.createElement('div');
        tempDiv.innerHTML = memberHTML;
        
        // Ajouter le premier enfant du div temporaire à la grille
        teamGrid.appendChild(tempDiv.firstElementChild);
    });
}

// Afficher une erreur
function displayTeamError(error) {
    const teamGrid = document.querySelector('.team-grid');
    if (!teamGrid) return;
    
    console.error('Erreur équipe:', error);
    
    teamGrid.innerHTML = `
        <div class="error-message" style="grid-column: 1 / -1;">
            <i class="fas fa-exclamation-triangle"></i>
            <p>Impossible de charger les membres de l'équipe.</p>
            <small>${error.message || 'Erreur inconnue'}</small>
        </div>
    `;
}

// Écouteurs d'événements pour les données GitHub
document.addEventListener(window.GitHubData.EVENTS.CONTRIBUTORS_READY, function(e) {
    console.log('Événement CONTRIBUTORS_READY reçu');
    displayTeamMembers(e.detail);
});

document.addEventListener(window.GitHubData.EVENTS.ERROR, function(e) {
    console.log('Événement ERROR reçu');
    displayTeamError(e.detail);
}); 