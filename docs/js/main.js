// Fonction de recherche
function handleSearch() {
    const searchInput = document.querySelector('#nav-right input');
    const searchTerm = searchInput.value.toLowerCase();
    
    // Rediriger vers la page de documentation avec le terme de recherche
    window.location.href = `html/index.html?search=${encodeURIComponent(searchTerm)}`;
}

// Fonction pour activer/désactiver la recherche
function toggleSearch() {
    const navRight = document.querySelector('#nav-right');
    navRight.classList.toggle('search-active');
    
    // Si la barre de recherche devient active, mettre le focus dessus
    if (navRight.classList.contains('search-active')) {
        setTimeout(() => {
            document.querySelector('#nav-right input').focus();
        }, 300); // Délai court pour attendre la fin de l'animation
    }
}

// Attendre que le DOM soit chargé
document.addEventListener('DOMContentLoaded', function() {
    const searchButton = document.querySelector('#nav-right button');
    const searchInput = document.querySelector('#nav-right input');

    // Activation de la recherche au clic sur la loupe
    searchButton.addEventListener('click', function(e) {
        const navRight = document.querySelector('#nav-right');
        
        // Si la barre est active et contient du texte, lancer la recherche
        if (navRight.classList.contains('search-active') && searchInput.value.trim() !== '') {
            handleSearch();
        } else {
            // Sinon, basculer la visibilité
            toggleSearch();
            e.preventDefault(); // Empêcher l'envoi du formulaire
        }
    });

    // Recherche en appuyant sur Entrée
    searchInput.addEventListener('keypress', function(e) {
        if (e.key === 'Enter') {
            handleSearch();
        }
    });
    
    // Fermer la recherche en appuyant sur Escape
    document.addEventListener('keydown', function(e) {
        if (e.key === 'Escape') {
            document.querySelector('#nav-right').classList.remove('search-active');
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

let currentIndex = 0;
const screenshots = [
    'img/screenshots/screen1.png',
    'img/screenshots/screen2.png',
    'img/screenshots/screen3.png'
    // Ajoutez d'autres screenshots selon besoin
];

function changeImage(index) {
    // Utilisation du modulo pour un défilement circulaire
    currentIndex = ((index % screenshots.length) + screenshots.length) % screenshots.length;
    
    const mainImage = document.getElementById('currentScreenshot');
    mainImage.src = screenshots[currentIndex];
    
    // Mise à jour des thumbnails
    const thumbnails = document.querySelectorAll('.thumbnail');
    thumbnails.forEach((thumb, i) => {
        thumb.classList.toggle('active', i === currentIndex);
    });
    
    // Défilement des thumbnails
    const thumbnailsContainer = document.querySelector('.thumbnails');
    const thumbnail = document.querySelector('.thumbnail');
    const scrollOffset = thumbnail.offsetWidth + parseInt(getComputedStyle(thumbnail).marginRight);
    thumbnailsContainer.style.transform = `translateX(-${scrollOffset * Math.max(0, currentIndex - 1)}px)`;
}

function nextImage() {
    changeImage(currentIndex + 1);
}

function prevImage() {
    changeImage(currentIndex - 1);
}

// Gestion des touches clavier
document.addEventListener('keydown', (e) => {
    if (e.key === 'ArrowLeft') {
        prevImage();
    } else if (e.key === 'ArrowRight') {
        nextImage();
    }
});

// Gestion du swipe sur mobile
let touchStartX = 0;
let touchEndX = 0;

const mainScreenshot = document.querySelector('.main-screenshot');

mainScreenshot.addEventListener('touchstart', e => {
    touchStartX = e.changedTouches[0].screenX;
});

mainScreenshot.addEventListener('touchend', e => {
    touchEndX = e.changedTouches[0].screenX;
    handleSwipe();
});

function handleSwipe() {
    const swipeThreshold = 50;
    const swipeLength = touchEndX - touchStartX;
    
    if (Math.abs(swipeLength) > swipeThreshold) {
        if (swipeLength > 0) {
            prevImage();
        } else {
            nextImage();
        }
    }
}