// Configuration du dépôt et des membres de l'équipe
const TEAM_MEMBERS = [
    {
        username: 'Lounol72',    // Nom d'utilisateur GitHub
        name: 'Louis Subtil',    // Nom complet
        role: 'Lead Developer',
        description: 'Responsable de l\'architecture globale du jeu et du développement du moteur de combat.'
    },
    {
        username: 'AstrozR',  // Remplacez par le nom d'utilisateur réel
        name: 'Alban Borde',
        role: 'Game Developer & Level Designer',
        description: 'Conception des mécaniques de jeu et implémentation des fonctionnalités de gameplay.'
    },
    {
        username: 'Natp24109',  // Remplacez par le nom d'utilisateur réel
        name: 'Nathan Pasquier',
        role: 'Game Developer & Level Designer',
        description: 'Conception des mécaniques de jeu et implémentation des fonctionnalités de gameplay.'
    },
    {
        username: 'Okaal',  // Remplacez par le nom d'utilisateur réel
        name: 'William Littre',
        role: 'Game Developer & Level Designer',
        description: 'Conception des mécaniques de jeu et implémentation des fonctionnalités de gameplay.'
    }
];

// Fonction pour récupérer les détails d'un membre à partir de son nom d'utilisateur GitHub
async function fetchGitHubUser(username) {
    try {
        const response = await fetch(`https://api.github.com/users/${username}`);
        if (!response.ok) {
            throw new Error(`Erreur HTTP: ${response.status}`);
        }
        return await response.json();
    } catch (error) {
        console.error(`Erreur lors de la récupération des données pour ${username}:`, error);
        return null;
    }
}

// Génération du HTML pour un membre de l'équipe
function createTeamMemberHTML(member, githubData) {
    // Utiliser l'avatar GitHub si disponible, sinon utiliser un placeholder
    const avatarUrl = githubData?.avatar_url || `https://via.placeholder.com/150?text=${member.name.charAt(0)}`;
    const githubUrl = githubData?.html_url || `https://github.com/${member.username}`;
    
    // Récupérer d'autres informations intéressantes de GitHub
    const location = githubData?.location || '';
    const bio = githubData?.bio || '';
    const repos = githubData?.public_repos || 0;
    const followers = githubData?.followers || 0;
    
    return `
        <div class="team-member">
            <div class="member-avatar">
                <img src="${avatarUrl}" alt="Photo de ${member.name}" onerror="this.src='https://via.placeholder.com/150?text=${member.name.charAt(0)}'">
            </div>
            <div class="member-info">
                <h3>${member.name}</h3>
                <div class="member-role">${member.role}</div>
                <p>${member.description}</p>
                ${bio ? `<p class="member-bio">"${bio}"</p>` : ''}
                ${location ? `<div class="member-location"><i class="fas fa-map-marker-alt"></i> ${location}</div>` : ''}
                <div class="member-github-stats">
                    ${repos ? `<span><i class="fas fa-code-branch"></i> ${repos} dépôts</span>` : ''}
                    ${followers ? `<span><i class="fas fa-users"></i> ${followers} followers</span>` : ''}
                </div>
                <div class="member-links">
                    <a href="${githubUrl}" target="_blank" rel="noopener noreferrer" title="GitHub"><i class="fab fa-github"></i></a>
                </div>
            </div>
        </div>
    `;
}

// Chargement et affichage des membres de l'équipe
async function loadTeamMembers() {
    const teamGrid = document.querySelector('.team-grid');
    if (!teamGrid) return;
    
    // Afficher un indicateur de chargement
    teamGrid.innerHTML = `
        <div class="loading-animation" style="grid-column: 1 / -1;">
            <i class="fas fa-spinner fa-spin"></i> Chargement de l'équipe...
        </div>
    `;
    
    try {
        // Récupérer les données GitHub pour chaque membre
        const memberPromises = TEAM_MEMBERS.map(async (member) => {
            const githubData = await fetchGitHubUser(member.username);
            return { member, githubData };
        });
        
        // Attendre que toutes les requêtes soient terminées
        const results = await Promise.all(memberPromises);
        
        // Vider le conteneur
        teamGrid.innerHTML = '';
        
        // Ajouter chaque membre à la grille
        results.forEach(({ member, githubData }) => {
            const memberHTML = createTeamMemberHTML(member, githubData);
            
            // Créer un élément div temporaire pour pouvoir insérer le HTML
            const tempDiv = document.createElement('div');
            tempDiv.innerHTML = memberHTML;
            
            // Ajouter le premier enfant du div temporaire (le membre de l'équipe) à la grille
            teamGrid.appendChild(tempDiv.firstElementChild);
        });
        
    } catch (error) {
        console.error('Erreur lors du chargement des membres de l\'équipe:', error);
        teamGrid.innerHTML = `
            <div class="error-message" style="grid-column: 1 / -1;">
                <i class="fas fa-exclamation-triangle"></i>
                <p>Impossible de charger les membres de l'équipe.</p>
                <small>${error.message}</small>
            </div>
        `;
    }
}

// Charger les membres de l'équipe au chargement de la page
document.addEventListener('DOMContentLoaded', function() {
    // Vérifier si nous sommes sur la page de l'équipe
    if (document.querySelector('.team-grid')) {
        loadTeamMembers();
    }
}); 