// Gestionnaire de la galerie de captures d'écran pour mobile
document.addEventListener('DOMContentLoaded', function() {
  // Fonction pour vérifier si l'appareil est mobile
  function isMobileDevice() {
    return window.innerWidth < 768;
  }
  
  // Ne s'exécute que sur les appareils mobiles
  if (!isMobileDevice()) return;
  
  // Fonction d'initialisation principale
  function initGallery() {
    // Nettoyage préalable - supprimer tout élément de navigation existant pour éviter les doublons
    const existingNavs = document.querySelectorAll('.screenshot-nav');
    const existingIndicators = document.querySelectorAll('.screenshot-indicators');
    
    existingNavs.forEach(nav => nav.remove());
    existingIndicators.forEach(indicator => indicator.remove());
    
    // Sélectionner tous les éléments nécessaires
    const screenshotItems = document.querySelectorAll('.screenshot-item');
    const screenshotContainer = document.querySelector('.screenshots');
  
    // Ne continuer que s'il y a des captures d'écran
    if (!screenshotItems.length || !screenshotContainer) return;
    
    // Variables pour suivre l'état
    let currentIndex = 0;
    let touchStartX = 0;
    let touchEndX = 0;
    
    // Fonction pour configurer la galerie mobile simplifiée
    function setupMobileGallery() {
      // Créer le conteneur d'indicateurs
      const indicators = document.createElement('div');
      indicators.classList.add('screenshot-indicators');
      
      // Ajouter les éléments au DOM (uniquement les indicateurs, pas de boutons)
      screenshotContainer.appendChild(indicators);
      
      // Créer les indicateurs (points)
      screenshotItems.forEach((_, index) => {
        const dot = document.createElement('span');
        dot.classList.add('screenshot-dot');
        if (index === 0) dot.classList.add('active');
        dot.addEventListener('click', () => goToSlide(index));
        indicators.appendChild(dot);
      });
      
      // Masquer toutes les images sauf la première
      screenshotItems.forEach((item, index) => {
        if (index === 0) {
          item.classList.add('active');
        } else {
          item.classList.remove('active');
        }
      });
      
      // Ajouter l'indicateur de navigation
      const navigationHint = document.createElement('div');
      navigationHint.classList.add('navigation-hint');
      screenshotContainer.appendChild(navigationHint);
      
      // Gestion des événements tactiles pour le swipe
      screenshotContainer.addEventListener('touchstart', e => {
        touchStartX = e.changedTouches[0].screenX;
      });
      
      screenshotContainer.addEventListener('touchend', e => {
        touchEndX = e.changedTouches[0].screenX;
        handleSwipe();
      });
      
      // Ajouter l'écouteur de clic sur les images pour la navigation simplifiée
      screenshotItems.forEach(item => {
        item.addEventListener('click', handleImageClick);
      });
    }
    
    // Gestion du clic sur l'image
    function handleImageClick(e) {
      const rect = e.currentTarget.getBoundingClientRect();
      const x = e.clientX - rect.left; // Position x relative à l'image
      
      // Si le clic est sur la moitié droite, image suivante, sinon image précédente
      if (x > rect.width / 2) {
        nextSlide();
      } else {
        prevSlide();
      }
    }
    
    // Fonctions pour la navigation
    function goToSlide(index) {
      // Cacher toutes les images
      screenshotItems.forEach(item => {
        item.classList.remove('active');
      });
      
      // Mettre à jour les indicateurs
      const dots = document.querySelectorAll('.screenshot-dot');
      dots.forEach(dot => dot.classList.remove('active'));
      
      // Afficher l'image actuelle
      currentIndex = index;
      screenshotItems[currentIndex].classList.add('active');
      dots[currentIndex].classList.add('active');
    }
    
    function nextSlide() {
      currentIndex = (currentIndex + 1) % screenshotItems.length;
      goToSlide(currentIndex);
    }
    
    function prevSlide() {
      currentIndex = (currentIndex - 1 + screenshotItems.length) % screenshotItems.length;
      goToSlide(currentIndex);
    }
    
    function handleSwipe() {
      // Seuil de 50px pour considérer un swipe
      const swipeThreshold = 50;
      if (touchEndX < touchStartX - swipeThreshold) {
        // Swipe vers la gauche - image suivante
        nextSlide();
      } else if (touchEndX > touchStartX + swipeThreshold) {
        // Swipe vers la droite - image précédente
        prevSlide();
      }
    }
    
    // Initialiser la galerie
    setupMobileGallery();
  }
  
  // Appeler l'initialisation
  initGallery();
  
  // Réagir aux changements de taille d'écran
  window.addEventListener('resize', function() {
    // Si on passe en desktop
    if (!isMobileDevice()) {
      // Rétablir l'affichage normal des éléments
      const screenshotItems = document.querySelectorAll('.screenshot-item');
      screenshotItems.forEach(item => {
        item.style.display = 'block';
        item.classList.remove('active');
      });
      
      // Supprimer les éléments de navigation mobile
      const navButtons = document.querySelectorAll('.screenshot-nav');
      const indicators = document.querySelector('.screenshot-indicators');
      navButtons.forEach(button => button.remove());
      if (indicators) indicators.remove();
    } 
    // Si on repasse en mobile
    else if (isMobileDevice() && !document.querySelector('.screenshot-nav')) {
      initGallery();
    }
  });
}); 