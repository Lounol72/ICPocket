// Fonction de recherche
function handleSearch() {
    const searchInput = document.querySelector('#nav-right input');
    const searchTerm = searchInput.value.toLowerCase();
    
    // Rediriger vers la page de documentation avec le terme de recherche
    window.location.href = `html/index.html?search=${encodeURIComponent(searchTerm)}`;
}

// Attendre que le DOM soit chargé
document.addEventListener('DOMContentLoaded', function() {
    const searchButton = document.querySelector('#nav-right button');
    const searchInput = document.querySelector('#nav-right input');

    // Recherche au clic sur le bouton
    searchButton.addEventListener('click', handleSearch);

    // Recherche en appuyant sur Entrée
    searchInput.addEventListener('keypress', function(e) {
        if (e.key === 'Enter') {
            handleSearch();
        }
    });

    // Ajout du gestionnaire pour le bouton de téléchargement
    const downloadButton = document.getElementById('downloadButton');
    if (downloadButton) {
        downloadButton.addEventListener('click', function() {
            window.location.href = 'https://github.com/Lounol72/ICPocket/releases';
        });
    }
});

// Ajouter un objet de données searchables
const searchableContent = {
    'combat': {
        title: 'Système de Combat',
        description: 'Combats au tour par tour stratégiques',
        link: '#features'
    },
    'map': {
        title: 'Carte du Monde',
        description: 'Monde ouvert à explorer',
        link: '#features'
    },
    'pokemon': {
        title: 'Créatures',
        description: 'Découvrez les créatures uniques',
        link: '#features'
    },
    // Ajoutez d'autres éléments recherchables
};

function searchContent(term) {
    const results = [];
    term = term.toLowerCase();
    
    for (const [key, value] of Object.entries(searchableContent)) {
        if (key.includes(term) || 
            value.title.toLowerCase().includes(term) || 
            value.description.toLowerCase().includes(term)) {
            results.push(value);
        }
    }
    
    displayResults(results);
}

function displayResults(results) {
    const resultsContainer = document.querySelector('.results-container');
    const searchResults = document.getElementById('searchResults');
    
    if (results.length === 0) {
        resultsContainer.innerHTML = '<p>Aucun résultat trouvé</p>';
    } else {
        resultsContainer.innerHTML = results.map(result => `
            <div class="search-result">
                <h4>${result.title}</h4>
                <p>${result.description}</p>
                <a href="${result.link}">En savoir plus</a>
            </div>
        `).join('');
    }
    
    searchResults.style.display = 'block';
}